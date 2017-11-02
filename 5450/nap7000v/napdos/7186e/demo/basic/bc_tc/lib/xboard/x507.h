/********************************************************************/
/*  7188EX/7188XB/7186EX + X507 head file                           */
/*                                                                  */
/*  [11, Oct, 2005] by Liam                                         */
/*  [23, Mar, 2007] by Liam                                         */
/********************************************************************/
/********************************************************************/
/*  X507: 4 D/I channels                                            */
/*        4 D/O channels                                            */
/*        COM3 (RS-422)                                             */
/********************************************************************/

#ifndef __X507_H
#define __X507_H

#ifdef __cplusplus
extern "C" {
#endif

int X507_Init(void);
/*  Always returns 0    */

unsigned X507_GetLibVersion(void);
/*  Current version is 2.01 (return 0x0201) */

int X507_Read_All_DI(void);
/*  Return data = 0x00~0x0f
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

int X507_Read_One_DI(int iChannel);
/*  iChannel = 0 ~ 3
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

void X507_Write_All_DO(int iOutValue);
/*  iOutValue: 0x00 ~ 0x0f  */

void X507_Write_One_DO(int iChannel, int iStatus);
/*  iChannel = 0 ~ 3
    iStatus = 1 => Status is ON
    iStatus = 0 => Status is OFF    */

int X507_Read_All_DO(void);
/*  Return data =  0x00~0x0f    */

int X507_Read_One_DO(int iChannel);
/*  iChannel = 0 ~ 3
    Return  1 => ON
    Return  0 => OFF    */

#ifdef __cplusplus
}
#endif

#endif