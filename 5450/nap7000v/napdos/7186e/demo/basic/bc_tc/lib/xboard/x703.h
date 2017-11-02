/********************************************************************/
/*  7188XB,7188XG,7188EX,7188EG,uPAC-7186EG + X703 head file        */
/*                                                                  */
/*  [Oct 16, 2008] by Vic [Ver 1.001]                               */
/*  [May 16, 2011] by Nicholas                                      */
/********************************************************************/
/********************************************************************/
/*  X703: 3-axis encoder counter                                    */
/********************************************************************/
#ifndef __X703_H
#define __X703_H

#define NOERROR             0   
#define INVAILDCHANNEL      -1
#define MODE_ERROR          -2 
#define HARDWARE_ERROR      -3
#define INVAILDFUNCTION     -4

#ifdef __cplusplus //for C++ compile
extern "C"{
#endif

int X703_GetHardwareID(void);
/*
   Get X703 hardware ID
*/

int X703_GetLatticeVersion(void);
/*This function only can be used for X703 whose hardware ID is 14 */

int X703_GetLibVersion(void);
/*Return 0x0000~0xFFFF*/

void X703_GetLibDate(char *LibDate);

int X703_Init(void);
/* Initialize X703 

   Return 0 ==>success
*/
int X703_SetMode(int ch, int Mode);
/*
   Set encoder counting mode
   
   channel ch=1~3
   Mode=1 CW/CCW counting mode
        2 Pulse/Direction counting mode
        3 quadrant counting mode
   
   Return CHANNEL_OUT_RANGE
          MODE_ERROR
          OK                          
*/
int X703_GetMode(int ch, int* Mode);
/*
   Get encoder counting mode
   
   channel ch=1~3
   Mode=0 stop mode
        1 CW/CCW counting mode
        2 Pulse/Direction counting mode
        3 quadrant counting mode
        
   Return CHANNEL_OUT_RANGE
          OK                         
*/
int X703_GetLineStatus(int ch, int* A_Status,int* B_Status,int* C_Status);
/*
   Read A/B/Z status
   
   channel ch=1~3
   Read A status *A_Status=0/1
   Read B status *B_Status=0/1
   Read Z status *C_Status=0/1
   
   Return CHANNEL_OUT_RANGE 
          OK                 
*/
int X703_GetIndex(int ch,int* index);
/*
   Read Index(Z) value
   
   channel ch=1~3
   index=0/1
   
   Return CHANNEL_OUT_RANGE
          OK
*/

int X703_GetIndexLatchStatus(int ch, int* latch, int* status);

int X703_SetIndexLatchStatus(int ch, int status);

int X703_SetXOR(int ch, int Xor);
/*
   Set XOR value 
   
   channel ch=1~3
   Xor=0/1
   
   Return CHANNEL_OUT_RANGE
          MODE_ERROR
          OK
*/
int X703_GetXOR(int ch, int* Xor);
/*
   Get XOR value
   
   channel ch=1~3
   *Xor=0/1   
   
   Return CHANNEL_OUT_RANGE
          OK      
*/
int X703_Read24BitEncoder(int ch, long* EnCode24);
/*
   Read 24bit counter value
   
   channel ch=1~3
   *EnCode24=0x000000~0xFFFFFF
    
   Return CHANNEL_OUT_RANGE
          OK      
          
   Note: If the hardware ID is 14, it is recommended to use X703_Read32BitEncoder function
*/

int X703_Read32BitEncoder(int ch, long* EnCode32);
/*
   This function only can be used for X703 whose hardware ID is 14 
   
   Read 32bit counter value
   
   channel ch=1~3
   *EnCode24=0x00000000~0xFFFFFFFF
    
   Return CHANNEL_OUT_RANGE
          OK  
   
*/

int X703_ResetEncoder(int ch);
/*
   Reset encoder ==> Clear A/B/Z and 24 bit counter
   
   channel ch=1~3
   
   Return CHANNEL_OUT_RANGE
          OK   
*/
int X703_SetPresetValue(int ch, long presetVal);

//int X703_ReadFreq(int ch, float* freq);	//no frequency function

int X703_ClearLatchedIndex(int ch);


#ifdef __cplusplus
}
#endif

#endif
