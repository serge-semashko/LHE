/*
EEPROM-w: 1. Input a value and stores it in an EEPROM block 1 peer address.
          2. Value will automatically increase by 1.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)

Compile mode: Large

Project: EEPROM-w.c
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
    int i, j;
    int addr=0, block, data;
    
    InitLib();
    
    Print("Warning:The block of 0 address is used by system\n");
    Print("\r\nPlease insert a value writting to 1 block of EEPROM only: ");
    Scanf("%d", &data);

    EE_WriteEnable();
    
    while(addr<256)
    {
        WriteEEP(1, addr, data);
        Print("\rWritting the value %d to address %d of EEPROM\n", data, addr);
        addr=addr++, data=data++;
    }
    
    EE_WriteProtect();
}