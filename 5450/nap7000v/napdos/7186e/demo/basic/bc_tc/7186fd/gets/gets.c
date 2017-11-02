/*
Compiler: BC++ 3.1,
          Turbo C++ 1.01 (3.01)

Compile mode: large

Project: Gets.c
         ..\..\lib\7186el.lib
         ..\..\lib\mfs_vnnn.lib

Hardware: uPAC-7186EX-FD
*/

#include <stdlib.h>
#include "..\..\lib\7186e.h"
#include "..\..\lib\MFS.h"

void main(void)
{
    int iRet, iFileHandle;
    char Data[128];
    
    InitLib();
    
    /* Initiate the MFS library */
    if((iRet=mfs_Init())!=64)
    {
        Print("MFS.lib initiate failed [%d]\n", iRet);
        return;
    }
    
    iFileHandle=mfs_OpenFile(DISKA, "Test.txt", "r");
    if(iFileHandle>0)
    {
        Print("Reading from Test.txt...\n");
        
        iRet=mfs_Gets(iFileHandle, Data, 128);  //max length is 128 bytes.
        if(iRet>0) Print("Data=%s\n", Data);
        
        mfs_CloseFile(iFileHandle);
        Print("done.");
    }
    else
        Print("Open A:\Test.txt error\n");
        
    mfs_Stop();
}