/********************************************************************/
/*  7188EX + X119 head file                                         */
/*                                                                  */
/*  [07, Oct, 2005] by Liam                                         */
/*  [22, Mar, 2007] by Liam                                         */
/********************************************************************/
/********************************************************************/
/*  X119:  7 D/I channels                                           */
/*         7 D/O channels                                           */
/********************************************************************/

#ifndef __X119_H
#define __X119_H

#ifdef __cplusplus
extern "C" {
#endif

int X119_Init(void);
/*  Always returns 0    */

unsigned X119_GetLibVersion(void);
/*  Current version is 2.01 (return 0x0201) */

int X119_Read_All_DI(void);
/*  Return data =  0x00~0x7f
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

int X119_Read_One_DI(int iChannel);
/*  iChannel = 0 ~ 6
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

void X119_Write_All_DO(int iOutValue);
/*  iOutValue: 0x00 ~ 0x7f  */

void X119_Write_One_DO(int iChannel, int iStatus);
/*  iChannel = 0 ~ 6
    iStatus = 1 => Status is ON
    iStatus = 0 => Status is OFF    */

int X119_Read_All_DO(void);
/*  Return data =  0x00~0x7f    */

int X119_Read_One_DO(int iChannel);
/*  iChannel = 0 ~ 6
    Return 1 => ON
    Return 0 => OFF */

#ifdef __cplusplus
}
#endif

#endif