/********************************************************************/
/*  7188EX/7188XB/7186EX + X324 head file                           */
/*                                                                  */
/*  [19, May, 2008] by Liam                                         */
/********************************************************************/
/********************************************************************/
/*  X324:   4 12-Bit D/A channels (Analog output) 0.0~5.0V          */
/*          4 D/O channels                                          */
/********************************************************************/

#ifndef __X324_H
#define __X324_H

#ifdef __cplusplus
extern "C" {
#endif

#define X324_DigitalOut X324_Write_All_DO

int X324_Init(void);
/*  Return value: 0   ==> success
    Return value: <>0 ==> error
    Bit0: 1 ==> (Ch0)Reads A/D Gain falure      ==> uses default value
    Bit1: 1 ==> (Ch0)Reads A/D Offset falure    ==> uses default value
    Bit2: 1 ==> (Ch1)Reads D/A Gain falure      ==> uses default value
    Bit3: 1 ==> (Ch1)Reads D/A Offset falure    ==> uses default value
    Bit4: 1 ==> (Ch2)Reads A/D Gain falure      ==> uses default value
    Bit5: 1 ==> (Ch3)Reads A/D Offset falure    ==> uses default value
    Bit6: 1 ==> (Ch4)Reads D/A Gain falure      ==> uses default value
    Bit7: 1 ==> (Ch3)Reads D/A Offset falure    ==> uses default value  */

unsigned X324_GetLibVersion(void);
/*  Current version is 2.01 (return 0x0201) */

float X324_Read_DA_CalibrationGain(int iChannel);
/*  Return 5.0 when no setting in EEPROM    */

float X324_Read_DA_CalibrationOffset(int iChannel);
/*  Return 5.0 when no setting in EEPROM    */

void X324_AnalogOut(int iChannel, float fValue);
/*  iChannel: 0~3
    fValue: +0.0 ~ +5.0 */

void X324_Write_All_DO(int iOutValue);
/*  iOutValue: 0x00 ~ 0x0F  */

void X324_Write_One_DO(int iChannel, int iStatus);
/*  iChannel = 0 ~ 3
    iStatus = 1 => Status is ON
    iStatus = 0 => Status is OFF    */

int X324_Read_All_DO(void);
/*  return data = 0x00 ~ 0x0F   */

int X324_Read_One_DO(int iChannel);
/*  iChannel = 0 ~ 3
    Return 1 => ON
    Return 0 => OFF */

extern float X324_fDA_Gain[4], X324_fDA_Offset[4];

#ifdef __cplusplus
}
#endif

#endif
