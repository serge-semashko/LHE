/*
AO_22_26.c: Demo program for 7022/87022/87026 connected to COM port of i-7188/uPAC-7186/i-8000.
            User must use DCON Utility to set Address, Baudrate, CheckSum before 
            running the demo.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp) 
          MSC 6.0,
          MSVC 1.52.
      
Compile mode: large

Project: AO_22_26.c 
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)

Hardware: i-7188/uPAC-7186/i-8000 <== COM port ==> 7K or 87K modelue

[Oct 30, 2008] by Liam
*/

#include <stdlib.h>
#include <stdio.h>
#include "..\..\lib\7186e.h"

int WriteAO_22_26(int iComPort, int iAddress, int iChannel, int iCheckSum, int iTimeout, float fValue);
int SetAO_22_26_DataFormat(int iComPort, int iAddress, int iCheckSum, int iTimeout, int iDataFormat);
int SetAO_22_26_Config(int iComPort, int iAddress, int iChannel, int iCheckSum, int iTimeout, int iRangeCode, int iSlewRate);
int ReadAO_22_26_DataFormat(int iComPort, int iAddress, int iCheckSum, int iTimeout, int *iDefaultDataFormat);
int ReadAO_22_26_Config(int iComPort, int iAddress, int iChannel, int iCheckSum, int iTimeout, int *iRangeCode_Original, int *iSlewRate_Original);

void main(void)
{
    int iRet, iAddress, iChannel, iComPort, iCheckSum, iTimeout;
    int iSlewRate, iSlewRate_Original;
    int iRangeCode, iRangeCode_Original;
    int iDataFormat, iDataFormat_Original;
    unsigned char cTemp[10];
    unsigned long lBaudrate;
    float fValue;
    
    InitLib();
    
    Print("***********************************\r\n");
    Print("*  AO_22,AO_26 demo for Com Port  *\r\n");
    Print("*                                 *\r\n");
    Print("*         [Oct 30, 2008]          *\r\n");
    Print("***********************************\r\n");
    Print("\r\n");  
        
    Print("Please Input ComPort Number:");
    LineInput(cTemp, 10);
    sscanf(cTemp, "%d", &iComPort);  
        
    Print("Please Input ComPort's Baudrate:");
    LineInput(cTemp,10);
    sscanf(cTemp, "%ld", &lBaudrate);
    
    Print("Please Select Checksum(0:Disable or 1:Enable):");
    LineInput(cTemp, 10);
    sscanf(cTemp, "%d", &iCheckSum);
    
    Print("Please Input module's address(0~255) =");
    LineInput(cTemp, 10);
    sscanf(cTemp, "%d", &iAddress);
    
    Print("Please Input Timeout:");
    LineInput(cTemp,10);
    sscanf(cTemp, "%d", &iTimeout);
    
    InstallCom(iComPort, lBaudrate, 8, 0, 1);

    // Begin configuration 
    Print("\n");
    Print("******** Type Code ********\n\r");
    Print("* (0)     0 ~ 20   mA     *\n\r");
    Print("* (1)     4 ~ 20   mA     *\n\r");
    Print("* (2)     0 ~ 10    V     *\n\r");
    Print("***************************\n\r");
    Print("Please input Type Code(0,1,2) = ");
    LineInput(cTemp, 10);
    sscanf(cTemp, "%d", &iRangeCode);
    
    iSlewRate=0x0;  //0 = Immediate output
    
    iDataFormat=0;  //Dataforamt: 0==> Engineer Unit format
    
    iRet=ReadAO_22_26_DataFormat(iComPort, iAddress, iCheckSum, iTimeout, &iDataFormat_Original);
    if(iRet==NoError)
    {
        if(iDataFormat_Original==iDataFormat)
        {
            //The present configuration is the same as the input configuration.
            //So there is no need to write into the EEPROM.
        }
        else 
            SetAO_22_26_DataFormat(iComPort, iAddress, iCheckSum, iTimeout, iDataFormat);   // Default set.
    }
    else
        Print("Read DataFormat Error!\n\r");

    // Begin to write AO.
    for(;;)
    {
        Print("Please Input Channel Number(0 or 1) = ");
        LineInput(cTemp, 10);
        sscanf(cTemp, "%d", &iChannel);
        
        Print("Please Input Value(float) = ");
        LineInput(cTemp, 20);
        sscanf(cTemp, "%f", &fValue);
        
        iRet=ReadAO_22_26_Config(iComPort, iAddress, iChannel, iCheckSum, iTimeout, &iRangeCode_Original, &iSlewRate_Original);
        if(iRet==NoError)
        {
            if(iRangeCode_Original==iRangeCode && iSlewRate_Original==iSlewRate)
            {
                //The present configuration is the same as the input configuration.
                //So there is no need to write into the EEPROM.
            }
            else
                SetAO_22_26_Config(iComPort, iAddress, iChannel, iCheckSum, iTimeout, iRangeCode, iSlewRate); 
        }
        else
            Print("Read Configuration Error!\n\r");
        
        iRet=WriteAO_22_26(iComPort, iAddress, iChannel, iCheckSum, iTimeout, fValue);
        if(iRet==NoError)
        {
            Print("Output success!!\n\r");
        }
        else
            Print("AO receive response error. Error Code = %d\n\r",iRet);
    }
}

