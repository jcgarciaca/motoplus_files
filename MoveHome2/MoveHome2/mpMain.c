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
		//puts("Envia semaforo");
		mpTaskDelay(2500);
	}
}

void mpTask2(void)
{
	//TODO: Add the code for this task
	LONG respuesta, rData;
	MP_VAR_INFO sData;
	STATUS status;
	int i;

	MP_PMOVJ_SEND_DATA posiciones;
	MP_STD_RSP_DATA rspData;

	sData.usIndex = 0;
	sData.usType = MP_RESTYPE_VAR_B;

	// posiciones de Home
	posiciones.sCtrlGrp = 0;
	posiciones.lSpeed = 2000;
	posiciones.sToolNo = 0;

	for (i = 0; i < MAX_NO_OF_AXES; i++) {
		posiciones.lPos[i] = 0;
	}

	while (1) {
		status = mpSemTake(semid, WAIT_FOREVER);
		if (status == 0) { // recibe el semaforo
			respuesta = mpGetVarData(&sData, &rData, 1); // leer variable B000
			if (respuesta == 0) { // lectura correcta
				if (rData == 1) { // B000 = 1
					puts("Mueve robot a Home");
					mpPulseMOVJ(&posiciones, &rspData);
				}
			}
		}
	}
}
