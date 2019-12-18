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
#include <pthread.h>
#include "fnd.h"

#define FND_DRIVER_NAME      "/dev/perifnd"

#ifndef __FND_DRV_H__
#define __FND_DRV_H__

#define MAX_FND_NUM   6
#define FND_DATA_BUFF_LEN   (MAX_FND_NUM + 2)
#define ONE_SEG_DISPLAY_TIME_USEC   1000
#define MODE_STATIC_DIS      0
#define MODE_TIME_DIS      1
#define MODE_COUNT_DIS      2


int timerun = 0;
int number = 0;

typedef struct FNDWriteDataForm_tag 
{
   char   DataNumeric[FND_DATA_BUFF_LEN];
   char   DataDot[FND_DATA_BUFF_LEN];
   char   DataValid[FND_DATA_BUFF_LEN];   
}
stFndWriteForm,*pStFndWriteForm;
#endif


/*static pthread_t fndTh_id;
static void *fndThFunc(void* arg)
{
      while(1)
      {
       
      if(timerun==1)
      {
      
      struct tm *ptmcur;
      time_t tTime;
      ptmcur = localtime(&tTime);
      
      number = ptmcur->tm_hour * 10000;
      number += ptmcur->tm_min *100;
      number += ptmcur->tm_sec;
      printf ("Time\r\n");

   
      usleep(1000);
   }
   
   else 
   {      
   printf ("Do nothing\r\n");
   }
   usleep(1000*1000);   
   }
}*/

int fndDisp(int num , int dotflag)
{
   int fd;
   int temp,i;
   stFndWriteForm stWriteData;
   
   for (i = 0; i < MAX_FND_NUM ; i++ )
   {
      stWriteData.DataDot[i] = (dotflag & (0x1 << i)) ? 1 : 0;  
      stWriteData.DataValid[i] = 1;
   }
   // if 6 fnd
   temp = num % 1000000;
   stWriteData.DataNumeric[0]= temp /100000;

   temp = num % 100000;
   stWriteData.DataNumeric[1]= temp /10000;

   temp = num % 10000;
   stWriteData.DataNumeric[2] = temp /1000;

   temp = num %1000;
   stWriteData.DataNumeric[3] = temp /100;

   temp = num %100;
   stWriteData.DataNumeric[4] = temp /10;

   stWriteData.DataNumeric[5] = num %10;

   fd = open(FND_DRIVER_NAME,O_RDWR);
   if ( fd < 0 )
   {
      perror("driver open error.\n");
      return 0;
   }   
   //pthread_create(&fndTh_id, NULL, fndThFunc, NULL);
   write(fd,&stWriteData,sizeof(stFndWriteForm));

   close(fd);
   return 1;
}

int fndOff(void)
{
   int fd,i;
   stFndWriteForm stWriteData;
   
   for (i = 0; i < MAX_FND_NUM ; i++ )
   {
      stWriteData.DataDot[i] =  0;  
      stWriteData.DataNumeric[i] = 0;
      stWriteData.DataValid[i] = 0;
   }
   fd = open(FND_DRIVER_NAME,O_RDWR);
   if ( fd < 0 )
   {
      perror("driver open error.\n");
      return 0;
   }   
   write(fd,&stWriteData,sizeof(stFndWriteForm));
   //pthread_cancel(fndTh_id);
   close(fd);
   return 1;
}
