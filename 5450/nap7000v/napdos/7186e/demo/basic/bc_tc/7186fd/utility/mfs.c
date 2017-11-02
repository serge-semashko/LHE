/*
MFS.c: Demo program shows how to use the uPAC-7186EX-FD to implement a file system.

Compiler: BC++ 3.1, 
          Turbo C++ 1.01 (3.01)
      
Compile mode: large

Project: MFS.c 
         ..\lib\7186el.lib
         ..\lib\mfs_vnnn.lib
         
Hardware: uPAC-7186EX-FD
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>
#include "..\..\lib\7186E.h"
#include "..\..\lib\MFS.h"

char* DiskName(int DiskNumber);

void ReadFile(int Disk);
void ReadFile_Getc(int Disk);
void WriteFile(int Disk);
void AppendFile(int Disk);
void DeleteFile(int Disk);
void TestWritingSpeed(int Disk);
void TestReadingSpeed(int Disk);

char far sFileData[32767], sStandardData[32767];

void main(void)
{
    FILE_DATA fdata;
    int i, iAction, iRet, iFileHandle, iFileno, iFileSize, iVer;
    char cData;
    char FileName[20], LibDate[16];
    int CurrentDiskNumber=DISKA;  // which disk we are using

    InitLib();
    iRet=mfs_Init();
    if(iRet!=64)
    {
        Print("The version of FAT is not suitable for the current version\r\n");
        Print("of function library.\r\n");
        Print("Please reset the Flash (all files will not be retained)\r\n");
        Print("Do you want to reset the Flash (Y/N)?");
        cData=Getch();
        if(cData=='y' || cData=='Y')
        {
            iRet=mfs_ResetFlash();
            if(iRet==NoError)
                Print("ResetFlash() success.\r\n");
            else
                Print("ResetFlash() failed! Error code=%d\r\n", iRet);
        }
    }
    
    iVer=mfs_GetLibVersion();
    mfs_GetLibDate((unsigned char *) &LibDate);
    Print("\r\nFunction library ver %X.%02X build %s\r\n", (iVer>>12)&0xff, (iVer>>4)&0xff, LibDate);
    
    for(;;)
    {
        Print("\r\n");
        Print("0)Show File information\r\n");
        Print("1)Read File with mfs_ReadFile()\r\n");
        Print("2)Read File with mfs_Getc()\r\n");
        Print("3)Write File\r\n");
        Print("4)Append File\r\n");
        Print("5)Delete File\r\n");
        Print("6)Delete All Files\r\n");
        Print("7)Reset Flash\r\n");
        Print("8)Set Current Disk\r\n");
        Print("9)Quit\r\n");
        Print("10)Test Writing Speed\n\r");
        Print("11)Test Reading Speed\n\r");
        Print("Please Select(0~7): ");
        Scanf("%d", &iAction);
        
        if(iAction==9)
        {
            mfs_Stop();
            break;
        }
            
        Print("\r\n");
        if(iAction==0)  //show fileinformation
        {
            if((iFileno=mfs_GetFileNo(CurrentDiskNumber))>=0)
            {
                for(i=0; i<iFileno; i++)
                {
                    iRet=mfs_GetFileInfoByNo(CurrentDiskNumber, i, (FILE_DATA *) &fdata);
                    if(!iRet)
                    {
                        memset(FileName, 0x0, 14);
                        memcpy(FileName, fdata.fname, 12);
                        Print("%s%12s    %4ld bytes\r\n", DiskName(CurrentDiskNumber), FileName, fdata.size);
                    }
                }  
                Print("\r\nThere are %d File(s) on %s\r\n", iFileno, DiskName(CurrentDiskNumber));
                
                Print("%16lu bytes used\r\n", mfs_GetUsedSize(CurrentDiskNumber));
                Print("%16lu bytes free\r\n", mfs_GetFreeSize(CurrentDiskNumber));
                Print("%16lu bytes bad\r\n", mfs_GetBadSzie(CurrentDiskNumber));
            }
            else // if the parameter disk is not either 0 or 1, returns a negative value 
            {
                Print("GetFileNo() failed! Error code= %d\r\n", iFileno);
            }
        }
        
        if(iAction==1)  // Read file
        {
            ReadFile(CurrentDiskNumber);
        }
        
        if(iAction==2)  // shows how to use the mfs_Seek(), mfs_Tell() and mfs_Getc() functions
        {
            ReadFile_Getc(CurrentDiskNumber);
        }
        
        if(iAction==3)  // write file
        {
            WriteFile(CurrentDiskNumber);
        }
        
        if(iAction==4)  // Append file
        {
            AppendFile(CurrentDiskNumber);
        }
        
        if(iAction==5)  // delete file
        {
            DeleteFile(CurrentDiskNumber);
        }
        
        if(iAction==6)  // delete all files
        {
            iRet=mfs_DeleteAllFiles(CurrentDiskNumber);
            Print("DeleteAllFiles on %s iRet=%d\r\n", DiskName(CurrentDiskNumber), iRet);
        }
        
        if(iAction==7)
        {
            iRet=mfs_ResetFlash();
            if(iRet==NoError)
                Print("mfs_ResetFlash() ok.\r\n");
            else
                Print("mfs_ResetFlash() error. iRet=%d\r\n", iRet);
        }

        if(iAction==8)  // set current disk
        {
            char disk_letter;

            Print("Enter disk letter a or b : ");
            disk_letter=Getch();    // wait for one character from user
            switch(disk_letter)
            {
                case 'A' :
                case 'a' :
                    CurrentDiskNumber=DISKA;
                    break;
                case 'B' :
                case 'b' :
                    CurrentDiskNumber=DISKB;
                    break;
                default:
                    Print("%c is an invalid disk letter\r\n", disk_letter);
            }
            
            Print("Current disk is %s\r\n", DiskName(CurrentDiskNumber));
        }
         
        if(iAction==10)
        {
            TestWritingSpeed(CurrentDiskNumber);
        }
        
        if(iAction==11)
        {
            TestReadingSpeed(CurrentDiskNumber);
        }
         
        Print("\r\nPress any key to continue...\r\n");
        Getch();
    }
}

// generate string showing disk name and separator
char* DiskName(int DiskNumber)
{
    char* retval;

    switch(DiskNumber)
    {
        case DISKA :
            retval="A:";
            break;
        case DISKB :
            retval="B:";
            break;
        default:
            retval="UnknownDisk:";
    }
    
    return retval;
}

void ReadFile(int Disk)
{
    int iFileHandle,iRet;
    char sFileName[16];
    char cData;
    
    Print("Please input file name:");
    LineInput(sFileName, 12);
    Print("Read file %s%s\n\r", DiskName(Disk), sFileName);

    if((iFileHandle=mfs_OpenFile(Disk, sFileName, "r"))>0)
    {
        Print("OpenFile() :%s%s success, File handle=%d.....\r\n", DiskName(Disk), sFileName, iFileHandle);
        
        while(!mfs_EOF(iFileHandle))
        {
            iRet=mfs_ReadFile(iFileHandle, &cData, 1);
            if(iRet>0)
            {
                Print("%c[%02X] ", cData, cData);
            }    
            else
                Print("\r\nData=Null\r\n");
        }
        
        Print("\r\n");

        iRet=mfs_CloseFile(iFileHandle);
        if(iRet!=NoError)
            Print("CloseFile() failed! Error code=%d\r\n", iRet);
    }
    else
        Print("OpenFile(): %s%s failed! Error code=%d\r\n", DiskName(Disk), sFileName, iFileHandle);
}

// shows how to use the mfs_Seek(), mfs_Tell() and mfs_Getc() functions
void ReadFile_Getc(int Disk)
{
    int iFileHandle, iFileSize, iRet;
    char sFileName[16];
    char cData;
    
    Print("Please input file name:");
    LineInput(sFileName, 12);
    Print("Read file %s%s\r\n", DiskName(Disk), sFileName);

    if((iFileHandle=mfs_OpenFile(Disk, sFileName, "r"))>0)
    {
        Print("OpenFile() :%s%s success, File handle=%d.....\r\n",DiskName(Disk), sFileName, iFileHandle);
        
        mfs_Seek(iFileHandle, 0, SEEK_END);
        iFileSize=mfs_Tell(iFileHandle);
        Print("%s%s     %d bytes\r\n", DiskName(Disk), sFileName,iFileSize);

        mfs_Seek(iFileHandle, 0, SEEK_SET);
        while(iFileSize>0)
        {
            cData=mfs_Getc(iFileHandle);
            Print("%c[%02X] ", cData, cData);
            
            iFileSize--;
        }
        Print("\r\n");

        iRet=mfs_CloseFile(iFileHandle);
        if(iRet!=NoError)
            Print("CloseFile() failed! Error code=%d\r\n", iRet);
    }
    else
        Print("OpenFile() %s%s failed! Error code=%d\r\n", DiskName(Disk), sFileName, iFileHandle);
}

void WriteFile(int Disk)
{
    int iFileHandle;
    unsigned uRet;
    char sFileName[16], sFileData[80];

    Print("Please input file name:");
    LineInput(sFileName, 12);
    Print("Write file %s%s\r\n", DiskName(Disk), sFileName);

    if((iFileHandle=mfs_OpenFile(Disk, sFileName, "w"))>0)
    {
        Print("OpenFile() :%s%s success, File handle=%d.....\r\n", DiskName(Disk), sFileName, iFileHandle);

        Print("Input string (< 80 bytes):");
        LineInput(sFileData, 80);
        
        uRet=mfs_WriteFile(iFileHandle, sFileData, strlen(sFileData));
        if(uRet>0)
            Print("Write data success.\r\n");
        else
            Print("Write data failed! Error code=%u\r\n", uRet);

        uRet=mfs_CloseFile(iFileHandle);
        if(uRet==NoError)
            Print("Close file(%d): %s success\r\n", iFileHandle, sFileName);
        else
            Print("Close file(%d): %s %s failed, error code=%d\r\n", iFileHandle, DiskName(Disk), sFileName, uRet);
    }
    else
        Print("OpenFile(): %s%s failed! Error:%d\r\n", DiskName(Disk), sFileName, iFileHandle);
}

void AppendFile(int Disk)
{
    int iFileHandle, iRet;
    char sFileName[16], sFileData[80];
    
    Print("Please input file name:");
    LineInput(sFileName, 12);
    Print("Append file (%s)\r\n", sFileName);

    iFileHandle=mfs_OpenFile(Disk, sFileName, "a");
    if(iFileHandle>0)
    {
        Print("Open file :%s success, File handle=%d.....\r\n", sFileName, iFileHandle);

        Print("Input string (< 80 bytes):");
        LineInput(sFileData, 80);
        iRet=mfs_WriteFile(iFileHandle, sFileData, strlen(sFileData));
        if(iRet>0)
            Print("Append data success\r\n");
        else
            Print("Append data failed, error code=%d\r\n", iRet);
            
        iRet=mfs_CloseFile(iFileHandle);
        if(iRet==NoError)
            Print("Close file(%d):%s success\r\n", iFileHandle, sFileName);
        else
            Print("Close file(%d):%s failed, error code=%d\r\n", iFileHandle, sFileName, iRet);
    }
    else
        Print("Open file: %s failed, error code=%d\r\n", sFileName, iFileHandle);
}

void DeleteFile(int Disk)
{
    int iRet;
    char sFileName[16];
    
    Print("Please input file name:");
    LineInput(sFileName, 12);
    Print("Delete file %s%s\r\n", DiskName(Disk), sFileName);
    
    iRet=mfs_DeleteFile(Disk, sFileName);
    if(iRet==NoError)
        Print("DeleteFile() success.\r\n");
    else
        Print("DeleteFile() failed! Error code=%d\r\n", iRet);
}

void TestWritingSpeed(int Disk)
{
    int i, iFileHandle;
    int uRet;

    if((iFileHandle=mfs_OpenFile(Disk, "Speed.txt", "a"))>0)
    {
        Print("OpenFile(): %s:Speed.txt success, File handle=%d.....\r\n", DiskName(Disk), iFileHandle);

        //Prepare data: 0,1,2,...,8,9,0,1,....
        for(i=0; i<32766; i++)
            sFileData[i]='0'+i%10;
        sFileData[32766]=0;

        for(i=0; i<10; i++)
        {
            uRet=mfs_Puts(iFileHandle, sFileData);
            if(uRet<=0)
            {
                Print("Loop%d Write data failed! Error code=%d\r\n",i, uRet);
				break;
            }
            else
                Print(".");
            Delay_1(1);
        }
        
        uRet=mfs_CloseFile(iFileHandle);
        if(uRet==NoError)
            Print("CloseFile(): %s:Speed.txt successed.\r\n", DiskName(Disk));
        else
            Print("CloseFile(): %s:Speed.txt failed! Error code=%d\r\n", DiskName(Disk), uRet);
    }
    else
        Print("OpenFile(): %s:Speed.txt failed! Error:%d\r\n", DiskName(Disk), iFileHandle);
}

void TestReadingSpeed(int Disk)
{
    int i, iFileHandle;
    int uRet;

    //Prepare standard data: 0,1,2,...,8,9,0,1,.... 
    for(i=0; i<32766; i++)
        sStandardData[i]='0'+i%10;
    sStandardData[32766]=0;
       
    if((iFileHandle=mfs_OpenFile(Disk, "Speed.txt", "r"))>0)
    {
        Print("OpenFile(): %s:Speed.txt success, File handle=%d.....\r\n", DiskName(Disk), iFileHandle);
        for(i=0; i<10; i++)
        {
            //memset(sFileData,0,11);
            uRet=mfs_Gets(iFileHandle, sFileData, 32767);
            if(uRet<=0)
            {
                Print("Loop%d Read data failed! Error code=%u\r\n", i, uRet);
                break;
            }
            Print("Length=%d\n\r", uRet);
            
            if(memcmp(sFileData, sStandardData, 32766))
            {
                Print("Loop%d Check data failed! Error code=%u\r\n", i, uRet);
                Print("File,Standard\n\r");
                for(i=0; i<1024; i++)
                {
                    if(sStandardData[i]!=sFileData[i])
                    Print("{Data[%d] %c:%c [%02d]:[%02d]} ", i, sStandardData[i], sFileData[i], sStandardData[i],sFileData[i]);
                }
                Print("\n\r");
                break;
            }
            else Print(".");
        }
        if(i==10) Print("Reading ok\n\r");
        
        
        uRet=mfs_CloseFile(iFileHandle);
        if(uRet!=NoError)
            Print("CloseFile(): %s:Speed.txt failed! Error code=%d\r\n", DiskName(Disk), uRet);
    }
    else
        Print("OpenFile(): %s:Speed.txt failed! Error:%d\r\n", DiskName(Disk), iFileHandle);
}