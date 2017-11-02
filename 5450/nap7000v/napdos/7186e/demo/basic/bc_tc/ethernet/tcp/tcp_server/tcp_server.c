/*
TCP_Server.c: TCP Server Demo program for 7186EX series
          A server listen and wait to handle the coming connections

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01)
          
Compile mode: large

Project: TCP_Server.C
         ..\..\lib\7186el.lib
         ..\..\lib\tcp_dm32.lib

Details:
    Setup a server, wait for connections.
    While a connection has been established, receive data from clients.
    
[30 Apr, 2008] by Liam
[Aug,3,2011] by Nicholas
[March,19,2012] by Nicholas
*/

#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "..\..\lib\7186e.h"
#include "..\..\lib\Tcpip32.h"

#define MaxClentSocket 32
#define NUM_OF_SERVICE_PORT 2

fd_set rfds,s_fds;    // server socket set of socket file descriptors for reading
unsigned ActiveSkt[2]={0, 0};

struct timeval seltime={0,20};

unsigned short service_port[NUM_OF_SERVICE_PORT]={10000, 10001};
int socketPort[MaxClentSocket];

void Port10000(int current_socket)
{
    // current_socket: Socket number
    int iRet;
    char buf[1024];
	iRet=recv(current_socket, buf, 1024, 0);
    if(iRet<=0)
    {
        shutdown(current_socket,2);
		closesocket(current_socket);
		FD_CLR(current_socket,&s_fds);
		socketPort[current_socket]=-1;
		if(current_socket>=16)
			ActiveSkt[1]&=~(1<<(current_socket-16));
		else
			ActiveSkt[0]&=~(1<<current_socket);
    }
    else
	{
		buf[iRet]=0x0;
		send(current_socket, buf, strlen(buf), 0);
	}
}

void Port10001(int current_socket)
{
    // current_socket: Socket number  
    int iRet;
    char buf[1024];
	iRet=recv(current_socket, buf, 1024, 0);
    if(iRet<=0)
    {
        shutdown(current_socket,2);
		closesocket(current_socket);
		FD_CLR(current_socket,&s_fds);
		socketPort[current_socket]=-1;
		if(current_socket>=16)
			ActiveSkt[1]&=~(1<<(current_socket-16));
		else
			ActiveSkt[0]&=~(1<<current_socket);
    }
    else
	{
		buf[iRet]=0x0;
		send(current_socket, buf, strlen(buf), 0);    
	}
}

void main(void)
{
	struct sockaddr_in serverAdd[2];
	struct sockaddr_in clientAdd;	
	int clientAddLen;
	int new_con,i,c,err;
	int s[2];
	
	InitLib();
	
	//Step 1: Initialize TCP Server
	err=NetStart();
	if(err<0)
	{
		Print("Initialize TCP Server failed \n\r");
		return;
	}
	
	//Step 2: Create sockets and bound to names then listen for connections
	for(i=0; i<2; i++)
	{
		s[i]=socket(PF_INET, SOCK_STREAM, 0);
		
		memset(&serverAdd[i], 0, sizeof(serverAdd[i]));
		
		serverAdd[i].sin_family= AF_INET;
		serverAdd[i].sin_addr.s_addr= 0;
		serverAdd[i].sin_port= htons(service_port[i]);
		
		bind(s[i], (struct sockaddr *)&serverAdd[i], sizeof(serverAdd[i]));
		
		listen(s[i], MaxClentSocket-1);
	}
  
	//Step 3: Service loop:     3-1: wait on sockets and accept
	//                          3-2: perform service, receive and send
	for(;;)
	{
		YIELD();
		//Step 3-1: Wait for activity on sockets and accept a connection
		for(i=0; i<2; i++)
		{
			FD_ZERO(&rfds);
			FD_SET(s[i],&rfds);
			
			new_con=selectsocket(MaxClentSocket, &rfds, NULL, NULL, &seltime);	//wait for activity on a set of sockets
			if(new_con>0)
			{
				memset( &clientAdd, 0, sizeof(clientAdd));
				clientAddLen = sizeof(clientAdd);
				c = accept(s[i],(struct sockaddr *)&clientAdd, &clientAddLen);
				Print("create socket %d \n\r",c);
				if(c<0)
				{
					shutdown(c,2);
					closesocket(c);
				}
				else if(c>0 && c<MaxClentSocket)
				{
					FD_SET(c,&s_fds);
					socketPort[c]=service_port[i];
					if(c>=16)
						ActiveSkt[1]|=1<<(c-16);
					else
						ActiveSkt[0]|=1<<c;
					
					SOCKET_SET_TCP_KEEP_ALIVE_ON(c);
				}
			}
		}
		//Step 3-2: perform service, receive and send
		for(i=1; i<MaxClentSocket; i++)
		{
			if(FD_ISSET(i,&s_fds))
			{
				if(!ioctlsocket(i,FIONREAD,&err) && (err>0))
				{
					if(socketPort[i]==10000)
					{
						Port10000(i);
					}
					else if(socketPort[i]==10001)
					{
						Port10001(i);
					}
				}
			}
		}
		//Press ESC to terminate the program.
		if(Kbhit() && Getch()==27)
		{
			Nterm();
			return;
		}
	}
}
