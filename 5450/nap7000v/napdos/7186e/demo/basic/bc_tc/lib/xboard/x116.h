/********************************************************************/
/*  7188EX/7188XB/7186EX + X116 head file                           */
/*                                                                  */
/*  [06, Oct, 2005] by Liam                                         */
/*  [22, Mar, 2007] by Liam                                         */
/********************************************************************/
/********************************************************************/
/*  X116:  4 D/I channels                                           */
/*         6 Relay Output channels                                  */
/********************************************************************/

#ifndef __X116_H
#define __X116_H

#ifdef __cplusplus
extern "C" {
#endif

int X116_Init(void);
/*  Always returns 0    */

unsigned X116_GetLibVersion(void);
/*  Current version is 2.00 (return 0x0200) */

int X116_Read_All_DI(void);
/*  Return data =  0x00~0x0f
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

int X116_Read_One_DI(int iChannel);
/*  iChannel = 0 ~ 3
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

void X116_Write_All_DO(int iOutValue);
/*  iOutValue: 0x00 ~ 0x3f  */

void X116_Write_One_DO(int iChannel, int iStatus);
/*  iChannel = 0 ~ 5
    iStatus = 1 => Status is ON
    iStatus = 0 => Status is OFF    */

int X116_Read_All_DO(void);
/*  Return data =  0x00~0x3f    */

int X116_Read_One_DO(int iChannel);
/*  iChannel = 0 ~ 5
    Return 1 => ON
    Return 0 => OFF */

#ifdef __cplusplus
}
#endif

#endif