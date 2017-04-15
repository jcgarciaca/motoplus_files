#include "motoPlus.h"

#define	CYCLETIME  	50			//[msec]
#define	RTC_TIME  	1			//[msec]

extern SEM_ID semid;

STATUS GetBVar(UINT16 index, long *value);

void mpTask1(void) {
	int run_cnt, cycletime;
	LONG rt, BValue;
	
	puts("Activate mpTask1...");

	run_cnt = 0;
	cycletime = 0;

	while (1) {
		run_cnt++;
		mpSemGive(semid);

		rt = GetBVar(1, &BValue);
		if (BValue == 0) {
			cycletime = CYCLETIME / RTC_TIME;
		}
		else {
			cycletime = BValue * 10;
		}
		mpTaskDelay(cycletime);
	}
}

void mpTask2(void) {
	int run_cnt, case1state, turn;
	STATUS status;
	LONG rt, BVvalue;

	puts("Activate mpTask2...");

	run_cnt = 0;
	BVvalue = 0;
	case1state = 0;

	while (1) {
		run_cnt++;
		status = mpSemTake(semid, WAIT_FOREVER);
		if (status == ERROR) {
			printf("semTake Error![%d]\n", run_cnt);
		}

		rt = GetBVar(0, &BVvalue);

		if (BVvalue != 1) case1state = 0;

		switch (BVvalue)
		{
		case 0:
			turn = 0;
			break;

		case 1:
			if (case1state == 0) {

			}
		default:
			break;
		}
	}
}
