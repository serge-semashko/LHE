/********************************************************************/
/*  7188EX/7188XB/7186EX + X304 head file                           */
/*                                                                  */
/*  [21, May, 2002] by Kevin Version : 1.00                         */
/*  [Set 11, 2009] by Liam Version: 2.02                            */
/*                 Fix bug:                                         */
/*                      Modify the X304_Init() for Rev. 5.4         */
/*                      (modified the sequence of code)             */
/*                 Added new functions:                             */
/*                      1. X304_SetDelay()                          */
/*                      2. X304_GetDelay()                          */
/*  [Dec, 21, 2010] by Nicholas Version: 2.03                       */
/*  [Dec, 14, 2011] by Nicholas Version: 2.04                       */
/********************************************************************/
/********************************************************************/
/*  X304: 3 12-Bit A/D channels (Analog input) +/-5V                */
/*        1 12-Bit D/A channels (Analog output) +/-5V               */
/*        4 D/I channels                                            */
/*        4 D/O channels                                            */
/********************************************************************/
/********************************************************************/
/*	[Caution]                                                       */
/*	The EEPROM block 7 on X board is used to store A/D calibration  */
/*	settings. When you use the EEPROM on X board, do not overwrite  */
/*  it.                                                             */
/********************************************************************/

#ifndef __X304_H
#define __X304_H

#ifdef __cplusplus
extern "C" {
#endif

#define X304_DigitalIn      X304_Read_All_DI
#define X304_DigitalOut     X304_Write_All_DO

#define X304_SetAdChannel(Channel,X304_iMem_DO) outp(0, X304_iMem_DO|(Channel<<6))

int X304_Init(void);
/*  Return value: 0   ==> success
    Return value: <>0 ==> error
    Bit0: 1 ==> Reads A/D Gain falure
    Bit1: 1 ==> Reads A/D Offset falure
    Bit2: 1 ==> Reads D/A Gain falure
    Bit3: 1 ==> Reads D/A Offset falure */

unsigned X304_GetLibVersion(void);
/*  Current version is 2.04 (return 0x0204) */

float X304_Read_AD_CalibrationGain(void);
/*  Return 10.0 when no setting in EEPROM   */

float X304_Read_AD_CalibrationOffset(void);
/*  Return 10.0 when no setting in EEPROM   */

float X304_Read_DA_CalibrationGain(void);
/*  Return 10.0 when no setting in EEPROM   */

float X304_Read_DA_CalibrationOffset(void);
/*  Return 10.0 when no setting in EEPROM   */

float X304_AnalogIn(int iChannel);
/*  iChannel: 0~2   */

void X304_AnalogOut(int iChannel, float fValue);
/*  iChannel: 0
    fValue: -5.0 ~ 5.0  */

int X304_Read_All_DI(void);
/*  Return data = 0x00~0x0f
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
    Logic low level (0V ~ +1V)  */

int X304_Read_One_DI(int iChannel);
/*  iChannel = 0 ~ 3
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

void X304_Write_All_DO(int iOutValue);
/*  iOutValue: 0x00 ~ 0x0f  */

void X304_Write_One_DO(int iChannel, int iStatus);
/*  iChannel = 0 ~ 3
    iStatus = 1 => Status is ON
    iStatus = 0 => Status is OFF    */

int X304_Read_All_DO(void);
/*  Return data = 0x00~0x0f */

int X304_Read_One_DO(int iChannel);
/*  iChannel = 0 ~ 3
    Return  1 => ON
    Return  0 => OFF    */

void X304_SetDelay(int X304_AiChannelDelayTime); 
/* Delay time range: 0ms~20ms  , Delay Unit: 0.01ms */

int X304_GetDelay(void); //Unit: 0.01ms

void X304_AnanlogInSetChannel(int iChannel);
int X304_AnalogInHex(int iChannel);
float X304_AnalogInHexToFloat(int iValue);

int X304_AnalogIn_no(int iChannel,unsigned no);
/* no: 1~32767  */

extern float    X304_fAD_Gain, X304_fAD_Offset;
extern float    X304_fDA_Gain, X304_fDA_Offset;

#ifdef __cplusplus
}
#endif

#endif

