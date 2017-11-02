/*
topmem.c: Demonstration of the AllocateTopMemory() function.
         
Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01)

Compile mode: Large

Project: topmem.c
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)
        
Hordware: i-7188/uPAC-7186/i-8000

Note: download topmem.exe to Flash and run it, will get:
        7186EX_UDP>run
        addr=7FF9:0000
        First time run the program(topmem.exe)
        set data flag and set data to "0x12 0x34 0xAB 0xCD"
        Wait WDT to reset system...
        7186EX_UDP>run      <--- after system reset by WDT, run it again
        addr=7FF9:0000
        Find last data flag
        data:0x12 0x34 0xAB 0xCD    <--- the data saved before system reset by WDT will not lost. 
        
        7186EX_UDP>mcb
        066C:0000 -> |        | type ='M'; m_psp=0x0040; m_size=0x0280(640)[10240]
        08ED:0000 -> |        | type ='M'; m_psp=0x0040; m_size=0x0280(640)[10240] 
        0B6E:0000 -> |* Free *| type ='M'; m_psp=0x0000; m_size=0x7489(29833)[477328]
        7FF8:0000 -> |        | type ='Z'; m_psp=0x0040; m_size=0x0007(7)[112]
        
        7186EX_UDP>

[Oct 30, 2008] by Liam
*/

#include <stdlib.h>
#include "..\..\lib\7186e.h"

void main(void)
{
    unsigned char far *data=AllocateTopMemory(100);
    
    InitLib();
    
    if(data)
    {
        Print("addr=%Fp\n\r", data);
        
        if(data[0]==0x71 && data[1]==0x88)
        {
            Print("Find last data flag\n\r");
            Print("data:0x%02X 0x%02X 0x%02X 0x%02X\n\r", data[2], data[3], data[4], data[5]);
        }
        else
        {
            Print("First time run the program(topmem.exe)\r\n");
            Print("set data flag and set data to \"0x12 0x34 0xAB 0xCD\"\r\n");
            data[0]=0x71;
            data[1]=0x88;
            data[2]=0x12;
            data[3]=0x34;
            data[4]=0xAB;
            data[5]=0xCD;
            Print("Wait WDT to reset system...");
            EnableWDT();
            for(;;) ;
        }
    }
    else
    {
        Print("Allocate memory error!");
        return;
    }
}