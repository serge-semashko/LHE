/********************************************************************/
/*  7188EX/7188XB/7186EX + X310 head file                           */
/*                                                                  */
/*	[11, Oct, 2005] by Liam                                         */
/*  [22, Mar, 2007] by Liam                                         */
/*  [Dec, 14, 2011] by Nicholas                                     */
/********************************************************************/
/********************************************************************/
/*  X310:   2 12-Bit A/D channels (Analog input)                    */
/*              ch0: 0~20mA                                         */
/*              ch1: 0~10V                                          */
/*          2 12-Bit D/A channels (Analog output)0.0~10.0V          */
/*          3 D/I channels                                          */
/*          3 D/O channels                                          */
/********************************************************************/
/********************************************************************/
/*	[Caution]                                                       */
/*	The EEPROM block 7 on X board is used to store A/D calibration  */
/*	settings. When you use the EEPROM on X board, don't overwrite   */
/*  it.                                                             */
/********************************************************************/

#ifndef __X310_H
#define __X310_H

#ifdef __cplusplus
extern "C" {
#endif

#define X310_DigitalIn      X310_Read_All_DI
#define X310_DigitalOut     X310_Write_All_DO

#define X310_SetAdChannel(Channel,X310_iMem_DO) outp(0, X310_iMem_DO|(Channel<<4))

int X310_Init(void);
/*  Return value: 0   ==> success
    Return value: <>0 ==> error
    Bit0: 1 ==> (Ch0)Reads A/D Gain falure		==> uses default value
    Bit1: 1 ==> (Ch0)Reads A/D Offset falure	==> uses default value
    Bit2: 1 ==> (Ch0)Reads D/A Gain falure		==> uses default value
    Bit3: 1 ==> (Ch0)Reads D/A Offset falure 	==> uses default value
    Bit4: 1 ==> (Ch1)Reads A/D Gain falure		==> uses default value
    Bit5: 1 ==> (Ch1)Reads A/D Offset falure	==> uses default value
    Bit6: 1 ==> (Ch1)Reads D/A Gain falure		==> uses default value
    Bit7: 1 ==> (Ch1)Reads D/A Offset falure 	==> uses default value  */

unsigned X310_GetLibVersion(void);
/*  Current version is 2.02 (return 0x0202) */

float X310_Read_AD_CalibrationGain(int iChannel);
/*  Return 10.0 when no setting in EEPROM   */

float X310_Read_AD_CalibrationOffset(int iChannel);
/*  Return 10.0 when no setting in EEPROM   */

float X310_Read_DA_CalibrationGain(int iChannel);
/*  Return 10.0 when no setting in EEPROM   */

float X310_Read_DA_CalibrationOffset(int iChannel);
/*  Return 10.0 when no setting in EEPROM   */

float X310_AnalogIn(int iChannel);
/*  iChannel: 0~1   */

void X310_AnalogOut(int iChannel,float fValue);
/*  iChannel: 0~1
    fValue: +0.0 ~ +10.0    */

int X310_Read_All_DI(void);
/*  Return data =  0x00~0x07
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

int X310_Read_One_DI(int iChannel);
/*  iChannel = 0 ~ 2
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

void X310_Write_All_DO(int iOutValue);
/*  iOutValue: 0x00 ~ 0x07  */

void X310_Write_One_DO(int iChannel, int iStatus);
/*  iChannel = 0 ~ 2
    iStatus = 1 => Status is ON
    iStatus = 0 => Status is OFF    */

int X310_Read_All_DO(void);
/*  return data = 0x00 ~ 0x07   */

int X310_Read_One_DO(int iChannel);
/*  iChannel = 0 ~ 2
    Return 1 => ON
    Return 0 => OFF */

void X310_SetDelay(int X310_AiChannelDelayTime);
/* Delay time range: 0ms~20ms  , Delay Unit: 0.01ms */

int X310_GetDelay(void); //Unit: 0.01ms
    
void X310_AnanlogInSetChannel(int iChannel);
int X310_AnalogInHex(int iChannel);
float X310_AnalogInHexToFloat(int iValue, int iChannel);

int X310_AnalogIn_no(int iChannel,unsigned no);
/* no: 1~32767  */
extern float X310_fAD_Gain[2], X310_fAD_Offset[2];
extern float X310_fDA_Gain[2], X310_fDA_Offset[2];

#ifdef __cplusplus
}
#endif

#endif

