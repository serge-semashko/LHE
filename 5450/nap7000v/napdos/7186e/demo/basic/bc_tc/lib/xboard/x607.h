/********************************************************************/
/*  7188EX/7188XB/7186EX + X607 head file                           */
/*                                                                  */
/*  [20, Jun, 2002] by David Version : 1.00                         */
/*  [11, Oct, 2005] by Liam                                         */
/*  [23, Mar, 2007] by Liam                                         */
/********************************************************************/
/********************************************************************/
/*  X607: 128K SRAM Battery BAckup (10 year)                        */
/********************************************************************/
/********************************************************************/
/* Funtion:   128K Byte SRAM (1k Block,one Block is 128 Byte)       */
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
/* Block 1021  |   |   |   |               |   |   |   |            */
/*             |---+---+---+ ------------- +---+---+---+            */
/* Block 1022  |   |   |   |               |   |   |   |            */
/*             |---+---+---+ ------------- +---+---+---+            */
/* Block 1023  |   |   |   |               |   |   |   |            */
/*             |---+---+---+ ------------- +---+---+---+            */
/* Block 1024  |   |   |   |               |   |   |   |            */
/*             |---+---+---+ ------------- +---+---+---+            */
/*                                                                  */
/********************************************************************/

#ifndef __X607_H
#define __X607_H

#ifdef __cplusplus
extern "C" {
#endif

#define NoError 0
#define OffsetError -2
 
int X607_Init(void);
/*  If you want to read /write the SRAM of XBoard,please run initial function first.
    Return  0
    READ/WRITE access function by [block,offset] mode 
    READ/WRITE access size = 1 Byte
    block = 0~1023, offset = 0~127, data = 1Byte(0~255) */

unsigned X607_GetLibVersion(void);
/*  Current version is 2.01 (return 0x0201) */

int X607_Read(int block, int offset);
/*  return   0       OK
            -1       BlockError
            -2       OffsetError
            other    data   */
            
int X607_Write(int block, int offset, unsigned char data);
/*  return   0    OK
            -1    BlockError
            -2    OffsetError

    READ/WRITE access function by [absolute address] mode
    READ/WRITE access size = len*1 Byte
    address = 0~(128*1024-1) ,len = 0~32768   */

int X607_ReadFn(unsigned long address, int len, unsigned char *data);
/*  return   0  OK  */

void X607_WriteFn(unsigned long address, int len, unsigned char *data);

#ifdef __cplusplus
}
#endif

#endif
