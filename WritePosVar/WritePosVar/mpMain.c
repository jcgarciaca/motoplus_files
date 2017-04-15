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

typedef struct
{
	USHORT usType;
	USHORT usIndex;
	CHAR VariableType;
	CHAR config;
	USHORT ToolNo_UserNo;
	LONG ulPosdata[10];
} MP_POSVAR_DATA2;

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
		mpTaskDelay(5000);
	}
}

void mpTask2(void)
{
	STATUS status;
	LONG respuesta;

	LONG rData;

	MP_VAR_INFO info;
	MP_POSVAR_DATA2 sData;

	MP_POSVAR_DATA mty;

	info.usType = MP_RESTYPE_VAR_B;
	info.usIndex = 0;

	
	
	//TODO: Add the code for this task
	while (1) {
		status = mpSemTake(semid, WAIT_FOREVER);
		if (status == 0) { // recibe semaforo
			respuesta = mpGetVarData(&info, &rData, 1);
			if (respuesta == 0) { // lectura BYTE correcta
				if (rData == 1) { // B000 = 1: Guarda PULSE
					sData.usType = MP_RESTYPE_VAR_ROBOT;
					sData.usIndex = 100;
					sData.VariableType = 0;
					sData.ToolNo_UserNo = 1;
					sData.ulPosdata[1] = 500;
					sData.ulPosdata[2] = 300;
					sData.ulPosdata[3] = 200;
					sData.ulPosdata[4] = 180;
					sData.ulPosdata[5] = 0;
					sData.ulPosdata[6] = 0;
					mpPutPosVarData(&sData, 1);
					puts("Completo");
				}
				else if (rData == 2) { // B000 = 2: Guarda ROBOT
					sData.usType = MP_RESTYPE_VAR_ROBOT;
					sData.usIndex = 100;
					sData.VariableType = 17;
					sData.ToolNo_UserNo = 1;
					sData.ulPosdata[1] = 500;
					sData.ulPosdata[2] = 300;
					sData.ulPosdata[3] = 200;
					sData.ulPosdata[4] = 180;
					sData.ulPosdata[5] = 0;
					sData.ulPosdata[6] = 0;
					mpPutPosVarData(&sData, 1);
					puts("Completo");
				}
				else if (rData == 3) { // B000 = 3: Guarda BASE
					sData.usType = MP_RESTYPE_VAR_ROBOT;
					sData.usIndex = 100;
					sData.VariableType = 16;
					sData.ToolNo_UserNo = 1;
					sData.ulPosdata[1] = 500;
					sData.ulPosdata[2] = 300;
					sData.ulPosdata[3] = 200;
					sData.ulPosdata[4] = 180;
					sData.ulPosdata[5] = 0;
					sData.ulPosdata[6] = 0;
					mpPutPosVarData(&sData, 1);
					puts("Completo");
				}
			}
		}
	}
}
