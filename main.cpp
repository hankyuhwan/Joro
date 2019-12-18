#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <linux/input.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <ctime>

#include "led.h"
#include "buzzer.h"
#include "button.h"
#include "textlcd.h"
#include "touch.h"
#include "fnd.h"

off_t size = 1;
off_t presize = 1;
char cmd;
char cmdnum;
int fd, sd;
int bat = 100;
time_t curr_time;
struct tm *curr_tm;
int nowtime = 0;
struct sockaddr_in serv_addr;
int to = 0;

using namespace std;

void caltime(int tid)
{
	while(1)
	{
		curr_time = time(NULL);
		curr_tm = localtime(&curr_time);
		nowtime = (curr_tm->tm_hour)*10000;
		nowtime = nowtime + (curr_tm->tm_min)*100;
		nowtime = nowtime + curr_tm->tm_sec;
	}
}

void disbat(int tid)
{
	while(1)
	{
		msleep(20000);
		bat = bat - 1;
	}
}

void error_handling(char *message)
{
     fputs(message, stderr);
     fputc('\n', stderr);
     exit(1);
}

void make_cmd(int tid)
{
	while(1)
           {
                /* 원하는 데이터를 입력 */
                while(1)
                 {
                        fd = open("./cmd.txt",O_RDONLY);
                        size = lseek(fd,(off_t)0,SEEK_END);
                        if(size>presize) break;
                        close(fd);
                }
                   lseek(fd,(off_t)-2,SEEK_END);
                   presize = presize + 1;
                   read(fd, &cmd, 1);
                   printf("%c",cmd);
		   cmdnum = (int)cmd - '0';
                   write(sd, &cmd, 1);
                 close(fd);
         }
           close(fd);
           close(sd);
}

int main (int argc, char **argv)
{
	char *cont;
	int number;
	thread t[10];

	if(argc!=3)
	{
		printf("Usage : %s <IP> <port>\n",argv[0]);
		exit(1);
	}
	
	sd=socket(PF_INET, SOCK_STREAM, 0);
	if(sd == -1)
	    error_handling("socket() error");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	
	if( connect(sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1 )
		error_handling("connect() error!");


	t[1] = thread(make_cmd,1);
	t[2] = thread(disbat,1);
	t[3] = thread(caltime,1);
	
	printf ("Hello!\r\n");
	if (ledInit()== 0)
	{
		printf ("LED init Failed!\r\n");
		return 0;
	}
	if (buzzerInit() == 0)
	{
		printf ("Buzzer init Failed!\r\n");
		return 0;
	}
	if (buttonInit() == 0)
	{
		printf ("Button init Failed!\r\n");
		return 0;
	}
	
	textlcdInit(1, "Welcome");
	textlcdWrite();
	int messageID = msgget (MESSAGE_ID, IPC_CREAT|0666);	//To Receive Message from Library.
	BUTTON_MSG_T rxMsg;
	dingdongdang();
	while (1)
	{		
		if(cmd=='1')
		{
			twinkle();
			textlcdInit(2, "go A" );
			textlcdWrite();
			dingdongdang();
			textlcdWrite();
			 fndDisp( number , 1);
		}
		else if(cmd=='2')
		{
			twinkle();
			textlcdInit(2, "go B" );
			textlcdWrite();
			fndDisp( number , 1);
			
		}
		else if(cmd=='3')
		{
			twinkle();
			battery(bat);
		}
		
		msgrcv(messageID, &rxMsg, sizeof(rxMsg) - sizeof(long int),0 ,0);
		printf ("ReceivedMessage:%d-%d",rxMsg.keyInput, rxMsg.pressed);
		//rxMsg.pressed == 0 -> Really Pressed.
		int scale = 0;
		
		switch(rxMsg.keyInput)
		{
			case KEY_HOME: 
				printf("Volume up key):");
				scale = 0; 
				//my_multiple_arg->timerun==1;
				
			break;
			case KEY_BACK: 
				printf("Home key):"); 
				scale = 1;
				//timerun=0;
				//	number=111111;
			break;
			case KEY_SEARCH: 
				printf("Search key):"); 
				scale = 2;
				
				//	number=827325;
			break;
			case KEY_MENU: 
				printf("Back key):");
				scale = 3;
					//timerun=0;
			break;
			case KEY_VOLUMEUP: 
				printf("Menu key):"); 
				scale = 4;
			break;
			case KEY_VOLUMEDOWN:
				printf("Volume down key):");
				scale = 5;
			break;
		}
		
		
		

		if (rxMsg.pressed != 0)
		{
			switch(scale)
			{
				case 0:
					textlcdInit(2, "Today" );
					textlcdWrite();
					number=191218;
					to = 0;
					fndDisp(number,1);
					break;
				case 1:
					textlcdInit(2, "Time");
					textlcdWrite();
					fndDisp(nowtime,1);
					to = 1;
					break;
				case 2:
					textlcdInit(2, "Battery");
					textlcdWrite();
					battery(bat);
					to = 0;
					fndDisp( bat , 1);
					break;	
				case 3:
					textlcdInit(2, "no");
					textlcdWrite();
					break;	
				case 4:
					textlcdInit(2, "no");
					textlcdWrite();
					break;	
				case 5:
					textlcdInit(2, "no");
					textlcdWrite();
			}
		}
		else
		{
			ledOnOff(scale, 0);
			buzzerStopSong();			
		}
		
	}

	ledExit();
	buzzerExit();
	buttonExit();
	return 1;
}
