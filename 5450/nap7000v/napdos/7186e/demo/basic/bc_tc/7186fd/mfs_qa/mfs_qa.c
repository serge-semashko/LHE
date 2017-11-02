/*
MFS_QA.c: Quality Assurence program.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01 (3.01)
      
Compile mode: large

Project: MFS_QA.c 
         ..\..\lib\7186el.lib
         ..\..\lib\mfs_vnnn.lib
         
Hardware: uPAC-7186EX-FD

[Jul 31, 2013] Updade for testing mfs_v221.lib
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>
#include "..\..\lib\7186e.h"
#include "..\..\lib\MFS.h"

#define TEST_LENGTH 32767
#define TEST_LOOP   10

void Test_Dir(void);
void WriteFile(int Disk);
void AppendFile(int Disk);
void DeleteFile(int Disk);

void Test_WritingSpeed(int EnableWriteVerify);
void Test_ReadingSpeed(void);
void Test_PutsSpeed(int EnableWriteVerify);
void Test_GetsSpeed(void);
void Test_PutsGets(void);
void Test_Openfile(void);
void Test_Seek_Tell(void);
void Test_TimeStamp(void);
void Test_TimeStamp_Read(void);
void Test_PutsGets_SameTime(void);
void Test_PutsGets_EOF(void);

void Test_DumpFile(void);
void Test_DumpNVRAM(void);

char far sFileData[TEST_LENGTH];
char far sStandardData[TEST_LENGTH];

void main(void)
{
    unsigned char *MFS_QA_Date=__DATE__;
    int iRet, iAction, iQuit=0;
    unsigned iVer;
    char cData;
    char LibDate[16];

    InitLib();
    
    iRet=mfs_Init();
    if(iRet!=64)
    {
        Print("The version of FAT is not suitable for the current version\n");
        Print("of function library.\n");
        Print("Please reset the Flash (all files will not be retained)\n");
        Print("Do you want to reset the Flash (Y/N)?");
        
        if((cData=Getch())=='y' || cData=='Y')
        {
            if((iRet=mfs_ResetFlash())==NoError)
                Print("ResetFlash() ok.\n");
            else
            {
                Print("ResetFlash() failed! Error code=%d\n", iRet);
                return;
            }
        }
    }

    iVer=mfs_GetLibVersion();
    mfs_GetLibDate((unsigned char *) LibDate);
    
    Print("==============================================\n");
    Print("  Quality Assurence Program for MFS. \n\n");
    Print("  MFS_V%03X.Lib build %02d [%s]      \n", iVer>>4, iVer&0x0F, LibDate);
    Print("  MFS_QA.exe [%s]                    \n", MFS_QA_Date);
    Print("==============================================\n");
    
    while(!iQuit)
    {
        Print("\n");
        Print("0)Quit\n");
        Print("1)Dir\n");
        Print("2)Del all files\n");
        Print("3)WriteFile Speed(Disable WriteVerify)\n");
        Print("4)WriteFile Speed(Enable WriteVerify)\n");
        Print("5)ReadFile Speed\n");
        Print("6)Puts Speed(Disable WriteVerify)\n");
        Print("7)Puts Speed(Enable WriteVerify)\n");
        Print("8)Gets Speed\n");
        Print("9)Puts & Gets functionality\n");
        Print("10)Opening 10 file handles\n");
        Print("11)Tell & Seek\n");
        Print("12)Time Stamp\n");
        Print("==== beta testing ====\n");
        Print("13)Dump File\n");
        Print("14)Dump NVRAM\n");
        Print("15)Test_PutsGets_SameTime\n");
        Print("16)Test_PutsGets_EOF\n");

        Print("Please Select(0~16): ");
        Scanf("%d", &iAction);
        
        if(iAction==0)
        {
            iQuit=1;
            mfs_Stop();
            
            continue;
        }
        
        Putch('\n');
        if(iAction==1) Test_Dir();
        if(iAction==2)
        {
            mfs_DeleteAllFiles(DISKA);
            mfs_DeleteAllFiles(DISKB);
        }
        if(iAction==3) Test_WritingSpeed(0); //DisableWriteVerify
        if(iAction==4) Test_WritingSpeed(1); //EnableWriteVerify
        if(iAction==5) Test_ReadingSpeed();
        if(iAction==6) Test_PutsSpeed(0); //DisableWriteVerify
        if(iAction==7) Test_PutsSpeed(1); //EnableWriteVerify
        if(iAction==8) Test_GetsSpeed();
        if(iAction==9) Test_PutsGets();
        if(iAction==10) Test_Openfile();
        if(iAction==11) Test_Seek_Tell();
        if(iAction==12) Test_TimeStamp();
        
        //==== beta testing =========
        if(iAction==13) Test_DumpFile();
        if(iAction==14) Test_DumpNVRAM();
        if(iAction==15) Test_PutsGets_SameTime();
        if(iAction==16) Test_PutsGets_EOF();
        
        Print("\nPress any key to continue...\n");
        Getch();
    }
}

void Test_Dir(void)
{
    int i, iRet, iFileno, iDisk;
    char fileName[16];
    FILE_DATA fdata;
    
    for(iDisk=DISKA; iDisk<=DISKB; iDisk++)
    {
        Print("Disk %c file lisk:\n", iDisk==DISKA?'A':'B');
        
        if((iFileno=mfs_GetFileNo(iDisk))>=0)
        {
            for(i=0; i<iFileno; i++)
            {
                iRet=mfs_GetFileInfoByNo(iDisk, i, (FILE_DATA *) &fdata);
                if(iRet==NoError)
                {
                    memset(fileName, 0x0, sizeof(fileName));
                    memcpy(fileName, fdata.fname, 12);
                    
                    Print("%d)%12s    %4ld bytes\n", i, fileName, fdata.size);
                }
            }
            
            Print("\nThere are %d File(s)\n", iFileno);
            
            Print("%12lu bytes used\n", mfs_GetUsedSize(iDisk));
            Print("%12lu bytes free\n", mfs_GetFreeSize(iDisk));
            Print("%12lu bytes bad\n", mfs_GetBadSzie(iDisk));
        }
        else Print("GetFileNo() failed! Error code= %d\n", iFileno);
    }
}

void Test_PutsSpeed(int EnableWriteVerify)
{
    /*
    Test item: Puts
    Description:  
        Calls Puts to write strings to "String.txt".
    */
    int iFileHandle;
    int i, iRet, iFaile=0;
    long lLastTimeTicks, lTimeDiff;

    if(EnableWriteVerify) mfs_EnableWriteVerify();
    else mfs_DisableWriteVerify();
    
    if((iFileHandle=mfs_OpenFile(DISKA, "String.txt", "w"))>0)
    {
        Print("OpenFile(): A:String.txt ok, File handle=%d.....\n", iFileHandle);

        //Prepare data: 0,1,2,...,8,9,0,1,....
        for(i=0; i<TEST_LENGTH-1; i++)
            sFileData[i]='0'+i%10;
        sFileData[TEST_LENGTH-1]=0; //add a null character to the end of the string
        
        lLastTimeTicks=GetTimeTicks();
        for(i=0; i<TEST_LOOP; i++)
        {
            iRet=mfs_Puts(iFileHandle, sFileData);
            if(iRet<=0)
            {
                Print("Loop%d Puts failed! Error code=%d\n", i, iRet);
                iFaile=1;
                break;
            }
            Putch('.');
        }
        lTimeDiff=GetTimeTicks()-lLastTimeTicks;
        
        if((iRet=mfs_CloseFile(iFileHandle))==NoError)
        {
            Print("\rCloseFile(): A:String.txt ok.\n");
            if(iFaile==0) 
                Print("\nTest result=ok. Puts %ld Bytes takes %ldms(%5.1fKB/Sec)\n", (long) TEST_LOOP*(long)TEST_LENGTH,
                                                                                     lTimeDiff,
                                                                                     (float) TEST_LOOP*(float)TEST_LENGTH/lTimeDiff); 
        }
        else 
            Print("\rCloseFile(): A:String.txt failed! Error code=%d\n", iRet);
    }
    else
    {
        Print("OpenFile(): A:String.txt failed! Error:%d\n", iFileHandle);
        iFaile=1;
    }
    
    if(iFaile || iRet)
        Print("\nTest result=failed!\n");
    else
        Print("\nTest result=ok!\n");
}

