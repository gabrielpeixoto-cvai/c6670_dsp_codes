/**
 *   @file  infrastructure_multicoremode.c
 *
 *   @brief   
 *      This is the QMSS infrastructure mode example code that can run on multiple cores. 
 *      Accumulator used to interrupt different cores they are mapped to.
 *      Shows synchronization uses queues
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2009, Texas Instruments, Inc.
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  \par
*/

#include <xdc/std.h>
#include <xdc/runtime/IHeap.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Memory.h>
#include <xdc/runtime/Error.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/heaps/HeapBuf.h>
#include <ti/sysbios/heaps/HeapMem.h>
#include <xdc/cfg/global.h>
#include <string.h>

/* QMSS LLD include */
#include <ti/drv/qmss/qmss_drv.h>
#include <ti/drv/qmss/qmss_firmware.h>

/* CPPI LLD include */
#include <ti/drv/cppi/cppi_drv.h>
#include <ti/drv/cppi/cppi_desc.h>


/* CSL RL includes */
#include <ti/csl/csl_chip.h>
#include <csl_intc.h>
#include <csl_intcAux.h>
#include <ti/csl/csl_cacheAux.h>
#include <ti/csl/csl_qm_queue.h>

/* Device specific include */
#include "qmssPlatCfg.h"

/************************ USER DEFINES ********************/

#define MAPPED_VIRTUAL_ADDRESS      0x81000000

#define NUMBER_OF_CORES             qmss_EXAMPLE_NUM_CORES
#define SYSINIT                     0
#define NUM_ITERATION               1 * NUMBER_OF_CORES

#define CPPI_FREE_TX_QUE_MGR        0
#define CPPI_FREE_TX_QUE_NUM        736

#define CPPI_FREE_RX_QUE_MGR        0
#define CPPI_FREE_RX_QUE_NUM        737

#define CPPI_COMPLETION_QUE_MGR     0
#define CPPI_COMPLETION_QUE_NUM     1000

#define QMSS_SYNC_CFG_QUE_MGR       0
#define QMSS_SYNC_CFG_QUE_NUM       2000

#define QMSS_SYNC_QUE_MGR           0
#define QMSS_SYNC_QUE_NUM           3000

#define QMSS_FREE_SYNC_QUE_MGR      0
#define QMSS_FREE_SYNC_QUE_NUM      4000

#define NUM_MONOLITHIC_DESC         64
#define SIZE_MONOLITHIC_DESC        160
#define MONOLITHIC_DESC_DATA_OFFSET 16
#define NUM_SYNC_DESC               32
#define SIZE_SYNC_DESC              32
#define SIZE_DATA_BUFFER            16
#define NUM_PACKETS                 8

/* MPAX segment 2 registers */
#define XMPAXL2                     0x08000010 
#define XMPAXH2                     0x08000014


/************************ GLOBAL VARIABLES ********************/
#pragma DATA_ALIGN (linkingRAM0, 16)
UInt64              linkingRAM0[NUM_MONOLITHIC_DESC + NUM_SYNC_DESC];
/* Descriptor pool [Size of descriptor * Number of descriptors] */
#pragma DATA_ALIGN (monolithicDesc, 16)
UInt8                   monolithicDesc[SIZE_MONOLITHIC_DESC * NUM_MONOLITHIC_DESC];
#pragma DATA_ALIGN (syncDesc, 16)
UInt8                   syncDesc[SIZE_SYNC_DESC * NUM_SYNC_DESC];
#pragma DATA_ALIGN (dataBuff, 16)
UInt8                   dataBuff[SIZE_DATA_BUFFER];
/* List address for accumulator - twice the number of entries for Ping and Pong page */
#pragma DATA_ALIGN (hiPrioList, 16)
Uint32                  hiPrioList[34];
/* CPDMA configuration */
Cppi_CpDmaInitCfg       cpdmaCfg;
/* Tx channel configuration */
Cppi_TxChInitCfg        txChCfg;
/* Rx channel configuration */
Cppi_RxChInitCfg        rxChCfg;
/* Rx flow configuration */
Cppi_RxFlowCfg          rxFlowCfg;
/* QMSS configuration */
Qmss_InitCfg            qmssInitConfig;
/* Memory region configuration information */
Qmss_MemRegInfo         memInfo;
/* Handle to CPPI heap */
IHeap_Handle            cppiHeap;
/* Accumulator configuration */
Qmss_AccCmdCfg          cfg;
/* Indicates the core test is running on */
UInt32                  coreNum;

Cppi_Handle             cppiHnd;
Cppi_ChHnd              rxChHnd, txChHnd;
Qmss_QueueHnd           txQueHnd, rxQueHnd, rxFreeQueHnd, txCmplQueHnd, txFreeQueHnd, syncQueHnd, syncFreeQueHnd, syncCfgQueHnd;
Cppi_DescCfg            descCfg;
Qmss_DescCfg            syncDescCfg;
Cppi_FlowHnd            rxFlowHnd;

CSL_IntcEventHandlerRecord  Record[2];
CSL_IntcParam               vectId;
CSL_IntcObj                 hiPrioIntcObj;
CSL_IntcHandle              hiPrioIntcHnd;
CSL_IntcContext             context;

#pragma DATA_SECTION (isQMSSInitialized, ".qmss");
volatile UInt32             isQMSSInitialized;

#pragma DATA_SECTION (runCount, ".qmss");
UInt32                      runCount;

/************************ EXTERN VARIABLES ********************/

/* QMSS device specific configuration */
extern Qmss_GlobalConfigParams  qmssGblCfgParams;
/* CPPI device specific configuration */
extern Cppi_GlobalConfigParams  cppiGblCfgParams;

/*************************** FUNCTIONS ************************/
/**
 *  @b Description
 *  @n  
 *      The function is used to get the handle to the CPPI memory heap.
 *      If the application is run on a multiple cores then place the CPPI global 
 *      variables in shared memory.  
 *
 *  @retval
 *      Not Applicable
 */
static Void cppiHeapInit ()
{
    cppiHeap = HeapMem_Handle_upCast (cppiSharedHeap);
}

/**
 *  @b Description
 *  @n  
 *      Utility function which converts a local GEM L2 memory address 
 *      to global memory address.
 *
 *  @param[in]  addr
 *      Local address to be converted
 *
 *  @retval
 *      Computed L2 global Address
 */
