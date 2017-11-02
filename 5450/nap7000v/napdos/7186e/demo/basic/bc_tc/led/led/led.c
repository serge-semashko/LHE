/*
LED.c: Shows how to use the DelayMs function to switch the LED ON or OFF.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.

Compile mode: Large

Project: LED.c
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)
         
Hordware: i-7188/uPAC-7186/i-8000

Detail description:
    Function used:
        TimerOpen --> begin to use i-7188/uPAC-7186/i-8000 timer
        TimerClose --> stop to use timer
        DelayMs  -->  delay time interval. unit is ms
    Press 'q' to quit program.
        
[Oct 30, 2008] by Liam
*/

#include <stdio.h>
#include "..\..\lib\7186e.h"

void main(void)
{
    int LedMode=0;
    unsigned long value;
    int quit=0;
    
    InitLib();
    
    Print("\n\rTest DelayMs() ...");
    Print("\n\rNow LED must flash once every second");
    Print("\n\rPress 'q' to quit\n\r");
    
    LedOff();
    
    while(!quit)
    {
        if(Kbhit())
        {
            switch(Getch())
            {
                case 'q':
                    quit=1;
                    break;
            }
        }
        
        Delay(500);
        if(LedMode)
        {
            LedMode=0;
            LedOff();
        }
        else
        {
            LedMode=1;
            LedOn();
        }
    }
    
    LedOn();
}