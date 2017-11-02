/*
Hello.c : Reads library version and flash memory size.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.
          
Compile mode: large

Project: Hello.c
         ..\..\Lib\7186el.Lib

Detail description:
    User can use MiniOS7 utility to load hello.exe to uPAC-7186. 

Hardware: uPAC-7186

[Oct 30, 2008] by Liam
*/

#include "..\..\lib\7186e.h"

void main(void)
{ 
    int iType, iVer;
    
    InitLib();
    
    iType=Is7186e();
    
    if(iType)
    {
        iVer=GetLibVersion();
        Print("Hello uPAC-7186EX! (Flash memory is %d K)\r\n", iType);
        Print("Library version is %d.%02d\r\n", iVer>>8, iVer&0xff);
    }
    else
    {
        Print("Hello PC!, this program is not run under uPAC-7186EX.");
    }
}