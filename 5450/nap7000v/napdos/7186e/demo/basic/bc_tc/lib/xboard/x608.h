/********************************************************************/
/*  7188EX/7188XB/7186EX + X608 head file                           */
/*                                                                  */
/*  [12, Sep, 2002] by Jason  Version : 1.00                        */
/*  [11, Oct, 2005] by Liam                                         */
/*  [23, Mar, 2007] by Liam                                         */
/********************************************************************/
/********************************************************************/
/*  X608: 512K SRAM Battery BAckup (10 years)                       */
/********************************************************************/
/*************************** M5M51008 *******************************/
/* Funtion:   512K Byte SRAM (1k Block, one Block is 128 Byte)      */
/*                                                                  */
/* Date:       2002/6/20                                            */
/* ControlPin: /OE ---> /RD                                         */
/*             /S1 ---> /CS                                         */
/*              S2 ---> DIO14                                       */
/*              /W ---> /WR                                         */
/*                                                                  */
/*                     offset 0~127 bytes                           */
/*               0   1   2                  125 126 127 byte        */
/*             ----+---+---+---------------+---+---+---+            */
/* Block 0     |   |   |   |               |   |   |   |            */
/*             |---+---+---+ ------------- +---+---+---+            */
/* Block 1     |   |   |   |               |   |   |   |            */
/*             |---+---+---+ ------------- +---+---+---+            */
/*                 :                               :                */
/*                 :                               :                */
/*                 :                               :                */
/*                 :                               :                */
/*                 :                               :                */
/*             ----+---+---+---------------+---+---+---+            */
/* Block 4092  |   |   |   |               |   |   |   |            */
/*             |---+---+---+ ------------- +---+---+---+            */
/* Block 4093  |   |   |   |               |   |   |   |            */
/*             |---+---+---+ ------------- +---+---+---+            */
/* Block 4094  |   |   |   |               |   |   |   |            */
/*             |---+---+---+ ------------- +---+---+---+            */
/* Block 4095  |   |   |   |               |   |   |   |            */
/*             |---+---+---+ ------------- +---+---+---+            */
/*                                                                  */
/********************************************************************/

#ifndef __X608_H
#define __X608_H

#ifdef __cplusplus
extern "C" {
#endif

#define NoError 0
#define OffsetError -2
 
int X608_Init(void);
/*  If you want to read /write the SRAM of XBoard, please run initial function first.
    Return  0
    READ/WRITE access function by ( block, offset ) mode 
    READ/WRITE access size = 1 Byte
    block = 0~1023, offset = 0~127, data = 1Byte(0~255) */

unsigned X608_GetLibVersion(void);
/*  Current version is 2.01 (return 0x0201) */
    
int X608_Read(int block,int offset);
/*  return   0       OK
            -1       BlockError
            -2       OffsetError
            other    data    */
            
int X608_Write(int block, int offset,unsigned char data);
/*  return   0    OK
            -1    BlockError
            -2    OffsetError

    READ/WRITE access function by ( address ) mode
    READ/WRITE access size = len*1 Byte
    address = 0~(512*1024-1), len = 0~65535 */
    
int X608_ReadFn(unsigned long address, int len, unsigned char *buf);
/*  Return  0   OK  */

void X608_WriteFn(unsigned long address, int len, unsigned char *data);

#ifdef __cplusplus
}
#endif

#endif