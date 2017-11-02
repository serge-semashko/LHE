/*
AO_24.c: Demo program for 7024/87024 connected to COM port of i-7188/uPAC-7186/i-8000.
         User must use DCON Utility to set Address, Baudrate, CheckSum before running the demo.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.
      
Compile mode: large

Project: AO_24.c 
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)

Hardware: i-7188/uPAC-7186/i-8000 <== COM port ==> 7024/87024
    
[Oct 30, 2008] by Liam
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "..\..\lib\7186e.h"

int ReadAO_24_Config(int iComPort, int iAddress, int iCheckSum, int iTimeout, int *iRangeCode_Original, int *iSlewRate_Original);
int WriteAO_24(int iComPort, int iAddress, int iChannel, int iCheckSum, int iTimeout, float fValue);
int SetAO_24_Config(int iComPort, int iAddress, int iChannel, int iCheckSum, int iTimeout, int iRangeCode);

void main(void)
{
    int iRet,iAddress, iChannel, iComPort, iCheckSum, iTimeout;
    int iRangeCode, iRangeCode_Original;
    int iSlewRate, iSlewRate_Original;
    unsigned char cTemp[10];
    unsigned long lBaudrate;
    float fValue;
    
    InitLib();
    
    Print("/***********************************\r\n");
    Print("/*     AO_24 demo for COM port     *\r\n");
    Print("/*                                 *\r\n");
    Print("/*          [Oct 30, 2008]         *\r\n");
    Print("/***********************************\r\n");
    Print("\n\r");                     
    
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
    sscanf(cTemp,"%d", &iTimeout);
    
    // Begin configuration
    InstallCom(iComPort, lBaudrate, 8, 0, 1);
    
    iRangeCode=0x32;    // 0x32: 0 to 10V
    
    iSlewRate=0x0;  // 0x0 = Immediate output
    
    iRet=ReadAO_24_Config(iComPort, iAddress, iCheckSum, iTimeout, &iRangeCode_Original, &iSlewRate_Original);
    if(iRet==NoError)
    {
        if((iRangeCode_Original==iRangeCode) && (iSlewRate_Original==iSlewRate))
        {
            // The present configuration is the same as the input configuration. 
            // So there is no need to write into the EEPROM.
        }
        else
            SetAO_24_Config(iComPort, iAddress, iCheckSum, iTimeout, iRangeCode, iSlewRate);
    }
    else
       Print("Read Configuration Error");

    // Begin to write AO.
    for(;;)
    {
        Print("Please Input Channel Number(0~3) = ");
        LineInput(cTemp,10);
        sscanf(cTemp,"%d",&iChannel);
        
        Print("Please Input Value(float) = ");
        LineInput(cTemp,20);
        sscanf(cTemp,"%f",&fValue);
        
        iRet=WriteAO_24(iComPort,iAddress,iChannel,iCheckSum,iTimeout,fValue);
        if(iRet==NoError)
        {
            Print("Output success!!\n\r");
        }
        else
        {     
            Print("AO error. Error Code = %d\n\r",iRet);
        }
    }
}

//=====================================================================
//====== The following functions are for 7024, 87024.               ===
//====== You can copy the following functions to your own program.  ===
//=====================================================================
int ReadAO_24_Config(int iComPort, int iAddress, int iCheckSum, int iTimeout,
                     int *iRangeCode_Original, int *iSlewRate_Original)
{
    /*
    DCON protocol to read 7024, 87024
    Send command: $AA2                        
    Valid response: !AATTCCFF                  
    
    iComPort: 1~8                        
    iAddress: 0 ~ 255                    
    iCheckSum: 0=Disable or 1=Enable 
    iTimeout: unit=ms     
    *iRangeCode_Original: pointer to record original range code
         For detailed specifications, refer to           
            CD:Napdos\DCON\IO_Module\87k_modules.htm
            CD:Napdos\7000\Manual                        
    *iSlewRate_Original: pointer to record original slew rate    
         For detailed specifications, refer to            
            CD:Napdos\DCON\IO_Module\87k_modules.htm
            CD:Napdos\7000\Manual                       
    */
    
    unsigned char InBufCom[20], OutBufCom[20];
    int iRet;
    
    sprintf(OutBufCom, "$%02X2", iAddress);
    
    iRet=SendCmdTo7000(iComPort, OutBufCom, iCheckSum);
    
    iRet=ReceiveResponseFrom7000_ms(iComPort, InBufCom, iTimeout, iCheckSum);
    
    if(iRet==NoError)
    {
        if(InBufCom[0]== '!')   // Valid response first character: !
        {
            *iRangeCode_Original=((ascii_to_hex(InBufCom[3])<<4)&0xF0)+(ascii_to_hex(InBufCom[4])&0xF);
            *iSlewRate_Original=((ascii_to_hex(InBufCom[7]))&0x7)+((ascii_to_hex(InBufCom[8]))&0xC);
            return NoError;
        }   
        else
            return -1;  // Output string error.
    }
    else
        return iRet;
}

