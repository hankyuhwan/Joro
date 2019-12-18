#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "led.h"

#define LED_DRIVER_NAME		"/dev/periled"

static int fd = 0;
static int ledStatus = 0;

int ledInit(void)
{
	fd = open(LED_DRIVER_NAME,O_RDWR);
	if ( fd < 0 )
	{
		printf("driver (//dev//cnled) open error.\n");
		return 0;
	}
	ledOffAll();
	return 1;
}
int ledOnOff(int ledNum, int OnOff)
{
	int i=1;
	i= i<< ledNum;
	ledStatus = ledStatus & (~i);
	if (OnOff) ledStatus = ledStatus | i;
	write(fd, &ledStatus,4);
	return 1;
}
int ledOnAll(void)
{
	ledStatus=0xFFFFFFFF;
	write(fd,&ledStatus,4);
}
int ledOffAll(void)
{
	ledStatus=0x0;
	write(fd,&ledStatus,4);
}
int ledExit(void)
{
	ledOffAll();
	close(fd);
}

int m_sleep(int ms)
{
	usleep(ms*1000);
}

int twinkle()
{
	while(1){
	ledStatus=0b10101010;;
	m_sleep(1000);
	ledStatus=0b01010101;;
	m_sleep(1000);
	}
}

int battery(int batNum)
{
	if(batNum >= 80)
	{
		ledStatus=0b11111111;
	}
	else if(batNum < 80 && batNum >= 60)
	{
		ledStatus=0b01111111;
	}
	else if(batNum < 60 && batNum >= 40)
	{
		ledStatus=0b00111111;
	}
	else if(batNum < 40 && batNum >= 20)
	{
		ledStatus=0b00011111;
	}
	else if(batNum < 20 && batNum >= 10)
	{
		ledStatus=0b00001111;
	}
	else if(batNum < 10 && batNum >= 0)
	{
		ledStatus=0b00000111;
	}
	else
	{
		ledStatus=0b00111111;
	}
	
	write(fd, &ledStatus,4);
	return 1;
	
}
