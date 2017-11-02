/********************************************************************/
/*  7188EX/7188XB/7186EX + X520 head file                           */
/*                                                                  */
/*  [11, Nov, 2010] by Nicholas                                     */
/********************************************************************/
/********************************************************************/
/*  X520: 2 D/O channels                                            */
/*        1 D/I channel                                             */
/*        COM3 (RS-232)                                             */
/*        COM4 (RS-232)                                             */
/*        COM5 (RS-232)                                             */
/*        COM6 (RS-232)                                             */
/********************************************************************/

#ifndef __X520_H
#define __X520_H

#ifdef __cplusplus
extern "C" {
#endif

int X520_Init(void);
/*  Always retun 0  */

unsigned X520_GetLibVersion(void);
/*  Current version is 2.02 (return 0x0202) */

int X520_Read_All_DI(void);
/*  Return data =  0x00~0x01
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

int X520_Read_One_DI(int iChannel);
/*  iChannel = 0
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

void X520_Write_All_DO(int iOutValue);
/*  iOutValue: 0x00 ~ 0x03  */

void X520_Write_One_DO(int iChannel, int iStatus);
/*  iChannel = 0 ~ 1
    iStatus = 1 => Status is ON
    iStatus = 0 => Status is OFF    */

int X520_Read_All_DO(void);
/*  return data = 0x00~0x03 */

int X520_Read_One_DO(int iChannel);
/*  iChannel = 0 ~ 1
    Return  1 => ON
    Return  0 => OFF    */
    
#ifdef __cplusplus
}
#endif

#endif