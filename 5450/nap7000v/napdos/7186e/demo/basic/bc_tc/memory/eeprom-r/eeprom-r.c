/*
EEPROM-r.c: Reads the data that has been written to the EEPROM.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          
Compile mode: Large

Project: EEPROM-r.c
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)
         
Hordware: i-7188/uPAC-7186/i-8000

Detail description:
    Following list is where the user should avoid to write to
        For i-7188E(Xserver)    ==> block 0, 1, 2 and 7
        For i-7188E-MTCP        ==> block 0, 1, 2, 3, 4 and 7
        For uPAC-7186EX(Xserver)==> block 0, 1, 2 and 7
        For uPAC-7186EX-MTCP    ==> block 0, 1, 2, 3, 4 and 7
        For 8x1x                ==> block 7
        For 8x3x (Xserver)      ==> block 0 and 7
        For 8000E-MTCP          ==> block 0, 1, 2, 3 and 7

[Oct 30, 2008] by Liam
*/

#include <stdlib.h>
#include "..\..\lib\7186e.h"

void main(void)
{
    int i, j, addr=0, data;
    
    InitLib();
    
    while(addr<256)
    {
        DelayMs(10);
        Print("\r\n The value writing to 1 block address %d of EEPROM is %d", addr, ReadEEP(1, addr));
        
        addr++;
        if (addr%25==0)
        {
            Print("\n\rPress any key to continue...");
            Getch();
        }
    }
}