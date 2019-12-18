#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "textlcddrv.h"
#include "textlcd.h"

#define TEXTLCD_DRIVER_NAME		"/dev/peritextlcd"

static int fd;

	stTextLCD  stlcd; 
	
	unsigned int linenum = 0;

	int len; 

int textlcdWrite()
{

	
		fd = open(TEXTLCD_DRIVER_NAME,O_RDWR);
	if ( fd < 0 )
	{
		perror("driver (//dev//peritextlcd) open error.\n");
		return 1;
	}
	
	int value = write(fd,&stlcd,sizeof(stTextLCD));
	printf ("Value:%d\r\n",value);
	
	return 1;
}

int textlcdInit(int number, char *  text)
{
		
	memset(&stlcd,0,sizeof(stTextLCD));
	
		linenum = number;//strtol(number,NULL,10);
	printf("linenum :%d\n", linenum);
	
	if ( linenum == 1) // firsst line
	{
		stlcd.cmdData = CMD_DATA_WRITE_LINE_1;
	}
	else if ( linenum == 2) // second line
	{
		stlcd.cmdData = CMD_DATA_WRITE_LINE_2;
	}
	else 
	{
		printf("linenum : %d  wrong .  range (1 ~ 2)\n", linenum);
		return 1; 
	}
	printf("string:%s\n",text);
	len = strlen(text);
	if ( len > COLUMN_NUM)
	{
		memcpy(stlcd.TextData[stlcd.cmdData - 1],text,COLUMN_NUM);
	}
	else
	{
		memcpy(stlcd.TextData[stlcd.cmdData - 1],text,len);
	}
	stlcd.cmd = CMD_WRITE_STRING;
	
	
	return 1;
}


