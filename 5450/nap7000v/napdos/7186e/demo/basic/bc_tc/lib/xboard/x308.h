/********************************************************************/
/*  7188EX/7188XB/7186EX + X308 head file                           */
/*                                                                  */
/*  [Oct, 17, 2003] by Jason Version: 1.00                          */
/*  [07, Oct, 2005] by Liam                                         */
/*	[22, Mar, 2007] by Liam                                         */
/*  [Dec, 21, 2010] by Nicholas Version: 2.03                       */
/*  [Dec, 14, 2011] by Nicholas Version: 2.04                       */
/********************************************************************/
/********************************************************************/
/*  X308:   4 12-Bit A/D channels (Analog input) 0.0~10.0V          */
/*          6 D/O channels                                          */
/********************************************************************/
/********************************************************************/
/*	[Caution]                                                       */
/*	The EEPROM block 7 on X board is used to store A/D calibration  */
/*	settings. When you use the EEPROM on X board, do not overwrite  */
/*  it.                                                             */
/********************************************************************/

#ifndef __X308_H
#define __X308_H

#ifdef __cplusplus
extern "C" {
#endif

#define X308_DigitalIn      X308_Read_All_DI
#define X308_DigitalOut     X308_Write_All_DO

#define X308_SetAdChannel(Channel,X308_iMem_DO) outp(0, ((Channel)<<6)|X308_iMem_DO)

int X308_Init(void);
/*  Return value: 0   ==> success
    Return value: <>0 ==> error
        Bit0: 1 ==> Reads A/D Gain falure
        Bit1: 1 ==> Reads A/D Offset falure */
        
unsigned X308_GetLibVersion(void);
/*  Current version is 2.04 (return 0x0204) */

float X308_Read_AD_CalibrationGain(void);
/*  Return 10.0 when no setting in EEPROM   */

float X308_Read_AD_CalibrationOffset(void);
/*  Return 10.0 when no setting in EEPROM   */

float X308_AnalogIn(int iChannel);
/*  iChannel = 0~3 ----> ch1~ch4
    Return data = 0 ~ +10.0 */

void X308_Write_All_DO(int iOutValue);
/*  iOutValue: 0x00 ~ 0x3f  */

void X308_Write_One_DO(int iChannel, int iStatus);
/*  iChannel = 0 ~ 5
    iStatus = 1 => Status is ON
    iStatus = 0 => Status is OFF    */

int X308_Read_All_DO(void);
/*  Return data =  0x00 ~ 0x3f  */

int X308_Read_One_DO(int iChannel);
/*  iChannel = 0 ~ 5
    Return 1 => ON
    Return 0 => OFF */

void X308_SetDelay(int Delayms);
/* Delay time range: 0ms~20ms  , Delay Unit: 0.01ms */

int X308_GetDelay(void); //Unit: 0.01ms
    
void X308_AnanlogInSetChannel(int iChannel);
int X308_AnalogInHex(int iChannel);
float X308_AnalogInHexToFloat(int iValue);

int X308_AnalogIn_no(int iChannel,unsigned no);
/* no: 1~32767  */
extern float 	X308_fAD_Gain, X308_fAD_Offset;

#ifdef __cplusplus
}
#endif

#endif