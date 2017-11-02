/* 
Receive.c: Receive data from the COM port

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.
          
Compile mode: large

Project: Receive.c
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)

Detail description:
    Slv_COM.c and Receive.c are a little diffent. 
    Slv_COM.c is non-blocked mode,
    Receive.c is blocked mode.

Hardware: i-7188/uPAC-7186/i-8000

[Oct 30, 2008] by Liam
*/

#include <string.h>
#include <stdio.h>
#include "..\..\lib\7186e.h"

// Uses COM port to receive data with 0x0D [Enter].
int Receive_Data(int iPort,unsigned char* cInBuf,unsigned long lTimeout);

void main(void)
{
    int iLength,iValue;
    int iCommandCOMPort;
    float fValue;
    unsigned char cData[100];

    InitLib();   

    iCommandCOMPort=1;
    
    InstallCom(iCommandCOMPort, 115200, 8, 0, 1);
    
    ToComStr(iCommandCOMPort, "Input a value(integer: -32768~ +32767): ");
    iLength=Receive_Data(iCommandCOMPort, cData, 10000);    // timeout=10 seconds
    if(iLength>0)
    {
        sscanf(cData, "%d", &iValue);
        printCom(iCommandCOMPort, "\r\nValue=%d\r\n", iValue);
    }
    else
    {
        printCom(iCommandCOMPort, "Keyin timeout!\r\n");
    }

    printCom(iCommandCOMPort, "\r\nInput a value(float):");
    iLength=Receive_Data(iCommandCOMPort, cData, 10000);    // timeout=10 seconds
    if(iLength>0)
    {
        sscanf(cData, "%f", &fValue);
        printCom(iCommandCOMPort, "\n\rValue=%f\n\r", fValue);
    }
    else
    {
        printCom(iCommandCOMPort, "Keyin timeout!\n\r");
    }

    Delay(10);  // Wait for all data is transmited to COM port
    RestoreCom(iCommandCOMPort);
}

/*=====================================================================*/
/*==== Following functions are used to receive data from COM port.  ===*/
/*==== You can copy the functions to your own program.              ===*/
/*=====================================================================*/
/*
The following two functions are implemented by [block] method.

Block method: After calling the function, the CPU will be blocked in the
              function till it gets the 0x0D [Enter] or timeout.
*/
    
int Receive_Data(int iPort, unsigned char* cInBuf, unsigned long lTimeout)
{
    /* 
    Uses COM port to receive data with a terminative char.
    
    iPort:    COM port number to receive data.
              0: COM0, 1: COM1, 2: COM2  .....
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
    
    lStartTime=GetTimeTicks();
    for(;;)
    {
        while(IsCom(iPort))     // check COM port
        {
            cChar=ReadCom(iPort);
            if(cChar=='\r')     // the terminal char is 0x0D
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

int Receive_Data_Length(int iPort, unsigned char* cInBuf, int iLength, long lTimeout)
{
    /* 
    Uses COM port to receive string (fixed data length). 
    
    iPort:    COM port number to receive data.
              0: COM0, 1: COM1, 2: COM2  .....
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

    lStartTime=GetTimeTicks();
    for(;;)
    {
        while(IsCom(iPort))     // check COM port
        {
            cInBuf[iIndex++]=ReadCom(iPort);
            if(iIndex>=iLength)
            {
                cInBuf[iIndex]=0;
                return iIndex;  // return data length
            }
                
            lStartTime=GetTimeTicks();  // refresh data timeout
        }
        
        if((GetTimeTicks()-lStartTime)>=lTimeout)
            return -1;  // receive data timeout
            
        RefreshWDT();
    }
}