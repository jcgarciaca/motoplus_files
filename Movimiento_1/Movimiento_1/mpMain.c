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
	LONG ulPosdata[9];
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
	LONG resp;
	MP_VAR_INFO sData;
	LONG rData;

	LONG cycletime;

	sData.usType = MP_RESTYPE_VAR_B;
	sData.usIndex = 1;

	while (1) {
		mpSemGive(semid);
		resp = mpGetVarData(&sData, &rData, 1);
		if (rData == 0)
			cycletime = 50;
		else
			cycletime = rData * 10;

		mpTaskDelay(cycletime);
	}
}

void mpTask2(void)
{
	//TODO: Add the code for this task
	MP_POSVAR_DATA2 mp_posvar_data;
	STATUS status;
	MP_VAR_INFO sData;
	LONG rData;
	LONG resp;
	int i;

	MP_CTRL_GRP_SEND_DATA snd_data;
	MP_CART_POS_RSP_DATA cart_resp_data;

	MP_IMOV_SEND_DATA mp_imov_send_data;
	MP_STD_RSP_DATA mp_std_rsp_data;

	sData.usType = MP_RESTYPE_VAR_B;
	sData.usIndex = 0;

	snd_data.sCtrlGrp = 0; // R1

	// IMOV
	mp_imov_send_data.sCtrlGrp = 0;
	mp_imov_send_data.lSpeed = 500;
	mp_imov_send_data.sVType = 0;
	mp_imov_send_data.sFrame = 1; // Robot coordinate
	mp_imov_send_data.sToolNo = 0;

	for (i = 0; i < MAX_NO_OF_AXES; i++) {
		mp_imov_send_data.lPos[i] = 0;
	}

	while (1) {
		status = mpSemTake(semid, WAIT_FOREVER);
		if (status == OK) {
			resp = mpGetCartPos(&snd_data, &cart_resp_data);
			resp = mpGetVarData(&sData, &rData, 1);
			if (rData == 1) { // incremental move
				mp_imov_send_data.lPos[0] = 10000;
				mpIMOV(&mp_imov_send_data, &mp_std_rsp_data);
			}
			else
				puts("ERROR !!");
		}
	}
}


/*
memset(&mp_posvar_data.ulPosdata[0], 0, (sizeof(long) * 9));
memcpy(&mp_posvar_data.ulPosdata[0], &cart_resp_data.lPos[0], (sizeof(long) * 6));
mp_posvar_data.config = (unsigned char)(cart_resp_data.sConfig);
mp_posvar_data.ToolNo_UserNo = 0;
mp_posvar_data.usType = MP_RESTYPE_VAR_ROBOT;
mp_posvar_data.usIndex = 10;
mp_posvar_data.VariableType = 16;
*/