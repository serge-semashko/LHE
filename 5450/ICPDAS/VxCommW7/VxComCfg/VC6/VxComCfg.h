
#ifndef __VXCOMCFG__
#define __VXCOMCFG__

#include "windows.h"

#ifndef EXPORTS
#ifdef __cplusplus       // for C++ users
     #define EXPORTS extern "C" __declspec (dllimport)
#else                    // for C   users
     #define EXPORTS __declspec (dllimport)
#endif
#endif

#define VxC_NoError                   0  // OK
#define VxC_DriverNotInstalled        1  // Open Driver Error
#define VxC_DriverInstalled           2  // Driver installed but not started
#define VxC_DriverRunning             3  // Driver installed and is running
#define VxC_OpenSCMError              4  // Failed to open SCM; 9x doesnot supports SCM
#define VxC_StartDriverError          5  // Failed to start driver
#define VxC_StopDriverError           6  // Failed to stop driver
#define VxC_TimeoutError              7  // Timeout occurred when starting/stopping driver
#define VxC_OpenDriverRegistryError   8  // Failed to open driver registry
#define VxC_ReadDriverRegistryError   9  // Failed to read from driver registry
#define VxC_WriteDriverRegistryError 10  // Failed to write into driver registry
#define VxC_FileOpenError            11  // Failed to open file
#define VxC_FileReadError            12  // Failed to read file
#define VxC_IPIsRequired             13  // IP command is missing in the file
#define VxC_InvalidParameter         14  // Invalid command or parameter line
#define VxC_COMPortNotAvailable      15  // Specified COM port already assigned by other device
#define VxC_COMPortDuplicated        16  // Assigned COM port is duplicated
#define VxC_NoDeviceSetting          17  // No device setting in the file
#define VxC_OpenSerialRegistryError  18  // Failed to open Serial Device registry
#define VxC_FileWriteError           19  // Failed to write file
#define VxC_InvalidVersion           20  // Should assign Version= 1.1 at first line
#define VxC_InvalidCommand           21  // Invalid Command
#define VxC_InvalidOption            22  // Invalid Option Name
#define VxC_InvalidValue             23  // Invalid Value/data/setting
#define VxC_InvalidStructSize        24  // Invalid data struct size of SERVER_DATA
#define VxC_InvalidDeviceName        25  // Invalid Deivce Name in the registry

#endif

// ********************************************************************************
// VxC_ImportConfigFile
//
// Parameters :
//    pszConfigFile    : [Input]  The configuration file path and name.
//    pLastErrorLine   : [Output] Indicates which line in the file has error.
//    pwRebootRequired : [Output] Indicates this computer need or needn't to reboot
//                                after importing configuration file.
// Return :
//    Returns error code indicating something error, or returns 0 indicating no error.
//
// Notes :
//        In Windows NT/2K/XP, users needn't to reboot the computer if everything is 
//    OK! But in Windows 9x/ME, users may need to reboot it after change the settings.
//
// ********************************************************************************
EXPORTS WORD WINAPI 
VxC_ImportConfigFile(PCHAR pszConfigFile, PWORD pLastErrorLine, PWORD pwRebootRequired);