int SetAO_24_Config(int iComPort,int iAddress,int iCheckSum,int iTimeout,
                    int iRangeCode,int iSlewRate)
{    
    /*
    DCON protocol to write to 7024, 87024
    Send command: %AANNTTCCFF
    valid Response: !AA
    
    iComPort: 1~8
    iAddress: 0 ~ 255
    iCheckSum: 0=Disable or 1=Enable
    iTimeout: unit=ms
    iRangeCode: For detailed specifications, refer to
                  CD:Napdos\DCON\IO_Module\87k_modules.htm
                  CD:Napdos\7000\Manual
    iSlewRate: For detailed specifications, refer to
                  CD:Napdos\DCON\IO_Module\87k_modules.htm
                  CD:Napdos\7000\Manual
    */
    
    unsigned char InBufCom[20], OutBufCom[20], Slew[10], ObtainDefault[20], ShowDefault[20];
    int SlewRate_low, SlewRate_high, SlewRate, iFormat, iDefaultBaud;
    int iRet,iHex;
    
    sprintf(ObtainDefault,"$%02X2",iAddress);
    SendCmdTo7000(iComPort,ObtainDefault,iCheckSum);
    iRet=ReceiveResponseFrom7000_ms(iComPort,ShowDefault,iTimeout,iCheckSum);
    
    if(iRet==NoError)
    {
        if(ShowDefault[0]== '!')
        {
            sscanf(ShowDefault+5,"%04X",&iDefaultBaud);
            iDefaultBaud=iDefaultBaud >> 8;
        }
        else
            Print("Default Baudrate error!\n\r");
    }
    else
        Print("Receive Default Command error!!\n\r");
    
    OutBufCom[0]=0x25;  // 0x25 = %
    
    // FF =1bit Not used + 1bit checksum + Slew rate(4bits) + Dataformat(2bits)
    // Dataformat & Not used == 0
    SlewRate_low=(iSlewRate << 2)&0x0C;
    SlewRate_high=(iSlewRate >> 2)&0x3;
    sprintf(Slew, "%01X%01X", SlewRate_high, SlewRate_low);
    sscanf(Slew, "%02X", &iHex);
    iFormat=(iCheckSum << 6) | iHex;
    iFormat=iFormat & 0x7C;
    
    sprintf(OutBufCom+1,"%02X%02X%02X%02X%02X", iAddress, iAddress, iRangeCode, iDefaultBaud, iFormat);
    iRet=SendCmdTo7000(iComPort, OutBufCom, iCheckSum);
    
    iRet=ReceiveResponseFrom7000_ms(iComPort, InBufCom, iTimeout, iCheckSum);
    
    if(iRet==NoError)
    {
        if(InBufCom[0]== '!')   // Valie response first character: !
        {
            return NoError;
        }
        else
            return -1;          // Response string error. 
    }
    else
        return iRet;
}

int WriteAO_24(int iComPort, int iAddress, int iChannel, int iCheckSum, int iTimeout, float fValue)
{
    /*
    DCON protocol to write to 7024, 87024
    Send command: #AAN(data)
    Valid Response: >
    
    iComPort: 1~8
    iAddress: 0 ~ 255
    iCheckSum: 0=Disable or 1=Enable
    iTimeout: unit=ms
    iChannel: 0 ~ 3
    fValue: output value
    */
    
    int iRet;
    unsigned char OutBufCom[20], InBufCom[20];
    
    if(fValue>0)
    {
        sprintf(OutBufCom, "#%02X%01d+%06.3f", iAddress, iChannel, fabs(fValue));
    }
    else
    {
        sprintf(OutBufCom,"#%02X%01d-%06.3f", iAddress, iChannel, fabs(fValue));
    }
    
    SendCmdTo7000(iComPort, OutBufCom, iCheckSum);
    
    iRet=ReceiveResponseFrom7000_ms(iComPort, InBufCom, iTimeout, iCheckSum);   
        
    if(iRet==NoError)
    {
        if(InBufCom[0] =='>')   // Valid response first character: >
        {
            return NoError;
        }
        else
            return -1;          // Response string error.
    }
    else 
        return iRet;
}