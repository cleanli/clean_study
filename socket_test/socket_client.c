/* File Name: client.c */  
  
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<errno.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<sys/un.h>  
#include <unistd.h>
  
#define MAXLINE 4096  
  
  
int main(void)  
{  
    int    sockfd, n,rec_len;  
    char    recvline[4096], sendline[4096];  
    char    buf[MAXLINE];  
    struct sockaddr_un    servaddr;  
  
    struct sockaddr_un* sun_addr = 0;
  
  
    if( (sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){  
    printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);  
    exit(0);  
    }  
  
  
    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sun_family = AF_UNIX;  
    strcpy(servaddr.sun_path, "my_test2");
  
  
    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_un)) < 0){  
    printf("connect error: %s(errno: %d)\n",strerror(errno),errno);  
    exit(0);  
    }  
  
  
    printf("send msg to server: \n");  
    memset(sendline, 0, 4096);
    strcpy(sendline, "xxxxxxxxxxxxxyyyyyyyyyy\n");
    //if( send(sockfd, sendline, strlen(sendline), 0) < 0)  
    if( write(sockfd, sendline, strlen(sendline)) < 0)  
    {  
    printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
    exit(0);  
    }  
    if((rec_len = recv(sockfd, buf, MAXLINE,0)) == -1) {  
       perror("recv error");  
       exit(1);  
    }  
    buf[rec_len]  = '\0';  
    printf("Received : %s ",buf);  
    close(sockfd);  
    exit(0);  
}  