static UInt32 l2_global_address (UInt32 addr)
{
	UInt32 coreNum;

	/* Get the core number. */
	coreNum = CSL_chipReadReg (CSL_CHIP_DNUM); 

	/* Compute the global address. */
	return (addr + (0x10000000 + (coreNum * 0x1000000)));
}

/**
 *  @b Description
 *  @n  
 *      Utility function that prints entry count in various queues
 *
 *  @param[in]  msg
 *      Status message to be printed
 *
 *  @retval
 *      Not Applicable
 */
static Void printQueueStats (char *msg)
{
    Qmss_Result     result;
        
    System_printf ("\n--------------------Queue status CORE %d----------------------\n", coreNum);
    System_printf ("                    %s\n\n", msg);

    result = Qmss_getQueueEntryCount (txFreeQueHnd);
    System_printf ("Tx Free Queue %d Entry Count            : %d \n", txFreeQueHnd, result);

    result = Qmss_getQueueEntryCount (rxFreeQueHnd);
    System_printf ("Rx Free Queue %d Entry Count            : %d \n", rxFreeQueHnd, result);

    result = Qmss_getQueueEntryCount (txCmplQueHnd);
    System_printf ("Tx completion Queue %d Entry Count     : %d \n", txCmplQueHnd, result);

    result = Qmss_getQueueEntryCount (syncQueHnd);
    System_printf ("Sync Queue %d Entry Count              : %d \n", syncQueHnd, result);

    result = Qmss_getQueueEntryCount (syncFreeQueHnd);
    System_printf ("Sync free Queue %d Entry Count         : %d \n", syncFreeQueHnd, result);

    result = Qmss_getQueueEntryCount (syncCfgQueHnd);
    System_printf ("Sync Cfg Queue %d Entry Count          : %d \n", syncCfgQueHnd, result);

    System_printf ("-------------------------------------------------------------\n\n");  
}

/**
 *  @b Description
 *  @n  
 *      Closes queues, channel and flow used in a single iteration.
 *
 *  @retval
 *      Not Applicable
 */
static Void cleanup (Void)
{
    Qmss_Result     result;
        
    /* Close Rx queue */
    if ((result = Qmss_queueClose (rxQueHnd)) != CPPI_SOK)
        System_printf ("Error Core %d : Closing Rx queue error code : %d\n", coreNum, result);
    
    /* Close Tx queue */
    if ((result = Qmss_queueClose (txQueHnd)) != CPPI_SOK)
        System_printf ("Error Core %d : Closing tx queue error code : %d\n", coreNum, result);

    /* Disable Tx channel */
    if ((result = Cppi_channelDisable (txChHnd)) != CPPI_SOK)
        System_printf ("Error Core %d : Disabling Tx channel error code : %d\n", coreNum, result);

    /* Disable Rx channel */
    if ((result = Cppi_channelDisable (rxChHnd)) != CPPI_SOK)
        System_printf ("Error Core %d : Disabling Rx channel error code : %d\n", coreNum, result);

    /* Close Tx channel */
    if ((result = Cppi_channelClose (txChHnd)) != CPPI_SOK)
        System_printf ("Error Core %d : Closing Tx channel error code : %d\n", coreNum, result);

    /* Close Rx channel */
    if ((result = Cppi_channelClose (rxChHnd)) != CPPI_SOK)
        System_printf ("Error Core %d : Closing Rx channel error code : %d\n", coreNum, result);

    /* Close Rx flow */
    if ((result = Cppi_closeRxFlow (rxFlowHnd)) != CPPI_SOK)
        System_printf ("Error Core %d : Closing Rx flow error code : %d\n", coreNum, result);
}

/**
 *  @b Description
 *  @n  
 *
 *      Initializes the system
 *      It performs the following
 *          - Initializes the Queue Manager low level driver
 *          - COnfigures descriptor memory region
 *          - Initializes the CPPI low level driver
 *          - Opens the CPPI CPDMA in queue manager
 *          - Initializes descriptors and pushes to free queue
 *          - Opens synchronization queues
 *  @retval
 *      Not Applicable.
 */
