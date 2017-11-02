/*
Flash.c: 1. To check Flash size
         2. If you want write to segment 0xD000 --> set base 0xD000,
            but must make sure your ROM-DISK.IMG size <= 64K bytes
         3. When you erase sector 0xD000,ROM Disk(A:) will be damaged.
         4. Base can be 0xD000 or 0xE000. ----- for 7186E(D)/DOS 256K Flash Memory
         ** Press 'q' to quit this program. 
         
Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)

Compile mode: Large

Project: Flash.c
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)
         
Hordware: i-7188/uPAC-7186/i-8000

[Oct 30, 2008] by Liam
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "..\..\lib\7186e.h"

unsigned base=0xE000;
unsigned offset=0;
extern unsigned char ProtectMode;
char *Prompt="\n\rFlash>";

void RunCmd(char *cmd);

void ShowHelp(void)
{
    Print("\n\rFlash.exe Ver 1.02. Utility for 7186E(D)(/512) Flash Memory.");
    Print("\n\rCommand: Erase --> Erase current segment to 0xFF");
    Print("\n\r         W offset(Hex) data(Hex) --> Write data(byte) to Seg:off");
    Print("\n\r         'f'('F') data(Hex) --> Fill current block with data");
    Print("\n\r         D offset(Hex) --> Dump data(64 bytes) from Seg:off");
    Print("\n\r         Enter --> Dump next 64 bytes");
    Print("\n\r         'u','U' --> Dump Last 64 bytes");
    Print("\n\r         'q','Q' --> quit to 7186R System");
    Print("\n\r         's'('S') segment(Hex) --> set new segment");
}

void main(void)
{ 
    int quit=0;
    int key;
    char cmd[80];
    int idx=0;
    unsigned int id, fid;
    
    InitLib();

    id=Is7186e();
    ShowHelp();

    fid=FlashReadId();
    Print("\n\rFlash ID=%X -->", fid);
    if((fid&0xff)==1)
        Print("AMD");
    else if((fid&0xff)==0xC2)
        Print("MXIC");
    else
        Print("???");
  
    if((fid>>8)==0xB0)
        Print("29F002T(256K)");
    else if((fid>>8)==0xA4)
        Print("29F040(512K)");
    else if((fid>>8)==0xB5)
        Print("512K");
    else
        Print("???(???K)");
    
    Print("\n\rProtect status:(1:protected, 0:nonprotected");
    
    if(id==512)
    {
        for(idx=0; idx<8; idx++)
        {
            Print("\n\rSector 0x%04X:%c",0x8000+idx*0x1000,(ProtectMode&(0x80U>>idx))? '1':'0');
        }
    }
    else if(id==256)
    {
        for(idx=0;idx<3;idx++)
        {
            Print("\n\rSector 0x%04X:%c",0xC000+idx*0x1000,(ProtectMode&(0x40U>>idx))? '1':'0');
        }
        
        Print("\n\rSector 0xF000:%c",(ProtectMode&(0x40U>>3))? '1':'0');
        Print("\n\rSector 0xF800:%c",(ProtectMode&(0x40U>>4))? '1':'0');
        Print("\n\rSector 0xFA00:%c",(ProtectMode&(0x40U>>5))? '1':'0');
        Print("\n\rSector 0xFC00:%c",(ProtectMode&(0x40U>>6))? '1':'0');
    }
    
    idx=0;
    Print("\n\rCurrent segment is %4X", base);
    Print(Prompt);
    while(!quit)
    {
        key=Getch();
        Putch(key);
        if(key=='\r')
        {
            Putch('\n');
            cmd[idx]=0;
            if(cmd[0]=='q' || cmd[0]=='Q')
                quit=1;
            else
            {
                RunCmd(cmd);
                idx=0;
                Print(Prompt);
            }
        }
        else if(key=='\b')
        {
            if(idx)
                cmd[--idx]=0;
        }
        else
        {
            cmd[idx++]=key;
        }
    }
}

void Dump(void)
{
    int i, j;
    unsigned char far *data;

    for(j=0; j<4; j++)
    {
        data=(unsigned char far *)((((long)base)<<16)+offset);
        Print("[%04X:%04X] ", base, offset);
        
        for(i=0; i<16; i++)
        {
            Print("%02X ", *(data+i));
        }
        
        Print("  ");
        for(i=0; i<16; i++)
        {
            Print("%c", *(data+i)<0x20 ? '.':*(data+i));
        }
        
        offset+=16;
        Print("\n\r");
    }
}

void CheckSegment(unsigned base, int data)
{
    unsigned i;
    unsigned error=0;
    
    for(i=0; i<65535; i++)
    {
        if(data != FlashRead(base,i))
        {
            error++;
            Print("\n\rError on offset %04X,data is %02X,anykey to continue,"
                  "'q' to stop verify.", i, FlashRead(base,i));
            
            if(Getch()=='q') 
                goto EndCheck;
        }
    }
    
    if(data != FlashRead(base, i))
    {
        error++;
        Print("\n\rError on offset %04X,data is %02X", i, FlashRead(base, i));
    }
    
EndCheck:
    Print("\n\rVerify,Find %u errors", error);
}

void RunCmd(char *cmd)
{ 
    unsigned dataoffset;
    unsigned data, i;

    switch(cmd[0])
    {
        case 0:
            Dump();
            break;
        
        case 'f': case 'F':
            sscanf(cmd+1, "%x", &data);
            for(i=0; i<65535; i++)
            {
                FlashWrite(base, i, data);
            }
            FlashWrite(base, i ,data);
            CheckSegment(base, data);
            break;
            
        case 'd': case 'D':
            sscanf(cmd+1, "%x", &offset);
            Dump();
            break;
            
        case 'w': case 'W':
            sscanf(cmd+1,"%x %x", &dataoffset, &data);
            Print("\n\rBefore write %04X:%04X is 0x%02X", base, dataoffset, FlashRead(base, dataoffset));
            FlashWrite(base,dataoffset,data);
            Print("\n\rAfter write %04X:%04X is 0x%02X", base, dataoffset, FlashRead(base, dataoffset));
            if(data != FlashRead(base, dataoffset))
            {
                Print("\n\rWriteFlash Error");
            }
            break;
            
        case 's': case 'S':
            sscanf(cmd+1, "%x", &base);
            Print("\n\rSet segment to %4X", base);
            break;
            
        case 'u': case 'U':
            offset-=128;
            Dump();
            break;
            
        case 'E': case 'e':
            if(!stricmp(cmd, "ERASE"))
            {
                FlashErase(base);
                CheckSegment(base, 0xff);
            }
            break;
            
        case 'h': case 'H': case '?':
            ShowHelp();
            break;
    }
}