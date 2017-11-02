/*
7K87K.c: Demo program for 7K and 87K module general function.

Compiler: BC++ 3.1,
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp) 
          MSC 6.0,
          MSVC 1.52.
          
Compile mode: large

Project: 7K87K.c
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)
         
Hardware: i-7188/uPAC-7186/i-8000 <== COM port ==> 7K or 87K modelue

[Oct 30, 2008] by Liam

=== English comments ==========================================
This demo program is for all functions of 7k and 87K I/O modules.
Users can refer the demo to excute every commands list in 7k and
 87K I/O module's document. For example: $AAM, #AA, @AA, @AA6... etc.

If users wants to use AI,AO,DI,DO,DIO, we have prepared several
demos. Uers can copy the code from these demos then paste to 
their own program.

=== Chinese comments ==========================================
硂絛ㄒ琌皐癸 87K I/O 家舱场
ㄏノ把σ絛ㄒㄓㄏノ– 87K I/O 家舱㏑もい
ㄒ $AAM, #AA, @AA, @AA6... 单单

安ㄏノ璶ㄏノ AI, AO, DI, DO, DIO 硂ㄇи竒非称
计瞷Θ絛ㄒ祘Α盢硂ㄇ祘Α絏钡狡籹祘Αいㄓㄏノ
*/

#include <stdio.h>
#include "..\..\lib\7186e.h"

void main(void)
{
    unsigned char InBuf[20];   
    
    InitLib();
    
    //Step 1:
    InstallCom_2(115200L, 8, 0, 0); //(Baudrate, Data bit, Parity bit, Stop bit)
    
    //Step 2:
    SendCmdTo7000(2, "$01M", 0); 
        /*
        COM2, Command="$AAM" which AA=01, Checksum=disabled
        
        For example: command="$01M" ===> Gets module name at address 01.
        For detailed command sets, refer to 
            CD:Napdos\DCON\IO_Module\87k_modules.htm or 
            CD:Napdos\7000\Manual\ 
        */
        
    //Step 3:
    ReceiveResponseFrom7000_ms(2, InBuf, 100, 0); 
        /*
        COM2, 
        Put received data to InBuf
        Timeout=100 ms
        Checksum=disabled
        */ 
    
    //Step 4:
    Print("InBuf=%s", InBuf);
        
    //Step 5:
    RestoreCom2();
}