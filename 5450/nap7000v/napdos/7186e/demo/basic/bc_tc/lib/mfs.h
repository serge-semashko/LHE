/******************************************************************
    MinOS7 File System head file
    For uPAC-7186EX-FD, uPAC-5000-FD, iPAC-8441-FD and iPAC-8841-FD
    
    Note: head file version is different to lib file version
*******************************************************************/
/*
Version 2.2.1 [Jul 31, 2013]
Modify:
    1. Improve the speed/performance of writing data to the file
    
Fixed Bug:
    1. MFS does not free up the space that the deleted files occupied, 
       so that the file-system would stop functioning when the disk space is full 
    2. Fixed the bug where the mfs_Init() returns incorrect result when all disk space is occupied
    3. mfs_WriteFile() may fail with an append operation on an existing file

=============================================================================
Version 1.1.0 [Jan 07, 2009]
    1. Re-defines function name from X600Fs_xxx to mfs_xxxx
       (The library must be mfs_v210.lib or later.)
    2. Removes internal-use functions and variables.
       Users don'e need them.
    3. Lists software specifications of the MiniOS7 File System.
    4. Lists read/write performance.
    
=============================================================================
Version 1.0.5 [Mar 07, 2008]
    First release version.
*/

/* Software Specification

Disk number: 2 (0 for diskA, 1 for DiskB)
Disk size  : 1/2 size of the flash memory size
File number: 
     64MB Flash: 456 files max. for each disk
    256MB Flash: 1023 files max. for each disk

File size  : 
     64MB Flash: 31MB max. for each file
    256MB Flash: 126MB max. for each file

File name  : A total of 12 characters including the dot separator

File operation modes:
            1. Read: Open a file for reading
            2. Write: Truncate an existing file to zero length or 
                      create a new file for writing
            3. Append: Open for appending (writing at end of file)
                       The file is created if it does not exist

File handle: 10 max. for each disk.
    For reading operation mode: 
        the 10 file handles can all be used for reading operation 
        on each disk. Total 20 files can be opened for reading mode.
    For writing and appending operation modes: 
        only 1 file handle can be used for writing operation on all disks.
            
Writing verification: Yes. Default is enabled.
                   Calling EnableWriteVerification and DisableWriteVerification
                   can change the setting.
                   
Error check: If the controller is shutdown during writing data to 
             the Flash memory, the file system can automatically close the file.
             The last writing data will be lost but others were safely stored.
             
Writing speed:
    mfs_WriteFile:
         64MB Flash | 139.7 KB/Sec (verification enabled) (default)
                    | 166.8 KB/Sec (verification disabled)
        ------------+----------------------------------------------
        256MB Flash | 166.5 KB/Sec (verification enabled) (default)
                    | 215.4 KB/Sec (verification disabled)
                    
    mfs_Puts:
         64MB Flash | 135.0 KB/Sec (verification enabled) (default)
                    | 160.2 KB/Sec (verification disabled)
        ------------+----------------------------------------------
        256MB Flash | 161.0 KB/Sec (verification enabled) (default)
                    | 206.7 KB/Sec (verification disabled)
        
Reading speed: 
                      mfs_ReadFile      mfs_Gets
        ------------+----------------------------------------------
         64MB Flash | 776.7 KB/Sec      431.4 KB/Sec
        ------------+----------------------------------------------
        256MB Flash | 732.6 KB/Sec      417.9 KB/Sec
    
Max. length of writing data: 32767 bytes.
Max. length of reading data: 32767 bytes.

Hardware resource occupied:
    NVRAM:all of the 31 bytes.
    EEPROM: Not used.
*/

