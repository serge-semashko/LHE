/* 
DT.c: Reads the date and time from a RTC and prints it on a monitor
      (user can set the date and time).

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52. 

Compile mode: Large

Project: DT.c
         ..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)

Hardware: i-7188/uPAC-7186/i-8000

[Oct 30, 2008] by Liam
*/

#include <stdlib.h>
#include "..\lib\7186e.h"

void main(void)
{
    int year, month, day, hour, min, sec;
    int oyear, omonth, oday, ohour, omin, osec;
    int quit=0;
    
    InitLib();
    
    oyear=omonth=oday=ohour=omin=osec=-1;
    
    Print("Start dt.exe (Press q to stop program)\r\n");
    while(!quit)
    {
        if(Kbhit())
        {
            switch(Getch())
            {
                case 'q':
                    quit=1;
                    break;
                    
                case 'd':
                    SetDate(2000, 2, 29);   // set date to 02/29/2000 
                    break;
                    
                case 't':
                    SetTime(23, 59, 0);
                    break;
                    
                case 'y':
                    SetDate(2000, 2, 28);   // set date to 02/28/2000
                    SetTime(23, 59, 0);
                    break;
            }
        }
        
        GetDate(&year,&month,&day);
        GetTime(&hour,&min,&sec);
        if(oyear!=year||omonth!=month||oday!=day||ohour!=hour||omin!=min ||osec!=sec)
        {
            oyear=year;
            omonth=month;
            oday=day;
            ohour=hour;
            omin=min;
            osec=sec;
            
            Print("Date=%02d/%02d/%04d  Time=%02d:%02d:%02d\n\r", month, day, year, hour, min, sec);
        }
    }
}
