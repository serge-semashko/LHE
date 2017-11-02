/*
Flash-r.c: To read the value writing in flash memory address from 1 to 65535.
          **To press 'q' to quit this program.
         
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
    int data;
    unsigned e=0x9000, i=0;
    char c;
    
    InitLib();
    
    while(i<65535)
    {
        FlashRead(e, i);
        Print("\r\nThe value which segment %u of Flash Member is %d", i, FlashRead(e, i));
        i++;
        
        if(i%100==0)
        {
            Print("\r\nPress q to quit or any key to continue...");
            
            c=Getch();
            if((c=='q') || (c=='Q'))
                return;
        }
    }
}