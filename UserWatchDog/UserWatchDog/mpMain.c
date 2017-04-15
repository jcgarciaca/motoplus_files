//mpMain.c
//
//This contains mpUsrRoot which is the entry point for your MotoPlus application

//ADDITIONAL INCLUDE FILES 
//(Note that motoPlus.h should be included in every source file)
#include "motoPlus.h"
#include "funclib.h"

#define debugPrint ON
#define DVAL_NUM 20

//GLOBAL DATA DEFINITIONS
int nTaskID1;
int nTaskID2;
SEM_ID semid;
LONG DVvalue[DVAL_NUM];
int delay[2];
long clearCNT[2];


MP_USR_WDG_ROUTINE wdRoutine[2];
MP_WDG_HANDLE handle[2];

//FUNCTION PROTOTYPES
void mp_seg_mon_task(void);
void mp_para_get_task(void);
void fncwRoutine0(MP_WDG_HANDLE);
void fncwRoutine1(MP_WDG_HANDLE);
void getParameter(void);

//FUNCTION DEFINITIONS
void mpUsrRoot(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10)
{	
	//TODO: Add additional intialization routines.

	//Creates and starts a new task in a seperate thread of execution.
	//All arguments will be passed to the new task if the function
	//prototype will accept them.
	mpTaskDelay(10000);
	semid = mpSemBCreate(SEM_Q_FIFO, SEM_EMPTY);

#ifdef debugPrint == ON
	printf("mpCreateSemid semid: %p\n", semid);
#endif


	nTaskID1 = mpCreateTask(MP_PRI_IP_CLK_TAKE, MP_STACK_SIZE, (FUNCPTR)mp_seg_mon_task,
						arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
	nTaskID2 = mpCreateTask(MP_PRI_TIME_NORMAL, MP_STACK_SIZE, (FUNCPTR)mp_para_get_task,
						arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
	
	//Ends the initialization task.
	mpExitUsrRoot;
}

void mp_para_get_task(void)
{
	//TODO: Add the code for this task
}

void mp_seg_mon_task(void)
{
	int rc, i;

	//TODO: Add the code for this task
#ifdef debugPrint == ON
	puts("Activate mp_seg_mon_task");
#endif

	wdRoutine[0] = (MP_USR_WDG_ROUTINE)fncwRoutine0;
	wdRoutine[1] = (MP_USR_WDG_ROUTINE)fncwRoutine1;

#ifdef debugPrint == ON
	printf("fnc ptr!! [%d] [%d]\n", wdRoutine[0], wdRoutine[1]);
#endif // debugPrint == ON

	getParameter();

	FOREVER{
		if (mpClkAnnounce(MP_INTERPOLATION_CLK) == ERROR) {
#ifdef debugPrint == ON
			puts("Err: RTP IP_CLK ANNOUNCE");
#endif // debugPrint == ON
			mpTaskSuspend(nTaskID1);
		}

		for (i = 0; i < 2; i++) {
			// D061 D062

		}


	}
}

void fncwRoutine0(MP_WDG_HANDLE lhandle) {
	if (WriteIO(1000050, 50) != 0) {
		puts("Error!! WriteIO\n");
	} 
	else {
		printf("wdRoutine0!! (WriteIO_0) [handle = %d]\n", lhandle);
	}
}

void fncwRoutine1(MP_WDG_HANDLE lhandle) {
	if (WriteIO(1000051, 51) != 0) {
		puts("Error!! WriteIO\n");
	}
	else {
		printf("wdRoutine1!! (WriteIO_1) [hadnle = %d]\n", lhandle);
	}
}

void getParameter() {
	LONG	DVvalue050, DVvalue051, DVvalue052, DVvalue053;
	int index, rc;


	//Save DVar oldvalue
	DVvalue050 = DVvalue[0];	//WdogCreate
	DVvalue051 = DVvalue[1];	//WdogDelete
	DVvalue052 = DVvalue[2];	//WdogStart
	DVvalue053 = DVvalue[3];	//WdogClear

	if (mpGetMultiDVar(50, DVvalue, DVAL_NUM) == 0) {
		// get index value D054
		if (DVvalue[4] < 2) {
			index = (int)DVvalue[4];
		}
		else {
			index = 0;
		}

		// get delay value D055 D056
		delay[0] = DVvalue[5];
		delay[1] = DVvalue[6];

		// get clear counter value D058 D059
		clearCNT[0] = DVvalue[8];
		clearCNT[1] = DVvalue[9];

		// watchdog create
		if (DVvalue050 == 0 && DVvalue[0] == 1) {
			handle[index] = mpUsrWdogCreate(delay[index], wdRoutine[index]);
			printf("WdogCreate(%d)!! [delay = %d] [wdRoutine = %d] [handle = %d]\n",
				index, delay[index], wdRoutine[index], handle[index]);
		}

		// watchdog delete
		if (DVvalue051 == 0 && DVvalue[1] == 1) {
			rc = mpUsrWdogDelete(handle[index]);
			printf("WdogDelete(%d)!! [handle = %d] [rc = %d]\n",
				index, handle[index], rc);
		}

		// watchdog start
		if (DVvalue052 == 0 && DVvalue[2] == 1) {
			rc = mpUsrWdogStart(handle[index]);
			printf("WdogStart(%d)!! [handle = %d] [rc = %d]\n",
				index, handle[index], rc);
		}

		// watchdog clear
		if (DVvalue053 == 0 && DVvalue[3] == 1) {
			rc = mpUsrWdogClear(handle[index]);
			printf("WdogClear(%d)!! [handle = %d] [rc = %d]\n",
				index, handle[index], rc);
		}
	}
}