void Test_GetsSpeed(void)
{
    /*
    Test item: Gets
    Description:
        Calls Gets to get strings from "String.txt" and check its correction.
    */
    int iFileHandle;
    int i, j, iRet, iFaile=0;
    long lLastTimeTicks, lTimeDiff;
    
    //Prepare standard data: 0,1,2,...,8,9,0,1,.... 
    for(i=0; i<TEST_LENGTH-1; i++)
        sStandardData[i]='0'+i%10;
    sStandardData[TEST_LENGTH-1]=0; //add a null character to the end of the string
       
    if((iFileHandle=mfs_OpenFile(DISKA, "String.txt", "r"))>0)
    {
        Print("OpenFile(): A:String.txt ok, File handle=%d.....\n", iFileHandle);
        
        lLastTimeTicks=GetTimeTicks();
        for(i=0; i<TEST_LOOP; i++)
        {
            _fmemset(sFileData, 0, TEST_LENGTH);
            
            iRet=mfs_Gets(iFileHandle, sFileData, TEST_LENGTH);
            if(iRet<=0)
            {
                Print("Loop%d Gets failed! Error code=%u\n", i, iRet);
                iFaile=1;
                break;
            }
            else if(_fmemcmp(sFileData, sStandardData, TEST_LENGTH-1)!=NoError)
            {
                iFaile=1;
                Print("Loop%d, Compare data failed! Error code=%u\n", i, iRet);
                Print("Standard: File\n");
                
                for(j=0; j<TEST_LENGTH-1; j++)
                {
                    if(sStandardData[j]!=sFileData[j])
                        Print("{Data[%d]%c:%c[%02d]:[%02d]} ", j, sStandardData[j], sFileData[j], sStandardData[j], sFileData[j]);
                    
                    if((j+1)%5==0) Putch('\n');
                    
                    Delay(3);
                }
                Putch('\n');
                
                break;
            }
            else Putch('.');
        }
        
        if(iFaile==0)
        {
            lTimeDiff=GetTimeTicks()-lLastTimeTicks;
            
            if((iRet=mfs_CloseFile(iFileHandle))==NoError)
            {
                Print("\rCloseFile(): A:String.txt ok.\n");
                Print("\nTest result=ok. Gets %ld Bytes takes %ldms(%5.1fKB/Sec)\n", (long) TEST_LOOP*(long)TEST_LENGTH,
                                                                                     lTimeDiff,
                                                                                     (float) TEST_LOOP*(float)TEST_LENGTH/lTimeDiff); 
            }
            else 
                Print("\rCloseFile(): A:String.txt failed! Error code=%d\n", iRet);
        }
    }
    else
    {
        Print("OpenFile(): A:String.txt failed! Error:%d\n", iFileHandle);
        iFaile=1;
    }
    
    if(iFaile || iRet)
        Print("\nTest result=failed!\n");
    else
        Print("\nTest result=ok!\r\n");
}

