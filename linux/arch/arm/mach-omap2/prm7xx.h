/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * DRA7xx PRM instance offset macros
 *
 * Copyright (C) 2013 Texas Instruments Incorporated - https://www.ti.com
 *
 * Generated by code originally written by:
 * Paul Walmsley (paul@pwsan.com)
 * Rajendra Nayak (rnayak@ti.com)
 * Benoit Cousson (b-cousson@ti.com)
 *
 * This file is automatically generated from the OMAP hardware databases.
 * We respectfully ask that any modifications to this file be coordinated
 * with the public linux-omap@vger.kernel.org mailing list and the
 * authors above to ensure that the autogeneration scripts are kept
 * up-to-date with the file contents.
 */

#ifndef __ARCH_ARM_MACH_OMAP2_PRM7XX_H
#define __ARCH_ARM_MACH_OMAP2_PRM7XX_H

#include "prcm-common.h"
#include "prm44xx_54xx.h"
#include "prm.h"

#define DRA7XX_PRM_BASE		0x4ae06000

#define DRA7XX_PRM_REGADDR(inst, reg)				\
	OMAP2_L4_IO_ADDRESS(DRA7XX_PRM_BASE + (inst) + (reg))


/* PRM instances */
#define DRA7XX_PRM_OCP_SOCKET_INST	0x0000
#define DRA7XX_PRM_CKGEN_INST		0x0100
#define DRA7XX_PRM_MPU_INST		0x0300
#define DRA7XX_PRM_DSP1_INST		0x0400
#define DRA7XX_PRM_IPU_INST		0x0500
#define DRA7XX_PRM_COREAON_INST		0x0628
#define DRA7XX_PRM_CORE_INST		0x0700
#define DRA7XX_PRM_IVA_INST		0x0f00
#define DRA7XX_PRM_CAM_INST		0x1000
#define DRA7XX_PRM_DSS_INST		0x1100
#define DRA7XX_PRM_GPU_INST		0x1200
#define DRA7XX_PRM_L3INIT_INST		0x1300
#define DRA7XX_PRM_L4PER_INST		0x1400
#define DRA7XX_PRM_CUSTEFUSE_INST	0x1600
#define DRA7XX_PRM_WKUPAON_INST		0x1724
#define DRA7XX_PRM_WKUPAON_CM_INST	0x1800
#define DRA7XX_PRM_EMU_INST		0x1900
#define DRA7XX_PRM_EMU_CM_INST		0x1a00
#define DRA7XX_PRM_DSP2_INST		0x1b00
#define DRA7XX_PRM_EVE1_INST		0x1b40
#define DRA7XX_PRM_EVE2_INST		0x1b80
#define DRA7XX_PRM_EVE3_INST		0x1bc0
#define DRA7XX_PRM_EVE4_INST		0x1c00
#define DRA7XX_PRM_RTC_INST		0x1c60
#define DRA7XX_PRM_VPE_INST		0x1c80
#define DRA7XX_PRM_DEVICE_INST		0x1d00

/* PRM clockdomain register offsets (from instance start) */
#define DRA7XX_PRM_WKUPAON_CM_WKUPAON_CDOFFS	0x0000
#define DRA7XX_PRM_EMU_CM_EMU_CDOFFS		0x0000

/* PRM.CKGEN_PRM register offsets */
#define DRA7XX_CM_CLKSEL_SYS					DRA7XX_PRM_REGADDR(DRA7XX_PRM_CKGEN_INST, 0x0010)

#endif