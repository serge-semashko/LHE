/* *****************************************************
Project: Client ping the server

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          
Compile mode: large

Project: ICMP.c
         ..\lib\7186el.lib
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
    Time server:
    time-a.nist.gov     129.6.15.28  
    time-b.nist.gov     129.6.15.29
    time.windows.com    207.46.248.43

[Nov 3, 2008] by Liam 
[Aug,3,2011] by Nicholas
*/

#include <stdlib.h>
#include "..\..\..\lib\7186e.h"
#include "..\..\..\lib\Tcpip32.h"

#define TaiwanTime  8   // GMT+8.00

struct pingm {
    struct ICMPhdr hdr;
    unsigned long ul[MAXBUF/4];
}pingm, *pp;

struct IcmpTime {
    unsigned short Identifier;
    unsigned short SequenceNumber;
    unsigned long OriTimeStamp;
    unsigned long ReceiveTimeStamp;
    unsigned long TransmitTimeStamp;
};

void main(int argn, char *argv[])
{
    int i, err, stat, conno, recsiz=64;
    long li1;
    unsigned long *ulp, ul1, temp;
    unsigned long FinishTimeStamp;
    unsigned char *cp;
    static char buff[MAXBUF];
    char *pname;
    unsigned seg=0;
    unsigned int hour, minu, sec;
    struct IcmpTime *timestamp;

    InitLib();
    
    // Initial the Ethernet adapter	
    err=NetStart();
	/*
    Return:
         0: ok.
        -1: function "llip" error
        -2: function "Ninit" error
        -3: function "Portinit" error
    */
    if(err==NoError)
        Print("Init Ethernet ok.\r\n");
    else
    {
        Print("Init Ethernet error: %d.\r\n",err);
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

    pingm.hdr.type=IC_TIMEQ;    // IC_ECHO
    pingm.hdr.scode=0;
    pingm.hdr.chksum=0;

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
        stat = 0;
        timestamp=pingm.hdr.param;
        timestamp->Identifier=htons(7186);
        timestamp->SequenceNumber=htons(seg++);
        timestamp->OriTimeStamp=TimeMS();
        Nwrite(conno, (char *)&pingm, recsiz+8);
        
        for(ul1=TimeMS(); TimeMS()-ul1<1000; )
        {
            YIELD();
            i=Nread(conno, buff, sizeof(buff));
        
            FinishTimeStamp=TimeMS();
            if(i<=0)
                continue;
        
            stat=1;
            pp=(struct pingm *) buff;
        
            if(pp->hdr.type!=IC_TIMER)  //IC_EREP
            {
                Print("funny response\r\n");
                return;
            }
        
            cp=SOCKET_IPADDR(conno).c;
            Print("%ld[%d.%d.%d.%d]", li1, cp[0], cp[1], cp[2], cp[3]);
            timestamp=pp->hdr.param;
            Print("id=%u seq#=%u Ori=%lu rec=%lu Trans=%lu ,Finish=%lu\n\r",
            htons(timestamp->Identifier),htons(timestamp->SequenceNumber),
            timestamp->OriTimeStamp,htonl(timestamp->ReceiveTimeStamp),
            htonl(timestamp->TransmitTimeStamp),FinishTimeStamp);
        
            // some server need not to change the byte order
            // Print("id=%u seq#=%u Ori=%lu rec=%lu Trans=%lu\n\r",
            //        htons(timestamp->Identifier),htons(timestamp->SequenceNumber),
            //        htonl(timestamp->OriTimeStamp),timestamp->ReceiveTimeStamp,
            //        timestamp->TransmitTimeStamp);
        }

        if(stat==0)
        Print("%4ld no reply\n\r", li1);

        if(Kbhit())
            if(Getch()=='q')    // press the 'q' key to quit
                break;
    }
    
    Nclose(conno);
    Nterm();
        
    temp=(htonl(timestamp->ReceiveTimeStamp)+1)+(FinishTimeStamp-timestamp->OriTimeStamp)/2;

    Print("temp: %ld\r\n", temp);
    hour=(temp/1000)/3600;  //60*60
    minu=((temp/1000)-hour*60*60)/60;   //60f
    sec=(temp/1000)-hour*60*60-minu*60; 
    SetTime((hour+TaiwanTime)%24, minu, sec);
    Print("New Time hour:%d ,minu:%d ,sec:%d\r\n", (hour+TaiwanTime)%24, minu, sec);  
    
    Print("user terminated\r\n");
    Nclose(conno);
    Nterm();
}

