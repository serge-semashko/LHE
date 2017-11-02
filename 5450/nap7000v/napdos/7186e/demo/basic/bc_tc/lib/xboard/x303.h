/********************************************************************/
/*  7188EX/7188XB/7186EX + X303 head file                           */
/*                                                                  */
/*  [07, Oct, 2005] by Liam                                         */
/*  [23, Mar, 2007] by Liam                                         */
/*  [Dec, 14, 2011] by Nicholas                                     */
/********************************************************************/
/********************************************************************/
/*  X303: 1 12-Bit A/D channels (Analog input) +/-5V                */
/*        1 12-Bit D/A channels (Analog output) +/-5V               */
/*        4 D/I channels                                            */
/*        6 D/O channels                                            */
/********************************************************************/
/********************************************************************/
/*	[Caution]                                                       */
/*	The EEPROM block 7 on X board is used to store A/D calibration  */
/*	settings. When you use the EEPROM on X board, do not overwrite  */
/*  it.                                                             */
/********************************************************************/
#ifndef __X303_H
#define __X303_H

#ifdef __cplusplus
extern "C" {
#endif

#define X303_DigitalIn      X303_Read_All_DI
#define X303_DigitalOut     X303_Write_All_DO

int X303_Init(void);
/*  Return value: 0   ==> success
    Return value: <>0 ==> error
    Bit0: 1 ==> (Ch0)Reads A/D Gain falure
    Bit1: 1 ==> (Ch0)Reads A/D Offset falure
    Bit2: 1 ==> (Ch0)Reads D/A Gain falure
    Bit3: 1 ==> (Ch0)Reads D/A Offset falure    */

unsigned X303_GetLibVersion(void);
/*  Current version is 2.04 (return 0x0204) */

float X303_Read_AD_CalibrationGain(void);
/*  Return 10.0 when no setting in EEPROM   */

float X303_Read_AD_CalibrationOffset(void);
/*  Return 10.0 when no setting in EEPROM   */

float X303_Read_DA_CalibrationGain(void);
/*  Return 10.0 when no setting in EEPROM   */

float X303_Read_DA_CalibrationOffset(void);
/*  Return 10.0 when no setting in EEPROM   */

float X303_AnalogIn(void);
/*  Return data = -5.0 ~ 5.0    */

void X303_AnalogOut(float fValue);
/*  fValue: -5.0 ~ 5.0  */

int X303_Read_All_DI(void);
/*  Return data = 0x00~0x0f
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

int X303_Read_One_DI(int iChannel);
/*  iChannel = 0 ~ 3
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

void X303_Write_All_DO(int iOutValue);
/*  iOutValue: 0x00 ~ 0x3f  */

void X303_Write_One_DO(int iChannel, int iStatus);
/*  iChannel = 0 ~ 5
    iStatus = 1 => Status is ON
    iStatus = 0 => Status is OFF    */

int X303_Read_All_DO(void);
/*  Return data =  0x00~0x3f    */

int X303_Read_One_DO(int iChannel);
/*  iChannel = 0 ~ 5
    Return 1 => ON
    Return 0 => OFF */
    
int X303_AnalogInHex(void);
float X303_AnalogInHexToFloat(int iValue);

int X303_AnalogIn_no(unsigned no);
/* no: 1~32767  */

extern float 	X303_fAD_Gain, X303_fAD_Offset;
extern float 	X303_fDA_Gain, X303_fDA_Offset;

#ifdef __cplusplus
}
#endif

#endif