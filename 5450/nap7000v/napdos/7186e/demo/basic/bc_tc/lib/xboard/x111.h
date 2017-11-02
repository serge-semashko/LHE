/********************************************************************/
/*  7188EX/7188XB/7186EX + X111 head file                           */
/*                                                                  */
/*  [06, Oct, 2005] by Liam                                         */
/*  [22, Mar, 2007] by Liam                                         */
/********************************************************************/
/********************************************************************/
/*  X111: 13 D/O channels                                           */
/*                                                                  */
/********************************************************************/

#ifndef __X111_H
#define __X111_H

#ifdef __cplusplus
extern "C" {
#endif

int X111_Init(void);
/*  Always returns 0    */

unsigned X111_GetLibVersion(void);
/*  Current version is 2.00 (return 0x0200) */

void X111_Write_All_DO(int iOutValue);
/*  iOutValue: 0x0000 ~ 0x3F7F  */

void X111_Write_One_DO(int iChannel, int iStatus);
/*  iChannel = 0 ~ 12
    iStatus = 1 => Status is ON
    iStatus = 0 => Status is OFF    */

int X111_Read_All_DO(void);
/*  return data = 0x00 ~ 0x3F7F */

int X111_Read_One_DO(int iChannel);
/*  iChannel = 0 ~ 12
    Return 1 => ON
    Return 0 => OFF */

#ifdef __cplusplus
}
#endif

#endif