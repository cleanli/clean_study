/* File Name: server.c */  
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<errno.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<sys/un.h>  
#include <unistd.h>
#include <stddef.h>
#define DEFAULT_PORT 8000  
#define MAXLINE 4096  
int main(void)  
{  
    int    socket_fd, connect_fd;  
    struct sockaddr_un     servaddr, clientaddr;  
    char    buff[4096];  
    int     n;  
    struct sockaddr_un* sun_addr = 0;
    //初始化Socket  
    memset(&servaddr, 0, sizeof(servaddr));  
    strcpy(servaddr.sun_path, "my_test2");
    servaddr.sun_family = AF_UNIX;  
    unlink(servaddr.sun_path);
    if( (socket_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1 ){  
    printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);  
    exit(0);  
    }  
    //初始化  
    int server_len = strlen("mytest2")  + offsetof(struct sockaddr_un, sun_path);
  
    //将本地地址绑定到所创建的套接字上  
    if( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_un)) == -1){  
    printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);  
    exit(0);  
    }  
    //开始监听是否有客户端连接  
    if( listen(socket_fd, 10) == -1){  
    printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);  
    exit(0);  
    }  
    printf("======waiting for client's request======\n");  
    {  
//阻塞直到有客户端连接，不然多浪费CPU资源。  
        int sz = sizeof(clientaddr);
        if( (connect_fd = accept(socket_fd, (struct sockaddr*)&clientaddr, &sz)) == -1){  
        printf("accept socket error: %s(errno: %d)",strerror(errno),errno);  
        //continue;  
	return -1;
	}
    }  
    printf("get accepted\n");
//接受客户端传过来的数据  
    //n = recv(connect_fd, buff, MAXLINE, 0);  
    n = read(connect_fd, buff, MAXLINE);  
    printf("recv msg from client: %s\n", buff);  
    send(connect_fd, buff, n, 0);
//向客户端发送回应数据  
    close(connect_fd);  
    close(socket_fd);  
}