//========================================================================
//====== The following functions are for 7022, 87022 and 87026.        ===
//====== You can copy the following functions to your own program.     ===
//========================================================================
int ReadAO_22_26_DataFormat(int iComPort, int iAddress, int iCheckSum, int iTimeout, int *iDataFormat_Original)    
{
    /*
    DCON protocol to read 7022, 87022 and 87026
    Send command: $AA2          
    Valid Response: !AATTCCFF                   
    
    iComPort: 1~8                         
    iAddress: 0 ~ 255                    
    iCheckSum: 0=Disable or 1=Enable   
    iTimeout: unit=ms   
    iDataFormat:   0==> Engineer Unit format        
                   1==> Percent of Span format      
                   2==> Hexadecimal format          
  
    */
    unsigned char InBufCom[20], OutBufCom[10];
    int iRet;    
    
    sprintf(OutBufCom, "$%02X2", iAddress);        
    SendCmdTo7000(iComPort, OutBufCom, iCheckSum);    
    iRet=ReceiveResponseFrom7000_ms(iComPort, InBufCom, iTimeout, iCheckSum);   
                                                                        
    if(iRet==NoError)
    {
        if(InBufCom[0]== '!')   //Valid response first character: !
        {
            *iDataFormat_Original=(ascii_to_hex(InBufCom[8]))&0x3;    
            return NoError;
        }    
        else
            return -1;
    }
    else
        return iRet; 
}

int SetAO_22_26_DataFormat(int iComPort, int iAddress, int iCheckSum, int iTimeout, int iDataFormat)
{    
    /*
    DCON protocol to write to 7022, 87022 and 87026
    Send command: %AANNTTCCFF
    Valid Response: !AA
    
    iComPort: 1~8                                                    
    iAddress: 0 ~ 255                                              
    iCheckSum: 0=Disable or 1=Enable                               
    iTimeout: unit=ms
    iDataFormat: 0==> Engineer Unit format  
                 1==> Percent of Span format
                 2==> Hexadecimal format    
    */
    int iRet,iDefaultBaud;
    unsigned char InBufCom[20], OutBufCom[20], ObtainDefault[20], ShowDefault[20];
    
    sprintf(ObtainDefault, "$%02X2", iAddress);
    SendCmdTo7000(iComPort, ObtainDefault, iCheckSum);
    iRet=ReceiveResponseFrom7000_ms(iComPort, ShowDefault, iTimeout, iCheckSum);    
    
    if(iRet==NoError)
    {
        if(ShowDefault[0]== '!')
        {
            sscanf(ShowDefault+5, "%04X", &iDefaultBaud);
            iDefaultBaud=iDefaultBaud >> 8;
            Print("iDefaultBaud==%X\n\r", iDefaultBaud);
        }
        else
            Print("Default Baudrate error!\n\r");
    }
    else
        Print("Receive Default Command error!!\n\r");
    
    OutBufCom[0]=0x25; // % = 0x25
    if(iCheckSum==0)
    {
        sprintf(OutBufCom+1, "%02X%02X3F%02X0%01X", iAddress, iAddress, iDefaultBaud, iDataFormat);
    }
    else
        sprintf(OutBufCom+1, "%02X%02X3F%02X4%01X", iAddress, iAddress, iDefaultBaud, iDataFormat);
    
    SendCmdTo7000(iComPort, OutBufCom, iCheckSum);    
    
    iRet=ReceiveResponseFrom7000_ms(iComPort, InBufCom, iTimeout, iCheckSum);   
    
    if(iRet==NoError)
    {
        if(InBufCom[0]== '!')    //Valid response first character: !
        {
            return NoError;
        }    
        else
            return -1;//Response string error. 
    }
    else
        return iRet;    
}

