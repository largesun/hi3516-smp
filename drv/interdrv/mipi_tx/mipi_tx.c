/******************************************************************************
Copyright (C), 2016, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : mipi_tx.c
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2016/11/8
Last Modified :
Description   : This program is free software; you can redistribute it and/or modify
              : it under the terms of the GNU General Public License as published by
              : the Free Software Foundation; either version 2 of the License, or
              : (at your option) any later version.
              : This program is distributed in the hope that it will be useful,
              : but WITHOUT ANY WARRANTY; without even the implied warranty of
              : MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
              : GNU General Public License for more details.
              : You should have received a copy of the GNU General Public License
              : along with this program;
Function List :
******************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include "hi_osal.h"
#include "type.h"
#include "hi_mipi_tx.h"
#include "mipi_tx_hal.h"

/****************************************************************************
 * macro definition                                                         *
 ****************************************************************************/
#define MIPI_TX_DEV_NAME           "hi_mipi_tx"
#define MIPI_TX_PROC_NAME          "mipi_tx"

#define HIMEDIA_DYNAMIC_MINOR      255

#define NULL    0L


typedef struct
{
    combo_dev_cfg_t dev_cfg;
} mipi_tx_dev_ctx_t;
/****************************************************************************
 * global variables definition                                              *
 ****************************************************************************/
static osal_dev_t* mipi_tx_dev;

mipi_tx_dev_ctx_t mipi_tx_dev_ctx;
/****************************************************************************
 * function definition                                                      *
 ****************************************************************************/
static int mipi_tx_check_comb_dev_cfg(combo_dev_cfg_t *p_dev_cfg)
{
    return 0;
}

static int mipi_tx_set_combo_dev_cfg(combo_dev_cfg_t *p_dev_cfg)
{
    int ret;

    ret = mipi_tx_check_comb_dev_cfg(p_dev_cfg);
    if (ret < 0)
    {
        HI_ERR("mipi_tx check combo_dev config failed!\n");
        return -1;
    }

    /* set controler config */
    mipi_tx_drv_set_controller_cfg(p_dev_cfg);

    /* set phy config*/
    mipi_tx_drv_set_phy_cfg(p_dev_cfg);


    osal_memcpy(&mipi_tx_dev_ctx, p_dev_cfg, sizeof(combo_dev_cfg_t));

    return ret;
}

static int mipi_tx_set_cmd(cmd_info_t *cmd_info)
{
    mipi_tx_drv_set_cmd_info(cmd_info);

    return 0;
}

static void mipi_tx_enable(void)
{
    output_mode_t output_mode;

    output_mode = mipi_tx_dev_ctx.dev_cfg.output_mode;

    mipi_tx_drv_enable_input(output_mode);
}

static long mipi_tx_ioctl(unsigned int cmd, unsigned long arg, void* private_data)
{
    int ret = 0;

    switch (cmd)
    {
        case HI_MIPI_TX_SET_DEV_CFG:
        {
            combo_dev_cfg_t *pstcombo_dev_cfg = NULL;

            pstcombo_dev_cfg = (combo_dev_cfg_t *)arg;

            MIPITX_CHECK_NULL_PTR(pstcombo_dev_cfg);

            ret = mipi_tx_set_combo_dev_cfg(pstcombo_dev_cfg);
            if (ret < 0)
            {
                HI_ERR("mipi_tx set combo_dev config failed!\n");
                ret = -1;
            }
            break;
        }

        case HI_MIPI_TX_SET_CMD:
        {
            cmd_info_t *cmd_info = NULL;

            cmd_info = (cmd_info_t *)arg;

            MIPITX_CHECK_NULL_PTR(cmd_info);

            ret = mipi_tx_set_cmd(cmd_info);
            if (ret < 0)
            {
                HI_ERR("mipi_tx set cmd failed!\n");
                ret = -1;
            }
            break;
        }

        case HI_MIPI_TX_ENABLE:
        {
            mipi_tx_enable();
            break;
        }

        default:
        {
            HI_ERR("invalid mipi_tx ioctl cmd\n");
            ret = -1;
            break;
        }
    }

    return ret;
}

