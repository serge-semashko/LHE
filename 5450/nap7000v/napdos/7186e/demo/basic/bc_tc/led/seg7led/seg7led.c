/*
Seg7led.c : Controls the LED and 5-digit 7-segment LED.

Compiler: BC++ 3.1,
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.

Compile mode: Large

Project: Seg7led.c
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)
         
Hardware: i-7188/uPAC-7186/i-8000
    
[Oct 30, 2008] by Liam
*/

#include <stdlib.h>
#include "..\..\lib\7186e.h"

void main(void)
{
    int i, data;
    
    InitLib();
    
    for(;;)
    {
        for(i=0; i<10; i++)
        {
            if(i==0 || i==2 || i==4 ||i==6 || i==8)
                LedOn();    //To switch the LED ON 
            else
                LedOff();   //To switch the LED OFF
            
            //Show any segment of the 5-Digit Led
            Show5DigitLed(1, i);
            Show5DigitLed(2, i);
            Show5DigitLed(3, i);
            
            //Show any segment of the 5-Digit Led and also can show the DOT '.'.
            Show5DigitLedWithDot(4, 0xA);    //Show the fourth led is 'A'. 
            Show5DigitLedWithDot(5, 0xE);    //Show the fifth led is 'E'.
            
            if(Kbhit())
            {
                data=Getch();
                if(data=='q' || data=='Q')
                {
                    return;
                }
            }
            
            Delay(500); //Show the value of 7seg in every 500 ms.
        }
    }
}