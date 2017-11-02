/*
Project: Ethernet Client demo program with timer interrupt function

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01)
          
Compile mode: large

Project: TCP_Client.c
         ..\..\lib\7186el.lib
         ..\..\lib\tcp_dm32.lib

Details:
    This demo sends string "11test" to another 7186EX/8000E that running a Xserver program. 
    The Xserver default supports command "11" for testing. It will responses a string in 
    "<11(Data)>" format. 
    In this demo, the data is "test", thus each time, the program should receive "<11test>", 
    totally 8 bytes.                                    
                                                                      
[Nov 3, 2008] by Liam
[Aug,3,2011] by Nicholas   
[March,19,2012] by Nicholas
*/

#include <stdlib.h>
#include "..\..\lib\7186e.h"
#include "..\..\lib\Tcpip32.h"

int iLoop=0;
extern long ET_TOUT;    // basic timeout value

fd_set c_fds;   // client socket set of socket file descriptors for reading

void Interrupt_Fun(void)
{    
    //DO is used to generate square wave for testing.
    
    iLoop++;
}

int Ethernet_Connect(unsigned char *cRemoteIP,int iRemoteTCPPort,long lConnect_Timeout)
{
	int s,iTemp,iRet;
	struct sockaddr_in sktRemoteServer;
	unsigned long lStartTimeTick;
	
	s=socket(PF_INET,SOCK_STREAM,0);
	if(s>=0)
	{
		YIELD();
		memset(&sktRemoteServer, 0, sizeof(sktRemoteServer)); // bzero is a unix system call
		sktRemoteServer.sin_family = AF_INET;
		sktRemoteServer.sin_addr.s_addr = inet_addr(cRemoteIP);
		sktRemoteServer.sin_port = htons(iRemoteTCPPort);
		YIELD();
		
		SOCKET_NOBLOCK(s);   //Set Non-blocking mode
		ET_TOUT=300L;  //300ms, ET_TOUT has less than 800ms (WatchDog reboot time)
		iRet=connect(s, (struct sockaddr *)&sktRemoteServer, sizeof(sktRemoteServer));
		if(iRet) // unsuccessful connection not 0
		{
			lStartTimeTick=GetTimeTicks();
			
			if(errno==EINPROGRESS)
			{
				while((GetTimeTicks()-lStartTimeTick)<lConnect_Timeout)
				{
					RefreshWDT();
					YIELD();
					if(SOCKET_ISOPEN(s))
					{
						ET_TOUT=300L;
						iTemp=s;
						break;
					}
					else
					{
						//Print("Open socket error\n\r");
						iTemp=-1;
					}
				}
				if(iTemp==-1)
					closesocket(s);    
				s=iTemp;
			}
			else
			{
				closesocket(s);
				s=-1; 
			}
		}
	}
	else
		s=-1;

	if(s>=0)
	{
		SOCKET_BLOCK(s);
		FD_SET(s,&c_fds);
		return s;     
	}
	else
	{
		RefreshWDT(); 
		return -1;
	}
}

void main(void)
{
	int iRet;
	int iSocket,err,iLength;	
	char sInBuffer[100];
	
	InitLib();
	
	//Step1. Initial the Ethernet.
	iRet=NetStart();
	if(iRet<0)
	{
		Print("Initialize TCP Server failed \n\r");
		return;
	}
	
	//Step2. Connect to remote 7186EX/8000E or PC.
reconnect:
	iSocket=Ethernet_Connect("192.168.255.1",10000, 1000); //IP:192.168.255.1, Port:10000, Connection timeout:1000ms
	if(iSocket>=0)
	{
		Print("Connect ok, socket=%d\n\r", iSocket);
	}
	else
	{
		Print("Connect error.\n\r");
		goto reconnect;
	}
	
	//Step3. Install a timer interrupt function.  
	InstallUserTimerFunction_us(10000, Interrupt_Fun);//install timer interrupt.

	//Step4. Begin the loop function
	for(;;)
    {
        if(iLoop>=500) //Send "11test" to the remote 7186EX/8000E or PC every 500 ms.
        {
            YIELD();
            iRet= send(iSocket, "11test", 6, 0); //6 bytes, 0:MSG_OOB
            YIELD();
            iLoop=0;
        }
        
		iLength=0;
		if(FD_ISSET(iSocket,&c_fds))
		{
			if(!ioctlsocket(iSocket,FIONREAD,&err) && (err>0))
			{
				iLength=recv(iSocket,sInBuffer,50,0);
				if(iLength<= 0)
				{
					shutdown(iSocket,2);
					closesocket(iSocket);
					FD_CLR(iSocket,&c_fds);	//remove socket
					Print("Reconnect \n\r");
					goto reconnect;
				}
				else
				{
					sInBuffer[iLength]=0;
					Print("%s\n\r", sInBuffer);
				}
			}
		}
        //Press ESC to terminate the program.
        if(Kbhit() && Getch()==27)
		{
			shutdown(iSocket,2);
			closesocket(iSocket);
			FD_CLR(iSocket,&c_fds);	//remove socket
			StopUserTimerFun();
			Nterm();
			return;
		}
    }
}