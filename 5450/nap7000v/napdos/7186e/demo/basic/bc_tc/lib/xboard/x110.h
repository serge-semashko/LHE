/********************************************************************/
/*  7188EX/7188XB/7186EX + X110 head file                           */
/*                                                                  */
/*  [06, Oct, 2005] by Liam                                         */
/*  [22, Mar, 2007] by Liam                                         */
/********************************************************************/
/********************************************************************/
/*  X110: 14 D/I channels                                           */
/*                                                                  */
/********************************************************************/

#ifndef __X110_H
#define __X110_H

#ifdef __cplusplus
extern "C" {
#endif

#define X110_DigitalIn      X110_Read_All_DI

int X110_Init(void);
/*  Always returns 0    */

unsigned X110_GetLibVersion(void);
/*  Current version is 2.00 (return 0x0200) */

int X110_Read_All_DI(void);
/*  Return data =  0x0000~0x3fff
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

int X110_Read_One_DI(int iChannel);
/*  iChannel = 0 ~ 13
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

#ifdef __cplusplus
}
#endif

#endif