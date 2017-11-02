/********************************************************************/
/*  7188EX/7188XB/7186EX + X305D head file                          */
/*                                                                  */
/*  [29, Jan, 2008] by Liam     version 1.00                        */
/*	[Dec 29, 2011] by Nicholas Version: 2.02			            */
/********************************************************************/
/********************************************************************/
/*  X305D:  8 12-Bit A/D channels (Analog input) 0~20mA             */
/*          2 D/I channel                                           */
/*          2 D/O channel                                           */
/********************************************************************/
/********************************************************************/
/*	[Caution]                                                       */
/*	The EEPROM block 7 on X board is used to store A/D calibration  */
/*	settings. When you use the EEPROM on X board, do not overwrite  */
/*  it.                                                             */
/********************************************************************/

#ifndef __X305D_H
#define __X305D_H

#ifdef __cplusplus
extern "C" {
#endif

#define X305D_SetAdChannel(Channel,X305D_iMem_DO) outp(0, ((Channel)<<4)|X305D_iMem_DO)

int X305D_Init(void);
/*  Return value: 0   ==> success
    Return value: <>0 ==> error
    Bit0: 1 ==> (Ch0)Reads A/D Gain falure
    Bit1: 1 ==> (Ch0)Reads A/D Offset falure    */
    
unsigned X305D_GetLibVersion(void);
/*  Current version is 1.01 (return 0x0101) */

float X305D_Read_AD_CalibrationGain(void);
/*  Return 10.0 when no setting in EEPROM   */

float X305D_Read_AD_CalibrationOffset(void);
/*  Return 10.0 when no setting in EEPROM   */

float X305D_AnalogIn(int iChannel);
/*  iChannel = 0~7 ----> ch1~ch8
    return data = 0.0mA ~ 20.0mA    */

int X305D_Read_All_DI(void);
/*  Return data =  0x00~0x03
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

int X305D_Read_One_DI(int iChannel);
/*  iChannel = 0 ~ 1
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

void X305D_Write_All_DO(int iOutValue);
/*  iOutValue: 0x0 ~ 0x3  */

void X305D_Write_One_DO(int iChannel, int iStatus);
/*  iChannel = 0 ~ 1
    iStatus = 1 => Status is ON
    iStatus = 0 => Status is OFF    */

int X305D_Read_All_DO(void);
/*  Return data =  0x00 ~ 0x03  */

int X305D_Read_One_DO(int iChannel);
/*  iChannel = 0 ~ 1
    Return 1 => ON
    Return 0 => OFF */
    
void X305D_AnanlogInSetChannel(int iChannel);
void X305D_SetDelay(int X305D_AiChannelDelayTime);
/* Delay time range: 0ms~20ms  , Delay Unit: 0.01ms */

int X305D_GetDelay(void); //Unit: 0.01ms

int X305D_AnalogInHex(int iChannel);
float X305D_AnalogInHexToFloat(int iValue);

extern float    X305D_fAD_Gain, X305D_fAD_Offset;

#ifdef __cplusplus
}
#endif

#endif