static Int32 sysInit (Void)
{
    Qmss_Result             result;
    UInt32                  numAllocated; 
    UInt8                   isAllocated;
#ifdef L2_CACHE
    uint32_t                *xmpaxPtr;
#endif

    System_printf ("\n-----------------------Initializing---------------------------\n");

    /* Reset the variable to indicate to other cores system init is not yet done */
    isQMSSInitialized = 0;

    /* Initilaize the number of times the test was run to zero */
    runCount = 0;

    /* Initialize the heap in shared memory for CPPI data structures */ 
    cppiHeapInit ();

#ifdef L2_CACHE
    /* Set L2 cache to 512KB */
    CACHE_setL2Size (CACHE_512KCACHE);
#endif
    
    System_printf ("Core %d : L1D cache size %d. L2 cache size %d.\n", coreNum, CACHE_getL1DSize(), CACHE_getL2Size());

#ifdef L2_CACHE
    /* Define an MPAX segment in the virtual (CGEM) address space. 
     * Map MSMC physical address to virtual address.
     * Configure all +rwx permissions.
     */

    /* Phy address base: 0x0C00 0000
     * Size: 1MB (0x13 according to encoding)
     * Virtual address: 0x8100 0000 
     * Permission: 0xFF
     * MAR used: (0x8100 0000 >> 24) = 129
     */

    /* map using MPAX segment 2 registers */
    xmpaxPtr  = (uint32_t *)(XMPAXH2);
    *xmpaxPtr = ((MAPPED_VIRTUAL_ADDRESS >> 12) << 12) | (0x13);

    xmpaxPtr  = (uint32_t *)(XMPAXL2);
    *xmpaxPtr = ((0x0c000000 >> 12) << 8) | (0xFF);
    
    /* Enable caching for MAR 129. CSL does not define these MARS. Define a macro */
    CACHE_enableCaching ((MAPPED_VIRTUAL_ADDRESS) >> 24);
#endif

    memset ((Void *) &qmssInitConfig, 0, sizeof (Qmss_InitCfg));
    memset ((Void *) &linkingRAM0, 0, sizeof (linkingRAM0));
    /* Set up the linking RAM. Use the internal Linking RAM. 
     * LLD will configure the internal linking RAM address and maximum internal linking RAM size if 
     * a value of zero is specified.
     * Linking RAM1 is not used */

#ifdef INTERNAL_LINKING_RAM 
    qmssInitConfig.linkingRAM0Base = 0;
    qmssInitConfig.linkingRAM0Size = 0;
    qmssInitConfig.linkingRAM1Base = 0;
    qmssInitConfig.maxDescNum      = NUM_MONOLITHIC_DESC + NUM_SYNC_DESC;
#else
    qmssInitConfig.linkingRAM0Base = (UInt32) l2_global_address((Uint32)&linkingRAM0[0]);
    qmssInitConfig.linkingRAM0Size = 0x3FFF;
    qmssInitConfig.linkingRAM1Base = 0;
    qmssInitConfig.maxDescNum      = NUM_MONOLITHIC_DESC + NUM_SYNC_DESC;
#endif

#ifdef xdc_target__bigEndian
    qmssInitConfig.pdspFirmware[0].pdspId = Qmss_PdspId_PDSP1;
    qmssInitConfig.pdspFirmware[0].firmware = (void *) &acc48_be;
    qmssInitConfig.pdspFirmware[0].size = sizeof (acc48_be);
#else
    qmssInitConfig.pdspFirmware[0].pdspId = Qmss_PdspId_PDSP1;
    qmssInitConfig.pdspFirmware[0].firmware = (void *) &acc48_le;
    qmssInitConfig.pdspFirmware[0].size = sizeof (acc48_le);
#endif

    /* Initialize Queue Manager SubSystem */
    result = Qmss_init (&qmssInitConfig, &qmssGblCfgParams);
    if (result != QMSS_SOK)
    {
        System_printf ("Error Core %d : Initializing Queue Manager SubSystem error code : %d\n", coreNum, result);
        return -1;
    }

    /* Start Queue Manager SubSystem */
    result = Qmss_start ();
    if (result != QMSS_SOK)
    {
        System_printf ("Core %d : Error starting Queue Manager error code : %d\n", coreNum, result);
    }

    /* Initialize CPPI LLD */
    result = Cppi_init (&cppiGblCfgParams);
    if (result != CPPI_SOK)
    {
        System_printf ("Error Core %d : Initializing CPPI LLD error code : %d\n", coreNum, result);
    }

    /* Set up QMSS CPDMA configuration */
    memset ((Void *) &cpdmaCfg, 0, sizeof (Cppi_CpDmaInitCfg));
    cpdmaCfg.dmaNum = Cppi_CpDma_QMSS_CPDMA;

    /* Open QMSS CPDMA */
    cppiHnd = (Cppi_Handle) Cppi_open (&cpdmaCfg);
    if (cppiHnd == NULL)
    {
        System_printf ("Error Core %d : Initializing QMSS CPPI CPDMA %d\n", coreNum, cpdmaCfg.dmaNum);
        return -1;
    }

    /* Setup memory region for monolithic descriptors */
    memset ((Void *) &monolithicDesc, 0, SIZE_MONOLITHIC_DESC * NUM_MONOLITHIC_DESC);
    memInfo.descBase = (UInt32 *) l2_global_address ((UInt32) monolithicDesc);
    memInfo.descSize = SIZE_MONOLITHIC_DESC;
    memInfo.descNum = NUM_MONOLITHIC_DESC;
    memInfo.manageDescFlag = Qmss_ManageDesc_MANAGE_DESCRIPTOR;
    memInfo.memRegion = Qmss_MemRegion_MEMORY_REGION_NOT_SPECIFIED;
    memInfo.startIndex = 0;

    result = Qmss_insertMemoryRegion (&memInfo);
    if (result < QMSS_SOK)
    {
        System_printf ("Error Core %d : Inserting memory region %d error code : %d\n", coreNum, memInfo.memRegion, result);
    }
    else
        System_printf ("Core %d : Memory region %d inserted\n", coreNum, result);

    /* Setup memory region for Sync descriptors */
    memset ((Void *) &syncDesc, 0, SIZE_SYNC_DESC * NUM_SYNC_DESC);
    memInfo.descBase = (UInt32 *) l2_global_address ((UInt32) syncDesc);
    memInfo.descSize = SIZE_SYNC_DESC;
    memInfo.descNum = NUM_SYNC_DESC;
    memInfo.manageDescFlag = Qmss_ManageDesc_MANAGE_DESCRIPTOR;
    memInfo.memRegion = Qmss_MemRegion_MEMORY_REGION_NOT_SPECIFIED;
    memInfo.startIndex = 0;

    result = Qmss_insertMemoryRegion (&memInfo);
    if (result < QMSS_SOK)
    {
        System_printf ("Error Core %d : Inserting memory region %d error code : %d\n", coreNum, memInfo.memRegion, result);
    }
    else
        System_printf ("Core %d : Memory region %d inserted\n", coreNum, result);

    /* Setup the descriptors for transmit free queue */
    /* Memory region obtained is zero since there is only Qmss_insertMemoryRegion() call. 
     * else find the memory region using Qmss_getMemoryRegionCfg() */
    descCfg.memRegion = Qmss_MemRegion_MEMORY_REGION0;
    descCfg.descNum = NUM_MONOLITHIC_DESC / 2;
    descCfg.destQueueNum = CPPI_FREE_TX_QUE_NUM;
    descCfg.queueType = Qmss_QueueType_STARVATION_COUNTER_QUEUE;
    descCfg.initDesc = Cppi_InitDesc_INIT_DESCRIPTOR;
    descCfg.descType = Cppi_DescType_MONOLITHIC;
    descCfg.epibPresent = Cppi_EPIB_NO_EPIB_PRESENT;

    descCfg.cfg.mono.dataOffset = MONOLITHIC_DESC_DATA_OFFSET;
    
    /* Descriptor should be recycled back to Queue Number 1000 */
    descCfg.returnQueue.qMgr = CPPI_COMPLETION_QUE_MGR;
    descCfg.returnQueue.qNum = CPPI_COMPLETION_QUE_NUM;

    /* Initialize the descriptors and push to free Queue */
    if ((txFreeQueHnd = Cppi_initDescriptor (&descCfg, &numAllocated)) < 0)
	{
        System_printf ("Error Core %d : Initializing Tx descriptor error code: %d \n", coreNum, txFreeQueHnd);
		return -1;
	}
    else
        System_printf ("Core %d : Number of Tx descriptors requested : %d. Number of descriptors allocated : %d \n", 
            coreNum, descCfg.descNum, numAllocated);

    /* Setup the descriptors for receive free queue */
    descCfg.memRegion = Qmss_MemRegion_MEMORY_REGION0;
    descCfg.descNum = NUM_MONOLITHIC_DESC / 2;
    descCfg.destQueueNum = CPPI_FREE_RX_QUE_NUM;
    descCfg.queueType = Qmss_QueueType_STARVATION_COUNTER_QUEUE;
    descCfg.initDesc = Cppi_InitDesc_INIT_DESCRIPTOR;
    descCfg.descType = Cppi_DescType_MONOLITHIC;
    descCfg.epibPresent = Cppi_EPIB_NO_EPIB_PRESENT;
    descCfg.cfg.mono.dataOffset = MONOLITHIC_DESC_DATA_OFFSET;
    
    /* Descriptor should be recycled back to Queue Number 1000 */
    descCfg.returnQueue.qMgr = QMSS_PARAM_NOT_SPECIFIED;
    descCfg.returnQueue.qNum = QMSS_PARAM_NOT_SPECIFIED;
    
    /* Initialize the descriptors and push to free Queue */
    if ((rxFreeQueHnd = Cppi_initDescriptor (&descCfg, &numAllocated)) < 0)
	{
        System_printf ("Error Core %d : Initializing Rx descriptor error code: %d \n", coreNum, rxFreeQueHnd);
		return -1;
	}
    else
        System_printf ("Core %d : Number of Rx descriptors requested : %d. Number of descriptors allocated : %d \n", 
            coreNum, descCfg.descNum, numAllocated);

    /* Setup the descriptors for sync free queue */
    syncDescCfg.memRegion = Qmss_MemRegion_MEMORY_REGION1;
    syncDescCfg.descNum = NUM_SYNC_DESC;
    syncDescCfg.destQueueNum = QMSS_FREE_SYNC_QUE_NUM;
    syncDescCfg.queueType = Qmss_QueueType_GENERAL_PURPOSE_QUEUE;
    
    /* Initialize the descriptors and push to free Queue */

    if ((syncFreeQueHnd = Qmss_initDescriptor (&syncDescCfg, &numAllocated)) < 0)
	{
        System_printf ("Error Core %d : Initializing Sync free descriptor error code: %d \n", coreNum, syncFreeQueHnd);
		return -1;
	}
    else
        System_printf ("Core %d : Number of Sync free descriptors requested : %d. Number of descriptors allocated : %d \n", 
            coreNum, syncDescCfg.descNum, numAllocated);


    /* Opens transmit completion queue. */
    if ((txCmplQueHnd = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, CPPI_COMPLETION_QUE_NUM, &isAllocated)) < 0)
	{
        System_printf ("Error Core %d : Opening Tx Completion Queue Number\n", coreNum);
		return -1;
	}
    else
        System_printf ("Core %d : Tx Completion Queue Number     : %d opened\n", coreNum, txCmplQueHnd);

    /* Opens sync queue */
    if ((syncQueHnd = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, QMSS_SYNC_QUE_NUM, &isAllocated)) < 0)
	{
        System_printf ("Error Core %d : Opening Sync Queue Number\n", coreNum);
		return -1;
	}
    else
        System_printf ("Core %d : Sync Queue Number              : %d opened\n", coreNum, syncQueHnd);

    /* Opens sync Configuration queue */
    if ((syncCfgQueHnd = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, QMSS_SYNC_CFG_QUE_NUM, &isAllocated)) < 0)
	{
        System_printf ("Error Core %d : Opening Sync Cfg Queue Number\n", coreNum);
		return -1;
	}
    else
        System_printf ("Core %d : Sync Cfg Queue Number          : %d opened\n", coreNum, syncCfgQueHnd);

    /* Opens sync free queue. */

    System_printf ("Core %d : Sync Free Queue Number         : %d opened\n", coreNum, syncFreeQueHnd);
    System_printf ("Core %d : Receive Free Queue Number      : %d opened\n", coreNum, rxFreeQueHnd);
    System_printf ("Core %d : Transmit Free Queue Number     : %d opened\n", coreNum, txFreeQueHnd);

    System_printf ("Core %d : System initialization completed\n", coreNum, txFreeQueHnd);

    /* Indicate to other cores system init is done */
    isQMSSInitialized = 1;

