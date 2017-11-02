/* 
Slv_COM.c: Slave COM port demo for (request/reply) or (command/response) application.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.
          
Compile mode: large

Project: Slv_COM.c
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)

Detail description:
    Slv_COM.c and Receive.c are a little diffent. One is non-blocked mode,
    one is blocked mode.
    
    PC sends commands to i-7188/uPAC-7186/i-8000, and get back responses is 
    a very typical application.
    In this kind appliction, one COM port of i-7188/uPAC-7186/i-8000 must 
    acts as a command decoder.
    This demo shows how to do this and how to use another COM port or LED 
    to show information to help you debug your program.

Hardware: i-7188/uPAC-7186/i-8000

[Oct 30, 2008] by Liam
*/

#include <string.h>
#include <stdio.h>
#include "..\..\lib\7186e.h"

// Uses COM port to receive data with a terminative char.
// If time between two byte grater than lTimeout, abort receiving.
int Receive_Data(int iPort, unsigned char* cInBuf, char cTerminator, long lTimeout);

void main(void)
{
    int iQuit=0;
    int iLength;
    int iCommandCOMPort,iDebugCOMPort;
    unsigned char cData[100];    
    
    InitLib();   

    iCommandCOMPort=1;
    iDebugCOMPort=2;
    
    InstallCom(iCommandCOMPort, 115200, 8, 0, 1);
    InstallCom(iDebugCOMPort, 115200, 8, 0, 1);
	
	printCom(iCommandCOMPort,"send command: c1(command1), c2(command2), q(quit program)\n\r");
    ToComStr(iDebugCOMPort, "Debug information: Start program\n\r");

    while(!iQuit)
    {
        iLength=Receive_Data(iCommandCOMPort, cData, 0xD, 3000);   // Terminator is 0xD= CR, timeout=3000 ms
        
        if(iLength>0)
        {
            if(!strcmp(cData, "c1"))
            {
                ToComStr(iDebugCOMPort,"Debug information: Command 1\n\r");
                ToComStr(iCommandCOMPort,"Command 1\n\r");
            }
            else if(!strcmp(cData, "c2"))
            {
                ToComStr(iDebugCOMPort, "Debug information: Command 2\n\r");
                ToComStr(iCommandCOMPort, "Command 2\n\r");
            }
            else if(!strcmp(cData, "q"))
            {   
                ToComStr(iDebugCOMPort, "Debug information: Quit program\n\r");
                Delay(100);     // Wait for all data are transmited to COM port
                iQuit=1;
            }
            else
            {
                printCom(iDebugCOMPort, "Debug information: Unknown command : %s\n\r", cData);
                printCom(iCommandCOMPort, "Unknown command : %s\n\r", cData);
            }
        }
        else if(iLength==-1)
        {
            ToComStr(iDebugCOMPort, "Debug information: Keyin timeout!!!\n\r");
            ToComStr(iCommandCOMPort, "Keyin timeout!!!\n\r");
        }
    }
    
    RestoreCom(iCommandCOMPort);
    RestoreCom(iDebugCOMPort);
}
    
/*=====================================================================*/
/*==== Following functions are used to receive data from COM port.  ===*/
/*==== You can copy the functions to your own program.              ===*/
/*=====================================================================*/
/*
The following two functions are implemented by [non-block] method.

Non-Block method: After calling the function, if there is no data in the 
                  COM port input buffer, the CPU skip the function and 
                  go to execute next code.
*/
    
int Receive_Data(int iPort, unsigned char* cInBuf, char cTerminator, long lTimeout)
{
    /* 
    Uses COM port to receive data with a terminative char.
    
    iPort:    COM port number to receive data.
              0:COM0, 1:COM1, 2:COM2  .....
    *cInBuf:  Input buffer to receive data.
    cTerminator: what is the last byte ?
    lTimeout: timeout to receive data. (Unit: ms)
              The timeout is measured from last received byte 
              to the terminator.
    return: >0: length of received data
             0: doen't receive any data
            -1: timeout
    */
    
    unsigned char cChar;
    int iIndex=0;
    unsigned long lStartTime;

    if(IsCom(iPort))
    {
        lStartTime=GetTimeTicks();
        for(;;)
        {
            while(IsCom(iPort))     // check COM port
            {
                cChar=ReadCom(iPort);
                if(cChar==cTerminator)  // the terminal char is 0x0D
                {
                    cInBuf[iIndex]=0;   // Add the zero end to the data
                    return iIndex;      // return data length
                                        // (doesn't include the cTerminator)
                }
                else
                    cInBuf[iIndex++]=cChar;
                    
                lStartTime=GetTimeTicks();  // refresh data timeout
            }
            if((GetTimeTicks()-lStartTime)>=lTimeout)
            {
                cInBuf[iIndex]=0;   // Add the zero end to the data
                return -1;  // receive data timeout
            }
                
            RefreshWDT();
        }
    }
    else
        return 0;
}

int Receive_Data_Length(int iPort, unsigned char* cInBuf, int iLength, long lTimeout)
{
    /* 
    Uses COM port to receive string (fixed data length). 
    
    iPort:    COM port number to receive data.
              0:COM0, 1:COM1, 2:COM2  .....
    *cInBuf:  Input buffer to receive data.
    iLength:  how many bytes to receive?
    lTimeout: timeout to receive data. (Unit: ms)
              The timeout is measured from last received byte 
              to receive whole data.
    return: >0: length of received data
             0: doen't receive any data
            -1: timeout
    */
    
    unsigned char cChar;
    int iIndex=0;
    unsigned long lStartTime;

    if(IsCom(iPort))
    {
        lStartTime=GetTimeTicks();
        for(;;)
        {
            while(IsCom(iPort))     // check COM port
            {
                cInBuf[iIndex++]=ReadCom(iPort);
                if(iIndex>=iLength)
                {
                    cInBuf[iIndex]=0;
                    return iIndex;      // return data length
                }
                    
                lStartTime=GetTimeTicks();  // refresh data timeout
            }
            if((GetTimeTicks()-lStartTime)>=lTimeout)
                return -1;  // receive data timeout
                
            RefreshWDT();
        }
    }
    else
        return 0;
}