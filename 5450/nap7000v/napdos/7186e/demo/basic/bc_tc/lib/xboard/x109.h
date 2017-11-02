/********************************************************************/
/*  7188EX/7188XB/7186EX + X109 head file                           */
/*                                                                  */
/*  [06, Oct, 2005] by Liam                                         */
/*  [22, Mar, 2007] by Liam                                         */
/********************************************************************/
/********************************************************************/
/*  X109: 7 Relay Output channels                                   */
/*                                                                  */
/********************************************************************/

#ifndef __X109_H
#define __X109_H

#ifdef __cplusplus
extern "C" {
#endif

#define X109_DigitalOut     X109_Write_All_DO

int X109_Init(void);
/*  Always returns 0    */

unsigned X109_GetLibVersion(void);
/*  Current version is 2.01 (return 0x0201) */

void X109_Write_All_DO(int iOutValue);
/*  iOutValue: 0x00 ~ 0x7f  */

void X109_Write_One_DO(int iChannel, int iStatus);
/*  iChannel = 0 ~ 6
    Return  1 => ON
    Return  0 => OFF    */

int X109_Read_All_DO(void);
/*  Return data =  0x00~0x7f    */

int X109_Read_One_DO(int iChannel);
/*  iChannel = 0 ~ 6
    Return  1 => ON
    Return  0 => OFF    */

#ifdef __cplusplus
}
#endif

#endif