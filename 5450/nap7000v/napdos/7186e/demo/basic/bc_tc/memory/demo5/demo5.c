/*
Demo5.c: 1. To read and write NVRAM(0 to 30 totally 31 bytes)
         2. It can read and write NVRAM in any number of cycles
         3. Li-battery for data backup, 10 yeasr in the normal condition

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)

Compile mode: Large

Project: Demo5.c
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)
         
Hordware: i-7188/uPAC-7186/i-8000 

Note: ReadRTC, WriteRTC
      addr: 32-63 for NVRAM
      addr: 0-7 Time, Date
      addr: 8,Control register. Bit7=1:Write protect, 0:Write enable
      If set this bit=1, cannot write data to RTC(time, date, NVRAM)
      that is you can not set new time or date form ROM DOS command
      TIME, DATE. and you cannot write new data to NVRAM.

[Oct 30, 2008] by Liam
*/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
#include "..\..\lib\7186e.h"

void ReadRTC(int addr, int *data);
void WriteRTC(int addr, int data);

int data;

void main(void)
{ 
    unsigned int i=0, j=0;
    int idx, regidx, quit;
    char c, cmd[20];
    unsigned char NVRAM[31];

    InitLib();          // driver initial
    Init5DigitLed();    // 5-digit LED initial

    Show5DigitLed(3, 17);   // show - in 5-digit LED
    
    for(j=0; j<=8; j++)
    {
        ReadRTC(j, (int *)&i);
        Print("\n\rClock[%d]=%02X", j, i);
    }
    
    ReadRTC(7, (int *)&j);
    if(j&0x80)
        Print("\n\rCurrent is Write Protect");
    else
    {
        Print("\n\rCurrent is Write Enable");
        WriteRTC(7, 0x80);      // write protect
        Print("\n\rNow Set to Write Protect");
    }
    
    // read current NVRAM 0-31
    for(j=0; j<31; j++)
    {
        NVRAM[j]=ReadNVRAM(j);
    }
    
    Print("\n\rCurrent Values are:\n\r");
    for(j=0; j<31; j++)
        Print("%3d ", NVRAM[j]);
        
    for(j=0; j<31; j++)
        NVRAM[j]++;
    
    for(j=0; j<31; j++)
        WriteNVRAM(j, NVRAM[j]);    // write to NVRAM for test
    
    Print("\n\rAfter write Values are:\n\r");
    for(j=0; j<31; j++)
        Print("%3d ", ReadNVRAM(j));

    WriteRTC(7, 0x00);      // write enable
    Print("\n\rNow Set to Write Enable");
    for (j=0; j<31; j++)
        WriteNVRAM(j,NVRAM[j]);     // write to NVRAM for test
        
    Print("\n\rAfter write Values are:\n\r");
    for(j=0; j<31; j++)
        Print("%3d ", ReadNVRAM(j));
    
    quit=0;
    cmd[0]=0;
    idx=0;
    regidx=0;
    
    Print("\n\rdemo5>");
    while(!quit)
    {
        if(Kbhit())
        {
            switch(c=Getch())
            {
                case 'q':
                case 'Q':
                    quit=1;
                    break;
                    
                case '\r':
                    Putch('\r');
                    Putch('\n\r');
                    cmd[idx]=0;
                    switch(cmd[0])
                    {
                        case 'a':   /* set all to xx */
                        case 'A':
                            data=atoi(cmd+2);
                            for(j=0; j<31; j++)
                                WriteNVRAM(j, data);
                                
                            Ungetch('s');
                            Ungetch('\r');
                            break;
                            
                        case 's':   /* show all */
                        case 'S':
                            for(j=0; j<31; j++)
                                Print("%3d ", ReadNVRAM(j));
                            break;
                        
                        case 'r':   /* set register idx */
                        case 'R':
                            regidx=atoi(cmd+1);
                            Print("\n\rset idx to %d", regidx);
                            break;
                            
                        case 'd':   /* show current register value */
                        case 'D':
                            ReadRTC(regidx+32, &data);
                            Print("\n\ridx=%d data=%d", regidx, data);
                            break;
                        
                        default:
                            if(cmd[0]>='0' && cmd[0]<='9')
                            {
                                /* set value to current register */
                                data=atoi(cmd);
                                WriteNVRAM(regidx,data);
                                Print("\n\rwrite %d to index %d", data, regidx);
                            }
                    }
                    idx=0;
                    Print("\n\rdemo5>");
                    break;
                    
                case '\b':
                    if(idx)
                    {
                        cmd[--idx]=0;
                        Putch(c);
                    }
                    break;
                    
                default:
                    cmd[idx++]=c;
                    Putch(c);
                    break;
            }
        }
    }
}