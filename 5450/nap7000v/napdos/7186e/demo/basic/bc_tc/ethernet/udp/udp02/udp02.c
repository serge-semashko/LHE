/* 
UDP02.c: Use UDP send message to IP:10000. (A client program for UDP01.EXE)
         COM1 input ESC(27) to quit the program.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.
      
Compile mode: large

Project: UDP02.c 
         ..\..\lib\7186el.Lib
         ..\..\lib\tcpip32.lib 

Details:        
    run: UDP02 [server_ip]
         for example: 
         1. udp02                --> the default server ip=192.168.186.187
         2. udp02 192.168.255.2  --> connect to 192.168.255.2

Hardware: 7186EX series
    
[Nov 04, 2008] by Liam
[Aug,3,2011] by Nicholas
*/

#include <stdlib.h>
#include "..\..\lib\7186e.h"
#include "..\..\lib\Tcpip32.h"

uchar InBuf[1500], OutBuf[1500];
int OutIdx=0;
unsigned char MyIp[4];
//char *ServerIp="192.168.186.187";
char *ServerIp="10.1.0.60";

void main(int argc, char *argv[])
{ 
    int skt;
    int err, quit=0;
    int DataBytes;
    
    InitLib();
  
    if(argc>1)
    {
        ServerIp=argv[1];
    }
  
    err=NetStart();
	if(err<0)
	{
		Print("Init Ethernet error.\n\r");
		return;
	}
	GetIp(MyIp);
    Print("IP=%d.%d.%d.%d\r\n", MyIp[0], MyIp[1], MyIp[2], MyIp[3]);

    skt=Nopen(ServerIp, "UDP/IP", 10000, 0, S_NOCON | S_NOWA);
    if(skt<0)
    {
        Print("Cannot open a connection for UDP/IP!");
        Nterm();
        
        return;
    }  
  
    Print("Socket=%d\r\n", skt);
    SOCKET_NOBLOCK(skt);
    
    Print("\r\n[Send]: ");
    while(!quit)
    {
        YIELD();    // must add this line in the while loop
    
        DataBytes=Nread(skt, InBuf, 1500);
        if(DataBytes>0)
        {
            InBuf[DataBytes]=0;
            Print("Receive: "); 
            Print(InBuf);
            Print("\r\n[Send]: ");
        }

        if(Kbhit())
        {
            int data=Getch(); 
            switch(data)
            {
                case 27:
                    quit=1;
                    break;
                
                case '\r':
                    OutBuf[OutIdx++]=data;
                    Putch('\r');
                    Putch('\n');
                    Nwrite(skt, OutBuf, OutIdx);    
                    OutIdx=0;
                    break;
                
                default:
                    OutBuf[OutIdx++]=data;
                    Putch(data);
                    break; 
            }
        }
    }

    Nclose(skt);
    Nterm();
}