void Test_PutsGets(void)
{
    int iFileHandle;
    int i, j, iRet, iFaile=0;

    mfs_EnableWriteVerify();
    
    if((iFileHandle=mfs_OpenFile(DISKA, "PutsGets.txt", "w"))>0)
    {
        Print("OpenFile(): A:PutsGets.txt ok, File handle=%d.....\n", iFileHandle);
        for(i=0; i<100; i++)
        {
            iRet=mfs_Puts(iFileHandle, "0123456789abcdefghij"); //total 20 bytes
            if(iRet<=0)
            {
                Print("Loop%d Puts failed! Error code=%d\n", i, iRet);
                iFaile=1;
                break;
            }
            Putch('.');
        }
        
        if((iRet=mfs_CloseFile(iFileHandle))==NoError)
            Print("\rCloseFile(): A:PutsGets.txt ok.\n");
        else    
        {
            iFaile=1;
            Print("\rCloseFile(): A:PutsGets.txt failed! Error code=%d\n", iRet);
        }
    }
    else
    {
        iFaile=1;
        Print("OpenFile(): A:PutsGets.txt failed! Error:%d\n", iFileHandle);
    }
    
    //If Puts ok, then test Gets.
    if(iFaile==0)
    {
        if((iFileHandle=mfs_OpenFile(DISKA, "PutsGets.txt", "r"))>0)
        {
            Print("OpenFile(): A:PutsGets.txt ok, File handle=%d.....\n", iFileHandle);
            
            strcpy(sStandardData, "0123456789abcdefghij");
            for(i=0; i<100; i++)
            {
                _fmemset(sFileData, 0, 30); //String length = 20 bytes, prepare more space.
                
                iRet=mfs_Gets(iFileHandle, sFileData, 30);
                if(iRet<=0)
                {
                    iFaile=1;
                    Print("Loop%d Gets failed! Error code=%u\r\n", i, iRet);
                    
                    break;
                }
                else if(_fmemcmp(sFileData, sStandardData, 20)!=NoError)
                {
                    Print("Loop%d, Compare data failed! Error code=%u\n", i, iRet);
                    Print("Standard :File\r\n");
                    
                    for(j=0; j<20; j++)
                    {
                        if(sStandardData[j]!=sFileData[j])
                            Print("{Data[%d]%c:%c[%02d]:[%02d]} ", j, sStandardData[j], sFileData[j], sStandardData[j], sFileData[j]);
                        
                        if((j+1)%5==0) Putch('\n');
                        
                        Delay(3);
                    }
                    Putch('\n');
                    
                    iFaile=1;
                    break;
                }
                else Putch('.');
            }
            
            if(iFaile==0)
            {
                if((iRet=mfs_CloseFile(iFileHandle))==NoError) 
                    Print("\rCloseFile(): A:PutsGets.txt ok.\n");
                else
                {
                    iFaile=1;
                    Print("\rCloseFile(): A:PutsGets.txt failed! Error code=%d\n", iRet);
                }
            }
        }
        else
        {
            Print("OpenFile(): A:PutsGets.txt failed! Error:%d\n", iFileHandle);
            iFaile=1;
        }
    }
    
    if(iFaile || iRet)
        Print("\nTest result=failed!\n");
    else
        Print("\nTest result=ok!\n");
}

void Test_WritingSpeed(int EnableWriteVerify)
{
    /*
    Test item: Writing speed (calls WriteFile)
    Description:  
        Create a file "Speed.txt". 
    */
    
    int iFileHandle;
    int i, iRet, iFaile=0;
    long lLastTimeTicks, lTimeDiff;
    
    if(EnableWriteVerify) mfs_EnableWriteVerify();
    else mfs_DisableWriteVerify();
    
    if((iFileHandle=mfs_OpenFile(DISKA, "Speed.txt", "w"))>0)
    {
        Print("OpenFile(): A:Speed.txt ok, File handle=%d.....\n", iFileHandle);

        //Prepare data: 0,1,2,...,8,9,0,1,....
        for(i=0; i<TEST_LENGTH; i++)
            sFileData[i]='0'+i%10;
        
        lLastTimeTicks=GetTimeTicks();
        for(i=0; i<TEST_LOOP; i++)
        {
            iRet=mfs_WriteFile(iFileHandle, sFileData, TEST_LENGTH);
            if(iRet<=0)
            {
                Print("Loop%d Write data failed! Error code=%d\n", i, iRet);
                iFaile=1;
                break;
            }
            Putch('.');
        }
        lTimeDiff=GetTimeTicks()-lLastTimeTicks;
        
        if((iRet=mfs_CloseFile(iFileHandle))==NoError)
        {
            Print("\rCloseFile(): A:Speed.txt ok.\n");
            if(iFaile==0) 
                Print("\nTest result=ok. WriteFile %ld Bytes takes %ldms(%5.1fKB/Sec)\n", (long) TEST_LOOP*(long)TEST_LENGTH,
                                                                                          lTimeDiff,
                                                                                          (float) TEST_LOOP*(float)TEST_LENGTH/lTimeDiff); 
        }
        else
            Print("\rCloseFile(): A:Speed.txt failed! Error code=%d\n", iRet);
    }
    else
    {
        Print("OpenFile(): A:Speed.txt failed! Error:%d\n", iFileHandle);
        iFaile=1;
    }
    
    if(iFaile || iRet)
        Print("\nTest result=failed!\n");
    else
        Print("\nTest result=ok!\n");
}

