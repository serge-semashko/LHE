/*
UDPServer.c: UDP Server Demo program for 7186EX series
          A server listen and wait to handle the coming connections
          Details:
            Setup a server, wait for connections.
            While a connection has been established, receive data from clients.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.
      
Compile mode: large

Project: SERVER.c 
         ..\..\lib\7186el.Lib
         ..\..\lib\tcp_dm32.lib
         
Details:
    UDP Demo of using sendto() and recvfrom() functions.
    The acccept() and connect() functions isn't needed here since recvfrom() establishes the connection.

 >> int recvfrom(int s, char *buf, int len, int flags, struct sockaddr *from, int *fromlen);
    Receives a message from a connection. The recvfrom() function allows a connection to be made and a 
    message to be read from the connection. The flag MSG_WAITALL is not supported.
    
    Parameters:
      s     socket identifier;
      buf   Buffer in which information will be stored.
      len   Number of bytes to receive.
      flags Specifies optional behavior:
		    MSG_OOB returns urgent data.
		    MSG_PEEK returns information,allowing it to be read again on a subsequent call.
      from  Specifies the remote host to which the connection should be made.
      fromlen  Size of the from data structure.
    
    The finnal two arguments to recvfrom are similar to the final two arguments accept.
    
    Return Value:
       -1 Error.
      >=0 Number of bytes received.
      
 >> int sendto(int s, char *buf, int len, int flags, struct sockaddr *to, int tolen);
    Send a message. The sendto()function allows a connection to be made and a message to Be written to 
    the connection.
    
    Parameters:
      s     Socket identifier.
      buf   Buffer from which information will be sent.
      len   Number of bytes to send.
      flags Specifies optional behavior:
		    MSG_OOB sends the data as urgent data.
		    MSG_DONTROUTE ensures that the message is not sent through a default router.
      to    Specifies the remote host to which the connection should be made.
      tolen Size of the to data structure.

    Return Value:
       -1 Error.
      >=0 Number of bytes sent.

Hardware: 7186EX series
    
[Mar 22, 2008] by Liam
[Aug,3,2011] by Nicholas
*/

#include <stdlib.h>
#include <stdio.h>
#include "..\..\lib\7186e.h"
#include "..\..\lib\Tcpip32.h"

#define SOCKET_NUM      32      // define the maximum number of sockets 
#define Service_Port    10000   // define the service port for Server

fd_set rfds;    // set of socket file descriptors for reading

typedef struct tagSocket_State 
{
    int active;                 // socket is active
    int init;
    struct sockaddr_in  sin;    // client address
} Socket_State;

Socket_State socket_state[SOCKET_NUM];

struct ip host_ip;
extern int errno;   // error codes 

int UDP_server_initial(void)
{    
    int i, iRet;
 
    iRet=NetStart();
	if(iRet<0)
	{
		Print("Init Ethernet error.\n\r");
		return iRet;
	}

    FD_ZERO(&rfds);
  
    for(i=0; i<SOCKET_NUM; i++) 
    {
        socket_state[i].active=0;
        socket_state[i].init=0;
        memset(&socket_state[i].sin, 0, sizeof(socket_state[i].sin));     // bzero is a unix system call
    }
    
    return 0;   //no error
}

int Create_Socket(void)
{
    /*
    port_number: port number to listen

    return:  -1      sokcet create error
             -2      bind a socket error
            >=0      on success, returns a socket number
    */
 
    int type, protocol;
    int err;
    int iSocket;
    
    type=SOCK_DGRAM;    // UDP/IP
    protocol=0;         // always 0
    
    iSocket=err=socket(PF_INET, type, protocol);
  
    if(err<0) 
    {
        Nterm();
        return -1;
    }
    
    // addressing for master socket
    memset(&socket_state[iSocket].sin, 0, sizeof(socket_state[iSocket].sin));
    socket_state[iSocket].sin.sin_family=AF_INET;
    socket_state[iSocket].sin.sin_addr.s_addr=0L;   // INADDR_ANY is a 32bits zero value 
    socket_state[iSocket].sin.sin_port=htons(Service_Port);     // Port 10000
    
    err=bind(iSocket, (struct sockaddr *) &socket_state[iSocket].sin, sizeof(socket_state[iSocket].sin));
    if(err<0) 
    {
        closesocket(iSocket);
        Nterm();
        
        return -2;
    }

    socket_state[iSocket].active=1;

    return iSocket;     // err=0 for success
}

void UDP_server_shutdown(void)
{
    int i;
    
    FD_ZERO(&rfds);     // clear FD set  
    
    for(i=0; i<SOCKET_NUM; i++) 
    {
        if(socket_state[i].active)      // if socket lives, close it 
        {
            shutdown(i, 0);
            closesocket(i);
        }
    }
}

unsigned char buf[1024];    // buffer for receive data 
void main(void)
{
    struct sockaddr_in ssin;    // client Internet endpoing address
    int sin_len=sizeof(ssin);
    int iRet, i, iQuit, iByte;
    int iMaster_Socket;
   
    InitLib();

//Step 1: Initialize UDP Server
    iRet=UDP_server_initial();
    switch(iRet)
    {
        case -1:
            Print("Ninit() failed! error\r\n");
            exit(1);
            break;
                
        case -2:
            Print("Portinit() failed! error\r\n");
            exit(1);
            break;
    }

//Step 2: Create sockets and bound to names then listen for connections
    iMaster_Socket=iRet=Create_Socket();
    if(iRet<0)
        Print("cannot listen on port %d\r\n", Service_Port);

//Step 3: Service loop:
    iQuit=1;
    while(iQuit)    // service loop 
    {        
        YIELD();
        FD_ZERO(&rfds);
        
        for(i=0; i<SOCKET_NUM; i++)
        {
            if(socket_state[i].active)
                FD_SET(i, &rfds);
        }
            
        if(selectsocket(SOCKET_NUM, &rfds, NULL, NULL, NULL)<=0)
            continue;

//Step 3-1: perform service, receive and send
        for(i=0; i<SOCKET_NUM; i++)     // scan all possible sockets 
        {
            if(FD_ISSET(i, &rfds)) 
            {
                if(i==iMaster_Socket)
                {
                    // master socket -> ACCEPT() request
                    memset(&ssin, 0, sizeof(ssin));
                    iByte=recvfrom(i, buf, sizeof(buf), 0, (struct sockaddr*)&ssin, &sin_len);
                    if(iByte<0)
                        continue;

                    if(iByte>0)
                    {
                        buf[iByte]=0x0;
                        Print("%s\n\r",buf);
                        if(!strcmp(buf, "quit"))
                            iQuit=0;
                        else
                        {
                            sendto(i, buf, iByte, 0, (struct sockaddr*) &ssin, sin_len);
                        }
                    }
                }
                
                FD_CLR(i, &rfds);
            }
        }
    }

//Step 4: Shutdown the UDP server   
    UDP_server_shutdown();
}