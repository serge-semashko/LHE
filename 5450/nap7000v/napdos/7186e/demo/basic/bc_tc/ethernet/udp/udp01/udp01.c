/*
UDP01.c: Responds to the message received from a UDP client.
         Listen port is 10000
          
Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.
      
Compile mode: large

Project: UDP01.c 
         ..\..\lib\7186el.Lib
         ..\..\lib\tcp_dm32.lib 

Hardware: 7186EX series
    
[Nov 03, 2008] by Liam
[Aug,3,2011] by Nicholas
*/

#include "..\..\lib\7186e.h"
#include "..\..\lib\Tcpip32.h"

uchar InBuf[1500], OutBuf[1500];
unsigned char MyIp[4];

void main(int argc, char *argv[])
{   
    int skt;
    int err, quit=0;
    int DataBytes;
  
    InitLib();  
    
	err=NetStart();
	if(err<0)
	{
		Print("Init Ethernet error.\n\r");
		return;
	}
	GetIp(MyIp);
    Print("IP=%d.%d.%d.%d\r\n", MyIp[0], MyIp[1], MyIp[2], MyIp[3]);	

    skt=Nopen("*", "UDP/IP", 10000, 0, S_NOCON | S_NOWA);
    if(skt<0)
    {
        Print("Cannot open a connection for UDP/IP!");
        Nterm();
        
        return;
    }  
  
    Print("Socket=%d\r\n", skt);
    SOCKET_NOBLOCK(skt);
  
    while(!quit)
    {
        YIELD();    // must add this line in the while loop
        
        DataBytes=Nread(skt, InBuf, 1500);
        if(DataBytes>0)
        {
			Print("%s\n\r",InBuf);
            memcpy(OutBuf, InBuf, DataBytes);
            Nwrite(skt, OutBuf, DataBytes);
        }

        if(Kbhit())
        {
            switch(Getch())
            {
                case 'q':
                case 'Q':
                    quit=1;
                    break;
            }
        }
    }

    Nclose(skt);
    Nterm();
}