#ifdef L2_CACHE
    /* Writeback L2 */
    CACHE_wbL2 ((void *) &isQMSSInitialized, 4, CACHE_WAIT);
#else
    /* Writeback L1D */
    CACHE_wbL1d ((void *) &isQMSSInitialized, 4, CACHE_WAIT);
#endif
    printQueueStats ("After Initialization");
    return 0;
}

/**
 *  @b Description
 *  @n  
 *      Function initializes the common queue handles opened by producer core on consumer cores
 *
 *  @retval
 *      Not Applicable
 */
static Void getsysHandles (Void)
{
    UInt8           isAllocated;
    volatile Qmss_Result     result;

    /* Start Queue Manager SubSystem */
    System_printf ("Core %d : Waiting for QMSS to be initialized...\n\n", coreNum);

    /* Synchronize all consumer cores. They must wait for the producer core to finish initialization. */
   
    do{
#ifdef L2_CACHE
        /* Invalidate L2 */
        CACHE_invL2 ((void *) &isQMSSInitialized, 4, CACHE_WAIT);
#else
        CACHE_invL1d ((void *) &isQMSSInitialized, 4, CACHE_WAIT);
#endif
    } while (isQMSSInitialized == 0);

    System_printf ("\nCore %d : QMSS initialization done.\n\n", coreNum);

    result = Qmss_start ();
    if (result != QMSS_SOK)
    {
        System_printf ("Core %d : Error starting Queue Manager error code : %d\n", coreNum, result);
		return;
    }

    if ((rxFreeQueHnd = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, CPPI_FREE_RX_QUE_NUM, &isAllocated)) < 0)
    {
        System_printf ("Error Core %d : Opening Rx Free Queue Number\n", coreNum);
		return;
    }
    else
        System_printf ("Core %d : Rx Free Queue Number       : %d opened\n", coreNum, rxFreeQueHnd);

    if ((txFreeQueHnd = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, CPPI_FREE_TX_QUE_NUM, &isAllocated)) < 0)
    {
        System_printf ("Error Core %d : Opening Tx Free Queue Number\n", coreNum);
		return;
    }
    else
        System_printf ("Core %d : Tx Free Queue Number       : %d opened\n", coreNum, txFreeQueHnd);

    if ((txCmplQueHnd = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, CPPI_COMPLETION_QUE_NUM, &isAllocated)) < 0)
    {
        System_printf ("Error Core %d : Opening Tx Completion Queue Number\n", coreNum);
		return;
    }
    else
        System_printf ("Core %d : Tx Completion Queue Number : %d opened\n", coreNum, txCmplQueHnd);

    if ((syncQueHnd = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, QMSS_SYNC_QUE_NUM, &isAllocated)) < 0)
    {
        System_printf ("Error Core %d : Opening Sync Queue Number\n", coreNum);
		return;
    }
    else
        System_printf ("Core %d : Sync Queue Number          : %d opened\n", coreNum, syncQueHnd);
        
    if ((syncFreeQueHnd = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, QMSS_FREE_SYNC_QUE_NUM, &isAllocated)) < 0)
    {
        System_printf ("Error Core %d : Opening Sync Free Queue Number\n", coreNum);
		return;
    }
    else
        System_printf ("Core %d : Sync Free Queue Number     : %d opened\n", coreNum, syncFreeQueHnd);

    /* Opens sync Configuration queue */
    if ((syncCfgQueHnd = Qmss_queueOpen (Qmss_QueueType_GENERAL_PURPOSE_QUEUE, QMSS_SYNC_CFG_QUE_NUM, &isAllocated)) < 0)
	{
        System_printf ("Error Core %d : Opening Sync Cfg Queue Number\n", coreNum);
		return;
	}
    else
        System_printf ("Core %d : Sync Cfg Queue Number      : %d opened\n", coreNum, syncCfgQueHnd);

}

