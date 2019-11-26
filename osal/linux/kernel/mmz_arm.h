#ifndef __MMZ_ARCH_OPS_HEADER__
#define __MMZ_ARCH_OPS_HEADER__

#if defined(CONFIG_ARM64)
/* this function is used to change memory's property. */
static void dma_pages_remap(struct page* page, size_t size, pgprot_t prot) {}

static void dma_buffer_clear(struct page* page, size_t size)
{
	memset(page_address(page), 0, size);
	__flush_dcache_area(page_address(page), size);
}

static void mmb_dcache_flush(hil_mmb_t *mmb)
{
	__flush_dcache_area((void *)mmb->kvirt, (size_t)mmb->length);
}

static pgprot_t arch_kern_pgprot(int cache)
{
	if (cache)
		return PAGE_KERNEL;

	return __pgprot(PROT_NORMAL_NC);
}

#elif defined(CONFIG_ARM)
#include <asm/highmem.h>
#include <asm/pgtable.h>

static int __dma_update_pte(pte_t* pte, pgtable_t token,
		unsigned long addr, void* data)
{
	struct page* page = virt_to_page(addr);

	pgprot_t prot = *(pgprot_t*)data;

	set_pte_ext(pte, mk_pte(page, prot), 0);

	return 0;
}

extern void hisi_flush_tlb_kernel_range(unsigned long start, unsigned long end);

/* this function is used to change memory's property. */
static void dma_pages_remap(struct page* page, size_t size, pgprot_t prot)
{
	unsigned long start = (unsigned long) page_address(page);
	unsigned end = start + size;

	apply_to_page_range(&init_mm, start, size, __dma_update_pte, &prot);
	dsb();
	hisi_flush_tlb_kernel_range(start, end);
}

extern void __dma_clear_buffer(struct page* page, size_t size);

static void dma_buffer_clear(struct page* page, size_t size)
{
	__dma_clear_buffer(page, size);
}

static void mmb_dcache_flush(hil_mmb_t *mmb)
{
	__cpuc_flush_dcache_area((void*)mmb->kvirt, (size_t)mmb->length);

	outer_flush_range(mmb->phys_addr, mmb->phys_addr + mmb->length);
}

static pgprot_t arch_kern_pgprot(int cache)
{
	if (cache)
		return pgprot_kernel;

	return pgprot_noncached(pgprot_kernel);
}

#else
 #error no proper arch selected(CONFIG_ARM64/CONFIG_ARM)!
#endif

#endif
