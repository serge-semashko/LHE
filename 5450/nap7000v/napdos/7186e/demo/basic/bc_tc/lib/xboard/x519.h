/********************************************************************/
/*  7188EX/7188XB/7186EX + X519 head file                           */
/*                                                                  */
/*  [26, Oct, 2005] by Liam                                         */
/*  [23, Mar, 2007] by Liam                                         */
/********************************************************************/
/********************************************************************/
/*  X519: 8 D/I channels                                            */
/*        COM3 (RS-485)                                             */
/********************************************************************/

#ifndef __X519_H
#define __X519_H

#ifdef __cplusplus
extern "C" {
#endif

int X519_Init(void);
/*  Always returns 0    */

unsigned X519_GetLibVersion(void);
/*  Current version is 2.01 (return 0x0201) */

int X519_Read_All_DI(void);
/*  Return data = 0x00~0xff
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)   */

int X519_Read_One_DI(int iChannel);
/*  iChannel = 0 ~ 5
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

#ifdef __cplusplus
}
#endif

#endif