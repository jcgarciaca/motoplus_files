#include "motoPlus.h"

extern void mpTask1(void);
void ap_TCP_Sserver(ULONG portNo);

#define PORT 11000
#define BUFF_MAX 1023

void *moveRobot(char *string);

void servosON(void);
void servosOFF(void);
void convAngleToPulse(int grp_no, LONG angles[], LONG pulses[]);

void mpTask1(void) 
{
	puts("Activa mpTask1...");
	ap_TCP_Sserver(PORT);
}

void ap_TCP_Sserver(ULONG portNo) 
{
	int sockHandle, rc;
	struct  sockaddr_in serverSockAddr;
	printf("Simple TCP server\n");

	sockHandle = mpSocket(AF_INET, SOCK_STREAM, 0);
	
	if (sockHandle < 0)
		return;

	memset(&serverSockAddr, 0, sizeof(serverSockAddr));
	serverSockAddr.sin_family = AF_INET;
	serverSockAddr.sin_addr.s_addr = INADDR_ANY;
	serverSockAddr.sin_port = mpHtons(portNo);

	rc = mpBind(sockHandle, (struct sockaddr *)&serverSockAddr, sizeof(serverSockAddr));
	if (rc < 0)
		goto closeSockHandle;

	rc = mpListen(sockHandle, SOMAXCONN);
	if (rc < 0)
		goto closeSockHandle;

	while (1)
	{
		int     acceptHandle;
		struct  sockaddr_in     clientSockAddr;
		int     sizeofSockAddr;

		memset(&clientSockAddr, 0, sizeof(clientSockAddr));
		sizeofSockAddr = sizeof(clientSockAddr);

		acceptHandle = mpAccept(sockHandle, (struct sockaddr *)&clientSockAddr, &sizeofSockAddr);

		if (acceptHandle < 0)
			break;

		while (1)
		{
			int     bytesRecv;
			int     bytesSend;
			char    buff[BUFF_MAX + 1];

			memset(buff, 0, sizeof(buff));

			bytesRecv = mpRecv(acceptHandle, buff, BUFF_MAX, 0);

			if (bytesRecv < 0)
				break;

			if (strncmp(buff, "SRVON", 5) == 0)
				servosON();
			else if (strncmp(buff, "SRVOFF", 6) == 0)
				servosOFF();
			else if (strncmp(buff, "EXIT", 4) == 0)
				;//break;
			else
				moveRobot(buff);
			
			bytesSend = mpSend(acceptHandle, buff, bytesRecv, 0);
		}
		mpClose(acceptHandle);
	}
closeSockHandle:
	mpClose(sockHandle);

	return;
}


void *moveRobot(char *string)
{
	LONG angles[MP_GRP_AXES_NUM], pulses[MP_GRP_AXES_NUM];
	char *token;
	int index, i, rc;

	int grp_no;

	MP_PMOVJ_SEND_DATA sData;
	MP_STD_RSP_DATA rData;

	for (i = 0; i < MP_GRP_AXES_NUM; i++) {
		angles[i] = 0;
		pulses[i] = 0;
	}

	sData.sCtrlGrp = 0; // R1
	sData.lSpeed = 2000;
	sData.sToolNo = 0;

	// get the first token
	token = strtok(string, ", ");
	index = 0;

	grp_no = 0;

	// walk through other tokens
	while (token != NULL) {
		angles[index] = atoi(token);
		token = strtok(NULL, ", ");
		index++;
	}

	if (index == 6) {
		// mueve robot
		printf("Mueve el robot\n");
		for (i = 0; i < index; i++) {
			printf("Eje: %d\n", angles[i]);
		}
		convAngleToPulse(grp_no, angles, pulses);
		//sData.lPos = pulses;
		//memcpy(&sData.lPos[0], &pulses[0], MP_GRP_AXES_NUM);
		for (i = 0; i < MAX_NO_OF_AXES; i++) {
			sData.lPos[i] = pulses[i];
			printf("Pls: %d\n", sData.lPos[i]);
		}
		mpPulseMOVJ(&sData, &rData);
	}
	else {
		printf("Error en datos!!\n");
	}
}

void servosON(void) 
{
	MP_SERVO_POWER_SEND_DATA sData;
	MP_STD_RSP_DATA rData;

	sData.sServoPower = 1;

	mpSetServoPower(&sData, &rData);
}

void servosOFF(void)
{
	MP_SERVO_POWER_SEND_DATA sData;
	MP_STD_RSP_DATA rData;

	sData.sServoPower = 0;

	mpSetServoPower(&sData, &rData);
}

void convAngleToPulse(int grp_no, LONG angles[], LONG pulses[]) 
{
	//LONG sFactor, lFactor, uFactor, rFactor, bFactor, tFactor;
	int MAX_INDEX, i;
	LONG convFactors[6] = { 1435, 1300, 1377, 996, 824, 350 };

	MAX_INDEX = 6;

	for (i = 0; i < MAX_INDEX; i++) {
		pulses[i] = angles[i] * convFactors[i];
	}
}