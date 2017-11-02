/* 
Project: Client ping the server
    
Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          
Compile mode: large

Project: PING.c
         ..\lib\7186EL.lib
         ..\lib\tcp_dm32.lib
         
ICMP(Internet Control Message Protocol)
is normally used to communicate error messages between IP nodes,
both routers and hosts,but it is occasionally used by applications.

0--------7 8----------15 16------------31
   type       code            checksum

Ping type: 8 echo request
     type: 0 echo reply

TimeStamp  type: 13 timestamp request
           type: 14 timestamp reply

Example: ping 192.168.30.11 (remote host ip)
        
[Nov 3, 2008] by Liam 
[Aug,3,2011] by Nicholas
*/

#include <stdlib.h>
#include "..\..\..\lib\7186e.h"
#include "..\..\..\lib\Tcpip32.h"

void main(int argn, char *argv[])
{
    int i, err, stat, conno, recsiz=64;
    long li1;
    unsigned long *ulp, ul1;
    unsigned char *cp;
    static char buff[MAXBUF];
    char *pname;

    struct pingm {
        struct ICMPhdr hdr; 
        unsigned long ul[MAXBUF/4];
    }pingm, *pp;
    
    InitLib();

    // Initial the Ethernet adapter
    err=NetStart();
    if(err==NoError)
        Print("Init Ethernet ok.\r\n");
    else
    {
        Print("Init Ethernet error.\r\n");
        return;
    }
    
    if(argn==1)
    {
        Print("syntax: ping n.n.n.n\r\n");
        return;
    }

    for(i=1; i<argn; i++)
    {
        if(argv[i][0]=='/')
        {
            if(argv[i][1]=='s' || argv[i][1]=='S')
            {
                i++;
                recsiz=atoi(argv[i]);
            }
            else
            {
                Print("syntax: ping n.n.n.n\r\n");
                return;
            }
        }
        else
            pname=argv[i];
    }

    pingm.hdr.type=IC_ECHO;
    pingm.hdr.scode=0;
    pingm.hdr.chksum=0;
    ulp=(unsigned long *) pingm.hdr.param;
    ulp[0]=1;
    ulp[1]=2;
    ulp[2]=3;
    ulp[3]=4;
    
    conno=Nopen(pname, "ICMP/IP", 0, 0, 0);
    if(conno<0)
    {
        Print("connection fails\r\n");
        return;
    }

    i=MAXBUF-MESSH_SZ-LHDRSZ-28;
    if(recsiz>i)
    {
        Print("largest usable size %d\r\n", i);
        return;
    }

    SOCKET_RXTOUT(conno, 0);
    for(li1=1; ; li1++)
    {
        stat=0;
        Nwrite(conno, (char *) &pingm, recsiz+8);

        for(ul1=TimeMS(); TimeMS()-ul1<1000; )
        {
            YIELD();
            i=Nread(conno, buff, sizeof(buff));

            if(i<=0)
                continue;

            stat=1;
            pp=(struct pingm *) buff;

            if(pp->hdr.type!=IC_EREP)
            {
                Print("funny response\r\n");
                return;
            }

            cp=SOCKET_IPADDR(conno).c;
            Print("%4ld reply from %d.%d.%d.%d\r\n", li1, cp[0], cp[1], cp[2], cp[3]);
        }

        if(stat==0)
            Print("%4ld no reply\r\n", li1);
            
        if(Kbhit())
            if(Getch()=='q')    // press the 'q' key to quit
                break;
    }
    
    Nclose(conno);
    Nterm();
}

