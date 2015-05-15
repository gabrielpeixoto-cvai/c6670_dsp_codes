/*
 *  ======== main.c ========
 */
#include <stdio.h>
#include <stdlib.h>

#include <xdc/std.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>

/* PSC CSL definitions */
#include <ti/csl/csl_psc.h>
#include <ti/csl/csl_pscAux.h>
#include <ti/csl/csl_chip.h>

/*
 *  ======== taskFxn ========
 */
Void taskFxn(UArg a0, UArg a1)
{
	Uint32 coreNum = 0;
	coreNum = CSL_chipReadReg (CSL_CHIP_DNUM);
    printf("Hello Dave! This is core %d\n", coreNum);

    printf("exit taskFxn()\n");
}

/*
 *  ======== main ========
 */
Int main()
{ 
    Task_Handle task;
    Error_Block eb;

    System_printf("enter main()\n");

    Error_init(&eb);
    task = Task_create(taskFxn, NULL, &eb);
    if (task == NULL) {
        System_printf("Task_create() failed!\n");
        BIOS_exit(0);
    }

    BIOS_start();    /* does not return */
    return(0);
}
