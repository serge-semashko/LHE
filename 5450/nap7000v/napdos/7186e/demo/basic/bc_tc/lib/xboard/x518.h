/********************************************************************/
/*  7188EX/7188XB/7186EX + X518 head file                           */
/*                                                                  */
/*  [11, Oct, 2005] by Liam                                         */
/*  [23, Mar, 2007] by Liam                                         */
/********************************************************************/
/********************************************************************/
/*  X518: 4 D/I channel                                             */
/*        COM3 (RS-232 5-wire)                                      */
/********************************************************************/

#ifndef __X518_H
#define __X518_H

#ifdef __cplusplus
extern "C" {
#endif

int X518_Init(void);
/*  Always return 0 */

unsigned X518_GetLibVersion(void);
/*  Current version is 2.01 (return 0x0201) */

void X518_Write_All_DO(int iOutValue);
/*  iOutValue: 0x00 ~ 0x00ff    */

void X518_Write_One_DO(int iChannel, int iStatus);
/*  iChannel = 0 ~ 7
    iStatus = 1 => Status is ON
    iStatus = 0 => Status is OFF    */

int X518_Read_All_DO(void);
/*  Return data =  0x00~0x00ff  */

int X518_Read_One_DO(int iChannel);
/*  iChannel = 0 ~ 7
    Return  1 => ON
    Return  0 => OFF    */

#ifdef __cplusplus
}
#endif

#endif