/**
 *  @b Description
 *  @n  
 *
 *      Deinitializes the system
 *      It performs the following
 *          - Closes all open common queues
 *          - Closes CPDMA instance
 *          - Deinitializes CPPI LLD
 *  @retval
 *      Not Applicable.
 */
static Void sysExit (Void)
{
    Qmss_Result     result;
        
    System_printf ("\n--------------------Deinitializing---------------------------\n");

    printQueueStats ("Before exit");
    
    /* Close the queues */

    if ((result = Qmss_queueClose (rxFreeQueHnd)) != CPPI_SOK)
    {
        System_printf ("Core %d : Closing free queue Ref count : %d\n", coreNum, result);
        if (result > 0)
        {
            while (result != CPPI_SOK)
            {
                result = Qmss_queueClose (rxFreeQueHnd);
                System_printf ("Core %d : Closing free queue Ref count : %d\n", coreNum, result);
            }
        }
    }
    else
        System_printf ("Core %d : Receive free queue closed successfully. Ref count : %d\n", coreNum, result);

    if ((result = Qmss_queueClose (txCmplQueHnd)) != CPPI_SOK)
    {
        System_printf ("Core %d : Closing transmit completion queue Ref count : %d\n", coreNum, result);
        if (result > 0)
        {
            while (result != CPPI_SOK)
            {
                result = Qmss_queueClose (txCmplQueHnd);
                System_printf ("Core %d : Closing transmit completion queue Ref count : %d\n", coreNum, result);
            }
        }
    }
    else
        System_printf ("Core %d : Transmit completion queue closed successfully. Ref count : %d\n", coreNum, result);

    if ((result = Qmss_queueClose (txFreeQueHnd)) != CPPI_SOK)
    {
        System_printf ("Core %d : Closing transmit free queue Ref count : %d\n", coreNum, result);
        if (result > 0)
        {
            while (result != CPPI_SOK)
            {
                result = Qmss_queueClose (txFreeQueHnd);
                System_printf ("Core %d : Closing transmit free queue Ref count : %d\n", coreNum, result);
            }
        }
    }
    else
        System_printf ("Core %d : Transmit free queue closed successfully. Ref count : %d\n", coreNum, result);
   
    if ((result = Qmss_queueClose (syncQueHnd)) != CPPI_SOK)
    {
        System_printf ("Core %d : Closing sync queue Ref count : %d\n", coreNum, result);
        if (result > 0)
        {
            while (result != CPPI_SOK)
            {
                result = Qmss_queueClose (syncQueHnd);
                System_printf ("Core %d : Closing sync queue Ref count : %d\n", coreNum, result);
            }
        }

    }
    else
        System_printf ("Core %d : Sync queue closed successfully. Ref count : %d\n", coreNum, result);
    
    if ((result = Qmss_queueClose (syncFreeQueHnd)) != CPPI_SOK)
    {
        System_printf ("Core %d : Closing sync free queue Ref count : %d\n", coreNum, result);
        if (result > 0)
        {
            while (result != CPPI_SOK)
            {
                result = Qmss_queueClose (syncFreeQueHnd);
                System_printf ("Core %d : Closing sync free queue Ref count : %d\n", coreNum, result);
            }
        }
    }
    else
        System_printf ("Core %d : Sync free queue closed successfully. Ref count : %d\n", coreNum, result);

    if ((result = Qmss_queueClose (syncCfgQueHnd)) != CPPI_SOK)
    {
        System_printf ("Core %d : Closing sync cfg queue Ref count : %d\n", coreNum, result);
        if (result > 0)
        {
            while (result != CPPI_SOK)
            {
                result = Qmss_queueClose (syncCfgQueHnd);
                System_printf ("Core %d : Closing sync cfg queue Ref count : %d\n", coreNum, result);
            }
        }

    }
    else
        System_printf ("Core %d : Sync queue closed successfully. Ref count : %d\n", coreNum, result);

    /* Close CPPI CPDMA instance */
    if ((result = Cppi_close (cppiHnd)) != CPPI_SOK)
        System_printf ("Error Core %d : Closing CPPI CPDMA error code : %d\n", coreNum, result);
    else 
        System_printf ("Core %d : CPPI CPDMA closed successfully\n", coreNum);

    /* Deinitialize CPPI LLD */
    if ((result = Cppi_exit ()) != CPPI_SOK)
        System_printf ("Error Core %d : Exiting CPPI error code : %d\n", coreNum, result);
    else 
        System_printf ("Core %d : CPPI exit successful\n", coreNum);
}