#ifdef CONFIG_COMPAT
static long mipi_tx_compat_ioctl(unsigned int cmd, unsigned long arg, void* private_data)
{
    switch (cmd)
    {
        default:
        {
            break;
        }
    }

    return mipi_tx_ioctl(cmd, arg, private_data);
}
#endif

static int mipi_tx_init(void)
{
    mipi_tx_drv_init();

    return 0;
}

static void mipi_tx_exit(void)
{
    mipi_tx_drv_exit();
}

static int mipi_tx_proc_show(osal_proc_entry_t *s)
{
    osal_seq_printf(s, "\nModule: [MIPI_TX], Build Time["__DATE__", "__TIME__"]\n");

    return 0;
}

static int mipi_tx_open(void* private_data)
{
    return 0;
}

static int mipi_tx_release(void *private_data)
{
    return 0;
}

static osal_fileops_t mipi_tx_fops =
{
    .open           = mipi_tx_open,
    .release        = mipi_tx_release,
    .unlocked_ioctl = mipi_tx_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl   = mipi_tx_compat_ioctl,
#endif
};

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static int mipi_tx_freeze(osal_dev_t *pdev)
{
    return 0;
}

static int mipi_tx_restore(osal_dev_t *pdev)
{
    return 0;
}
#else
static int mipi_tx_freeze(osal_dev_t *pdev)
{
    return 0;
}

static int mipi_tx_restore(osal_dev_t *pdev)
{
    return 0;
}
#endif

static osal_pmops_t mipi_tx_pmops =
{
    .pm_freeze  = mipi_tx_freeze,
    .pm_restore = mipi_tx_restore,
};

int mipi_tx_module_init(void)
{
    int ret;
    osal_proc_entry_t *mipi_tx_proc_entry;

    mipi_tx_dev = osal_createdev(MIPI_TX_DEV_NAME);
    if (NULL == mipi_tx_dev)
    {
        HI_ERR("create mipi_tx device failed!\n");
        goto fail0;
    }

    mipi_tx_dev->fops  = &mipi_tx_fops;
    mipi_tx_dev->minor = HIMEDIA_DYNAMIC_MINOR;
    mipi_tx_dev->osal_pmops = &mipi_tx_pmops;

    ret = osal_registerdevice(mipi_tx_dev);
    if (0 != ret)
    {
        HI_ERR("register mipi_tx device failed!\n");
        goto fail1;
    }

    mipi_tx_proc_entry = osal_create_proc_entry(MIPI_TX_PROC_NAME, NULL);
    if (NULL == mipi_tx_proc_entry)
    {
        HI_ERR("create mipi_tx proc(%s) failed!\n", MIPI_TX_PROC_NAME);
        goto fail2;
    }

    mipi_tx_proc_entry->read = mipi_tx_proc_show;
    mipi_tx_proc_entry->write = NULL;

    ret = mipi_tx_init();
    if (0 != ret)
    {
        HI_ERR("hi_mipi_init failed!\n");
        goto fail3;
    }

    osal_printk("load mipi_tx driver successful!\n");
    return 0;

fail3:
    osal_remove_proc_entry(MIPI_TX_PROC_NAME, NULL);
fail2:
    osal_deregisterdevice(mipi_tx_dev);
fail1:
    osal_destroydev(mipi_tx_dev);
    mipi_tx_dev = NULL;
fail0:
    HI_ERR("load mipi_tx driver failed!\n");
    return -1;
}

void mipi_tx_module_exit(void)
{
    mipi_tx_exit();

    osal_remove_proc_entry(MIPI_TX_PROC_NAME, NULL);

    osal_deregisterdevice(mipi_tx_dev);

    osal_destroydev(mipi_tx_dev);
    mipi_tx_dev = NULL;

    osal_printk("unload mipi_tx driver ok!\n");
}

#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif /* End of #ifdef __cplusplus */
