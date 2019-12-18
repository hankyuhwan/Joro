#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "fnd.h"

#define FND_DRIVER_NAME		"/dev/perifnd"
#define MODE_STATIC_DIS		0
#define MODE_TIME_DIS		1
#define MODE_COUNT_DIS		2

int main(int argc , char **argv)
{
	int mode;
	int number,counter;
	
	if (argc <  2)
	{
		return 1;
	}
	
	if ( argv[1][0] == 's'  )
	{
		mode = MODE_STATIC_DIS;
		if ( argc < 3 )
		{
			perror(" Args number is less than 3\n");
			return 1;
		}
		number = atoi(argv[2]);
	}
	else if ( argv[1][0] == 't'  )
	{
		mode = MODE_TIME_DIS;
		if ( argc < 2 )
		{
			perror(" Args number is less than 2\n");
			return 1;
		}
	}
	else if ( argv[1][0] == 'c'  )
	{
		mode = MODE_COUNT_DIS;
		if ( argc < 3 )
		{
			perror(" Args number is less than 3\n");
			return 1;
		}
		number = atoi(argv[2]);
	}

	else
	{
		perror("option error \n");
		return 1;
	}
	printf("number :%d\n",number);
	// open  driver 
	if (mode == MODE_STATIC_DIS )
	{
	fndDisp(number,  mode);
	}
	else if(mode == MODE_TIME_DIS )
	{
		struct tm *ptmcur;
		time_t tTime;
		if ( -1 == time(&tTime) )
			return -1; 

		ptmcur = localtime(&tTime);

		number = ptmcur->tm_hour * 10000;
		number += ptmcur->tm_min *100;
		number += ptmcur->tm_sec;

		fndDisp(number ,  mode);
		
	}
	else if (mode == MODE_COUNT_DIS)
	{
		counter = 0;

		while(1)
		{
			if (!fndDisp(number ,  mode))
				break;

			counter++;
			sleep(1);
			if (counter > number )
				break;
		}

	}
	
	return 0;
}
