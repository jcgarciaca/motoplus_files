//mpMain.c
//
//This contains mpUsrRoot which is the entry point for your MotoPlus application

//ADDITIONAL INCLUDE FILES 
//(Note that motoPlus.h should be included in every source file)
#include "motoPlus.h"

//GLOBAL DATA DEFINITIONS
int nTaskID1;
int nTaskID2;

//FUNCTION PROTOTYPES
void mpTask1(void);
int skillCommandReceive(SYS2MP_SENS_MSG *msg);
//void mpTask2(int arg1, int arg2);

//FUNCTION DEFINITIONS
void mpUsrRoot(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10)
{	
	//TODO: Add additional intialization routines.

	//Creates and starts a new task in a seperate thread of execution.
	//All arguments will be passed to the new task if the function
	//prototype will accept them.
	nTaskID1 = mpCreateTask(MP_PRI_TIME_NORMAL, MP_STACK_SIZE, (FUNCPTR)mpTask1,
						arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
//	nTaskID2 = mpCreateTask(MP_PRI_TIME_NORMAL, MP_STACK_SIZE, (FUNCPTR)mpTask2,
//						arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
	
	//Ends the initialization task.
	mpExitUsrRoot;
}

void mpTask1(void)
{
	//TODO: Add the code for this task
	SYS2MP_SENS_MSG msg;
	int i;
	memset(&msg, CLEAR, sizeof(SYS2MP_SENS_MSG));
	
	while (1) {
		skillCommandReceive(&msg);
		
		switch (msg.main_comm)
		{
		case MP_SKILL_COMM:
			switch (msg.sub_comm)
			{
			case MP_SKILL_SEND:
				puts("SKILL Send");
				for (i = 0; i < 10; i++)
					printf("car[%d]: %d\n", i, msg.cmd[i]);
				break;
			case MP_SKILL_END:
				puts("SKILL End");
				break;
			default:
				puts("Default");
				break;
			}
			break;
		default:
			break;
		}
	}
}


int skillCommandReceive(SYS2MP_SENS_MSG *msg)
{
	mpEndSkillCommandProcess(MP_SL_ID1, msg);

	return (mpReceiveSkillCommand(MP_SL_ID1, msg));
}

/*
void mpTask2(int arg1, int arg2)
{
	//TODO: Add the code for this task
}
*/