void Test_ReadingSpeed(void)
{
    /*
    Test item: Reading speed (calls ReadFile)
    Description:  
        Read data from A:Speed.txt and check its correction.
        "Speed.txt" should be created advanced by TestWritingSpeed().
    */
    int iFileHandle;
    int i, j, iRet, iFaile=0;
    long lLastTimeTicks, lTimeDiff;

    //Prepare standard data: 0,1,2,...,8,9,0,1,.... 
    for(i=0; i<TEST_LENGTH; i++)
        sStandardData[i]='0'+i%10;
        
    if((iFileHandle=mfs_OpenFile(DISKA, "Speed.txt", "r"))>0)
    {
        Print("OpenFile(): A:Speed.txt ok, File handle=%d.....\n", iFileHandle);
        
        lLastTimeTicks=GetTimeTicks();
        for(i=0; i<TEST_LOOP; i++)
        {
            _fmemset(sFileData, 0, TEST_LENGTH);
            
            iRet=mfs_ReadFile(iFileHandle, sFileData, TEST_LENGTH);
            if(iRet<=0)
            {
                Print("Loop%d Read data failed! Error code=%u\n", i, iRet);
                iFaile=1;
                break;
            }
            else if(_fmemcmp(sFileData, sStandardData, TEST_LENGTH)!=NoError)
            {
                iFaile=1;
                Print("Loop%d, Compare data failed! Error code=%u\n", i, iRet);
                Print("Standard :File\n");
                
                for(j=0; j<TEST_LENGTH; j++)
                {
                    if(sStandardData[j]!=sFileData[j])
                    Print("{Data[%d] %c:%c [%02d]:[%02d]} ", j, sStandardData[j], sFileData[j], sStandardData[j], sFileData[j]);
                }
                Putch('\n');
                
                break;
            }
            else Putch('.');
        }
        
        if(i==TEST_LOOP)
        {
            lTimeDiff=GetTimeTicks()-lLastTimeTicks;
            
            if((iRet=mfs_CloseFile(iFileHandle))==NoError)
            {
                Print("\rCloseFile(): A:Speed.txt ok.\n");
                if(iFaile==0)
                    Print("\nTest result=ok. ReadFile %ld Bytes takes %ldms(%5.1fKB/Sec)\n", (long) TEST_LOOP*(long)TEST_LENGTH,
                                                                                             lTimeDiff,
                                                                                             (float) TEST_LOOP*(float)TEST_LENGTH/lTimeDiff); 
            }
            else
                Print("\rCloseFile(): A:Speed.txt failed! Error code=%d\n", iRet);
        }
        else
            Print("\nReadFile(): A:Speed.txt failed! Error code=%d\n", iRet);
    }
    else
    {
        Print("OpenFile(): A:Speed.txt failed! Error:%d\n", iFileHandle);
        iFaile=1;
    }
    
    if(iFaile || iRet)
        Print("\nTest result=failed!\n");
    else
        Print("\nTest result=ok!\n");
}

void Test_DumpFile(void)
{
    int iFileHandle;
    int i, iRet, iLength, Reading=1;
    char Key, sFileName[16];
    long lPosition=0;
       
    Print("File name: ");
    Scanf("%s", sFileName);
       
    if((iFileHandle=mfs_OpenFile(DISKA, sFileName, "r"))>0)
    {
        Print("OpenFile(): A:%s ok, File handle=%d.....\n", sFileName, iFileHandle);
        
        while(!mfs_EOF(iFileHandle) && Reading)
        {
            _fmemset(sFileData, 0, 512);
        
            iLength=mfs_ReadFile(iFileHandle, sFileData, 512);
            Print("iLength=%d\n", iLength);
            Print("mfs_Tell(iFileHandle)=%d\n", mfs_Tell(iFileHandle));
            
            if(iLength<=0)
            {
                Print("Reading data failed! Error code=%d\n", iLength);
                Reading=0;
            }
            else
            {
                Print("Position:%ld(%lX), Page%ld(%lX)\n", lPosition,lPosition,
                                                           (lPosition+32)/512,
                                                           (lPosition+32)/512);
                
                Print("Offset(%03ld)= ", mfs_Tell(iFileHandle)-iLength);
                lPosition+=iLength;
                for(i=0; i<iLength; i++)
                {
                    if(sFileData[i]==0 || sFileData[i]==0x0A || sFileData[i]==0x0D)
                        Print(" [%02X] ", sFileData[i]);
                    else
                        Print("%c[%02X] ", sFileData[i], sFileData[i]);
                    if((i+1)%16==0) Print("\nOffset(%03ld)= ", mfs_Tell(iFileHandle)-iLength+(i+1));
                }
                Print("\nPress 'q' to quit, other key to next page:");
                
                if((Key=Getch())=='q' || Key=='Q') Reading=0;
                Print("%c\r\n", Key);
            }
        }
        
        if((iRet=mfs_CloseFile(iFileHandle))==NoError)
            Print("\nCloseFile(): A:%s ok.\n", sFileName);
        else
            Print("\nCloseFile(): A:%s failed! Error code=%d\n", sFileName, iRet);
    }
    else
        Print("OpenFile(): A:%s failed! Error:%d\n", sFileName, iFileHandle);
}

