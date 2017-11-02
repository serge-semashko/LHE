/*
UDP_Client.c: UDP client demo program for 7186EX series module
          
Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.
      
Compile mode: large

Project: UDP_Client.c 
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
    
[Nov 03, 2008] by Liam
[Aug,3,2011] by Nicholas
*/

#include "..\..\lib\7186e.h"
#include "..\..\lib\Tcpip32.h"

#define BUFSIZE     1024        // read/write buffer size
#define SERVICE     10000       // define the service port for BBS Server
#define SOCKETS     32          // define the maximum number of sockets
#define QLEN    (SOCKETS-1)     // maximum connection queue length

void ProcessCmd(int socket, unsigned char *cmd);
char *Prompt="7186EX>";

int Quit=0;
fd_set rfds;    // set of socket file descriptors for reading

unsigned long t1;
struct sockaddr_in sin;     

char *ipaddr="192.168.186.189";
struct ip host_ip={{192, 168, 255, 2}, {255, 255, 0, 0}};

// when client is connected, receive data
int do_receive( int skt )
{
    int err=0;
    int sin_len=sizeof(sin);
    char buf[BUFSIZE];

	memset(buf,'\0',sizeof(buf));
    err=recvfrom(skt, buf, sizeof(buf), 0, (struct sockaddr*) &sin, &sin_len);

    if(err==0)
    {
        // disconnected
        shutdown(skt, 0);
        closesocket(skt);
        
        FD_CLR(skt, &rfds);
        
        return err;
    }
    else if(err<0)
    {
        // read error
        shutdown(skt, 0);
        closesocket(skt);
        
        FD_CLR(skt, &rfds);
    
        return err;
    }
    else      
        Print("%s\r\n", buf);  
    
    return 0;  
}

void main(int argc, char *argv[])
{
    struct sockaddr_in ssin;    // client Internet endpoing address
    int sin_len;                // length of sockaddr_in
    int client;          
    int ss;         // slave socket got from accept
    int type, protocol, err=0;
    int rc;         // return code of selectsocket
    int i;
    
    InitLib();
  
    if(argc>1)
        ipaddr=argv[1];
  	
	err=NetStart();
	if(err<0)
	{
		Print("Init Ethernet error.\n\r");
		Nterm();
		return;
	}

    err=client=socket(PF_INET, SOCK_DGRAM, 0);      // UDP/IP
    if(client<0)
    {
        Print("can't create socket! error %d\r\n", err);
        Nterm();        
        return;
    }

    // addressing for master socket
    memset(&sin, 0, sizeof(sin));   // bzero is a unix system call
    sin.sin_family=AF_INET;
    sin.sin_addr.s_addr=inet_addr(ipaddr);
    sin.sin_port=htons(SERVICE);
    
    t1=GetTimeTicks();

    if(connect(client, (struct sockaddr *) &sin, sizeof(sin)))
    {
        // on success return 0
        extern int errno;
        
        Print("connect() failed: %d\n", errno);
        return;
    }
    else
        Print("connect() success, time=%lu ms\n", GetTimeTicks()-t1);

    while(!Quit)
    {
        if(Kbhit())
        {
            switch(Getch())
            {
                case 'q':
                case 'Q': 
                    Quit=1;
                    break;
                
                case 'a':
                case 'A': 
                    err=writesocket(client, "10", 2);
                    do_receive(client);
                    break;
                
                case 'b':
                case 'B': 
                    err=writesocket(client, "01", 2);
                    do_receive(client);
                    break;
                
                default:
                    err=writesocket(client, "[1234567890]", 12);
					do_receive(client);
                    break;
            }
        }       
    }
    
    closesocket(client);
    Nterm();
}