#ifdef __cplusplus
extern "C" {
#endif
//=============================================================================
// Following defines are for function names update 
// from X600_202.Lib to MFS_V210.Lib
// User's code developed on X600_202.Lib can be compiled with MFS_V210.Lib 
// without any modification.
#define X600Fs_GetLength            X600Fs_GetFileSize
#define X600Fs_GetFileInfo          X600Fs_GetFileInfoByNo 

#define X600Fs_Init                 mfs_Init
#define X600Fs_Stop                 mfs_Stop
#define X600Fs_ResetFlash           mfs_ResetFlash
#define X600Fs_GetLibVersion        mfs_GetLibVersion
#define X600Fs_GetLibDate           mfs_GetLibDate
#define X600Fs_GetFileNo            mfs_GetFileNo
#define X600Fs_GetFreeSize          mfs_GetFreeSize
#define X600Fs_GetBadSzie           mfs_GetBadSzie
#define X600Fs_GetUsedSize          mfs_GetUsedSize
#define X600Fs_GetFileSize          mfs_GetFileSize 
#define X600Fs_GetFileInfoByName    mfs_GetFileInfoByName
#define X600Fs_GetFileInfoByNo      mfs_GetFileInfoByNo
#define X600Fs_DeleteAllFile        mfs_DeleteAllFiles
#define X600Fs_DeleteFile           mfs_DeleteFile

#define X600Fs_OpenFile             mfs_OpenFile
#define X600Fs_CloseFile            mfs_CloseFile
#define X600Fs_ReadFile             mfs_ReadFile
#define X600Fs_WriteFile            mfs_WriteFile
#define X600Fs_Getc                 mfs_Getc
#define X600Fs_Putc                 mfs_Putc
#define X600Fs_Gets                 mfs_Gets
#define X600Fs_Puts                 mfs_Puts

#define X600Fs_EOF                  mfs_EOF
#define X600Fs_Seek                 mfs_Seek
#define X600Fs_Tell                 mfs_Tell

#define X600Fs_EnableWriteVerify    mfs_EnableWriteVerify
#define X600Fs_DisableWriteVerify   mfs_DisableWriteVerify

//=============================================================================
#define NoError         0
#define TimeOut        -5
#define ValError        2
#define DeviceError     3
#define CheckError      4
#define NoInit	        5
#define BadPage         6
#define UsedPage        7
#define CrcError        8



//======================================================

#define _MAX_FILE_HANDLE		20

//Operation mode for mfs_Seek
#define _SEEK_SET 0
#define _SEEK_CUR 1
#define _SEEK_END 2


int mfs_Init(void);
/*
Initialize the file system. 
It must be called before any other functions are called.

Return:
    4: found  4MB flash ok
    8: found  8MB flash ok
   16: found 16MB flash ok
   32: found 32MB flash ok
   64: found 64MB flash ok
    
    0: No Flash found
   -1: The version of FAT (File Allocation Table) is not suitable for 
       the current version of file system.
       Please call mfs_ResetFlash to reset the Flash (all files will be lost).
*/


int mfs_Stop(void);
/*
Allocated buffers are freed upon closeing

Return:
    Always returns 0
*/

int mfs_ResetFlash(void);
/*
Initialize the file system. All files will lost.

Return:
    Always returns 0
*/


unsigned mfs_GetLibVersion(void);
/*
Gets the version number of function library

Return:
    Returns the library version number.
    0x2211 means version 2.2.1 build 01
*/ 


void mfs_GetLibDate(unsigned char *LibDate);
/*
Gets the create date of function library

Retun:
    Non return value
*/


int mfs_GetFileNo(int disk);
/*
Gets the total number of files stored in the NAND Flash

Parameters:
    disk: 0=DiskA, 1=DiskB

Return:
    0: No file in the NAND Flash
   >0: The actual total number of files stored in the NAND Flash
*/


long mfs_GetFreeSize(int disk);
/*
Gets the size of available space that can be used to append file

Parameters:
    disk: 0=DiskA, 1=DiskB
    
Return:
  >=0: On success, returns the size of available space
   -1: On failure or error
*/


long mfs_GetBadSzie(int disk);
/*
Gets the size of non-available space

Return:
  >=0: On success, returns the size of non-available space
   -1: On failure or error
*/


long mfs_GetUsedSize(int disk);
/*
Gets the size of used space

Parameters:
    disk: 0=DiskA, 1=DiskB
    
Return:
  >=0: The actual free size which can be written data
   <0: On failure or error
*/


long mfs_GetFileSize(int disk, char *file_name);
/*
Gets the size of file stored in the NAND Flash

Parameters:
    disk: 0=DiskA, 1=DiskB
    file_name: File that funciton read
    
Return:
    -1: no file found
    >0: bytes of the file
*/


int mfs_GetFileInfoByName(int disk, char *file_name, FILE_DATA *data);
/*
Uses the specified filename to retrieve file information

Parameters:
    disk: 0=DiskA, 1=DiskB
    fname: File that funciton read
    data: A FILE_DATA type
    
Return:
    0: On success
   <0: On failure or error, returns a negative value
*/


int mfs_GetFileInfoByNo(int disk, int filenumber, FILE_DATA* data);
/*
Uses the file number index to retrieve file information.

Parameters:
    disk: 0=DiskA, 1=DiskB
    filenumber: 0 ~ X600Fs_GetFileNo - 1
    data: A FILE_DATA type

Return:
    0: On success
   <0: On failure or error, returns a negative value
*/


int mfs_DeleteAllFiles(int disk);
/*
Delete all files stored in the NAND Flash

Parameters:
    disk: 0=DiskA, 1=DiskB

Return:
    0: All files are successfully deleted
   <0: On failure, returns a negative value
*/


int mfs_DeleteFile(int disk, char *file_name);
/*
Delete one selected file that has been written to the NAND Flash

Parameters:
    disk: 0=DiskA, 1=DiskB
    file_name: File that funcitons delete
    
Return:
    0: The selected file is successfully deleted
   <0: On failure, returns a negative value
*/


int mfs_OpenFile(int disk, char *file_name, char *mode);
/*
1. Opens a file with a file name
2. Creates a new file

Parameters:
    disk: 0=DiskA, 1=DiskB
    file_name: File that the functions open
    mode: defines the mode of the opened file (r, w, a)

Return:
   >0: On success, returns the file handle
    0: On failure or error
   -4: No free space to append a new file
*/


int mfs_CloseFile(int file_handle);
/*
Closes a file with a file handle
All buffers associated with the stream are flushed before closing

Parameter:
    file_handle: A file handle created by the mfs_OpenFile

Return:
    0: On success
   >0: On failure or error
*/


int mfs_ReadFile(int file_handle, char *data, int size);
/*
Reads a specified bytes of data from a file

Parameters:
    file_handle: A file handle created by the X600Fs_OpenFile
    data: Pointer assigned to a buffer which used to store read data.
    size: Bytes want to read. 32767 bytes Max.
    
Return:
   >0: Returns the bytes (1~32767) of data actually read
    0: On end-of-file or error
*/


int mfs_WriteFile(int file_handle, void *data, int size);
/*
Appends a specified bytes of data to a file.

Parameters:
    file_handle: A file handle created by the X600Fs_OpenFile
    data: Pointer to any object; the data written begins at buf
    size: length of data (1~32767 bytes)
    
Return:
    >0: On success, returns the bytes of data actually written
    -1: The file not be opened whth the X600Fs_OpenFile
    -2: On failure, returns a negative value
    -3: Data verification error
*/


int mfs_Getc(int file_handle);
/*
Gets a character from a file

Parameters:
    file_handle: A file handle created by the X600Fs_OpenFile
    
Return:
    On success, returns the character read
   -1: On end-of-file or error
*/


int mfs_Putc(int file_handle, char data);
/*
Outpus a character data to the file

Parameters:
    file_handle: A file handle created by the X600Fs_OpenFile
    data: A character to the file
    
Return:
   On success, returns the character data
  -1: On failure, returns a negative value
*/


int mfs_Gets(int file_handle, char *data, unsigned max_size);
/*
Gets a sting from a file.
It reads characters from a file into the string data. 
It stops when it reads either max_size - 1 characters 
or a newline character (0x0A), whichever comes first.

X600Fs_Gets doesn't retain the newline character.
It uses a null character (0x00) to replace the newline characterstops 
to mark the end of the string. 

Parameters:
    file_handle: A file handle created by the X600Fs_OpenFile
    data: Pointer assigned to a buffer which used to store read data.
    max_size: Length of string read
    
Return:
   >=0: On success, returns the number (1~32767) of bytes actually read
   <=0: On failure, returns a negative value
*/


int mfs_Puts(int file_handle, char *data);
/*
Outputs a string a file.
It also addes a newline character to the end of string in the file.

Parameters:
    file_handle: A file handle created by the X600Fs_OpenFile
    data: A string to the file
    
Return:
    On success, returns the last character written
   <0: On failure, returns a negative value
*/


int mfs_EOF(int file_handle);
/*
Macro that tests if end-of-file has been reached on a file

Parameters:
    file_handle: A file handle created by the X600Fs_OpenFile
    
Return:
    0: end-of-file has not been reached
    1: An end-of-file indicator was detected on the last input operation on the file
   -1: On error, returns a negative value
*/


int mfs_Seek(int file_handle, long offset, int whence);
/*
Repositions the file pointer of a file

Parameters:
    file_handle: A file handle created by the X600Fs_OpenFile 
    offset: Difference in bytes between whence and new position. Offset should be 0 or a value returned 
            by X600Fs_Tell.
    whence: One of three SEEK_xxx file pointer locations (0, 1, or 2)

Return:
    0: On success (the pointer is successfully moved)
   <1: On failure, reutrns a negative value
*/


long mfs_Tell(int file_handle);
/*
Returns the current file pointer

Parameters:
    file_handle: A file handle created by the X600Fs_OpenFile
    
Return:
  >=0: The current file pointer position
  -1L: Error
*/


void mfs_EnableWriteVerify(void);
/*
Enable the data verification. By default, the data verification is enabled.
If an error is detected while writing data to a file using mfs_WriteFile, 
the mfs_WriteFile will return a value of -3.
*/


void mfs_DisableWriteVerify(void);
/*
Disable the data verification.
*/

#ifdef __cplusplus
 }
#endif