void Test_Openfile(void)
{
    /*
    Test item:DiskA,DiskB can open 10 files at the same time.
    Description:  
        1. Create 25 files on DiskA and DiskB
           Steps:Create 1 file ==> give data ==> close it, repeat to create 25 files.
        2. Open and read the 25 files to check whether its data is correct.
           Steps:Open 25 files ==> read and compare data for opend files 
             ==> Close opend files.
             
            Each disk can open 10 files at the same time,
            On step2, only the first 10 files of each disk can be opend.
            The rest 15 files should return error code when opening.
    */
    int iFileHandle[2][25], disk, iLength;
    int i, iRet;
    char sFileName[16], sData[80], sStandardData[80];
    int iFaile=0;
    
    Print("============================================================\n");
    Print("Testing for opening 10 files in disk A and 10 files in disk B.\n"); 
    Print("============================================================\n");
    
    for(disk=DISKA; disk<=DISKB; disk++)
    {
        Print("Disk %c Creating files\n", disk==DISKA?'A':'B');
        
        for(i=0; i<25; i++)
        {
            sprintf(sFileName, "Test%02d.txt", i);
            
            iFileHandle[disk][i]=mfs_OpenFile(disk, sFileName, "w");
            if(iFileHandle[disk][i]>0)
            {
                sprintf(sData, "test data for disk%d: %s", disk, sFileName);
                
                iRet=mfs_Puts(iFileHandle[disk][i], sData);
                if(iRet<0) Print("Puts error\n");
                
                if((iRet=mfs_CloseFile(iFileHandle[disk][i]))!=NoError)
                    Print("CloseFile error\n");
                
                Print("%s ok. ", sFileName);
            }
            else
            {
                Print("OpenFile %s error\n", sFileName);
                iFaile=1;
            }
            
            if((i+1)%5==0) Putch('\n');
        }
        
        Putch('\n');
    }
    
    for(disk=DISKA; disk<=DISKB; disk++)
    {
        Print("Disk %c Reading files\n", disk==DISKA?'A':'B');
        
        for(i=0; i<25; i++)
        {
            sprintf(sFileName, "Test%02d.txt", i);
            
            iFileHandle[disk][i]=mfs_OpenFile(disk, sFileName, "r");
            if(iFileHandle[disk][i]>0)
            {
                sprintf(sStandardData, "test data for disk%d: %s", disk, sFileName);
                iLength=strlen(sStandardData);
                
                memset(sData, 0x0, sizeof(sData));
                
                iRet=mfs_Gets(iFileHandle[disk][i], sData, sizeof(sData));
                if(iRet==iLength)
                {
                    if(memcmp(sData, sStandardData, iLength)!=NoError)
                    {
                        Print("%s error!\n", sFileName);
                        iFaile=1;
                    }
                    else if(iFileHandle[disk][i]!=(disk*10+(i+1))) 
                    {
                        Print("File handle error!\n");
                        iFaile=1;
                    }
                    else Print("%s ok. ", sFileName);
                }
                else
                {
                    Print("%s Length error\n", sFileName);
                    iFaile=1;
                }
            }
            else
            {
                //The first 10 files should be opend ok.
                //The reset 15 files should not be opend.
                if(i<10)
                {
                    Print("Open %s error(%d)\n", sFileName, iFileHandle[disk][i]);
                    iFaile=1;
                }
                else Putch('.');
            }
            
            if((i+1)%5==0) Putch('\n');
        }
        
        Print("Disk %c Closing files\n", disk==DISKA?'A':'B');
        
        for(i=0; i<25; i++)
        {
            if(iFileHandle[disk][i]>0)
            {
                sprintf(sFileName, "Test%02d.txt", i);
                
                if((iRet=mfs_CloseFile(iFileHandle[disk][i]))==NoError)
                    Print("%s ok. ", sFileName);
                else
                {
                    Print("\nClose %s error(%d)\n", sFileName, iRet);
                    iFaile=1;
                }
            }
            else Putch('.');
            
            if((i+1)%5==0) Putch('\n');
        }
        
        Putch('\n');
    }
    
    Print("\nTest result=%s!\n", iFaile?"failed":"ok");
}

void Test_DumpNVRAM(void)
{
    struct nvram_fdata {
        unsigned mark;
        unsigned char fname[12];
        long F_Size;
        char far *F_StartAdd;
        char far *F_EndAdd;
    } *nvramFData;

    int i;
    unsigned char Data[31];
    char sFileName[16];
    long lValue;
    
    for(i=0; i<31; i++)
    {
        Data[i]=ReadNVRAM(i);
        if(Data[i]>=0x21 && Data[i]<=0x7E) //Print visiable characters.
            Print("%c[%02X] ", Data[i]&0xFF, Data[i]&0xFF);
        else
            Print(" [%02X] ", Data[i]&0xFF);
            
        if((i+1)%10==0) Putch('\n');
    }
    Putch('\n');
    
    Print("(0)  DiskA  FAT Current Index=%d\n", Data[0]);
    Print("(1)  DiskB  FAT Current Index=%d\n", Data[1]);
    Print("(2)  DiskA  FAT Previouse Index=%d\n", Data[2]);
    Print("(3)  DiskB  FAT Previouse Index=%d\n", Data[3]);
    
    nvramFData=(struct nvram_fdata *) &Data[4];
    
    Print("(4-5)Mark=%04X\n", nvramFData->mark);
    
    memset(sFileName, 0x0, sizeof(sFileName));
    memcpy(sFileName, nvramFData->fname, 12);
    Print("(6-17)File Name=%s\n", sFileName);
    
    Print("(18-21)[%02X] [%02X] [%02X] [%02X]File Size=%ld Bytes\n", Data[18]&0xFF, Data[19]&0xFF,
                                                                     Data[20]&0xFF, Data[21]&0xFF, nvramFData->F_Size);
    
    Print("(22-25)[%02X] [%02X] [%02X] [%02X]File Begin=%u:%u\n", Data[22]&0xFF, Data[23]&0xFF,
                                                                  Data[24]&0xFF, Data[25]&0xFF,
                                                                  FP_SEG(nvramFData->F_StartAdd), FP_OFF(nvramFData->F_StartAdd));
    
    Print("(26-29)[%02X] [%02X] [%02X] [%02X]File End=%u:%u\n", Data[26]&0xFF, Data[27]&0xFF,
                                                                Data[28]&0xFF, Data[29]&0xFF,
                                                                FP_SEG(nvramFData->F_EndAdd), FP_OFF(nvramFData->F_EndAdd));
    
    Print("File Operating Status:");
    switch(Data[30]&0xFF)
    {
        case 0: Print("0=_File_NonOpen\n"); break;
        case 1: Print("1=_File_Opening\n"); break;
        case 2: Print("2=_File_Writing\n"); break;
        case 3: Print("3=_File_WriteFinish\n"); break;
        case 30: Print("30=_FileStatusAddr\n"); break;
    }
}

