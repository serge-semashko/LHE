/*
7k87k_di.c :Demo program for 7K and 87K DI module connected to COM port of 7188/8000.
            User must use DCON Utility to set Address, Baudrate,CheckSum
            before running the demo.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.
      
Compile mode: large

Project: 7k87k_di.c 
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)

Hardware: i-7188/uPAC-7186/i-8000 <== COM port ==> 7K or 87K modelue 

[Oct 30, 2008] by Liam
*/

#include <stdio.h>
#include "..\..\lib\7186e.h"

int Read7K87K_DI(int iComPort, int iAddress, int iTotalChannel, int iCheckSum, int iTimeout, unsigned long *Value);

void main(void)
{
    int iAddress, iRet,iTotalChannel, iComPort, iCheckSum, iTimeout;
    unsigned char cTemp[10];
    unsigned long lValue;   
    unsigned long lBaudrate;
    
    InitLib();
        
    Print("********************************\r\n");
    Print("*   87K_DI demo for Com Port   *\r\n");
    Print("*                              *\r\n");
    Print("*        [Oct 30, 2008]        *\r\n");
    Print("********************************\r\n");
    Print("\r\n");
    
    Print("Please Input ComPort Number:");
    LineInput(cTemp, 10);
    sscanf(cTemp, "%d", &iComPort);  
        
    Print("Please Input ComPort's Baudrate:");
    LineInput(cTemp, 10);
    sscanf(cTemp, "%ld", &lBaudrate);
    
    Print("Please Select Checksum(0:Disable or 1:Enable):");
    LineInput(cTemp, 10);
    sscanf(cTemp, "%d", &iCheckSum);
    
    Print("Please Input module's address(0~255) =");
    LineInput(cTemp, 10);
    sscanf(cTemp, "%d", &iAddress);
    
    Print("Please Input module's channel number(0~32) =");
    LineInput(cTemp,10);
    sscanf(cTemp, "%d", &iTotalChannel);
    
    Print("Please Input Timeout:");
    LineInput(cTemp, 10);
    sscanf(cTemp, "%d", &iTimeout);
    
    InstallCom(iComPort, lBaudrate, 8, 0, 1);
    
    // Begin to read DI
    for(;;)
    {    
        iRet=Read7K87K_DI(iComPort,iAddress,iTotalChannel,iCheckSum,iTimeout,&lValue);  
                
        if(iRet==NoError)
        {
            Print("DI data = %-8lX\n\r",lValue);
        }  
        else
        {
            Print("DI receive response error. Error Code = %d\n\r",iRet);
        }  
    }
}

//=====================================================================
//====== Following functions are used to read 87K DI modules        ===
//====== You can copy the functions to your own program.            ===
//=====================================================================
int Read7K87K_DI(int iComPort, int iAddress, int iTotalChannel, int iCheckSum, int iTimeout, unsigned long *Value)
{
    /*
    DCON protocol to read DI value
    Send command: @AA
    valid Response: >(DI Data)
    
    iComPort: 1~8                   
    iAddress: 0 ~ 255               
    iCheckSum: 0=Disable or 1=Enable
    iTimeout: unit=ms
    *Value: pointer to record DI data
    */
    
    int iRet, iLenth;
    unsigned long lDIdata, lBit;
    unsigned char InBuf[20], OutBuf[10];
    
    sprintf(OutBuf,"@%02X", iAddress);
    SendCmdTo7000(iComPort, OutBuf, iCheckSum);
    iRet=ReceiveResponseFrom7000_ms(iComPort, InBuf, iTimeout, iCheckSum);     

    if(iRet==NoError)
    {
        if(InBuf[0]=='>')   // Valid response first character: >
        {
            if(iTotalChannel<=8)
            {
                sscanf(InBuf+1,"%lX",&lDIdata);
                lDIdata=lDIdata >> 8; 
                *Value=lDIdata;
                
            }
            else
            {
                sscanf(InBuf+1,"%lX",&lDIdata);
                *Value=lDIdata;
            }
            return NoError;
        }
        else    
        {
           return -1;   // Response string error.
        } 
    }      
    else   
        return iRet;
}
           
           