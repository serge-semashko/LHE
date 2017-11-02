/*
Nvram-r.c: 1. Reads the value that has been written to the NVRAM.
           2. The value will automatically increase by 1.
         
Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)

Compile mode: Large

Project: Nvram-r.c
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)
         
Hordware: i-7188/uPAC-7186/i-8000

[Oct 30, 2008] by Liam
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "..\..\LIB\7186e.h"

void main(void)
{
    int addr=0;
    unsigned char cmd[80], result[80];
    
    InitLib();
   
    while(addr<31)
    {
        DelayMs(10);
        Print("\n\rThe valume of writing to NVRAM address %d is %d", addr, ReadNVRAM(addr));
        
        addr=addr++;
    }
}