/*
7k87k_do.c :Demo program for 7K and 87K DO module connected to COM port of i-7188/uPAC-7186/i-8000.
            User must use DCON Utility to set Address, Baudrate,CheckSum
            before running the demo.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.
      
Compile mode: large

Project: 7k87k_do.c 
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)

Hardware: i-7188/uPAC-7186/i-8000 <== COM port ==> 7K or 87K modelue 
    
[Oct 30, 2008] by Liam
*/

#include <stdio.h>
#include "..\..\lib\7186e.h"

int Write7K87K_DO(int iComPort, int iAddress, int TotalChannel, int iCheckSum, int iTimeout, unsigned long iValue);
int Read7K87K_DO(int iComPort, int iAddress, int iTotalChannel, int iCheckSum, int iTimeout, unsigned long *Value);

void main(void)
{
    int iRet,iAddress, iTotalChannel, iComPort, iCheckSum, iTimeout;
    int iValue;
    unsigned char cTemp[10];
    unsigned long lBaudrate; 
    unsigned long lDOdata,lReadBackData;
    
    InitLib();
    
    Print("*********************************\r\n");
    Print("*  7K87K_DO demo for Com Port   *\r\n");
    Print("*                               *\r\n");
    Print("*         [Oct 30, 2008]        *\r\n");
    Print("*********************************\r\n");
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
    
    Print("Please Input Timeout:");
    LineInput(cTemp, 10);
    sscanf(cTemp, "%d", &iTimeout);
    
    Print("Please Input Total DO Channel Number =");
    LineInput(cTemp, 10);            
    sscanf(cTemp, "%d", &iTotalChannel);  
    
    InstallCom(iComPort,lBaudrate,8,0,1);

    // Begin to write DO.
    for(;;)
    {
        Print("Please Input DO Data(Hex) = ");
        LineInput(cTemp,10); 
        sscanf(cTemp,"%lX",&lDOdata);
        
        iRet=Write7K87K_DO(iComPort, iAddress, iTotalChannel, iCheckSum, iTimeout, lDOdata);
            
        if(iRet==NoError)
        {
            Delay(10); /*
                       Wait for DO status is changed.
                       If you don't need to confirm the DO is 
                       changed correctly, remove following codes.
                       */
            iRet=Read7K87K_DO(iComPort,iAddress,iTotalChannel,iCheckSum,iTimeout,&lReadBackData);
            
            if(iRet==NoError)
            {
                Print("Write DO success. The read back value is %-8lX.\n\r",lReadBackData);
            }
            else
            {
                Print("Read back DO error.\n\r ");
            }
        }
        else 
        {
            Print("Write DO error. Error Code = %d.\n\r",iRet);
        }
    }
}

//=====================================================================
//====== Following functions are for 87K DO modules                 ===
//====== You can copy the functions to your own program.            ===
//=====================================================================
int Write7K87K_DO(int iComPort, int iAddress, int iTotalChannel, int iCheckSum, int iTimeout, unsigned long lValue)
{
    /*
    DCON protocol to write DO value
    Send command: @AA(Data)
    valid Response: >
     
    iComPort: 1~8                   
    iAddress: 0 ~ 255               
    iCheckSum: 0=Disable or 1=Enable
    iTimeout: unit=ms
    iTotalChannel: 1~32
    */    
    int iRet, i;
    unsigned char OutBuf[20], InBuf[20];
    unsigned long iMask=0, x;
    
    // Creat DO mask.  For example: if DO channel= 5, the mask is 00011111.
    for(i=0; i<iTotalChannel; i++)  //Distinguish from channel number
    {
        x=1UL<<i;
        iMask+=x;
    }

    if(iTotalChannel<=8)
    {
        sprintf(OutBuf, "@%02X%02X", iAddress, (lValue & iMask));
    }
    else if(iTotalChannel <= 16 && iTotalChannel > 8)
    {
        sprintf(OutBuf, "@%02X%04X", iAddress, (lValue & iMask));  
    }
    else
    {
        sprintf(OutBuf, "@%02X%08lX", iAddress, (lValue & iMask));
    }
    
    iRet=SendCmdTo7000(iComPort, OutBuf, iCheckSum); 
    
    iRet=ReceiveResponseFrom7000_ms(iComPort, InBuf, iTimeout, iCheckSum);
    
    if(iRet==NoError)
    {
        if(InBuf[0]=='>')      //Valid response first character: >
        {
            return NoError;
        }
        else
            return -1;  //Response string error.
    }
    else 
        return iRet;
}

int Read7K87K_DO(int iComPort, int iAddress, int iTotalChannel, int iCheckSum, int iTimeout, unsigned long *Value)
{
    /*
    DCON protocol to read DI value
      Send command: @AA
      valid Response: >(DI/DO Data)
      
      iComPort: 1~8
      iAddress: 0 ~ 255
      iCheckSum: 0=Disable or 1=Enable
      iTimeout: unit=ms
    */
    
    int iRet;
    unsigned long ReadBack_Data; 
    unsigned char InBuf[20], OutBuf[10];
    
    sprintf(OutBuf, "@%02X", iAddress);
    iRet=SendCmdTo7000(iComPort, OutBuf, iCheckSum);

    iRet=ReceiveResponseFrom7000_ms(iComPort, InBuf, iTimeout, iCheckSum);
    if(iRet==NoError)
    {
        if(InBuf[0]=='>')   // Valid response first character: >
        {
            if(iTotalChannel<=8)
            {
                sscanf(InBuf+1, "%lX", &ReadBack_Data);
                *Value=ReadBack_Data >> 8; 
            }
            else
            {
                sscanf(InBuf+1, "%lX", &ReadBack_Data);
                *Value=ReadBack_Data;
            }
            return NoError;
        }
        else
        {
           return -1;  //Response string error.
        } 
    }      
    else
        return iRet;
}   