/**
 *  @b Description
 *  @n  
 *
 *      Used to send data from the produce core to te consumer core
 *      It performs the following
 *          - Opens transmit and receive channels
 *          - Opens transmit and receive queues
 *          - Programs receive flow
 *          - Programs accumulator
 *          - Sets transmit threshold
 *          - Gets a free transmit descriptor, initializes and pushes packet to transmit queue
 *  @retval
 *      Not Applicable.
 */
static Int32 send_data (UInt32 channel, Uint32 core)
{
    Cppi_Desc               *monoDescPtr;
    UInt32                  i;
    UInt8                   isAllocated;
    Qmss_Result             result;
    Cppi_DescTag            tag;
    Qmss_Queue              queInfo;

    /* Set up Tx Channel parameters */
    memset ((Void *) &txChCfg, 0, sizeof (Cppi_TxChInitCfg));
    
    txChCfg.channelNum = channel;
    txChCfg.priority = 0;
    txChCfg.filterEPIB = 0;
    txChCfg.filterPS = 0;
    txChCfg.aifMonoMode = 0;
    txChCfg.txEnable = Cppi_ChState_CHANNEL_DISABLE;
    
    /* Open Tx Channel */
    txChHnd = (Cppi_ChHnd) Cppi_txChannelOpen (cppiHnd, &txChCfg, &isAllocated);
    if (txChHnd == NULL)
    {
        System_printf ("Error Core %d : Opening Tx channel : %d\n", coreNum, txChCfg.channelNum);
        return -1;
    }
    else 
        System_printf ("Core %d : Opened Tx channel      : %d\n", coreNum, Cppi_getChannelNumber (txChHnd));

    /* Set up Rx Channel parameters */
    memset ((Void *) &rxChCfg, 0, sizeof (Cppi_RxChInitCfg));
    rxChCfg.channelNum = channel;
    rxChCfg.rxEnable = Cppi_ChState_CHANNEL_DISABLE;
    
    /* Open Rx Channel */
    rxChHnd = (Cppi_ChHnd) Cppi_rxChannelOpen (cppiHnd, &rxChCfg, &isAllocated);
    if (rxChHnd == NULL)
    {
        System_printf ("Error Core %d : Opening Rx channel : %d\n", coreNum, rxChCfg.channelNum);
        return -1;
    }
    else 
        System_printf ("Core %d : Opened Rx channel      : %d\n", coreNum, Cppi_getChannelNumber (rxChHnd));

    /* Opens transmit queue. This is the infrastructure queue */
    if ((txQueHnd = Qmss_queueOpen (Qmss_QueueType_INFRASTRUCTURE_QUEUE, 
                                    QMSS_INFRASTRUCTURE_QUEUE_BASE + channel, &isAllocated)) < 0)
	{
        System_printf ("Error Core %d : Opening Transmit Queue Number\n", coreNum);
		return -1;
	}
    else
        System_printf ("Core %d : Transmit Queue Number  : %d\n", coreNum, txQueHnd);

    /* Opens receive queue */
    if ((rxQueHnd = Qmss_queueOpen (Qmss_QueueType_HIGH_PRIORITY_QUEUE, 
                                    QMSS_HIGH_PRIORITY_QUEUE_BASE + channel, &isAllocated)) < 0)
	{
        System_printf ("Error Core %d : Opening Receive Queue Number\n", coreNum);
        return -1;
	}
    else
        System_printf ("Core %d : Receive Queue Number   : %d\n", coreNum, rxQueHnd);

    /* program the high priority accumulator */
    memset ((Void *) &hiPrioList, 0, sizeof (hiPrioList));

    cfg.channel = channel;
    cfg.command = Qmss_AccCmd_ENABLE_CHANNEL;
    cfg.queueEnMask = 0;
    cfg.listAddress = l2_global_address ((UInt32) hiPrioList);
    /* Get queue manager and queue number from handle */
    queInfo = Qmss_getQueueNumber (rxQueHnd);
    cfg.queMgrIndex = queInfo.qNum;
    cfg.maxPageEntries = NUM_PACKETS + 1;
    cfg.timerLoadCount = 0;
    cfg.interruptPacingMode = Qmss_AccPacingMode_NONE;
    cfg.listEntrySize = Qmss_AccEntrySize_REG_D;
    cfg.listCountMode = Qmss_AccCountMode_ENTRY_COUNT;
    cfg.multiQueueMode = Qmss_AccQueueMode_SINGLE_QUEUE;
    
    if ((result = Qmss_programAccumulator (Qmss_PdspId_PDSP1, &cfg)) != QMSS_ACC_SOK)
	{
        System_printf ("Error Core %d : Programming high priority accumulator for channel : %d queue : %d error code : %d\n",
                        coreNum, cfg.channel, cfg.queMgrIndex, result);
		return -1;
	}

    /* Set transmit queue threshold to high and when there is atleast one packet */
    /* Setting threshold on transmit queue is not required anymore. tx pending queue is not hooked to threshold. 
     * Qmss_setQueueThreshold (txQueHnd, 1, 1);
     */ 
        
    /* Setup Rx flow parameters */
    memset ((Void *) &rxFlowCfg, 0, sizeof (Cppi_RxFlowCfg));

    /* Configure flow 0 */
    rxFlowCfg.flowIdNum = channel % NUMBER_OF_CORES;
    /* Get queue manager and queue number from handle */
    queInfo = Qmss_getQueueNumber (rxQueHnd);
    rxFlowCfg.rx_dest_qnum = queInfo.qNum;
    rxFlowCfg.rx_dest_qmgr = queInfo.qMgr;
    rxFlowCfg.rx_sop_offset = MONOLITHIC_DESC_DATA_OFFSET;
    rxFlowCfg.rx_desc_type = Cppi_DescType_MONOLITHIC; 
    
    /* Get queue manager and queue number from handle */
    queInfo = Qmss_getQueueNumber (rxFreeQueHnd);
    rxFlowCfg.rx_fdq0_sz0_qnum = queInfo.qNum;
    rxFlowCfg.rx_fdq0_sz0_qmgr = queInfo.qMgr;

    /* Configure Rx flow */
    rxFlowHnd = (Cppi_FlowHnd) Cppi_configureRxFlow (cppiHnd, &rxFlowCfg, &isAllocated);
    if (rxFlowHnd == NULL)
    {
        System_printf ("Error Core %d : Opening Rx flow : %d\n", coreNum, rxFlowCfg.flowIdNum);
        return -1;
    }
    else 
        System_printf ("Core %d : Opened Rx flow         : %d\n", coreNum, Cppi_getFlowId(rxFlowHnd));

    System_printf ("Core %d : High priority accumulator programmed for channel : %d queue : %d\n", 
                        coreNum, cfg.channel, cfg.queMgrIndex);

    /* Enable transmit channel */
    if (Cppi_channelEnable (txChHnd) != CPPI_SOK)
    {
        System_printf ("Error Core %d : Enabling Tx channel : %d\n", coreNum, Cppi_getChannelNumber (txChHnd));
        return -1;
    }

    /* Enable receive channel */
    if (Cppi_channelEnable (rxChHnd) != CPPI_SOK)
    {
        System_printf ("Error Core %d : Enabling Rx channel : %d\n", coreNum, Cppi_getChannelNumber (rxChHnd));
        return -1;       
    }

    /* Fill in some data */
    for (i = 0; i < SIZE_DATA_BUFFER; i++) 
        dataBuff[i] = i;

    System_printf ("\nCore %d : Transmitting %d packets..........\n\n", coreNum, NUM_PACKETS);
    System_printf ("*************************************************************\n");  
    
    /* Send out 8 packets */
    for (i = 0; i < NUM_PACKETS; i++)
    {
        /* Get a free descriptor */
        if ((monoDescPtr = (Cppi_Desc *) Qmss_queuePop (txFreeQueHnd)) == NULL)
        {
            System_printf ("Error Core %d : Getting descriptor from Queue Number\n", coreNum, txFreeQueHnd);
            return -1;
        }

        /* Add data buffer */
        Cppi_setData (Cppi_DescType_MONOLITHIC, monoDescPtr, (UInt8 *) &dataBuff, SIZE_DATA_BUFFER);

        /* Set tag information */
        tag.destTagLo = 0;
        tag.destTagHi = 0;
        tag.srcTagLo = core;
        tag.srcTagHi = 0;

        Cppi_setTag (Cppi_DescType_MONOLITHIC, monoDescPtr, &tag);

        /* Set packet length */
        Cppi_setPacketLen (Cppi_DescType_MONOLITHIC, monoDescPtr, SIZE_DATA_BUFFER);

        /* Push descriptor to Tx queue */
        Qmss_queuePushDescSize (txQueHnd, (UInt32 *) monoDescPtr, MONOLITHIC_DESC_DATA_OFFSET);
    }
    
    return 0;
}

