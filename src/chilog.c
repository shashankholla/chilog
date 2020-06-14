/***************************************************************************************************************************
This code is written by Sudheendra H, Team Chimera.
***************************************************************************************************************************/

#include "chilog.h"

void setNames(struct ChiLOG *cl, char lifn[], char lfn[], char extn[])
{
	strcpy(cl->logInfo,  lifn);
	strcpy(cl->fileName,  lfn);
	strcpy(cl->extension,extn);
	return;
}

void getFileName(struct ChiLOG *cl)
{
	char Num[12];
	sprintf(Num,"%012d",cl->num);	//convert num to string 
	strcat(cl->fileName,Num);
	strcat(cl->fileName,".");
	strcat(cl->fileName,cl->extension);
	return;
}

int getFileNum(char * tempName)
{
	int temp = atoi(tempName);
	return (++temp);
}

void clogInit(struct ChiLOG *cl)
{
	char check;
	char tempName[32]="";
	int seekNum = strlen(cl->extension) + 1;

	if((cl->lif = fopen(cl->logInfo,"a+")) != NULL)
	{
		fscanf(cl->lif,"%c",&check);
		fseek(cl->lif, 0, SEEK_SET);
		if(check == 0)		//if it is a new file
		{
			printf("test done");
			fprintf(cl->lif,"#This file was generated by C logger\n#DO NOT DELETE or EDIT this file to avoid errors.\n#");
			cl->num = 0;
		}
		else
		{	//go to last line fetch the last used file name, increment index and open for appending
			fseek(cl->lif,-(seekNum),SEEK_END);
			fgets(tempName,13,cl->lif);		//read index of last log file, 12-digits
			cl->num = getFileNum(tempName);
		}
		
		getFileName(cl);
		fprintf(cl->lif,"\n");
		fprintf(cl->lif,cl->fileName);		//write file name to log info file
		fclose(cl->lif);
		cl->lf = fopen(cl->fileName,"a");	//open new log file for appending
		cl->bt = time(NULL);
		cl->cps = CLOCKS_PER_SEC;
		fprintf(cl->lf,"#This is a CAN frames log file. DO NOT EDIT or DELETE to avoid data loss.\n#CLOCKS_PER_SEC = %ld, Base time = %d\n#\n",cl->cps,cl->bt);
		fclose(cl->lf);
	}
	else
		perror("Can not find log info file ");

	return;
}

void clog(struct ChiLOG *cl, char * type, char * str)
{
	cl->lf = fopen(cl->fileName,"a");
	fprintf(cl->lf,"[%012ld.%012d] ",(time(NULL) - (cl->bt)),clock());
	fprintf(cl->lf," %s : %s\n",type,str);
	fclose(cl->lf);
	return;
}