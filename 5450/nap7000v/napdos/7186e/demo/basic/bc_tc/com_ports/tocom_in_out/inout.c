/* 
InOut.c: Reads/Writes the byte data via the COM port.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.
          
Compile mode: large

Project: InOut.c
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)

Detail description:
    This demo use block style loop to read data from COM Port,and exit the loop if 
    hit the ESC key.
    
    ToCom and ToComBufn will write Byte data to COM port,
    ToComStr will Send out a string(end with '\0') to COM port.
    In this demo,it also show how to use diffenent format of Byte data,
    such as unsigned char data(ASCII) 'A' will represent as 0x41 in hex format 
    or 65 in decimal format.

Hardware: i-7188/uPAC-7186/i-8000

[Oct 30, 2008] by Liam
*/

#include <stdio.h>
#include <conio.h>
#include "..\..\lib\7186e.h"    /* you must change this lib to suit the hardware that you want to use. */

void main(void)
{
    unsigned char outBuf[100];
    int iCommandCOMPort, i;
    
    iCommandCOMPort=1;
    
    InitLib();
    InstallCom(iCommandCOMPort, 115200L, 8, 0, 1);
    ToComBufn(iCommandCOMPort, "Use ToCom(port,char)\n", 21); 
    
    // ASCII
    outBuf[0]= 'A';
    outBuf[1]= 'B';
    outBuf[2]= 'C';
    outBuf[3]= 'D';
    
    // Hex -> ASCII visible character
    outBuf[4]=0x31;     // 0x31 = '1'
    outBuf[5]=0x65;     // 0x65 = 'e'
    
    // DEC -> ASCII visible character
    outBuf[6]=57;   // 57 = '9'
    outBuf[7]=43;   // 43 = '+'
    
    // Hex -> ASCII not visible character    
    outBuf[8]=0x07;     // 0x07 = bell
    outBuf[9]=0x0A;     // 0x0A = LF (line feed)
    
    // use ToCom(port,char) to send 1 byte to COM Port one by one.
    for(i=0; i<10; i++)
    {
        ToCom(iCommandCOMPort, outBuf[i]);
    }
    Delay(3000);
 
    ToComBufn(iCommandCOMPort, "Use ToComBufn(port,charBuf,bytes)\n", 31); 
    
    // use ToComBufn(port,charBuf,bytes) to send an array to COM Port at a time. 
    ToComBufn(iCommandCOMPort, outBuf, 10);   
    
    printCom(iCommandCOMPort, "Now please hit any key to show the ASCII Table,\n");
    printCom(iCommandCOMPort, "Hit the ESC to exit the program !\n");

    for(;;)
    {
        unsigned char item;
        
        if(IsCom(iCommandCOMPort))
        {
            item=ReadCom(iCommandCOMPort);
            if(item==0x1B) //"Hit the ESC to exit the program"
            {
                RestoreCom(iCommandCOMPort);
                return;
            }
            else
            {
                printCom(iCommandCOMPort, "ASCII(%c);Hex(%02X);DEC(%03d)\n\r", item, item, item);
            }
        }
    }
}