/*
SCANF.c: 1. Shows how to write a function to input data.
         2. Shows how to retrieve a string.
         3. Shows how to use C function: sscanf, or just use Scanf().
         ** MSC 6.0 cannot use Scanf(), because don't support function vsscanf()
        
Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          
Compile mode: large

Project: Scanf.C
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)

Hordware: i-7188/uPAC-7186/i-8000

Note: Before the program call InstallCom1(), it can use Kbhit(), Getch(), Putch(),Print(), Scanf()... 
      to send/receive data to/from COM1 of i-7188/uPAC-7186/i-8000.

      But after call Installcom1() these functions will not work until the program call RestoreCom1().
  
      All these I/O functions only apply to the main Command port (console port) Command port list
      i-7188EX      COM1
      uPAC-7186EX   COM1
      i-8000        COM1

[Oct 30, 2008] by Liam
[03, June, 2010] by Nicholas
*/

#include <stdlib.h>
#include <stdio.h>
#include "..\..\lib\7186e.h"

char buf[100];

void main(void)
{ 
    int inumber,i2;
    char string[80];
    int inputData;
    
    InitLib();

    // method 1: use LineInput + sscanf
    Puts("Please input integer(-32768 ~ +32767), string:");
    inputData=LineInput(buf, 99);
    if(inputData)
    {
        sscanf(buf, "%d %s", &inumber, string);
        Print("inumber=%d  string=%s", inumber, string);
    }
    else 
    {
        Puts("There is not any input.\r\n");
    }

    // method 2: use Scanf
    // user can try to key in 0, 1, 2 or 3 item, to check the return value of Scanf
    SetScanBuffer(buf, 99);     // set new buffer, so after call Scanf, can get the input string
    while(1)
    {
        Puts("\n\rPlease input integer(-32768 ~ +32767),string, integer again(input null space to quit):"); 
        
		inputData=Scanf("%d %s %d", &inumber, string, &i2);
				
		if( buf[0]>='0' && buf[0]<='9' )
		{			
			if(inputData == 1)
			{
				Print("inputData=%d integer =%d", inputData, inumber);
				Print("\n\rThe input string is:%s\n\r", buf);
			}
			else if(inputData == 2)
			{	
				Print("inputData=%d integer=%d string=%s ", inputData, inumber, string);
				Print("\n\rThe input string is:%s\n\r", buf);
			}
			else if(inputData == 3)
			{
				Print("inputData=%d integer1=%d string=%s integer2=%d", inputData, inumber, string, i2);
				Print("\n\rThe input string is:%s\n\r", buf);
			}			
		}
		else if(buf[0]==' ')
		{
			break;
		}
		else
		{		
			Print("The first input is not an integer.\n\r");
		}				
    }
    
    ResetScanBuffer();
}