void Test_Seek_Tell(void)
{
    /*
    Test item: Seek & Tell
    Description:  
        Create a file "SeekTell.txt". Call Tell, Seek, then read data 
        to check whether Tell and Seek work.
    */
    
    int iFileHandle, i, j, iMode;
    int iRet, iFaile=0;
    long lPosition, lOffset;

    if((iFileHandle=mfs_OpenFile(DISKA, "SeekTell.txt", "w"))>0)
    {
        Print("OpenFile(): A:SeekTell.txt ok, File handle=%d.....\n", iFileHandle);

        //Prepare data: 0,1,2,...,8,9,0,1,....
        for(i=0; i<TEST_LENGTH; i++)
            sFileData[i]='0'+i%10;
        
        iRet=mfs_WriteFile(iFileHandle, sFileData, TEST_LENGTH);
        if(iRet==TEST_LENGTH)
            Print("Write data ok.\n");
        else
        {
            Print("Write data failed! Error code=%d\n", iRet);
            iFaile=1;
        }
        
        if(iFaile==0)
        {
            iRet=mfs_CloseFile(iFileHandle);
            
            if(iRet==NoError) 
                Print("\nCloseFile(): A:SeekTell.txt ok.\n");
            else
            {
                Print("\nCloseFile(): A:SeekTell.txt failed! Error code=%d\n", iRet);
                iFaile=1;
            }
        } 
    }
    else
    {
        Print("OpenFile(): A:SeekTell.txt failed! Error:%d\n", iFileHandle);
        iFaile=1;
    }
    
    if(iFaile==0)
    {
        for(i=0; i<TEST_LENGTH; i++)
            sStandardData[i]='0'+i%10;
       
        if((iFileHandle=mfs_OpenFile(DISKA, "SeekTell.txt", "r"))>0)
        {
            Print("OpenFile(): A:SeekTell.txt ok, File handle=%d.....\n", iFileHandle);
            
            for(iMode=0; iMode<3; iMode++)   
            {
                if(iMode==_SEEK_SET) Print("\n_SEEK_SET mode:\n");
                if(iMode==_SEEK_CUR) Print("\n_SEEK_CUR mode:\n");
                if(iMode==_SEEK_END) Print("\n_SEEK_END mode:\n");
                
                for(i=0; i<10; i++)
                {    
                    _fmemset(sFileData, 0, 10);
                
                    lOffset=i*(TEST_LENGTH/10);
                    if(iMode==_SEEK_SET)
                        mfs_Seek(iFileHandle, lOffset, _SEEK_SET);
                    else if(iMode==_SEEK_CUR)
                    {
                        mfs_Seek(iFileHandle, lOffset, _SEEK_SET);
                        mfs_Seek(iFileHandle, 10, _SEEK_CUR);
                        lOffset+=10;
                    }
                    else if(iMode==_SEEK_END)
                    {
                        mfs_Seek(iFileHandle, 0L-lOffset-10L, _SEEK_END);
                        lOffset=TEST_LENGTH-lOffset-10L;
                    }
                    lPosition=mfs_Tell(iFileHandle);
                    
                    if(lPosition!=lOffset)
                    {
                        Print("Tell error, Tell=%ld, lOffset=%ld\n", lPosition, lOffset);
                        iFaile=1;
                        break;
                    }
                    
                    iRet=mfs_ReadFile(iFileHandle, sFileData, 10);
                    if(iRet<=0)
                    {
                        Print("Read data failed! Error code=%u\n", iRet);
                        iFaile=1;
                        break;
                    }
                    else if(_fmemcmp(sFileData, sStandardData+lOffset, 10)!=NoError)
                    {
                        Print("Compare data failed! Error code=%u\n", iRet);
                        Print("Standard :File\n");
                        for(j=0; j<10; j++)
                        {
                            if(sStandardData[lOffset+j]!=sFileData[j])
                            Print("{Data[%ld] %c:%c [%02d]:[%02d]} ", lOffset+j,
                                                                      sStandardData[lOffset+j],
                                                                      sFileData[j],
                                                                      sStandardData[lOffset+j],
                                                                      sFileData[j]);
                            Delay(5);
                        }
                        Putch('\n');
                        iFaile=1;
                        break;
                    }
                    else 
                    {
                        Print("%05ld ok. ", lOffset); //Compare data ok.
                        if((i+1)%5==0) Putch('\n');
                    }
                }
            }
        }
        
        if(iFaile==0)
        {
            if((iRet=mfs_CloseFile(iFileHandle))==NoError)
                Print("\nCloseFile(): A:SeekTell.txt ok.\n");
            else
                Print("\nCloseFile(): A:SeekTell.txt failed! Error code=%d\n", iRet);
        }
    }
    else
        Print("OpenFile(): A:SeekTell.txt failed! Error:%d\n", iFileHandle);
    
    if(iFaile || iRet)
        Print("\nTest result=failed!\n");
    else
        Print("\nTest result=ok!\n");
}

