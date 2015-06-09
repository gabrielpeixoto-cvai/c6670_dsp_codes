#include <stdio.h>
#include <stdlib.h>

#include <xdc/runtime/System.h>

/* IPC includes */
#include <ti/ipc/GateMP.h>
#include <ti/ipc/Ipc.h>
#include <ti/ipc/ListMP.h>
#include <ti/ipc/SharedRegion.h>
#include <ti/ipc/MultiProc.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/drv/qmss/qmss_firmware.h>
#include <ti/drv/cppi/cppi_drv.h>
#include <ti/drv/cppi/cppi_desc.h>

#include <ti/csl/csl_chip.h>
#include <ti/csl/csl_cacheAux.h>

//#include "cp_utils.h"

/* QMSS device specific configuration */
extern Qmss_GlobalConfigParams qmssGblCfgParams;
extern Cppi_GlobalConfigParams cppiGblCfgParams;

#define NUM_DESC 32
#define SIZE_DESC 128
#define MONOLITHIC_DESC_DATA_OFFSET 16

#define MULTICORE
#define QUEUE_TX_OP_NUM 800
#define QUEUE_TX_FREE_NUM 852
#define QUEUE_RX_OP_NUM 858
#define QUEUE_RX_FREE_NUM 859

#pragma DATA_ALIGN(mono_region, 16)
unsigned char host_mono_region[NUM_DESC * SIZE_DESC];

#pragma DATA_ALIGN(mono_region, 16)
unsigned char mono_region[NUM_DESC * SIZE_DESC];

#pragma DATA_SECTION(cppiHnd, ".cppi")
#pragma DATA_ALIGN(cppiHnd, 128)
Cppi_Handle cppiHnd = NULL;
Task_Params tp;

static UInt32 l2_global_address(UInt32 addr)
{
	/* Get the core number. */
	UInt32 coreNum = CSL_chipReadReg (CSL_CHIP_DNUM);
	/* Compute the global address. */
	return (addr + (0x10000000 + (coreNum * 0x1000000)));
}

Void myStartupFxn (Void)
{
    MultiProc_setLocalId(CSL_chipReadReg (CSL_CHIP_DNUM));
}

void generateData(Uint32 * vect, int lenght){
	int i;
	for(i=0; i<lenght; i++){
		vect[i]=(Uint32)i;
	}
}

void taskA(UArg a0, UArg a1) {
	Qmss_QueueHnd opq = (Qmss_QueueHnd) a0;
	Qmss_QueueHnd freeq = (Qmss_QueueHnd) a1;
	UInt32 symbol_buffer[SIZE_DESC/8];
	Cppi_Desc *desc;
	Cppi_DescTag tag;
	Qmss_Queue queInfo;

	queInfo = Qmss_getQueueNumber(freeq);

	do{
	// Wait for a free descriptor
	//breakpoint
	while ((desc = Qmss_queuePop(freeq)) == NULL)
		{}

	// Re-align descriptor address:
	desc = (Cppi_Desc *) ((UInt32) desc & ~0xf);
	// Generate time domain random symbol:
	//randomRealSymbol(symbol_buffer, SIZE_DESC/8);
	generateData(symbol_buffer, SIZE_DESC/8);

	// Cppi_setData - Copy data buffer to descriptor:
	Cppi_setData(Cppi_DescType_MONOLITHIC, desc, (UInt8 *) symbol_buffer, SIZE_DESC/2);

	// Set tag in descriptor:
	tag.destTagLo = 0;
	tag.destTagHi = 0;
	tag.srcTagLo = 0; /* Flow 0, the only one we created */
	tag.srcTagHi = 0;

	Cppi_setTag (Cppi_DescType_MONOLITHIC, desc, &tag);

	// Define where to recycle descriptor:
	Cppi_setReturnQueue(Cppi_DescType_MONOLITHIC, desc, queInfo);

	// Push descriptor to Tx operational queue:
	Qmss_queuePushDescSize(opq, desc, SIZE_DESC);
	Task_sleep(1000);
	}while(1);
	return;
}

