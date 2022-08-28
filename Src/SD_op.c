#include "SD_op.h"

/*char* filename = "booker.txt";
uint32_t bytesread;
char sdData[50];
char sdBooker[20] = "";
char sdSID[9] = "";
char sdTime[9] = "";
char icSID[9] = "";
char kpSID[9] = "";*/

char* filename = "booker.txt";
uint32_t bytesread;

void sdfile_read(struct student users[], int n){
	/*========using SDFatFS, SDPath, SDFile, filename, sdData, bytesread when calling this========*/
	//strcpy(Data,"Booker:Chan Tai Man SID:5180fcc0 StartTime:15:00:00 EndTime:15:45:00 ");
	f_mount(&SDFatFS, SDPath, 1);
	f_open(&SDFile, filename, FA_READ|FA_OPEN_ALWAYS);
	for(int i=0; i<n; i++){

		f_gets(users[i].sdData,100,&SDFile);
		strtok(users[i].sdData, "\n");
	}
}

void sdfile_parse(char txt[],char sd_Booker[],char sd_SID[],char sd_CID[],char sd_StartTime[],char sd_EndTime[]){
	/*========using sdData when calling this========*/
	char* p_booker = strstr(txt,"Booker:");
	char* p_sid = strstr(txt,"SID:");
	char* p_cid = strstr(txt,"CID:");
	char* p_Stime = strstr(txt,"StartTime:");
	char* p_Etime = strstr(txt,"EndTime:");
	int bookerlen = strlen(p_booker);
	int sidlen = strlen(p_sid);
	int cidlen = strlen(p_cid);
	int Stimelen = strlen(p_Stime);
	int Etimelen = strlen(p_Etime);
	strncpy(sd_Booker,p_booker+7,bookerlen - sidlen - 8); // booker == "Chan Tai Man"
	strncpy(sd_SID,p_sid+4,sidlen - cidlen - 5); // sid == "20802077"
	strncpy(sd_CID,p_cid+4,cidlen - Stimelen - 5); //cid == ""
	strncpy(sd_StartTime,p_Stime+10,Stimelen - Etimelen - 11); // Starttime == "15:00:00"
	strncpy(sd_EndTime,p_Etime+8,Etimelen - 9); //EndTime == "15:45:00"
	
	/*LCD_DrawString(50,200,time);
	LCD_DrawString(50,100,booker);
	LCD_DrawString(50,150,sid);*/
}

int data_match(char sd_Booker[],char sd_SID[],char sd_StartTime[],char booker_read[], char sid_read[], char time_read[]){
	if(memcmp(sd_Booker,booker_read,strlen(sd_Booker)) != 0){return 0;}
	if(memcmp(sd_SID,sid_read,strlen(sd_SID)) != 0){return 0;}
	if(memcmp(sd_StartTime,time_read,strlen(sd_StartTime)) != 0){return 0;}
	return 1;
}