//=== Following are demo show how to use timer interrupt function
//=== to log data to the 64MB flash memory.
int iLoop=0, iLastReadLoop, iMaxLoop=400, iTimeInterval=25, iLoggingData=0, TimerFileHandle;
int iHour, iMinute, iSecond;

void TimerFun(void)
{
    char Data[80];
    
    iLoop++;
    GetTime(&iHour, &iMinute, &iSecond);
    
    sprintf(Data, "No%04d@%02d:%02d:%02d", iLoop, iHour, iMinute, iSecond);
    mfs_Puts(TimerFileHandle, Data);
    if(iLoop>=iMaxLoop)
    {
        iLoop=0;
        iLoggingData=0;
        StopUserTimerFun();
    }
}
    
void Test_TimeStamp(void)
{
    int iRet,iFaile=0;
    long curTimeTicks, lTimeTicks=0;
    
    mfs_EnableWriteVerify();
    
    if((TimerFileHandle=mfs_OpenFile(DISKA, "Timer.txt", "w"))>0)
    {
        Print("OpenFile(): A:Timer.txt ok, File handle=%d.....\n", TimerFileHandle);
        
        iLoggingData=1;
        GetTime(&iHour, &iMinute, &iSecond);
        Print("Begin to log data since %02d:%02d:%02d\n", iHour, iMinute, iSecond);
        InstallUserTimerFunction_ms(iTimeInterval, TimerFun);
    }
    else
    {
        iFaile=1;
        Print("OpenFile(): A:Timer.txt failed! Error:%d\n", TimerFileHandle);
        Print("\nTest result=failed!\n");
    }
    
    while(iLoggingData)
    {
        // Do other things, for example:
        //   1. COM port communication
        //   2. I/O control to Xboard
        
        //Print (.) every second as a progress bar.
        if((curTimeTicks=GetTimeTicks())-lTimeTicks>1000l)
        {
            lTimeTicks=curTimeTicks;
            
            Putch('.');
        }
    }
    
    GetTime(&iHour, &iMinute, &iSecond);
    Print("\rEnd logging data at %02d:%02d:%02d\n", iHour, iMinute, iSecond);
    
    if((iRet=mfs_CloseFile(TimerFileHandle))==NoError) 
        Print("\nCloseFile(): A:Timer.txt ok.\n");
    else
    {
        iFaile=1;
        Print("\nCloseFile(): A:Timer.txt failed! Error code=%d\n", iRet);
    }
    
    if(iFaile || iRet)
        Print("\nTest result=failed!\n");
    else
        Print("\nTest result=ok!\n");
}

void Test_TimeStamp_Read(void)
{
    int iReadFileHandle;
    int i, iRet, iFaile=0;
    long curTimeTicks, lTimeTicks=0;
    
    char Data[80];
    
    mfs_EnableWriteVerify();
    
    if((TimerFileHandle=mfs_OpenFile(DISKA, "Timer.txt", "w"))>0)
    {
        Print("OpenFile(): A:Timer.txt ok, File handle=%d.....\n", TimerFileHandle);
        if((iReadFileHandle=mfs_OpenFile(DISKA, "Timer.txt", "r"))>0)
        {
            iLastReadLoop=0;
            Print("OpenFile(): A:Timer.txt for reading ok. File handle=%d....\n", iReadFileHandle);
        }
        
        iLoggingData=1;
        
        GetTime(&iHour, &iMinute, &iSecond);
        Print("Begin to log data since %02d:%02d:%02d\n", iHour, iMinute, iSecond);
        
        InstallUserTimerFunction_ms(iTimeInterval, TimerFun);
    }
    else
    {
        iFaile=1;
        Print("OpenFile(): A:Timer.txt failed! Error:%d\n", TimerFileHandle);
        Print("\nTest result=failed!\n");
    }
    
    while(iLoggingData)
    {
        // Do other things, for example:
        //   1. COM port communication
        //   2. I/O control to Xboard
        
        //Print (.) every second as a progress bar.
        if((curTimeTicks=GetTimeTicks())-lTimeTicks>1000)
        {
            lTimeTicks=curTimeTicks;
            
            Print(".");
        }
        
        if(iLoop>10)
        {
            if((iLoop+1)%10==0 && iLastReadLoop!=iLoop) 
            {
                mfs_Seek(iReadFileHandle, (iLoop-9)*16, _SEEK_SET);
                Print("Loop (%04d ==> %04d)\n", iLoop-9, iLoop);
                
                for(i=0; i<10; i++)
                {
                    mfs_Gets(iReadFileHandle, Data, 80);
                    Print("%s\n", Data);
                }
                iLastReadLoop=iLoop;
            }
        }
    }
    
    GetTime(&iHour, &iMinute, &iSecond);
    Print("End logging data at %02d:%02d:%02d\n", iHour, iMinute, iSecond);
    
    if((iRet=mfs_CloseFile(TimerFileHandle))==NoError)
    {
        Print("\nCloseFile(): A:Timer.txt ok.\n");
        if(!iFaile&&(iReadFileHandle>0))  iRet=mfs_CloseFile(iReadFileHandle); 
    }
    else
    {
        iFaile=1;
        Print("\nCloseFile(): A:Timer.txt failed! Error code=%d\r\n", iRet);
    }
    
    if(iFaile || iRet)
        Print("\nTest result=failed!\n");
    else
        Print("\nTest result=ok!\n");
}

