/*
Flash-w.c: 1. To write a value which you input to flash memory at segment 0x9000 which
              offset is 0 to 65535 and show it on the monitor.
           2. The value you input will write to Flash memory and automatically increase by 1.
           ** Press 'q' or 'Q' to quit program.
         
Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)

Compile mode: Large

Project: Flash.c
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
    int seg, data;
    char c;
    unsigned e=0x9000,i=0;
    
    InitLib();
    
    Print("\r\nPlease Input a value writting to segment 0x9000 of Flash Member: ");
    Scanf("%d", &seg);
    
    FlashErase(e);
    while(i<65535)
    {
        FlashWrite(e, i, seg);
        Print("\r\nThe value %d is writting to offset %d of Flash Memory", seg, i);
        i++;
        seg++;
        if(i%100==0)
        {
            Print("\r\nPress q to quit or any key to continue...");
            c=Getch();
            if ((c=='q') || (c=='Q'))
                return;
        }
    }
}