int ReadAO_22_26_Config(int iComPort, int iAddress, int iChannel, int iCheckSum, int iTimeout,
                        int *iRangeCode_Original, int *iSlewRate_Original)
{
    /*
    DCON protocol to write to 7022, 87022 and 87026
    Send command: $AA9N
    Valid Response: !AATS
    
    iComPort: 1~8
    iAddress: 0 ~ 255
    iChannel: 0 ~ 1
    iCheckSum: 0=Disable or 1=Enable
    iTimeout: unit=ms
    
    iRangeCode_Original: pointer to record original range code
        For detailed specifications, refer to
            CD:Napdos\DCON\IO_Module\87k_modules.htm
            CD:8000\Napdos\7000\Manual
    iSlewRate_Original: pointer to record origianl slew rate 
        For detailed specifications, refer to           
            CD:Napdos\DCON\IO_Module\87k_modules.htm
            CD:8000\Napdos\7000\Manual
    */
    unsigned char OutBufCom[20], InBufCom[20];
    int iRet;
    
    sprintf(OutBufCom, "$%02X9%01d", iAddress, iChannel);
    
    SendCmdTo7000(iComPort, OutBufCom, iCheckSum);   
    
    iRet=ReceiveResponseFrom7000_ms(iComPort, InBufCom, iTimeout, iCheckSum);   
    
    if(iRet==NoError)
    {
        if(InBufCom[0]== '!')    //Response first character: !
        {
            *iRangeCode_Original=(ascii_to_hex(InBufCom[3])&0x3);   
            *iSlewRate_Original=(ascii_to_hex(InBufCom[4])&0xF);  
            return NoError;
        }
        else
            return -1;
    }
    else
        return iRet;
}

int SetAO_22_26_Config(int iComPort, int iAddress, int iChannel, int iCheckSum, int iTimeout, int iRangeCode, int iSlewRate)
{
    /*
    DCON protocol to write to 7022, 87022 and 87026
    Send command: $AA9NTS
    Valid Response: !AA
    
    iComPort: 1~8
    iAddress: 0 ~ 255
    iCheckSum: 0=Disable or 1=Enable
    iTimeout: unit=ms
    iRangeCode: For detailed specifications, refer to
                CD:\Napdos\DCON\IO_Module\87k_modules.htm
                CD:\8000\Napdos\7000\Manual
    iSlewRate:  For detailed specifications, refer to
                CD:\Napdos\DCON\IO_Module\87k_modules.htm
                CD:\8000\Napdos\7000\Manual
    */
    int iRet, iDefaultBaud;
    unsigned char OutBufCom[20], InBufCom[20], ObtainDefault[20], ShowDefault[20];
    
    sprintf(OutBufCom, "$%02X9%01d%01d%01X", iAddress, iChannel, iRangeCode, iSlewRate);
    
    SendCmdTo7000(iComPort, OutBufCom, iCheckSum);  
   
    iRet=ReceiveResponseFrom7000_ms(iComPort, InBufCom, iTimeout, iCheckSum);  
    
    if(iRet==NoError)
    {
        if(InBufCom[0]== '!')  //Valid response first character: !
        {
            return NoError;
        }    
        else
            return -1;//Response string error. 
    }
    else
        return iRet;
}

int WriteAO_22_26(int iComPort, int iAddress, int iChannel, int iCheckSum, int iTimeout, float fValue)
{
    /*
    DCON protocol to write to 7022, 87022 and 87026
    Send command: #AAN(data)
    Valid Response: >
    
    iComPort: 1~8
    iAddress: 0 ~ 255
    iChannel: 0 ~ 1
    iCheckSum: 0=Disable or 1=Enable 
    iTimeout: unit=ms   
    fValue: analog output value
    */
    int iRet;
    unsigned char OutBufCom[20],InBufCom[20];

    sprintf(OutBufCom, "#%02X%01d%06.3f", iAddress, iChannel, fValue);  
 
    SendCmdTo7000(iComPort, OutBufCom, iCheckSum);
    
    iRet=ReceiveResponseFrom7000_ms(iComPort,InBufCom,iTimeout,iCheckSum);
    
    if(iRet==NoError)
    {
        if(InBufCom[0] =='>')   //Valid response first character: >
        {
            return NoError;
        }
        else
            return -1;          //Response string error.
    }
    else 
        return iRet;
}