/**
 *  @b Description
 *  @n  
 *
 *      ISR function process the received packets by reading the accumulator list. It recycles the received BDs.
 *      Send the sync signal.
 *  @retval
 *      Not Applicable.
 */
Void hiPrioInterruptHandler (UInt32 eventId)
{
    UInt32          channel, index, temp, count;
    UInt32          *buf;
    Void            *desc;
    Qmss_Result     result;

    channel = (eventId - 48) * 4 + coreNum;

    System_printf ("Core %d : HIGH PRIORITY Rx ISR - channel %d\n", coreNum, channel);

    /* Process ISR. Read accumulator list */
    temp  = l2_global_address ((UInt32) hiPrioList);
    temp &= 0xf0ffffff;
    buf   = (UInt32 *) temp;
    count = buf[0];

    System_printf ("Core %d :              Received %d packets\n", coreNum, count);

    /* Recycle Rx BDs */
    for (index = 0; index < count; index++)
        Qmss_queuePushDesc (rxFreeQueHnd, (Void *) buf[index + 1]);

    /* Set the EOI to indicate host is done processing. The page is freed by host to accumulator.
     * Accumulator can start writing to the freed page.
     */
    Qmss_ackInterrupt (channel, 1);
    Qmss_setEoiVector (Qmss_IntdInterruptType_HIGH, channel);

    /* Disable accumulator */
    if ((result = Qmss_disableAccumulator (Qmss_PdspId_PDSP1, channel)) != QMSS_ACC_SOK)
	{
        System_printf ("Error Core %d : Disabling high priority accumulator for channel : %d error code: %d\n",
                        coreNum, channel, result);
		return;
	}

    /* Clear the accumulator list after processing */
    //memset ((Void *) &hiPrioList, 0, sizeof (hiPrioList));

    System_printf ("Core %d :              Sending SYNC signal\n", coreNum);    
    for (index = 0; index < NUMBER_OF_CORES; index++)
    {
        /* Send the sync signal */
        if ((desc = Qmss_queuePop (syncFreeQueHnd)) != NULL)
        {
            Qmss_queuePushDesc (syncQueHnd, desc);
        }
        else
            System_printf ("Error Core %d : No SYNC descriptor!!!\n", coreNum);  
    }
#ifdef L2_CACHE
    CACHE_invL2 ((void *) &runCount, 4, CACHE_WAIT);
#else
    CACHE_invL1d ((void *) &runCount, 4, CACHE_WAIT);
#endif
    runCount++;

#ifdef L2_CACHE
    /* writeback L2 */
    CACHE_wbL2 ((void *) &runCount, 4, CACHE_WAIT);
#else
    /* Writeback L1D */
    CACHE_wbL1d ((void *) &runCount, 4, CACHE_WAIT);
#endif
}

/**
 *  @b Description
 *  @n  
 *      The functions initializes the INTC module.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static Int32 intcInit (Void)
{
    CSL_IntcGlobalEnableState   state;

    /* INTC module initialization */
    context.eventhandlerRecord = Record;
    context.numEvtEntries      = 2;
    if (CSL_intcInit (&context) != CSL_SOK) 
        return -1;

    /* Enable NMIs */
    if (CSL_intcGlobalNmiEnable () != CSL_SOK) 
        return -1;
 
    /* Enable global interrupts */
    if (CSL_intcGlobalEnable (&state) != CSL_SOK) 
        return -1;

    /* INTC has been initialized successfully. */
    return 0;
}

