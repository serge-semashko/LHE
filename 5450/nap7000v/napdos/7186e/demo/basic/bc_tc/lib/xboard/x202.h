/********************************************************************/
/*  7188EX/7188XB/7186EX + X202 head file                           */
/*                                                                  */
/*  [07, Oct, 2005] by Liam                                         */
/*  [23, Mar, 2007] by Liam                                         */
/*  [Dec, 14, 2011] by Nicholas                                     */
/********************************************************************/
/********************************************************************/
/*  X202: 7 A/D channels (0~20mA)                                   */
/*                                                                  */
/********************************************************************/
/********************************************************************/
/*	[Caution]                                                       */
/*	The EEPROM block 7 on X board is used to store A/D calibration  */
/*	settings. When you use the EEPROM on X board, do not overwrite  */
/*  it.                                                             */
/********************************************************************/

#ifndef __X202_H
#define __X202_H

#ifdef __cplusplus
extern "C" {
#endif

#define X202_SetAdChannel(Channel) outp(0, Channel&0x7)

int X202_Init(void);
/*  Return value: 0   ==> success
    Return value: <>0 ==> error
    Bit0: 1 ==> (Ch0)Reads A/D Gain falure
    Bit1: 1 ==> (Ch0)Reads A/D Offset falure */

unsigned X202_GetLibVersion(void);
/*  Current version is 2.02 (return 0x0202) */

float X202_Read_AD_CalibrationGain(void);
/*  Return 10.0 when no setting in EEPROM   */

float X202_Read_AD_CalibrationOffset(void);
/*  Return 10.0 when no setting in EEPROM   */

float X202_AnalogIn(int iChannel);
/*  Return data = 0.0 ~ 20.0    */

void X202_SetDelay(int Delayms);
/*  Delay time range: 0ms~20ms  , Delay Unit: 0.01ms */

int X202_GetDelay(void); //Unit: 0.01ms

void X202_AnanlogInSetChannel(int iChannel);
int X202_AnalogInHex(int iChannel);
float X202_AnalogInHexToFloat(int iValue);

int X202_AnalogIn_no(int iChannel,unsigned no);
/* no: 1~32767  */
extern float  X202_fAD_Gain, X202_fAD_Offset;

#ifdef __cplusplus
}
#endif

#endif