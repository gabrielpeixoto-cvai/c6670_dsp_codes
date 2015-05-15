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

/**
 * cannot access shared resource, why?
 *
 */


/**
 * TODO use semaphore for shared resource?
 */

Void task0(UArg a0, UArg a1){
	int c1=1;
	printf("a1=%d\n",c1);
}
Void task1(UArg a0, UArg a1){
	int c2=1;
	printf("a2=%d\n",c2);
}
Void task2(UArg a0, UArg a1){
	int c3=1;
	printf("a3=%d\n",c3);
}
Void task3(UArg a0, UArg a1){
	int c4=1;
	printf("a4=%d\n",c4);
}

/*
 *  ======== taskFxn ========
 */
Void taskMst(UArg a0, UArg a1)
{
	Uint32 coreNum = 0;
	coreNum = CSL_chipReadReg (CSL_CHIP_DNUM);

	Task_Handle tsk;
	if(coreNum==0){
		tsk = Task_create(task0, NULL, NULL);
		if (tsk == NULL) {
			System_printf("Task_create() failed!\n");
		    BIOS_exit(0);
		}
	}else if(coreNum==1){
		tsk = Task_create(task1, NULL, NULL);
				if (tsk == NULL) {
					System_printf("Task_create() failed!\n");
				    BIOS_exit(0);
				}
	}else if(coreNum==2){
		tsk = Task_create(task2, NULL, NULL);
				if (tsk == NULL) {
					System_printf("Task_create() failed!\n");
				    BIOS_exit(0);
				}
	}else if(coreNum==3){
		tsk = Task_create(task3, NULL, NULL);
				if (tsk == NULL) {
					System_printf("Task_create() failed!\n");
				    BIOS_exit(0);
				}

	}

}

/*
 *  ======== main ========
 */
Int main()
{ 
    Task_Handle task;
    Error_Block eb;

    printf("enter main()\n");

    Error_init(&eb);
    task = Task_create(taskMst, NULL, &eb);
    if (task == NULL) {
        System_printf("Task_create() failed!\n");
        BIOS_exit(0);
    }

    BIOS_start();    /* does not return */
    return(0);
}
