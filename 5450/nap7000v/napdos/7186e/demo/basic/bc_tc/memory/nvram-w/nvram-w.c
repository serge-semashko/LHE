/*
Nvram-w.c: 1. Input a value and write to NVRAM.
           2. The value will write to the address of NVRAM from 0 to 30.
           3. The value will automatically increase by 1 when it is werited to next address.
         
Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)

Compile mode: Large

Project: Nvram-w.c
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)
         
Hordware: i-7188/uPAC-7186/i-8000

[Oct 30, 2008] by Liam
*/

#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include "..\..\lib\7186e.h"

void main(void)
{
    int addr=0;
    int data,x,y;
    unsigned char cmd[80], result[80];
    
    InitLib();
    
    Print("Please insert a valume to write to NVRAM: ");
    Scanf("%d", &data);
    
    while(addr<31)
    {
        DelayMs(10);
        WriteNVRAM(addr, data);
        Print("\n\rWrite the valume %d to NVRAM address %d", data, addr);
        
        addr=addr++;
        data++;
    }
}