void taskB(UArg a0, UArg a1) {
	Qmss_QueueHnd opq = (Qmss_QueueHnd) a0;
	Qmss_QueueHnd freeq = (Qmss_QueueHnd) a1;
	Cppi_Desc *desc = NULL;
	UInt8 * descBuffer;
	UInt32 destLen;
	UInt8 i;
	UInt32 sample;

	while ((desc = Qmss_queuePop(opq)) == NULL) { }

	// Retrieve payload from monolithic descriptors:
	//breakpoint
	Cppi_getData (Cppi_DescType_MONOLITHIC, desc, &descBuffer, &destLen);

	for (i=0; i < destLen; i = i + 4)
		{
		sample = descBuffer[i];
		printf("%d\n",sample);
		/*sample = (descBuffer[i]) |
				 (descBuffer[i + 1] << 8) |
				 (descBuffer[i + 2] << 16) |
				 (descBuffer[i + 3] << 24);
		printf ("Sample: 0x%x\n", sample);*/
	}

	printf("Got descriptor\n");

	// Recycle descriptor:
	Qmss_queuePushDesc(freeq, desc);

	return;
}

void master_task() {
	UInt32 core_num = CSL_chipReadReg(CSL_CHIP_DNUM);
	/* ---------------------------- Initialization of QMSS ------------------------- */
	int ret;
	Qmss_InitCfg qmssInitConfig;
	Cppi_CpDmaInitCfg cpdmaCfg;
	Qmss_MemRegInfo mem_reg;
	Cppi_DescCfg cppi_cfg;
	unsigned int num_allocated;
	unsigned char is_allocated;

	Qmss_QueueHnd q_tx_op, q_rx_op, q_tx_free, q_rx_free;


if(core_num==0){

	memset(&qmssInitConfig, 0, sizeof(Qmss_InitCfg));

	qmssInitConfig.maxDescNum = NUM_DESC;

#ifdef MULTICORE
	qmssInitConfig.maxDescNum += NUM_DESC;
#endif 
	/* Initialize Queue Manager Sub System */
	ret = Qmss_init(&qmssInitConfig, &qmssGblCfgParams);
	if (ret != QMSS_SOK) {
		printf("Error initializing Queue Manager SubSystem error code : %d\n",
				ret);
		return;
	}

	if (Qmss_start() != QMSS_SOK) {
		printf("Error starting QMSS.\n");
		return;
	}
#ifndef MULTICORE
	/* Initialize CPPI */
	ret = Cppi_init(&cppiGblCfgParams);
	if (ret != CPPI_SOK) {
		printf("Error initializing CPPI: %d\n", ret);
		return;
	}

	/* Set up QMSS CPDMA configuration */
	memset((Void *) &cpdmaCfg, 0, sizeof(Cppi_CpDmaInitCfg));
	cpdmaCfg.dmaNum = Cppi_CpDma_QMSS_CPDMA;

	/* Open QMSS CPDMA singlecore*/
	Cppi_Handle cppiHnd = (Cppi_Handle) Cppi_open(&cpdmaCfg);
	if (cppiHnd == NULL) {
		printf("Error opening CPPI\n");
		return;
	}
#endif
	/* Populate the mem_reg struct so that QMSS becomes
	 * aware of this 'memory region', i.e. a place in
	 * memory that will eventually hold descriptors
	 */
	mem_reg.descBase = (unsigned int *) l2_global_address((UInt32) mono_region);
	mem_reg.descSize = SIZE_DESC;
	mem_reg.descNum = NUM_DESC;
	mem_reg.manageDescFlag = Qmss_ManageDesc_MANAGE_DESCRIPTOR;
	mem_reg.memRegion = Qmss_MemRegion_MEMORY_REGION_NOT_SPECIFIED;
	mem_reg.startIndex = 0;

	ret = Qmss_insertMemoryRegion(&mem_reg);
	/* if ret is positive (or zero), then insertMemoryRegion has worked */

	/* Populate the cppi_cfg struct so that Cppi_initDescriptor
	 * can 'format' the memory region, thus actually creating the
	 * descriptors.
	 * Finally, it will put all the descriptors in destQueueNum.
	 */
	cppi_cfg.memRegion = (Qmss_MemRegion) ret;
	cppi_cfg.descNum = NUM_DESC;
	cppi_cfg.destQueueNum = QUEUE_TX_FREE_NUM;
	cppi_cfg.queueType = Qmss_QueueType_GENERAL_PURPOSE_QUEUE;
	cppi_cfg.initDesc = Cppi_InitDesc_INIT_DESCRIPTOR;
	cppi_cfg.descType = Cppi_DescType_MONOLITHIC;
	cppi_cfg.cfg.mono.dataOffset = MONOLITHIC_DESC_DATA_OFFSET;
	q_tx_free = Cppi_initDescriptor(&cppi_cfg, &num_allocated);
#ifdef MULTICORE
	/* Populate the mem_reg struct so that QMSS becomes                     
         * aware of this 'memory region', i.e. a place in                       
         * memory that will eventually hold descriptors                         
         */
	/**
	*TODO ajeitar local de alocacao do DescBase
	*/                                                                     
        mem_reg.descBase = (unsigned int *) l2_global_address((UInt32) host_mono_region);
        mem_reg.descSize = SIZE_DESC;                                           
        mem_reg.descNum = NUM_DESC;                                             
        mem_reg.manageDescFlag = Qmss_ManageDesc_MANAGE_DESCRIPTOR;             
        mem_reg.memRegion = Qmss_MemRegion_MEMORY_REGION_NOT_SPECIFIED;         
        mem_reg.startIndex = 0;                                                 
                                                                                
        ret = Qmss_insertMemoryRegion(&mem_reg);                                
        /* if ret is positive (or zero), then insertMemoryRegion has worked */  
                                                                                
        /* Populate the cppi_cfg struct so that Cppi_initDescriptor             
         * can 'format' the memory region, thus actually creating the           
         * descriptors.                                                         
         * Finally, it will put all the descriptors in destQueueNum.            
         */                                                                     
        cppi_cfg.memRegion = (Qmss_MemRegion) ret;                              
        cppi_cfg.descNum = NUM_DESC;                                            
        cppi_cfg.destQueueNum = QUEUE_RX_FREE_NUM;                              
        cppi_cfg.queueType = Qmss_QueueType_GENERAL_PURPOSE_QUEUE;              
        cppi_cfg.initDesc = Cppi_InitDesc_INIT_DESCRIPTOR;                      
        cppi_cfg.descType = Cppi_DescType_MONOLITHIC;                           
        cppi_cfg.cfg.mono.dataOffset = MONOLITHIC_DESC_DATA_OFFSET;             
        q_rx_free = Cppi_initDescriptor(&cppi_cfg, &num_allocated);

	/* Initialize CPPI */                                           
        ret = Cppi_init(&cppiGblCfgParams);                             
        if (ret != CPPI_SOK) {                                          
        	printf("Error initializing CPPI: %d\n", ret);           
                return;                                                 
        }                                                               
                                                                                
        /* Set up QMSS CPDMA configuration */                           
        memset((Void *) &cpdmaCfg, 0, sizeof(Cppi_CpDmaInitCfg));       
        cpdmaCfg.dmaNum = Cppi_CpDma_QMSS_CPDMA;                        
                                                                                
        /* Open QMSS CPDMA */                                           
        Cppi_Handle hnd = (Cppi_Handle) Cppi_open(&cpdmaCfg);           
        if (hnd == NULL) {                                              
        	printf("Error opening CPPI\n");                         
        	return;                                                 
        }                                                               
        cppiHnd = hnd;                                                  
        CACHE_wbL1d ((void *) &cppiHnd, 128, CACHE_WAIT);

#endif
	/* ---------------------------- Setting up queues ------------------------- */
	int i;
	Cppi_Desc *c;

	/* Open the operation queue for the transmit side */
	q_tx_op = Qmss_queueOpen(
		Qmss_QueueType_GENERAL_PURPOSE_QUEUE,
		QUEUE_TX_OP_NUM,
		&is_allocated);

	/* Open the operation queue for the receive side */
	q_rx_op = Qmss_queueOpen(
		Qmss_QueueType_GENERAL_PURPOSE_QUEUE,
		QUEUE_RX_OP_NUM,
		&is_allocated);

	/* Open the free queue for the receive side */
	q_rx_free = Qmss_queueOpen(
		Qmss_QueueType_GENERAL_PURPOSE_QUEUE,
		QUEUE_RX_FREE_NUM,
		&is_allocated);
#ifdef MULTICORE
	/* Take half of the descriptors from the TX FDQ and put them in the RX FDQ */
	for (i=0; i<NUM_DESC/2; i++) {
		c = (Cppi_Desc*) Qmss_queuePop(q_tx_free);
		Qmss_queuePushDesc(q_rx_free, c);
	}
#endif
	/* ---------------------------- Configure PKTDMA ------------------------- */
#ifndef MULTICORE
	/* ------ Rx channel configuration --------- */
	//Cppi_RxChInitCfg rxChCfg;
	//rxChCfg.channelNum = CPPI_PARAM_NOT_SPECIFIED;
	//rxChCfg.rxEnable = Cppi_ChState_CHANNEL_DISABLE;

	//Cppi_ChHnd rxChHnd = (Cppi_ChHnd) Cppi_rxChannelOpen(cppiHnd, &rxChCfg, &is_allocated);
#endif
	/* ------ Tx channel configuration -------- */
	Cppi_TxChInitCfg txChCfg;
	txChCfg.channelNum = CPPI_PARAM_NOT_SPECIFIED;
	txChCfg.priority = 0;
	txChCfg.filterEPIB = 0;
	txChCfg.filterPS = 0;
	txChCfg.aifMonoMode = 0;
	txChCfg.txEnable = Cppi_ChState_CHANNEL_DISABLE;

	Cppi_ChHnd txChHnd = (Cppi_ChHnd) Cppi_txChannelOpen(cppiHnd, &txChCfg, &is_allocated);

	ret = Cppi_channelEnable(txChHnd);
#ifdef MULTICORE
}else{
#endif

 	/* ------ Rx channel configuration --------- */                         
        Cppi_RxChInitCfg rxChCfg;                                               
        rxChCfg.channelNum = CPPI_PARAM_NOT_SPECIFIED;                          
        rxChCfg.rxEnable = Cppi_ChState_CHANNEL_DISABLE;                        
                                                                                
        Cppi_ChHnd rxChHnd = (Cppi_ChHnd) Cppi_rxChannelOpen(cppiHnd, &rxChCfg, &is_allocated);
	
	/* ------ Rx flow configuration ------- */
	Cppi_RxFlowCfg rxFlowCfg;
	Qmss_Queue queInfo;
	memset((Void *) &rxFlowCfg, 0, sizeof (Cppi_RxFlowCfg));

	/* The 'deliver' part (where new information goes) */
	queInfo = Qmss_getQueueNumber(q_rx_op);
	rxFlowCfg.flowIdNum = CPPI_PARAM_NOT_SPECIFIED;
	rxFlowCfg.rx_dest_qnum = queInfo.qNum;
	rxFlowCfg.rx_dest_qmgr = queInfo.qMgr;
	rxFlowCfg.rx_sop_offset = MONOLITHIC_DESC_DATA_OFFSET;
	rxFlowCfg.rx_desc_type = Cppi_DescType_MONOLITHIC;
	/* The 'receive' part (where the free descriptors are) */
	queInfo = Qmss_getQueueNumber(q_rx_free);
	rxFlowCfg.rx_fdq0_sz0_qnum = queInfo.qNum;
	rxFlowCfg.rx_fdq0_sz0_qmgr = queInfo.qMgr;

	Cppi_FlowHnd rxFlowHnd = (Cppi_FlowHnd) Cppi_configureRxFlow(cppiHnd, &rxFlowCfg, &is_allocated);
	/* Should check if rxFlowHnd is NULL */

    if (rxFlowHnd == NULL)
    {
        printf ("Error: Opening Rx flow : %d\n", rxFlowCfg.flowIdNum);
        return;
    }
    else
        printf ("Opened Rx flow : %d\n",Cppi_getFlowId(rxFlowHnd));


	/* Enable the channels */

	ret = Cppi_channelEnable(rxChHnd);

}
	/* ---------------------------- Create Tasks ------------------------- */

	/* Create the operational tasks: one producer, one consumer.
	 * Let them know which queues we will be using.
	 */

	Task_Params_init(&tp);
	if (core_num == 0) {
		tp.arg0 = q_tx_op;
		tp.arg1 = q_tx_free;
		Task_create(taskA, &tp, NULL);

#ifdef MULTICORE
	}else{
#endif
		tp.arg0 = q_rx_op;
		tp.arg1 = q_rx_free;
		Task_create(taskB, &tp, NULL);
	}
}

void main() {
	Task_create((Task_FuncPtr) master_task, NULL, NULL);
	BIOS_start();
}



