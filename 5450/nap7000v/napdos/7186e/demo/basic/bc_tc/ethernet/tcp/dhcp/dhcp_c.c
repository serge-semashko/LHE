/*
Project: DHCP Demo
    
Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          
Compile mode: large

Project: DHCP_C.c
         ..\lib\7186el.lib
         ..\lib\tcp_dm32.lib

    The Dynamic Host Configuration Protocol (DHCP) provides configuration parameters to Internet hosts.
    DHCP consists of two components: a protocol for delivering host-specific configuration  parameters 
    from a DHCP server to a host and a mechanism for allocation of network addresses to hosts.

    DHCP is built on a client-server model, where designated DHCP server hosts allocate network addresses 
    and deliver configuration parameters to dynamically configured hosts.
    Throughout the remainder of this document, the term "server" refers to a host providing
    initialization

                Client                         Server
                  |                              |
          Begins initialization                  |
                  |                              |
                  |----------------------------->|
                  |         DHCPDISCOVER         |
                  |                          Determines
                  |                         configuration
                  |                              |
                  |<---------------------------- |
                  |           DHCPOFFER          |
                  |                              |
                  |                              |
                  |----------------------------->|
                  |          DHCPREQUEST         |
                  |                              |
                  |                    Commits configuration
                  |                              |
                  |<-----------------------------|
                  |            DHCPACK           |
                  |                              |
        Initialization complete                  |
                  |                              |
                  |----------------------------->|
                  |          DHCPRELEASE         |
                  |                        Discards lease
                  |               |              |
             Timeline diagram of messages exchanged between DHCP 
             client and servers when allocating a new network address

    Follow the steps below to get the information from DHCP server.
    Step1:
        Set 'bUseDhcp=1' to use the DHCP client function
    Step2:
        Declare the 3 variables for saving the new address.
        for using DHCP, Set the IP address/MASK to 0 before call Ninit().
    Step3:
        Set DhcpLeaseTime=100" 
        (100 means the desired length of lease time (Unit: sec))
    Step4
        Install_DHCP(); // must call Install_DHCP() for DHCP function to work. 
        Hardware: 7186E series

    Note:
        If the program NEED NOT the DHCP client functions, need not to call Install_DHCP(),
        then the codes for DHCP & DNS will not be link into the .exe file.

    Refer to RFC1533 and RFC2131 for more details about DHCP options
        http://www.faqs.org/rfcs/rfc1533.html
        http://www.apps.ietf.org/rfc/rfc2131.html
    for more details.

[May 2, 2008] by Liam  
[28, July, 2011] by Nicholas
*/

#include <stdlib.h>
#include "..\..\lib\7186e.h"
#include "..\..\lib\Tcpip32.h"

extern struct NETDATA *NetHost, *NetGateway;

int Ethernet_Init(void)
{
    /*
    Return:
         0: ok.
        -1: function "llip" error
        -2: function "Ninit" error
        -3: function "Portinit" error
    */
    int iRet;
    struct ip host_ip;

    GetSavedIp((struct ip*) &host_ip);
    iRet=lhip("7186E", &host_ip);
    if(iRet!=NoError)
        return -1;

    if(bUseDhcp)
    {
        // for using DHCP, Set the IP address, Mask and Gateway to 0 before call Ninit().
        *(long *) NetHost->Iaddr.c=
        *(long *) NetHost->Imask.c=
        *(long *) NetGateway->Iaddr.c=0L;
        
        DhcpLeaseTime=20;   // by default DhcpLeaseTime=0,
                            // that is the time is assigned by the DHCP server.
                            // the time unit is second.
        Install_DHCP();     // must call Install_DHCP() for DHCP function to work.
    }
    
    iRet=Ninit();
    if(iRet!=NoError)
        return -2;
    
    iRet=Portinit("*");     /* initiate network device */
    if(iRet<NoError)
    {
        Nterm();
        return -3;
    }

    return 0;
}

void main(void)
{
    struct ip *host_ip;
    STOPWATCH swDhcp;
    int err;

    //Step1. Initate the controller.
    InitLib();
    bUseDhcp=1;     // 0: use the IP stored in the EEPROM
                    // 1: use the DHCP client function
        
    //Step2. Initial the Ethernet adapter.
    err=Ethernet_Init();
    if(err==NoError)
        Print("Init Ethernet ok.\r\n");
    else
        Print("Init Ethernet error.\r\n");
    
    T_StopWatchStart(&swDhcp);  // start a timer for DHCP

    for(;;)
    {
        YIELD();
        if(nets[0].DHCPserver)
        {
            if(T_StopWatchGetTime(&swDhcp)>=nets[0].RenewalTime)
            {
                int rc;

                DisableWDT();
                rc=DHCPget(0, DhcpLeaseTime);
                EnableWDT();
                if(rc>=0)
                {
                    T_StopWatchStart(&swDhcp);
                }
                else 
                {
                    Print("[DHCP renewal error=%d]\r\n", rc);
                }
                
                Print("DHCP release successfully!!\r\n\r\n");
                Print("Set new IP to: %d.%d.%d.%d\r\n", NetHost->Iaddr.c[0], NetHost->Iaddr.c[1], NetHost->Iaddr.c[2], NetHost->Iaddr.c[3]);
                Print("Set new Mask to: %d.%d.%d.%d\r\n", NetHost->Imask.c[0], NetHost->Imask.c[1], NetHost->Imask.c[2], NetHost->Imask.c[3]);
                Print("Set new Gateway to: %d.%d.%d.%d\r\n", NetGateway->Iaddr.c[0], NetGateway->Iaddr.c[1], NetGateway->Iaddr.c[2], NetGateway->Iaddr.c[3]);
            }               
        }
                
        if(Kbhit())     //Press any key to quit the program
        { 
            Getch();
            break;
        }
        
        RefreshWDT();
    }
    
    Nterm();
}