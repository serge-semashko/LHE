/* 
EEPROM.c: Writes a value to the EEPROM and shows it on the monitor.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.
          
Compile mode: Large

Project: EEPROM.c
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)

Hardware: i-7188/uPAC-7186/i-8000

Detail description:
    Following list is where the user should avoid to write to
        For i-7188E(Xserver)    ==> block 0, 1, 2 and 7
        For i-7188E-MTCP        ==> block 0, 1, 2, 3, 4 and 7
        For uPAC-7186EX(Xserver)==> block 0, 1, 2 and 7
        For uPAC-7186EX-MTCP    ==> block 0, 1, 2, 3, 4 and 7
        For 8x1x                ==> block 7
        For 8x3x (Xserver)      ==> block 0 and 7
        For 8000E-MTCP          ==> block 0, 1, 2, 3 and 7
        
[Oct 30, 2008] by Liam
*/

#include <stdlib.h>
#include "..\..\lib\7186e.h"

void main(void)
{
    int iBlock, iAddress, iRet, iValue, iByte;
    float fValue;
    
    InitLib();
   
    // ===== For integer =====
    Print("Writes 3 integers (1234, 7188, 8000) to block 1.\r\n");
    EE_WriteEnable();
    iValue=1234;
    iRet=EE_MultiWrite(1, 0, 2, (char*)&iValue);    // block 1, address 0, 2 bytes
    iValue=7188;
    iRet=EE_MultiWrite(1, 2, 2, (char*)&iValue);    // block 1, address 2, 2 bytes
    iValue=8000;
    iRet=EE_MultiWrite(1, 4, 2, (char*)&iValue);    // block 1, address 4, 2 bytes
    EE_WriteProtect();
    
    Print("Gets 3 integers from block 1.\r\n"); 
    iRet=EE_MultiRead(1, 0, 2, (char*)&iValue);     // block 1, address 0, 2 bytes
    Print("  Block 1, Address 0 = %d\r\n", iValue);
    
    iRet=EE_MultiRead(1, 2, 2, (char*)&iValue);     // block 1, address 2, 2 bytes
    Print("  Block 1, Address 2 = %d\n\r", iValue);
    
    iRet=EE_MultiRead(1, 4, 2, (char*)&iValue);     // block 1, address 4, 2 bytes
    Print("  Block 1, Address 4 = %d\n\r", iValue);
    
    Print("Clears the 3 integers.\r\n");  
    EE_WriteEnable();
    iValue=1234;
    iRet=EE_MultiWrite(1, 0, 2, (char*)&iValue);    // block 1, address 0, 2 bytes
    if(iRet!=NoError) Print("Clears block 1, address 0 ===> error !\r\n");
    
    iRet=EE_MultiWrite(1, 2, 2, (char*)&iValue);    // block 1, address 2, 2 bytes
    if(iRet!=NoError) Print("Clears block 1, address 2 ===> error !\r\n");
    
    iRet=EE_MultiWrite(1, 4, 2, (char*)&iValue);    // block 1, address 4, 2 bytes
    if(iRet!=NoError) Print("Clears block 1, address 4 ===> error !\r\n");
    EE_WriteProtect();
    
    Print("Press any key to continue.\r\n\r\n");
    Getch();
    
    // ===== for float values =====
    Print("Writes 3 float values (1234.123, 7188.123, 8000.123) to block 1.\r\n");
    EE_WriteEnable();
    fValue=1234.123;
    iRet=EE_MultiWrite(1, 0x10, 4, (char*)&fValue);     // block 1, address 0x10 
    fValue=7188.123;
    iRet=EE_MultiWrite(1, 0x14, 4, (char*)&fValue);     // block 1, address 0x14
    fValue=8000.123;
    iRet=EE_MultiWrite(1, 0x18, 4, (char*)&fValue);     // block 1, address 0x18
    EE_WriteProtect();
    
    Print("Gets 3 float values from block 1.\r\n"); 
    iRet=EE_MultiRead(1, 0x10, 4, (char*)&fValue);      // block 1, address 0x10, 4 bytes
    Print("  Block 1, Address 10(hex) = %7.3f\r\n", fValue);
    
    iRet=EE_MultiRead(1, 0x14, 4, (char*)&fValue);      // block 1, address 0x14, 4 bytes
    Print("  Block 1, Address 14(hex) = %7.3f\r\n", fValue);
    
    iRet=EE_MultiRead(1, 0x18, 4, (char*)&fValue);      // block 1, address 0x18, 4 bytes
    Print("  Block 1, Address 18(hex) = %7.3f\r\n", fValue);
    
    Print("Clears the 3 float values.\r\n");
    EE_WriteEnable();
    fValue=0.0;
    iRet=EE_MultiWrite(1, 0x10, 4, (char*)&fValue);     // block 1, address 0x10
    if(iRet!=NoError) Print("Clears block 1, address 10(hex) ===> error !\r\n");
    
    iRet=EE_MultiWrite(1, 0x14, 4, (char*)&fValue);     // block 1, address 0x14
    if(iRet!=NoError) Print("Clears block 1, address 14(hex) ===> error !\r\n");
    
    iRet=EE_MultiWrite(1, 0x18, 4, (char*)&fValue);     // block 1, address 0x18
    if(iRet!=NoError) Print("Clears block 1, address 18(hex) ===> error !\r\n");
    EE_WriteProtect();
    
    Print("Press any key to continue.\r\n\r\n");
    Getch();

    // ===== for bytes =====
    Print("Writes 3 bytes ('A', 'B', 'C') to block 1.\r\n");
    EE_WriteEnable();
    iRet=EE_RandomWrite(1, 0x20, 'A');  // block1, address 0x20, byte='A'
    iRet=EE_RandomWrite(1, 0x21, 'B');  // block1, address 0x21, byte='B'
    iRet=EE_RandomWrite(1, 0x22, 'C');  // block1, address 0x22, byte='C'
    EE_WriteProtect();
    
    Print("Gets 3 bytes from block 1.\r\n"); 
    iByte=EE_RandomRead(1, 0x20);
    Print("  Block 1, Address 20(hex) = %c\r\n", iByte);
    
    iByte=EE_RandomRead(1, 0x21);
    Print("  Block 1, Address 21(hex) = %c\r\n", iByte);
    
    iByte=EE_RandomRead(1, 0x22);
    Print("  Block 1, Address 22(hex) = %c\r\n", iByte);
    
    Print("Clears the 3 bytes.\r\n");  
    EE_WriteEnable();
    EE_RandomWrite(1, 0x20, '_'); 
    if(iRet!=NoError)
        Print("Clears block 1, address 20(hex) ===> error !\r\n");
    
    EE_RandomWrite(1, 0x21, '_'); 
    if(iRet!=NoError)
        Print("Clears block 1, address 21(hex) ===> error !\r\n");
    
    EE_RandomWrite(1, 0x22, '_'); 
    if(iRet!=NoError)
        Print("Clears block 1, address 22(hex) ===> error !\r\n");
        
    EE_WriteProtect();
}
