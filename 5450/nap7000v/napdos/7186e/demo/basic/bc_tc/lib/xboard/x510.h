/********************************************************************/
/*  7188EX/7188XB/7186EX + X510(X510-128) head file                 */
/*                                                                  */
/*  [11, Oct, 2005] by Liam                                         */
/*  [23, Mar, 2007] by Liam                                         */
/********************************************************************/
/********************************************************************/
/*  X510: 5 D/I channels                                            */
/*        5 D/O channels                                            */
/*        COM3 (3-wire RS-232)                                      */
/*        EEPROM 128K*2 bytes                                       */
/********************************************************************/
/********************************************************************/
/*  EEPROM:                                                         */
/*    24LC1024, total=128K bytes, 2 blocks, 65536(64K) bytes/block  */
/*    In X510 there are two 24Lc1024, so totaly has 4 blocks.       */
/*                                                                  */
/*  In the following functions, the parameters:                     */
/*  Block, StartBlock: range=0 to 3                                 */
/*  Addr, StartAddr:  range=0 to 65535                              */
/********************************************************************/

#ifndef __X510_H
#define __X510_H

#ifdef __cplusplus
extern "C" {
#endif

#define GetSerialNumber     X510_GetSerialNumber

int X510_Init(void);
/*  Always returns 0    */

unsigned X510_GetLibVersion(void);
/*  Current version is 2.01 (return 0x0201) */

static void X510EE_Init(void);
int X510EE_RandomRead(int Block,unsigned Addr);
int X510EE_ReadNext(int Block);
int X510EE_MultiRead(int StartBlock,unsigned StartAddr,int no,char *databuf);
void X510EE_WriteEnable(void);
void X510EE_WriteProtect(void);
int X510EE_RandomWrite(int Block,unsigned Addr,int Data);
int X510EE_MultiWrite(int Block,unsigned Addr,int no,char *Data);
static void X510EE_InitWriteMode(unsigned CtrlByte);
static void X510EE_ReadAck(void);
int X510_Read_All_DI(void);
/*  Return data = 0x00~0x1f
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)*/

int X510_Read_One_DI(int iChannel);
/*  iChannel = 0 ~ 4
    Return 1 => open
                Logic high level (+3.5V ~ +30V)
    Return 0 => close to GND
                Logic low level (0V ~ +1V)  */

void X510_Write_All_DO(int iOutValue);
/*  iOutValue: 0x00 ~ 0x1f  */

void X510_Write_One_DO(int iChannel, int iStatus);
/*  iChannel = 0 ~ 4
    iStatus = 1 => Status is ON
    iStatus = 0 => Status is OFF    */

int X510_Read_All_DO(void);
/*  Return data = 0x00 ~ 0x1f   */

int X510_Read_One_DO(int iChannel);
/*  iChannel = 0 ~ 4
    Return  1 => ON
    Return  0 => OFF    */

int X510_GetSerialNumber(char *serial); 
/*  Use an array of size 8 to denote a serial number
    Return  1 => retrieve a serial number successfully
            0 => retrieve a serial number unsuccessfully
    
    if the retrieving serial number was 0x55, it denotes that the serial chip cannot be found
    if the retrieving serial number was 0xaa, it denotes that the cyclic redundancy check (CRC) is not correct  */

static void X510EE_Stop(void);
static void X510EE_WriteByte(unsigned char ByteData);

#ifdef __cplusplus
}
#endif

#endif
