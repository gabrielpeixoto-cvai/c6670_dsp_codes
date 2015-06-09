/**
 *   @file  cppi_device.c
 *
 *   @brief   
 *      This file contains the device specific configuration and initialization routines
 *      for CPPI Low Level Driver.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009, Texas Instruments, Inc.
 *  @n   Use of this software is controlled by the terms and conditions found 
 *  @n   in the license agreement under which this software has been supplied.
 *  ============================================================================ 
 *  \par
 */

/* CPPI Types includes */
#include <cppi_types.h>

/* CPPI includes */
#include <ti/drv/cppi/cppi_drv.h>

/* CSL RL includes */
#include <ti/csl/cslr_device.h>
#include <ti/csl/cslr_cppidma_global_config.h>
#include <ti/csl/cslr_cppidma_rx_channel_config.h>
#include <ti/csl/cslr_cppidma_rx_flow_config.h>
#include <ti/csl/cslr_cppidma_tx_channel_config.h>
#include <ti/csl/cslr_cppidma_tx_scheduler_config.h>
#include <ti/csl/csl_cppi.h>

/** @addtogroup CPPI_LLD_DATASTRUCT
@{ 
*/
/** @brief CPPI LLD initialization parameters */
Cppi_GlobalConfigParams cppiGblCfgParams[CPPI_MAX_CPDMA] =
{
    {
        /** CPDMA this configuration belongs to */
        Cppi_CpDma_SRIO_CPDMA,
        /** Maximum supported Rx Channels */
        16u,
        /** Maximum supported Tx Channels */
        16u,
        /** Maximum supported Rx Flows */
        20u,
        /** Priority for all Rx transactions of this CPDMA */
        0u,
        /** Priority for all Tx transactions of this CPDMA */
        0u,

        /** Base address for the CPDMA overlay registers */

        /** Global Config registers */
        (void *) CSL_SRIO_CONFIG_CPPI_DMA_GLOBAL_CFG_REGS,
        /** Tx Channel Config registers */
        (void *) CSL_SRIO_CONFIG_CPPI_DMA_TX_CFG_REGS,
        /** Rx Channel Config registers */
        (void *) CSL_SRIO_CONFIG_CPPI_DMA_RX_CFG_REGS,
        /** Tx Channel Scheduler registers */
        (void *) CSL_SRIO_CONFIG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
        /** Rx Flow Config registers */
        (void *) CSL_SRIO_CONFIG_CPPI_DMA_RX_FLOW_CFG_REGS,
    },
    {
        /** CPDMA this configuration belongs to */
        Cppi_CpDma_AIF_CPDMA,
        /** Maximum supported Rx Channels */
        129u,
        /** Maximum supported Tx Channels */
        129u,
        /** Maximum supported Rx Flows */
        129u,
        /** Priority for all Rx transactions of this CPDMA */
        0u,
        /** Priority for all Tx transactions of this CPDMA */
        0u,

        /** Base address for the CPDMA overlay registers */

        /** Global Config registers */
        (void *) CSL_AIF2_CFG_CPPI_DMA_GLOBAL_CFG_REGS,
        /** Tx Channel Config registers */
        (void *) CSL_AIF2_CFG_CPPI_DMA_TX_CFG_REGS,
        /** Rx Channel Config registers */
        (void *) CSL_AIF2_CFG_CPPI_DMA_RX_CFG_REGS,
        /** Tx Channel Scheduler registers */
        (void *) CSL_AIF2_CFG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
        /** Rx Flow Config registers */
        (void *) CSL_AIF2_CFG_CPPI_DMA_RX_FLOW_CFG_REGS,
    },
    {
        /** CPDMA this configuration belongs to */
        Cppi_CpDma_FFTC_A_CPDMA,
        /** Maximum supported Rx Channels */
        4u,
        /** Maximum supported Tx Channels */
        4u,
        /** Maximum supported Rx Flows */
        8u,
        /** Priority for all Rx transactions of this CPDMA */
        0u,
        /** Priority for all Tx transactions of this CPDMA */
        0u,

        /** Base address for the CPDMA overlay registers */

        /** Global Config registers */
        (void *) CSL_FFTC_A_CONFIG_CPPI_DMA_GLOBAL_CFG_REGS,
        /** Tx Channel Config registers */
        (void *) CSL_FFTC_A_CONFIG_CPPI_DMA_TX_CFG_REGS,
        /** Rx Channel Config registers */
        (void *) CSL_FFTC_A_CONFIG_CPPI_DMA_RX_CFG_REGS,
        /** Tx Channel Scheduler registers */
        (void *) CSL_FFTC_A_CONFIG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
        /** Rx Flow Config registers */
        (void *) CSL_FFTC_A_CONFIG_CPPI_DMA_RX_FLOW_CFG_REGS,
    },
    {
        /** CPDMA this configuration belongs to */
        Cppi_CpDma_FFTC_B_CPDMA,
        /** Maximum supported Rx Channels */
        4u,
        /** Maximum supported Tx Channels */
        4u,
        /** Maximum supported Rx Flows */
        8u,
        /** Priority for all Rx transactions of this CPDMA */
        0u,
        /** Priority for all Tx transactions of this CPDMA */
        0u,

        /** Base address for the CPDMA overlay registers */

        /** Global Config registers */
        (void *) CSL_FFTC_B_CONFIG_CPPI_DMA_GLOBAL_CFG_REGS,
        /** Tx Channel Config registers */
        (void *) CSL_FFTC_B_CONFIG_CPPI_DMA_TX_CFG_REGS,
        /** Rx Channel Config registers */
        (void *) CSL_FFTC_B_CONFIG_CPPI_DMA_RX_CFG_REGS,
        /** Tx Channel Scheduler registers */
        (void *) CSL_FFTC_B_CONFIG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
        /** Rx Flow Config registers */
        (void *) CSL_FFTC_B_CONFIG_CPPI_DMA_RX_FLOW_CFG_REGS,
    },
    {
        /** CPDMA this configuration belongs to */
        Cppi_CpDma_PASS_CPDMA,
        /** Maximum supported Rx Channels */
        24u,
        /** Maximum supported Tx Channels */
        9u,
        /** Maximum supported Rx Flows */
        32u,
        /** Priority for all Rx transactions of this CPDMA */
        0u,
        /** Priority for all Tx transactions of this CPDMA */
        0u,

        /** Base address for the CPDMA overlay registers */

        /** Global Config registers */
        (void *) CSL_PA_SS_CFG_CPPI_DMA_GLOBAL_CFG_REGS,
        /** Tx Channel Config registers */
        (void *) CSL_PA_SS_CFG_CPPI_DMA_TX_CFG_REGS,
        /** Rx Channel Config registers */
        (void *) CSL_PA_SS_CFG_CPPI_DMA_RX_CFG_REGS,
        /** Tx Channel Scheduler registers */
        (void *) CSL_PA_SS_CFG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
        /** Rx Flow Config registers */
        (void *) CSL_PA_SS_CFG_CPPI_DMA_RX_FLOW_CFG_REGS,
    },
    {
        /** CPDMA this configuration belongs to */
        Cppi_CpDma_QMSS_CPDMA,
        /** Maximum supported Rx Channels */
        32u,
        /** Maximum supported Tx Channels */
        32u,
        /** Maximum supported Rx Flows */
        64u,
        /** Priority for all Rx transactions of this CPDMA */
        0u,
        /** Priority for all Tx transactions of this CPDMA */
        0u,

        /** Base address for the CPDMA overlay registers */

        /** Global Config registers */
        (void *) CSL_QM_SS_CFG_CPPI_DMA_GLOBAL_CFG_REGS,
        /** Tx Channel Config registers */
        (void *) CSL_QM_SS_CFG_CPPI_DMA_TX_CFG_REGS,
        /** Rx Channel Config registers */
        (void *) CSL_QM_SS_CFG_CPPI_DMA_RX_CFG_REGS,
        /** Tx Channel Scheduler registers */
        (void *) CSL_QM_SS_CFG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
        /** Rx Flow Config registers */
        (void *) CSL_QM_SS_CFG_CPPI_DMA_RX_FLOW_CFG_REGS,
    },
    {
        /** CPDMA this configuration belongs to */
        Cppi_CpDma_FFTC_C_CPDMA,
        /** Maximum supported Rx Channels */
        (uint32_t) 4u,
        /** Maximum supported Tx Channels */
        (uint32_t) 4u,
        /** Maximum supported Rx Flows */
        (uint32_t) 8u,
        /** Priority for all Rx transactions of this CPDMA */
        (uint8_t) 0u,
        /** Priority for all Tx transactions of this CPDMA */
        (uint8_t) 0u,

        /** Base address for the CPDMA overlay registers */

        /** Global Config registers */
        (void *) CSL_FFTC_C_CONFIG_CPPI_DMA_GLOBAL_CFG_REGS,
        /** Tx Channel Config registers */
        (void *) CSL_FFTC_C_CONFIG_CPPI_DMA_TX_CFG_REGS,
        /** Rx Channel Config registers */
        (void *) CSL_FFTC_C_CONFIG_CPPI_DMA_RX_CFG_REGS,
        /** Tx Channel Scheduler registers */
        (void *) CSL_FFTC_C_CONFIG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
        /** Rx Flow Config registers */
        (void *) CSL_FFTC_C_CONFIG_CPPI_DMA_RX_FLOW_CFG_REGS,
    },    
    {
        /** CPDMA this configuration belongs to */
        Cppi_CpDma_BCP_CPDMA,
        /** Maximum supported Rx Channels */
        (uint32_t) 8u,
        /** Maximum supported Tx Channels */
        (uint32_t) 8u,
        /** Maximum supported Rx Flows */
        (uint32_t) 64u,
        /** Priority for all Rx transactions of this CPDMA */
        (uint8_t) 0u,
        /** Priority for all Tx transactions of this CPDMA */
        (uint8_t) 0u,

        /** Base address for the CPDMA overlay registers */

        /** Global Config registers */
        (void *) CSL_BCP_CONFIG_CPPI_DMA_GLOBAL_CFG_REGS,
        /** Tx Channel Config registers */
        (void *) CSL_BCP_CONFIG_CPPI_DMA_TX_CFG_REGS,
        /** Rx Channel Config registers */
        (void *) CSL_BCP_CONFIG_CPPI_DMA_RX_CFG_REGS,
        /** Tx Channel Scheduler registers */
        (void *) CSL_BCP_CONFIG_CPPI_DMA_TX_SCHEDULER_CFG_REGS,
        /** Rx Flow Config registers */
        (void *) CSL_BCP_CONFIG_CPPI_DMA_RX_FLOW_CFG_REGS,
    },    
};

/**
@}
*/

