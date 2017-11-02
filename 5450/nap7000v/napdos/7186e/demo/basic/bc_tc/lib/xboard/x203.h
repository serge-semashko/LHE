/********************************************************************/
/*  7188EX/7188XB/7186EX + X203 head file                           */
/*                                                                  */
/*  [07, Oct, 2005] by Liam                                         */
/*  [23, Mar, 2007] by Liam                                         */
/*  [Dec, 14, 2011] by Nicholas                                     */
/********************************************************************/
/********************************************************************/
/*  X203: 2 A/D channels  (0~20mA)                                  */
/*        2 D/I channels                                            */
/*        6 D/O channels                                            */
/********************************************************************/
/********************************************************************/
/*	[Caution]                                                       */
/*	The EEPROM block 7 on X board is used to store A/D calibration  */
/*	settings. When you use the EEPROM on X board, do not overwrite  */
/*  it.                                                             */
/********************************************************************/

#ifndef __X203_H
#define __X203_H

#ifdef __cplusplus
extern "C" {
#endif

#define X203_DigitalIn      X203_Read_All_DI
#define X203_DigitalOut     X203_Write_All_DO

#define X203_SetAdChannel(Channel,X203_iMem_DO) outp(0, ((Channel&0x3)<<6)|X203_iMem_DO)

int X203_Init(void);
/*  Return value: 0   ==> success
    Return value: <>0 ==> error
    Bit0: 1 ==> (Ch0)Reads A/D Gain falure
    Bit1: 1 ==> (Ch0)Reads A/D Offset falure    */
    
unsigned X203_GetLibVersion(void);
/*  Current version is 2.02 (return 0x0202) */

float X203_Read_AD_CalibrationGain(void);
/*  Return 10.0 when no setting in EEPROM   */

float X203_Read_AD_CalibrationOffset(void);
/*  Return 10.0 when no setting in EEPROM   */

float X203_AnalogIn(int iChannel);
/*  Return data = 0.0 ~ 20.0    */

int X203_Read_All_DI(void);
/*  Return data =  0x00~0x03
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND    */
//             Logic low level (0V ~ +1V)

int X203_Read_One_DI(int iChannel);
/*  iChannel = 0 ~ 1
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

void X203_Write_All_DO(int iOutValue);
/*  iOutValue: 0x00 ~ 0x3f  */

void X203_Write_One_DO(int iChannel, int iStatus);
/*  iChannel = 0 ~ 5
    iStatus = 1 => Status is ON
    iStatus = 0 => Status is OFF    */

int X203_Read_All_DO(void);
/*  Return data = 0x00~0x3f */

int X203_Read_One_DO(int iChannel);
/*  iChannel = 0 ~ 5
    Return 1 => ON
    Return 0 => OFF */

void X203_SetDelay(int Delayms);
/* Delay time range: 0ms~20ms  , Delay Unit: 0.01ms */

int X203_GetDelay(void); //Unit: 0.01ms
	
void X203_AnanlogInSetChannel(int iChannel);
int X203_AnalogInHex(int iChannel);
float X203_AnalogInHexToFloat(int iValue);

int X203_AnalogIn_no(int iChannel,unsigned no);
/* no: 1~32767  */
extern float    X203_fAD_Gain, X203_fAD_Offset;

#ifdef __cplusplus
}
#endif

#endif