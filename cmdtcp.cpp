#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <fcntl.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
   
#define BUFSIZE 30 
   
void error_handling(char *message); 
   
int main(int argc, char **argv) 
{ 
     int fd; 
     int sd; 
     off_t size = 1;
     off_t presize = 1;  
     char cmd; 
     int len; 
     struct sockaddr_in serv_addr; 
  
     if(argc!=3){ 
         printf("Usage : %s <IP> <port>\n", argv[0]); 
         exit(1); 
     } 
     
    /* 서버 접속을 위한 소켓 생성 */ 
    sd=socket(PF_INET, SOCK_STREAM, 0);    
    if(sd == -1) 
         error_handling("socket() error"); 
    memset(&serv_addr, 0, sizeof(serv_addr)); 
    serv_addr.sin_family=AF_INET; 
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]); 
    serv_addr.sin_port=htons(atoi(argv[2])); 
    	if( connect(sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1 ) 
        	error_handling("connect() error!"); 
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
	    write(sd, &cmd, 1);    
   	close(fd); 
   }
    /* 전송해 준것에 대한 감사의 메시지 전달 */ 
    close(fd); 
    close(sd); 
    return 0; 
} 
  
void error_handling(char *message) 
{ 
     fputs(message, stderr); 
     fputc('\n', stderr); 
     exit(1); 
}
