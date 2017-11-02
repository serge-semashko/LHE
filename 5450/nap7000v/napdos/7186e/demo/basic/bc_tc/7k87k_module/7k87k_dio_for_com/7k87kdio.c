/*
7k87kdio.c: Demo program for 7K and 87K DIO module connected to COM port of i-7188/uPAC-7186/i-8000.
            User must use DCON Utility to set Address, Baudrate,CheckSum
            before running the demo.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.
      
Compile mode: large

Project: 7k87kdio.c 
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)

Hardware: i-7188/uPAC-7186/i-8000 <== COM port ==> 7K or 87K modelue 

[Oct 30, 2008] by Liam
*/

#include <stdio.h>
#include "..\..\lib\7186e.h"

int Write7K87K_DIO_DO(int iComPort, int iAddress, int TotalChannel, int iCheckSum, int Timeout, int DOdata);             
int Read7K87K_DIO(int iComPort, int iAddress, int TotalChannel, int iCheckSum, int Timeout, unsigned int *DIdata, unsigned int *ReadbackDO);
void main(void)
{
    int DOdata,TotalDOChannel,iRet,iComPort,iAddress,iCheckSum,iTimeout;
    unsigned char cTemp[10];
    unsigned int DIdata,ReadbackDO;
    unsigned long lBaudrate;          
    
    InitLib();
    
    Print("*********************************\r\n");
    Print("*  7K87K_DIO demo for Com Port  *\r\n");
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
    
    Print("Please Input Total DO Channel Number =");
    LineInput(cTemp, 10);            
    sscanf(cTemp, "%d", &TotalDOChannel);  
    
    Print("Please Input Timeout:");
    LineInput(cTemp, 10);
    sscanf(cTemp, "%d", &iTimeout);
    
    InstallCom(iComPort, lBaudrate, 8, 0, 1);
    
    // Begin to write DO and read DI
    for(;;)
    {
        Print("Please Input DO Data(Hex) = ");
        LineInput(cTemp,10); 
        sscanf(cTemp, "%X", &DOdata);
        
        iRet=Write7K87K_DIO_DO(iComPort, iAddress, TotalDOChannel, iCheckSum, iTimeout, DOdata); 
        
        if(iRet==NoError)
        {
            Print("DO output OK!\n\r");
        }
        else
        {
            Print("Writing DO receive response error. Error Code = %d\n\r", iRet);
        }
        
        Delay(10); //Wait for DO status is changed.
                   //If you don't need to confirm the DO is changed correctly,
                   //remove following codes.
        iRet=Read7K87K_DIO(iComPort, iAddress, TotalDOChannel, iCheckSum, iTimeout, &DIdata, &ReadbackDO);
        if(iRet==NoError)
        {
            Print("DO data= %X\n\r", ReadbackDO);
            Print("DI data= %X\n\r", DIdata);
        }  
        else
        {
            Print("Reading DIO receive response error. Error Code = %d\n\r", iRet);
        }
    }
}

//=====================================================================
//==== Following functions are for 7K,87K DIO modules.               ==
//==== You can copy the functions to your own program.               ==
//===================================================================== 
int Write7K87K_DIO_DO(int iComPort,int iAddress,int TotalDOChannel,int iCheckSum,int iTimeout,int iDOdata)
{
    /*
    DCON protocol to write DO value
    Send command: @AA(Data)
    Valid Response: >
    
    iComPort: 1~8
    iAddress: 0 ~ 255
    iCheckSum: 0=Disable or 1=Enable
    iTimeout: unit=ms
    iTotalDOChannel: 1~16
    iDOdata: output data
    */
    
    int iRet, i, x;
    unsigned char OutBufCom[20],InBufCom[20];
    unsigned int iMask=0;
    
       //Creat DO mask.  For example: if DO channel= 5, the mask is 00011111. 
    for(i=0; i<TotalDOChannel; i++)       //Distinguish from channel number
    {
        x=1UL<<i;
        iMask|=x;
    }
    
    if(TotalDOChannel <= 8)
        sprintf(OutBufCom,"@%02X%02X",iAddress,(iDOdata & iMask));
    else 
        sprintf(OutBufCom,"@%02X%04X",iAddress,(iDOdata & iMask));  
    
    SendCmdTo7000(iComPort,OutBufCom,iCheckSum);
    
    iRet=ReceiveResponseFrom7000_ms(iComPort,InBufCom,iTimeout,iCheckSum);
    
    if(iRet==NoError)
    {
        if(InBufCom[0] =='>')       //Response first character: >
        {
            return NoError;
        }
        else
            return -1;    // Response string error.
    }
    else 
        return iRet;
}

int Read7K87K_DIO(int iComPort, int iAddress, int TotalDOChannel, int iCheckSum, int iTimeout, unsigned int *DIdata, unsigned int *ReadbackDO)
{
    /*
    DCON protocol to read DI value
    Send command: @AA
    Valid Response: >(DO Data)(DI Data)
    
    ComPort: 1~8
    Address: 0 ~ 255
    CheckSum: 0=Disable or 1=Enable
    iTimeout: unit=ms
    TotalDOChannel: 1~16
    *DIdata: pointer to record DI data
    *ReadbackDO: pointer to record DO readback data
    */
    
    int iRet;
    unsigned char InBufCom[20], OutBufCom[20];
    unsigned long DIO_value;
    
    sprintf(OutBufCom, "@%02X", iAddress);
    
    SendCmdTo7000(iComPort, OutBufCom, iCheckSum);  
    
    iRet=ReceiveResponseFrom7000_ms(iComPort, InBufCom, iTimeout, iCheckSum);
          
    if(iRet==NoError)
    {
        if(InBufCom[0]=='>')    // Response first character: >
        {
            if(TotalDOChannel<=4)
            {
                sscanf(InBufCom+1,"%04X",&DIO_value);
                *DIdata = DIO_value & 0x0F;
                *ReadbackDO = (DIO_value >> 8) & 0x0F; 
            }
            else if(TotalDOChannel > 4 && TotalDOChannel<= 8 )  
            {
                sscanf(InBufCom+1,"%04X",&DIO_value);
                *DIdata = DIO_value & 0xFF;
                *ReadbackDO = (DIO_value >> 8) & 0xFF; 
            }
            else  
            {
                sscanf(InBufCom+1,"%08lX",&DIO_value);
                *DIdata = DIO_value & 0xFFFF;
                *ReadbackDO = (DIO_value >> 16) & 0xFFFF;
            }
            return NoError;
        
        }
        else
            return -1;  // Response string error.
    }
    else 
        return iRet;
}