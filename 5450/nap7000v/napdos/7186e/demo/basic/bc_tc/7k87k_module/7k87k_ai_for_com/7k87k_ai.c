/*
7k87k_Ai.c :Demo program for 7K and 87K AI module connected to COM port of i-7188/uPAC-7186/i-8000.
            User must use DCON Utility to set Address, Baudrate,CheckSum before running the demo.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.
      
Compile mode: large

Project: 7k87k_Ai.c 
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib) 

Hardware: i-7188/uPAC-7186/i-8000 <== COM port ==> 7K or 87K modelue
    
[Oct 30, 2008] by Liam
*/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "..\..\lib\7186e.h"

int Set7K87KAI_Config(int iComPort, int iAddress, int iCheckSum, int iTimeout, int iRangeCode, int iFilter, int iDataFormat);
int Read7K87K_AI(int iComPort, int iAddress, int iChannel, int iCheckSum, int iTimeout, float *fValue);
int Read7K87K_AI_Hex(int iComPort, int iAddress, int iChannel, int iCheckSum, int iTimeout, int *iValue);
int Read7K87KAI_Config(int iComPortint, int iAddress, int iCheckSum, int iTimeout, int *iRangeCode, int *iDefaultFilter, int *iDataFormat_Original);

void main(void)
{
    int iRet, iAddress, iChannel, iComPort, iTimeout, iCheckSum;
    unsigned long lBaudrate;
    unsigned char cTemp[10];
    int iDataFormat, iDataFormat_Original;
    int iFilter, iFilter_Original;
    int iRangeCode, iRangeCode_Original;
    
    float fValue;
             
    InitLib();    
    
    Print("********************************\r\n");
    Print("*  7K87K_AI demo for Com Port  *\r\n");
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
    
    Print("Please Input Timeout:");
    LineInput(cTemp, 10);
    sscanf(cTemp, "%d", &iTimeout);
    
    InstallCom(iComPort, lBaudrate, 8, 0, 1); 
    iFilter=0;      // 0: 60Hz    
        
    iDataFormat=0;  //0: Engineering units
    
    iRangeCode=0x08;    //7017/87017:Range code 08(-10V to +10V)
     
    //To prevent reading EEPROM again.
    iRet=Read7K87KAI_Config(iComPort,
                            iAddress,
                            iCheckSum,
                            iTimeout,
                            &iRangeCode_Original,
                            &iFilter_Original,
                            &iDataFormat_Original);
    
    if(iRet==NoError)
    {
        if((iRangeCode_Original==iRangeCode) 
           && (iFilter_Original==iFilter) 
           && (iDataFormat_Original==iDataFormat))
        {
            // The original configuration is the same as the input configuration.
            // So there is no need to write into the EEPROM.
        }  
        else
        {
            Set7K87KAI_Config(iComPort, iAddress, iCheckSum, iTimeout, iRangeCode, iFilter, iDataFormat);  
        }
    }
    else
        Print("Read Configuration Error\r\n");

    for(;;)
    {    
        for(iChannel=0; iChannel<=7; iChannel++)
        {
            iRet=Read7K87K_AI(iComPort, iAddress, iChannel, iCheckSum, iTimeout, &fValue);
            if(iRet==NoError)
            {
                Print("Ch%d=%6.3f\n\r", iChannel, fValue);
            }
            else
            {
                Print("Read AI_%d error!!! Error Code =%d\r\n", iChannel, iRet);
            }
        }
        Print("\r\n");
    }
}

//=====================================================================
//====== The Following functions are used to read 7K AI modules.    ===
//====== You can copy the following functions to your own program.  ===
//=====================================================================
int Read7K87KAI_Config(int iComPort,
                       int iAddress,
                       int iCheckSum,
                       int iTimeout,
                       int *iRangeCode_Original,
                       int *iFilter_Original,
                       int *iDataFormat_Original)
{
    /*
    DCON protocol to read the AI Configuration
     Send  command: $AA2
     valid Response: !AATTCCFF
     
     iComPort: 1~8
     iAddress: 0 ~ 255
     iCheckSum: 0=Disable or 1=Enable
     iTimeout: unit=ms
     *iRangeCode_Original: pointer to record original range code
         For detailed specifications, refer to
            CD:8000\Napdos\DCON\IO_Module\hw_dcon_on_87kUnit
            CD:8000\Napdos\7000\Manual     
     
     *iFilter_Original: pointer to record original filter setting
                0 = 60Hz rejection 
                1 = 50Hz rejection 
                This setting is reserved for the I-87015/I-87015T and should be zero. 
     
     *iDataFormat_Original: pointer to record original data format
            0 = Engineer Unit Format 
            1 = % of FSR Format 
            2 = 2's Complement Hexdecimal Format 
            3 = Ohms in Engineer Unit Format For I-87013 Only 
    */                                
    
    unsigned char InBuf[20],OutBuf[20];
    int iRet;
    
    sprintf(OutBuf,"$%02X2",iAddress);
    
    iRet=SendCmdTo7000(iComPort,OutBuf,iCheckSum);
    
    iRet=ReceiveResponseFrom7000_ms(iComPort ,InBuf,iTimeout,iCheckSum);
    
    if(iRet==NoError)
    {
        if(InBuf[0]== '!')      //Valid response first character: !
        {
            *iRangeCode_Original= ((ascii_to_hex(InBuf[3])<<4)&0xF0)
                                   +(ascii_to_hex(InBuf[4])&0xF); 
            *iFilter_Original=((ascii_to_hex(InBuf[7])&0xF)>>3);
            *iDataFormat_Original= (ascii_to_hex(InBuf[8])&0xF);

            return NoError;
        }
        else
            return -1;          //Response string error.
    }
    else
        return iRet;
}