void Test_PutsGets_SameTime(void)
{
    int iFileHandle, iReadFileHandle;
    int i, iRet, iFaile=0;
    long lPosition;
    char Data[80];

    mfs_EnableWriteVerify();
    
    if((iFileHandle=mfs_OpenFile(DISKA, "PutsGets.txt", "a"))>0)
    {
        Print("OpenFile(): A:PutsGets.txt ok, File handle=%d.....\n", iFileHandle);
        iReadFileHandle=mfs_OpenFile(DISKA, "PutsGets.txt", "r");
        Print("OpenFile(): A:PutsGets.txt ok, File handle=%d.....\n", iReadFileHandle);

        for(i=0; i<10; i++)
        {
            iRet=mfs_Puts(iFileHandle, "0123456789abcdefghij"); //total 20 bytes
            if(iRet<=0)
            {
                iFaile=1;
                Print("Loop%d Puts failed! Error code=%d\r\n", i, iRet);
                
                break;
            }
            
            lPosition=mfs_Tell(iReadFileHandle);
            iRet=mfs_Gets(iReadFileHandle, Data, 80);
            if(iRet>0)
                Print("Loop%2d(%4ld)=%s\n", i, lPosition, Data);
            else
                Print("Loop%2d(%4ld)=non\n", i, lPosition);
        }
        
        iRet=mfs_CloseFile(iFileHandle);
        while(!mfs_EOF(iReadFileHandle))
        {
            lPosition=mfs_Tell(iReadFileHandle);
            iRet=mfs_Gets(iReadFileHandle, Data, 80);
            if(iRet>0)
                Print("Loop%2d(%4ld)=%s\n", i, lPosition, Data);
            else
                Print("Loop%2d(%4ld)=non\n", i, lPosition);
        }
        
        if((iRet=mfs_CloseFile(iReadFileHandle))==NoError)
            Print("\nCloseFile(): A:PutsGets.txt ok.\n");
        else
        {
            iFaile=1;
            Print("\nCloseFile(): A:PutsGets.txt failed! Error code=%d\n", iRet);
        }
    }
    else
    {
        iFaile=1;
        Print("OpenFile(): A:PutsGets.txt failed! Error:%d\n", iFileHandle);
    }
    
    if(iFaile || iRet)
        Print("\nTest result=failed!\n");
    else
        Print("\r\nTest result=ok!\n");
}

void Test_PutsGets_EOF(void)
{
    int iFileHandle;
    int i, iRet, iFaile=0;

    mfs_EnableWriteVerify();
    
    if((iFileHandle=mfs_OpenFile(DISKA, "PutsGets.txt", "w"))>0)
    {
        Print("OpenFile(): A:PutsGets.txt ok, File handle=%d\n", iFileHandle);
        for(i=0; i<100; i++)
        {
            iRet=mfs_Puts(iFileHandle, "0123456789abcdefghij"); //total 20 bytes
            if(iRet<=0)
            {
                iFaile=1;
                Print("Loop%3d Puts failed! Error code=%d\n", i, iRet);
                
                break;
            }
        }
        
        if((iRet=mfs_CloseFile(iFileHandle))==NoError)
            Print("CloseFile(): A:PutsGets.txt ok.\n\n");
        else
        {
            iFaile=1;
            Print("CloseFile(): A:PutsGets.txt failed! Error code=%d\n", iRet);
        }
    }
    else
    {
        iFaile=1;
        Print("OpenFile(): A:PutsGets.txt failed! Error:%d\n\n", iFileHandle);
    }
    
    //If Puts ok, then test Gets.
    if(iFaile==0)
    {
        if((iFileHandle=mfs_OpenFile(DISKA, "PutsGets.txt", "r"))>0)
        {
            Print("OpenFile(): A:PutsGets.txt ok, File handle=%d\n", iFileHandle);
            
            strcpy(sStandardData, "0123456789abcdefghij");

            while(!mfs_EOF(iFileHandle))
            {
                _fmemset(sFileData, 0, 30); //String length = 30 bytes, prepare more space
                
                iRet=mfs_Gets(iFileHandle, sFileData, 30);
                if(iRet<=0)
                {
                    iFaile=1;
                    Print("Gets() failed! Error code=%u\n", iRet);
                    
                    break;
                }
                else if(_fmemcmp(sFileData,sStandardData, 20)!=NoError)
                {
                    iFaile=1;
                    Print("Compare data failed! Error code=%u\n", iRet);
                    Print("Standard :File\n");
                    
                    for(i=0; i<20; i++)
                    {
                        if(sStandardData[i]!=sFileData[i])
                            Print("{Data[%d]%c:%c[%02d]:[%02d]} ", i, sStandardData[i], sFileData[i], sStandardData[i], sFileData[i]);
                        
                        if((i+1)%5==0) Putch('\n');
                        Delay(2);
                    }
                    
                    Putch('\n');
                    
                    break;
                }
                else Putch('.');
            }
            
            if(iFaile==0)
            {
                if((iRet=mfs_CloseFile(iFileHandle))==NoError)
                    Print("\rCloseFile(): A:PutsGets.txt ok.\n");
                else
                {
                    iFaile=1;
                    Print("\rCloseFile(): A:PutsGets.txt failed! Error code=%d\n", iRet);
                }
            }
        }
        else
        {
            Print("OpenFile(): A:PutsGets.txt failed! Error:%d\r\n", iFileHandle);
        }
    }
    
    if(iFaile || iRet)
        Print("\nTest result=failed!\n");
    else
        Print("\nTest result=ok!\n");
}