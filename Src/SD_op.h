#ifndef __SD_OP_H__
#define __SD_OP_H__

#include "fatfs.h"
#include "string.h"
#include "stm32f1xx_hal.h"
#include "lcd.h"



struct student{
	char sdData[100];
	char sdBooker[20];
	char sdSID[9];
	char sdStartTime[9];
	char sdEndTime[9];
	char sdCID[9];
	char kpSID[9];
};

void sdfile_read(struct student users[],int n);
void sdfile_parse(char txt[],char sd_Booker[],char sd_SID[],char sd_CID[],char sd_StartTime[],char sd_EndTime[]);
int data_match(char sd_Booker[],char sd_SID[],char sd_StartTime[],char booker_read[], char sid_read[], char time_read[]);





#endif

