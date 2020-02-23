#include <windows.h>  
#include <winsock.h>  
#include <iostream>  
#pragma comment(lib,"ws2_32.lib")  
  
#define NO_FLAGS_SET 0  
#define PORT (u_short)44965  
#define DEST_IP_ADDR "192.168.0.7" //Server address  
using namespace std;
  
//客户端 --VC++ Socket编程 简单的Tcp/ip客户端  
int main()  
{  
WSADATA Data;  
SOCKADDR_IN destSockAddr;  
SOCKET destSocket;  
unsigned long destAddr;  
int status;  
int numsnt;  
const char *toSendtxt="Test String";  
  
/* initialize the Windows Socket DLL */  
status=WSAStartup(MAKEWORD(1, 1), &Data);  
if (status != 0)  
   cerr << "ERROR: WSAStartup unsuccessful" << endl;  
  
/* convert IP address into in_addr form */  
destAddr=inet_addr(DEST_IP_ADDR);  
/* copy destAddr into sockaddr_in structure */  
memcpy(&destSockAddr.sin_addr, &destAddr, sizeof(destAddr));  
/* specify the port portion of the address */  
destSockAddr.sin_port=htons(PORT);  
/* specify the address family as Internet */  
destSockAddr.sin_family=AF_INET;  
  
/* create a socket */  
destSocket=socket(AF_INET, SOCK_STREAM, 0);  
if (destSocket == INVALID_SOCKET)  
{  
   cerr << "ERROR: socket unsuccessful" << endl;  
    
   status=WSACleanup();  
   if (status == SOCKET_ERROR)  
    cerr << "ERROR: WSACleanup unsuccessful" << endl;  
   return(1);  
}  
  
cout << "Trying to connect to IP Address: " << DEST_IP_ADDR << endl;  
  
/* connect to the server */  
status=connect(destSocket, (LPSOCKADDR)&destSockAddr, sizeof(destSockAddr));  
if (status == SOCKET_ERROR)  
{  
   cerr << "ERROR: connect unsuccessful" << endl;  
    
   status=closesocket(destSocket);  
   if (status == SOCKET_ERROR)  
    cerr << "ERROR: closesocket unsuccessful"  
    << endl;  
   status=WSACleanup();  
   if (status == SOCKET_ERROR)  
    cerr << "ERROR: WSACleanup unsuccessful"  
    << endl;  
   return(1);  
}  
  
cout << "Connected..." << endl;  
  
while(1)  
{  
   cout << "Sending..." << endl;  
   numsnt=send(destSocket, toSendtxt, strlen(toSendtxt) + 1, NO_FLAGS_SET);  
   if (numsnt != (int)strlen(toSendtxt) + 1)  
   {  
    cout << "Connection terminated" << endl;  
     
    status=closesocket(destSocket);  
    if (status == SOCKET_ERROR)  
     cerr << "ERROR: closesocket unsuccessful" << endl;  
    status=WSACleanup();  
    if (status == SOCKET_ERROR)  
     cerr << "ERROR: WSACleanup unsuccessful" << endl;  
    return(1);  
   }  
    
   /* Wait before sending the message again */  
   Sleep(1000);  
} /* while */  
}  
