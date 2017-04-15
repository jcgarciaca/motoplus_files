//mpMain.c
//
//This contains mpUsrRoot which is the entry point for your MotoPlus application

//ADDITIONAL INCLUDE FILES 
//(Note that motoPlus.h should be included in every source file)
#include "motoPlus.h"

//GLOBAL DATA DEFINITIONS
int nTaskID1;
int nTaskID2;
SEM_ID semid;

//FUNCTION PROTOTYPES
void mpTask1(void);
void mpTask2(void);
void print_position(MP_CART_POS_RSP_DATA rData);

//FUNCTION DEFINITIONS
void mpUsrRoot(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10)
{	
	//TODO: Add additional intialization routines.

	//Creates and starts a new task in a seperate thread of execution.
	//All arguments will be passed to the new task if the function
	//prototype will accept them.
	nTaskID1 = mpCreateTask(MP_PRI_TIME_NORMAL, MP_STACK_SIZE, (FUNCPTR)mpTask1,
						arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
	nTaskID2 = mpCreateTask(MP_PRI_TIME_NORMAL, MP_STACK_SIZE, (FUNCPTR)mpTask2,
						arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
	semid = mpSemBCreate(SEM_Q_FIFO, SEM_EMPTY);
	//Ends the initialization task.
	mpExitUsrRoot;
}

void mpTask1(void)
{
	//TODO: Add the code for this task
	while (1) {
		mpSemGive(semid);
		mpTaskDelay(2000);
	}
}

void mpTask2(void)
{
	//TODO: Add the code for this task
	STATUS status;
	LONG respuesta;

	MP_CTRL_GRP_SEND_DATA sData;
	MP_CART_POS_RSP_DATA rData;

	sData.sCtrlGrp = 0;

	while (1) {
		status = mpSemTake(semid, WAIT_FOREVER);
		if (status == 0) { // recibe semaforo
			respuesta = mpGetCartPos(&sData, &rData);
			if (respuesta == 0) { // captura posicion actual
				print_position(rData);
			}
		}		
	}
}

void print_position(MP_CART_POS_RSP_DATA rData)
{
	printf("x: %d\n", rData.lPos[0]);
	printf("y: %d\n", rData.lPos[1]);
	printf("z: %d\n", rData.lPos[2]);
	printf("Rx: %d\n", rData.lPos[3]);
	printf("Ry: %d\n", rData.lPos[4]);
	printf("Rz: %d\n\n", rData.lPos[5]);
}
