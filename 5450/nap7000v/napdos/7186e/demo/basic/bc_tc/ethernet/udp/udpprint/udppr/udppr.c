/*
UDPPr.c : Reads library version and print to console 7188eu.exe.

Compiler: BC++ 3.1,  
          Turbo C ++ 1.01(3.01) (free from http://community.borland.com/museum)
          MSC 6.0,
          MSVC 1.52.
          
Compile mode: large

Project: UDPPr.c
         ..\..\Lib\7186el.Lib 

Detail description:
    User can use MiniOS7 utility to load UDPPr.exe to uPac-7186E and show the data to console 7188eu.exe.

Hardware: uPac-7186E

[31/Aug/2010] by Nicholas
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\..\..\lib\7186e.h"

void main(void)
{ 
    int iVer;
	int i;
	int UDPconnect;
	int iNumber;
	float fNumber;
	double dDouble;	
	char cChar;
    char *str="The library version is ";
	char *str1="Hello uPac-7186E!";    
	
    InitLib();
	UDPconnect= Init_UDPConnection("192.168.255.1");//Set IP address for connection
	iNumber=121;
	fNumber=12.12121;
	dDouble=1234567.89;
	cChar='K';
	
	if(UDPconnect>= 0)//Connect success// UDPconnect: 0 ~ 31 socket
    {
        iVer=GetLibVersion();
		//Print data to console 7188eu.exe
		udpPrint(UDPconnect,"The integer iNumber is:");
		udpPrint(UDPconnect,"%d\n\r",iNumber);
		
		udpPrint(UDPconnect,"The float fNumber is:");
		udpPrint(UDPconnect,"%f\n\r",fNumber);
		
		udpPrint(UDPconnect,"The double dDouble is:");
		udpPrint(UDPconnect,"%e\n\r",dDouble);
		
		udpPrint(UDPconnect,"The char cChar is:");
		udpPrint(UDPconnect,"%c\n\r",cChar);
		
		udpPrint(UDPconnect,"%s",str);		
		udpPrint(UDPconnect,"%d\n\r",iVer);
		
		for(i=0; i<strlen(str1);i++)
		{			
			udpPrint(UDPconnect,"%c",str1[i]);
		}		
    }
    else
    {
		switch (UDPconnect)
		{
			case -1:
				Print("Ninit() failed!\n\r");
				break;
			case -2:
				Print("Portinit() failed!\n\r");
				break;
			case -3:
				Print("Can't create socket!\n\r");
				break;
			case -4:
				Print("Connect() failed!\n\r");
				break;
			default:
				Print("Hello PC! this program can't connect to PC.\n\r");
		}		
    }
}