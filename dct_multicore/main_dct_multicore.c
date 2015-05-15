/*
 *  ======== main_dct_multicore.c ========
 */

// C includes
#include <stdio.h>
#include <math.h>

//CCS includes
#include <xdc/std.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>

/*CSL*/
// To access register and indentify core number
#include <ti/csl/csl_chip.h>

//DEFINES
#define SIZE 2

/*
 *Task definitions
 *
 */

Void task0(UArg arg0, UArg arg1);
Void task1(UArg arg0, UArg arg1);
Void task2(UArg arg0, UArg arg1);
Void task3(UArg arg0, UArg arg1);

/*
 *  ======== taskMstr ========
 *  Calls different taskts depending on which core we are working
 */
Void taskMstr(UArg a0, UArg a1){
	Uint32 coreNum;
	coreNum = CSL_chipReadReg(CSL_CHIP_DNUM);

	Task_Params tskpr;
	Task_Params_init(&tskpr);

	/*
	 * no need to add semaphores to remove concurrency possibility
	 */
	if(coreNum == 0){
		Task_create(task0, NULL, NULL);
	}else if(coreNum == 1){
		Task_create(task1, NULL, NULL);
	}else if(coreNum == 2){
		Task_create(task2, NULL, NULL);
	}else{
		Task_create(task3, NULL, NULL);
	}


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
    task = Task_create(taskMstr, NULL, &eb);
    if (task == NULL) {
        System_printf("Task_create() failed!\n");
        BIOS_exit(0);
    }

    BIOS_start();    /* does not return */
    return(0);
}

Void task0(UArg a0, UArg a1){

double dctcoeff[SIZE][SIZE];
double input[2][2]={{1,1},{1,1}};
double result[2][2];
int i,j;
double alpha;
double denominator;

/*
*calculate the coeffs
*/
for(i=0;i<SIZE;i++){
        dctcoeff[0][i]= sqrt(1/(double)SIZE);
}

/*
*calculate alpha and denominator
*/
alpha = sqrt(2/(double)SIZE);
denominator = (double)2*SIZE;

/*
*generate final coeffs
*/
for(i=0;i<SIZE;i++){
        for(j=0;j<0;j++){
                dctcoeff[i][j]= alpha * cos(((2*j+1)*i*3.14159)/denominator);
        }
}

/*
*multiply for the input
*/
for(i=0;i<SIZE;i++){
        for(j=0;j<SIZE;j++){
                result[i][j]=input[i][j]*dctcoeff[i][j];
        }
}

/*
*print result
*/
for(i=0;i<SIZE;i++){
	for(j=0;j<SIZE;j++){
		printf("[%d][%d]=%lf\n",i,j,result[i][j]);
	}
}

}

Void task1(UArg a0, UArg a1){

double dctcoeff[SIZE][SIZE];
double input[2][2]={{2,2},{2,2}};
double result[2][2];
int i,j;
double alpha;
double denominator;

/*
*calculate the coeffs
*/
for(i=0;i<SIZE;i++){
        dctcoeff[0][i]= sqrt(1/(double)SIZE);
}

/*
*calculate alpha and denominator
*/
alpha = sqrt(2/(double)SIZE);
denominator = (double)2*SIZE;

/*
*generate final coeffs
*/
for(i=0;i<SIZE;i++){
        for(j=0;j<0;j++){
                dctcoeff[i][j]= alpha * cos(((2*j+1)*i*3.14159)/denominator);
        }
}

/*
*multiply for the input
*/
for(i=0;i<SIZE;i++){
        for(j=0;j<SIZE;j++){
                result[i][j]=input[i][j]*dctcoeff[i][j];
        }
}

/*
*print result
*/
for(i=0;i<SIZE;i++){
	for(j=0;j<SIZE;j++){
		printf("[%d][%d]=%lf\n",i,j,result[i][j]);
	}
}

}

Void task2(UArg a0, UArg a1){

double dctcoeff[SIZE][SIZE];
double input[2][2]={{3,3},{3,3}};
double result[2][2];
int i,j;
double alpha;
double denominator;

/*
*calculate the coeffs
*/
for(i=0;i<SIZE;i++){
        dctcoeff[0][i]= sqrt(1/(double)SIZE);
}

/*
*calculate alpha and denominator
*/
alpha = sqrt(2/(double)SIZE);
denominator = (double)2*SIZE;

/*
*generate final coeffs
*/
for(i=0;i<SIZE;i++){
        for(j=0;j<0;j++){
                dctcoeff[i][j]= alpha * cos(((2*j+1)*i*3.14159)/denominator);
        }
}

/*
*multiply for the input
*/
for(i=0;i<SIZE;i++){
        for(j=0;j<SIZE;j++){
                result[i][j]=input[i][j]*dctcoeff[i][j];
        }
}

/*
*print result
*/
for(i=0;i<SIZE;i++){
	for(j=0;j<SIZE;j++){
		printf("[%d][%d]=%lf\n",i,j,result[i][j]);
	}
}

}

Void task3(UArg a0, UArg a1){

double dctcoeff[SIZE][SIZE];
double input[2][2]={{4,4},{4,4}};
double result[2][2];
int i,j;
double alpha;
double denominator;

/*
*calculate the coeffs
*/
for(i=0;i<SIZE;i++){
        dctcoeff[0][i]= sqrt(1/(double)SIZE);
}

/*
*calculate alpha and denominator
*/
alpha = sqrt(2/(double)SIZE);
denominator = (double)2*SIZE;

/*
*generate final coeffs
*/
for(i=0;i<SIZE;i++){
        for(j=0;j<0;j++){
                dctcoeff[i][j]= alpha * cos(((2*j+1)*i*3.14159)/denominator);
        }
}

/*
*multiply for the input
*/
for(i=0;i<SIZE;i++){
        for(j=0;j<SIZE;j++){
                result[i][j]=input[i][j]*dctcoeff[i][j];
        }
}

/*
*print result
*/
for(i=0;i<SIZE;i++){
	for(j=0;j<SIZE;j++){
		printf("[%d][%d]=%lf\n",i,j,result[i][j]);
	}
}

}
