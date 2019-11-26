/******************************************************************************
Copyright (C), 2016, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : mipi_tx_hal.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2016/11/15
Last Modified :
Description   :
Function List :
******************************************************************************/
#ifndef __MIPI_TX_HAL_H__
#define __MIPI_TX_HAL_H__

#include "hi_mipi_tx.h"

void mipi_tx_drv_set_phy_cfg(combo_dev_cfg_t *p_dev_cfg);
void mipi_tx_drv_set_controller_cfg(combo_dev_cfg_t *p_dev_cfg);

void mipi_tx_drv_set_cmd_info(cmd_info_t *cmd_info);

void mipi_tx_drv_enable_input(output_mode_t output_mode);

int mipi_tx_drv_init(void);
void mipi_tx_drv_exit(void);

#endif
