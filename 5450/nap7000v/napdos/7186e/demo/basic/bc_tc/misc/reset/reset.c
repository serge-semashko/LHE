/*
Reset.c : Restores the initial values. 

Compiler: BC++ 3.1,
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.

Compile mode: Large

Project: Reset.c
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)
        
Hordware: i-7188/uPAC-7186/i-8000

Detail description:
        Restores the initial values.
        
        The time for reset 200 times is about 7 seconds.
        so the reset time is about 0.035 sec. (< 0.1 second)
        (It include the time to find files(autoexec.bat & rsttime.exe)
        
        The check what reason cause the CPU been reset, you can 
        type 'diag' in i-7188> or i-8000> prompt.
        There are 3 reasons can reset the CPU
            1. reset by power on
            2. reset by WDT
            3. reset by reset
        
        To test the program
                Step1. download the reset.exe to the controller.
                Step2. power off and then power on the controller.
                Step3. type 'diag' in the i-7188> or i-8000> prompt. 
                       (it should shows the CPU was reset by power on.)
                       
                Step4. type 'reset.exe' to run the demo program.
                       (you can see prompt's message)
                       
                Step5. press any key to stop to star's loop.
                       
                Step6. type 'diag' in the i-7188> or i-8000> prompt. 
                       (it should shows the CPU was reset by RESET Command.)
            
[Oct 30, 2008] by Liam
*/

#include <conio.h>
#include "..\..\lib\7186e.h"

void (far *Reset_Program)(void)=0xFFFF0000L;    // Program start address.

void main(void)
{ 
    InitLib();
    
    Print("Press any key to begin\n\r");
    Print("and press any key can reset the program.\r\n");
    Getch();
    
    for(;;)
    {
        if(Kbhit())
        {
            Print("uPAC-7186EX have reseted by software."); 
            DelayMs(10);    //Delay 10 ms to wait for all data is sent to COM port.
            
            Reset_Program();    // You can call the function or 
                                // replace it by following 3 asam codes. 
                       
                                // _asm push 0xffff
                                // _asm push 0
                                // _asm retf
        }
        else
            Print("*");     // Print '*' while the program is running.
    }
}