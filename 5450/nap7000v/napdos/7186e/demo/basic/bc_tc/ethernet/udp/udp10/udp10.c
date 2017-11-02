/* 
UDP10.C: UDP/IP socket demo By ICPDAS. Tim.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.
      
Compile mode: large

Project: UDP10.c 
         ..\..\lib\7186el.Lib
         ..\..\lib\tcp_dm32.lib 
         
Details:
        [RS-232]------[COM1/7186E]----[HUB]----[7186E/COM1]------[RS-232] 
           (A)            (B)                      (C)              (D)
   
        (A): RS-232 device 1
        (D): RS-232 device 2
        (B): i-7188En/uPAC-7186EX, IP1(192.168.1.10)
        (C): i-7188En/uPAC-7186EX, IP2(192.168.1.20)

        on (B) run "udp10 192.168.1.20"
        on (C) run "udp10 192.168.1.10"
        both i-7188En/uPAC-7186EX will be a server & client. use UDP/IP port 10000.

        (1) i-7188En/uPAC-7186EX will Listen PORT 10000, after any connection come in, 
            the input data from the socket will send to COM1 of 7186En.
        (2) i-7188En/uPAC-7186EX also will be a client, and connect to another i-7188En/uPAC-7186EX at 
            the TCP port 10000. After connect to the remote i-7188En/uPAC-7186EX, any data come from 
            COM1 will send to remote i-7188En/uPAC-7186EX by TCP packet.
    
[Dec 14, 2001] 
               1. Read COM1 setting from EEPROM.
                  P.S. using MiniOS7 Command : "setcom 1 [baud,data,parity,stop]" to set or display 
                  current setting value.
               2. Add the variable "ShowDebugMsg" to disable(0) or enable(1) show debug message.
               3. Add LED flash when connected to another server node.
 
[Nov 04, 2008] by Liam
[Aug,3,2011] by Nicholas
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "..\..\lib\7186e.h"
#include "..\..\lib\Tcpip32.h"

#define BUFSIZE     1500        /* read/write buffer size */
#define SERVICE     10000       /* define the service port for Server */
#define SOCKETS     32          /* define the maximum number of sockets */
#define QLEN        SOCKETS-1   /* maximum connection queue length */

typedef struct COM_DATA {
  unsigned long baud;
  char databit;
  char parity;
  char stopbit;
  unsigned char sum;
} *pCOM_DATA, COM_DATA;

int QuitMain=0;       
int ShowDebugMsg=0;
COM_DATA Com1Data;
fd_set rfds;  /* set of socket file descriptors for reading */

typedef struct socket_state {
    int active;     /* socket is active */
    struct sockaddr_in sin;     /* client address */
} socket_state;
socket_state sst[SOCKETS];    /* sockets state table */

STOPWATCH swLed;
// clear, shutdown and close all sockets
 void killsockets(void)
{
    int i;
    FD_ZERO(&rfds);   /* clear FD set */

    for(i=0; i<SOCKETS; i++)
    {
        if(sst[i].active)   /* if socket lives, kill it */
        {       
            shutdown(i, 0);
            closesocket(i);
        }
    }
}

char InBuf[BUFSIZE], OutBuf[BUFSIZE];
int RxCounter=0, RxCounterOut=0;
struct ip host_ip;

void main(int argc, char *argv[])
{
    int master, outskt;   /* master socket for listenning */
    int err;
    int i, len, Idx;
    char *IpStr;

    InitLib();
    
    if(argc>1)
    {
        IpStr=argv[1];
    }
    else
    {
        if(ShowDebugMsg)
            Print("please use : demo10 remote-IP-addr\r\n");
        
        exit(1);
    }
    
    err=NetStart();
	if(err<0)
	{
		if(ShowDebugMsg)
			Print("Init Ethernet error.\n\r");
		return;
	}

    master=Nopen("*", "UDP/IP", 10000, 0, S_NOCON | S_NOWA);
    outskt=Nopen(IpStr, "UDP/IP", 0, 10000, S_NOCON | S_NOWA);
    
    if(master<0 || outskt<0)
    {
        Print("Can not open a connection for UDP/IP");
        Nterm();
    
        return ;
    }
    SOCKET_NOBLOCK(master);

    // Get the setting value of COM1
    EE_MultiRead(0, 0, sizeof(COM_DATA), (char *) &Com1Data);
    {
        unsigned char sum=0;
        unsigned char *ptr=(unsigned char *) &Com1Data;
        int j;
        
        for(j=0; j<7; j++)
            sum+=*ptr++;
    
        if(sum!=Com1Data.sum)
        {
            // use the default setting [9600, N, 8, 1]
            Com1Data.baud=9600L,
            Com1Data.databit=8,
            Com1Data.parity=0,
            Com1Data.stopbit=1;
        }
    }
    
    if(ShowDebugMsg)
    {
        Print("Baud=%lu,data=%d,parity=%d\n\r",Com1Data.baud,Com1Data.databit,Com1Data.parity);
        DelayMs(100);
    }
    
    InstallCom1(Com1Data.baud, Com1Data.databit, Com1Data.parity, Com1Data.stopbit);
  
    T_StopWatchStart(&swLed);
    while(!QuitMain)
    {
        YIELD();    // must add this line in the while loop
        
        if(T_StopWatchGetTime(&swLed)>=500)
        {
            T_StopWatchStart(&swLed);
            LedToggle();
        }

        // socket to COM
        len=Nread(master, InBuf, sizeof(InBuf));
        if(len>0)
        {
            for(i=0; i<len; i++)
                ToCom1(InBuf[i]);   // just send all data to COM1
        }

        // ComToSocket
        Idx=0;
        while(IsCom1())
        {
            OutBuf[Idx++]=ReadCom1();
        }
    
        if(Idx)
            Nwrite(outskt, OutBuf, Idx);
    }
  
    killsockets();
    Nterm();
    RestoreCom1();
    
    LedOn();
    
    return;
}