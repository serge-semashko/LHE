/*
Compiler: BC++ 3.1,
          Turbo C++ 1.01 (3.01)

Compile mode: large

Project: Puts.c
         ..\..\lib\7186el.lib
         ..\..\lib\mfs_vnnn.lib

Hardware: uPAC-7186EX-FD
*/

#include <stdlib.h>
#include "..\..\lib\7186e.h"
#include "..\..\lib\MFS.h"

void main(void)
{
    int iFileHandle, iRet;
    
    InitLib();
    iRet=mfs_Init();
    if(iRet!=64) {
        Print("MFS init failed\n");
        return;
    }
    
    iFileHandle=mfs_OpenFile(DISKA, "Test.txt", "w");
    if(iFileHandle>0)
    {
        Print("Writing string to Test.txt...");
        mfs_Puts(iFileHandle, "test mfs on 64MB flash");
        mfs_CloseFile(iFileHandle);
        Print("done.");
    }
    else
        Print("Open A:\Test.txt error\n");
        
    mfs_Stop();
}