int Set7K87KAI_Config(int iComPort,int iAddress,int iCheckSum,int iTimeout,int iRangeCode,int iFilter,int iDataFormat)
{
    /*
    DCON protocol to write the AI Configuration
     send command: %AANNTTCCFF
     valid Response: !AA
     
     iComPort: 1~8
     iAddress: 0 ~ 255
     iCheckSum: 0=Disable or 1=Enable
     iTimeout: unit=ms
     iRangeCode: For detailed specifications, refer to
                    CD:8000\Napdos\DCON\IO_Module\hw_dcon_on_87kUnit
                    CD:8000\Napdos\7000\Manual
     
     iFilter:   0 = 60Hz rejection 
                1 = 50Hz rejection 
                This setting is reserved for the I-87015/I-87015T and should be zero. 
     
     iDataFormat:   0 = Engineer Unit Format 
                    1 = % of FSR Format 
                    2 = 2's Complement Hexdecimal Format 
                    3 = Ohms in Engineer Unit Format For I-87013 Only 
    */              
    
    unsigned char OutBuf[20],InBuf[20],ObtainDefault[20],ShowDefault[20];
    int iRet,iDefaultBaud,iDataFomat_HI;
    
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
    
    OutBuf[0]=0x25; // % = 0x25
    iDataFomat_HI=(iFilter << 3) | (iCheckSum << 2);
    sprintf(OutBuf+1,"%02X%02X%02X%02X%01X%01X",
            iAddress,iAddress, iRangeCode, iDefaultBaud, iDataFomat_HI, iDataFormat);
    iRet=SendCmdTo7000(iComPort, OutBuf, iCheckSum);
    
    iRet=ReceiveResponseFrom7000_ms(iComPort,InBuf,iTimeout,iCheckSum);
    
    if(iRet==NoError)
    {
        if(InBuf[0]== '!')       //Valid response first character: !
        {
            /*
            7K and 87K module needs almost 1 second to effect new settings.
            So, the program needs to delay 1 second to wait the new setting
            works. But 1 second is greater than the Watchdog timeout value (0.8 second).
            Thus we need to call "RefreshWDT" to prevent it from resetting the CPU 
            before the process can be completed.
            */
            Delay(500);
            RefreshWDT();
            Delay(500);
            RefreshWDT();
            return NoError;
        }
        else
            return -1;          //Response string error.
    }
    else
        return iRet;
}

//Engineer Unit Format
int Read7K87K_AI(int iComPort, int iAddress, int iChannel, int iCheckSum, int iTimeout, float *fValue)
{
    /*
    DCON protocol to read the AI value
    Send command: #AAN
    valid Response: >(data)
    
    iComPort: 1~8
    iAddress: 0 ~ 255
    iChannel: 0 ~ 7
    iCheckSum: 0=Disable or 1=Enable
    iTimeout: unit=ms
    *fValue: pointer to record analog input data
    */
    
    unsigned char OutBuf[20], InBuf[20];
    int iRet;

    sprintf(OutBuf,"#%02X%01X",iAddress,iChannel);
    iRet=SendCmdTo7000(iComPort,OutBuf,iCheckSum);

    iRet=ReceiveResponseFrom7000_ms(iComPort, InBuf, iTimeout, iCheckSum);   
    
    if(iRet==NoError)
    {
        if(InBuf[0]== '>')      //Valid response first character: >
        {
            *fValue=atof(InBuf+1);
            return NoError;   
        }
        else
            return -1;          //Response string error.
    }
    else
        return iRet;    
}

// 2's Complement Hexdecimal Format
int Read7K87K_AI_Hex(int iComPort, int iAddress, int iChannel, int iCheckSum, int iTimeout, int *iValue)       
{
    /*
    DCON protocol to read the AI value
    Send command: #AAN
    valid Response: >(data)
    
    iComPort: 1~8
    iAddress: 0 ~ 255
    iChannel: 0 ~ 7
    iCheckSum: 0=Disable or 1=Enable
    iTimeout: unit=ms
    *Value: pointer to record analog input data
    */

    unsigned char OutBuf[20], InBuf[20];
    int iRet, iHex;

    sprintf(OutBuf,"#%02X%01d", iAddress, iChannel);
    iRet=SendCmdTo7000(iComPort, OutBuf, iCheckSum);

    iRet=ReceiveResponseFrom7000_ms(iComPort, InBuf, iTimeout, iCheckSum);
    
    if(iRet==NoError)
    {
        if(InBuf[0]== '>')      //Valid response first character: >
        {
            sscanf(InBuf+1, "%04X", &iHex);
            *iValue=iHex;
            return NoError;
        }
        else
            return -1;          //Response string error.
    }
    else
        return iRet;
}