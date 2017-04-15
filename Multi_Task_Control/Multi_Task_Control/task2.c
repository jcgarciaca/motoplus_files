#include "motoPlus.h"

#define DebugPrint ON;

extern void mpTask1(void);
extern void mpTask2(void);

extern SEM_ID semid;

STATUS status;
//int run_cnt;

void mpTask1(void) {
	
	puts("Activate mpTask1...");
	
	while (1) {
		mpSemGive(semid);
#ifdef DebugPrint == ON
		puts("Give the semaphore...");
#endif
		mpTaskDelay(2500);
	}
	
}

void mpTask2(void) {
	STATUS status;
	int run_cnt;
	puts("Activate mpTask2...");	
	
	run_cnt = 0;

	while (1) {
		run_cnt++;

		status = mpSemTake(semid, WAIT_FOREVER);
#ifdef DebugPrint == ON
		puts("Take the semaphore...");
#endif
		if (status == ERROR)
			printf("semTakeError! [%d]\n", run_cnt);
		else
			printf("mpTask2 Running! [%d]\n", run_cnt);
	}
	
}