/**
 *  @b Description
 *  @n  
 *
 *      Function registers the high priority interrupt.
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */

static Int32 registerHiInterrupt (UInt32 coreNum, UInt32 qNum)
{
    CSL_IntcParam       vectId;
    Int16               channel, eventId;

    if (intcInit () < 0)
    {
        System_printf ("Error Core %d : Initializing interrupts\n", coreNum);
        return -1;           
    }

    channel = qNum - QMSS_HIGH_PRIORITY_QUEUE_BASE;
    if (channel < 0)
    {
        System_printf ("Invalid High Priority queue : %d\n", qNum);
        return -1;
    }

    if (coreNum != (channel % 4))
    {
        System_printf ("Invalid High Priority queue : %d and core number : %d combination\n", qNum, coreNum);
        return -1;
    }

    eventId = (channel / 4) + 48;

    System_printf ("Core %d : Registering High Priority interrupt channel : %d eventId : %d queue Number : %d\n", 
                    coreNum, channel, eventId, qNum);

    /* Open INTC */
    vectId = CSL_INTC_VECTID_10;
    hiPrioIntcHnd = CSL_intcOpen (&hiPrioIntcObj, eventId, &vectId, NULL);
    if (hiPrioIntcHnd == NULL)
        return -1;
 
    /* Bind ISR to Interrupt */
    Record[0].handler = (CSL_IntcEventHandler) &hiPrioInterruptHandler;
    Record[0].arg     = (Void *) eventId;
    CSL_intcPlugEventHandler (hiPrioIntcHnd, &Record[0]);

    /* Event Clear */
    CSL_intcHwControl (hiPrioIntcHnd, CSL_INTC_CMD_EVTCLEAR, NULL);

    /* Event Enable */
    CSL_intcHwControl (hiPrioIntcHnd, CSL_INTC_CMD_EVTENABLE, NULL);
    
    return 0;
}


/**
 *  @b Description
 *  @n  
 *
 *      Entry point for example code.
 *      This is an example code that shows producer core sending data to consumer 
 *      core. Synchronization between different cores.
 *      
 *  @retval
 *      Not Applicable
 */
Void main (Void)
{
    UInt32              channel, index;
    volatile UInt32     count;
    Cppi_Desc           *desc;

    System_printf ("**************************************************\n");
    System_printf ("************ QMSS Multicore Example **************\n");
    System_printf ("**************************************************\n");
    
    /* Get the core number. */
	coreNum = CSL_chipReadReg (CSL_CHIP_DNUM); 
    
    /* Core 0 is treated as the producer core that 
     * Initializes the system
     * setups the configuration
     * Pushes the data
     * De-initializes the system 
     */

    /* Initializes the system */
    if (coreNum == SYSINIT)
    {
        if (sysInit () < 0)
        {
            System_printf ("Error Core %d : Initializing QMSS\n", coreNum);
            return;           
        }
    }
    else
    {
        /* Get the handle for common queues on consumer cores */
        getsysHandles ();
    }
    for (channel = 0; channel < NUM_ITERATION; channel += NUMBER_OF_CORES)
    {
        /* Hookup interrupts */
        if (registerHiInterrupt (coreNum, QMSS_HIGH_PRIORITY_QUEUE_BASE + channel + coreNum) < 0)
        {
            System_printf ("Error Core %d : Registering interrupts\n", coreNum);
            return;           
        }

        /* Sync up all the cores after configuration is completed */
        if (coreNum == SYSINIT)
        {
            count = Qmss_getQueueEntryCount (syncCfgQueHnd);
            while (count != NUMBER_OF_CORES - 1)
                count = Qmss_getQueueEntryCount (syncCfgQueHnd);
        }
        else
        {
            if ((desc = (Cppi_Desc *) Qmss_queuePop (syncFreeQueHnd)) != NULL)
            {
                /* Push descriptor to sync free queue */
                Qmss_queuePushDesc (syncCfgQueHnd, (UInt32 *) desc);
            }
        }

        for (index = 0; index < NUMBER_OF_CORES; index++)
        {
            /* Run test on Core 0 */
            if (coreNum == SYSINIT)
            {
                if (send_data (channel + index, index) < 0)
                {
                    System_printf ("Error Core %d : Sending data on channel %d core\n", coreNum, channel + index, index);
                    return;           
                }
            }
            else
                System_printf ("*************************************************************\n"); 

            if (index != coreNum)            
                System_printf ("Core %d : Waiting for sync signal from core %d\n", coreNum, index);

            count = Qmss_getQueueEntryCount (syncQueHnd);
            while (count == 0)
                count = Qmss_getQueueEntryCount (syncQueHnd);
        
            if (index != coreNum)
                System_printf ("Core %d : Got sync signal\n", coreNum);
            
            System_printf ("*************************************************************\n\n");  
                
            if (coreNum == SYSINIT)
                printQueueStats("Before packet processing");

            /* Recycle the sync descriptors */
            if ((desc = (Cppi_Desc *) Qmss_queuePop (syncQueHnd)) != NULL)
            {
                /* Push descriptor to sync free queue */
                Qmss_queuePushDesc (syncFreeQueHnd, (UInt32 *) desc);
            }
            
            if (coreNum == SYSINIT)            
            {
                /* Recycle the Tx descriptors from Tx completion queue to Tx free queue */
                Qmss_queueDivert (txCmplQueHnd, txFreeQueHnd, Qmss_Location_TAIL);

                /* Close Rx/Tx queues, channels and flows used in this data transfer */
                cleanup ();
            }

            if (coreNum == SYSINIT)
                printQueueStats ("After packet processing");
        }
    }
    
    do
    {
#ifdef L2_CACHE
        CACHE_invL2 ((void *) &runCount, 4, CACHE_WAIT);
#else
        CACHE_invL1d ((void *) &runCount, 4, CACHE_WAIT);
#endif
    }while (runCount != NUMBER_OF_CORES);

    /* De-initializes the system */
    if (coreNum == SYSINIT)
        sysExit ();

    System_printf ("*******************************************************\n");
    System_printf ("************** QMSS Multicore Example Done ************\n");
    System_printf ("*******************************************************\n");

}
