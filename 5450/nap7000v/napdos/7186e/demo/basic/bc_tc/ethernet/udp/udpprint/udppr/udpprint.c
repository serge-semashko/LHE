#include <stdlib.h>
#include <stdio.h>
#include "..\..\..\lib\7186e.h"
#include "..\..\..\lib\tcpip32.h"

typedef char *va_list;
typedef int acpi_native_int;

#define _AUPBND (sizeof (acpi_native_int) - 1)
#define _ADNBND (sizeof (acpi_native_int) - 1)

#define _bnd(X, bnd) (((sizeof (X)) + (bnd)) & (~(bnd)))
#define va_arg(ap, T) (*(T *)(((ap) += (_bnd (T, _AUPBND))) - (_bnd (T,_ADNBND))))
#define va_end(ap) (void) 0
#define va_start(ap, A) (void) ((ap) = (((char *) &(A)) + (_bnd (A,_AUPBND))))

struct sockaddr_in sin;

struct ip host_ip={{192, 168, 255, 2}, {255, 255, 0, 0}};

int Init_UDPConnection(char *IPaddr)
{
    int iRet, UDPSkt;
    GetSavedIp((struct ip*) &host_ip.addr);
    if(SUCCESS==lhip("I7186EX", &host_ip)){
        //Print("set ip address OK.\n\r");
    }
    // mandatory Network Environment
    iRet=Ninit();    // initiate host environment
    if(iRet<0)
    {
        //Print("Ninit() failed! error %d\n\r", iRet);
        return -1;
    }
    iRet=Portinit("*");  // initiate network device
    if(iRet<0)
    {
        //Print("Portinit() failed! error %d\n\r", iRet);
        Nterm();
    
        return -2;
    }
    iRet=UDPSkt=socket(PF_INET, SOCK_DGRAM, 0);     // UDP/IP
    if(iRet<0)
    {
        //printCom1("can't create socket! error %d\r\n", iRet);
        return -3;
    }
    // addressing for master socket
    memset(&sin, 0, sizeof(sin));   // bzero is a unix system call
    sin.sin_family=AF_INET;
    sin.sin_addr.s_addr=inet_addr(IPaddr);
    sin.sin_port=htons(23);

    iRet=connect(UDPSkt, (struct sockaddr *) &sin, sizeof(sin));
    if(iRet<0)
    {
        closesocket(UDPSkt);
        
        //printCom1("connect() failed: %d\n", iRet);
        return -4;
    }
	iRet=Nwrite(UDPSkt," ", 1);
	if(iRet!=1)
    {
        closesocket(UDPSkt);
        
        //printCom1("Nwrite() failed: %d\n", iRet);
        return -5;
    }	
    return UDPSkt;
}

char sprint_buf[1024];
int udpPrint(int skt, const char *fmt, ...)
{
    va_list args;
    int n, iRet;
    
    va_start(args, fmt);
    n=vsprintf(sprint_buf, fmt, args);
    va_end(args);
    iRet=Nwrite(skt, sprint_buf, n);
    
    return iRet;
}