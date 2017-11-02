unit VxComCfg;

interface

type PSingle=^Single;
     PWord=^Word;
     PInteger=^Integer;

Const

	VxC_NoError                  =  0;  // OK
 	VxC_DriverNotInstalled       =  1;  // Open Driver Error
 	VxC_DriverInstalled          =  2;  // Driver installed but not started
 	VxC_DriverRunning            =  3;  // Driver installed and is running
 	VxC_OpenSCMError             =  4;  // Failed to open SCM; 9x doesnot supports SCM
 	VxC_StartDriverError         =  5;  // Failed to start driver
 	VxC_StopDriverError          =  6;  // Failed to stop driver
 	VxC_TimeoutError             =  7;  // Timeout occurred when starting/stopping driver
 	VxC_OpenDriverRegistryError  =  8;  // Failed to open driver registry
 	VxC_ReadDriverRegistryError  =  9;  // Failed to read from driver registry
 	VxC_WriteDriverRegistryError = 10;  // Failed to write into driver registry 
 	VxC_FileOpenError            = 11;  // Failed to open file
 	VxC_FileReadError            = 12;  // Failed to read file
 	VxC_IPIsRequired             = 13;  // IP command is missing in the file
 	VxC_InvalidParameter         = 14;  // Invalid command or parameter line
 	VxC_COMPortNotAvailable      = 15;  // Specified COM port already assigned by other device
 	VxC_COMPortDuplicated        = 16;  // Assigned COM port is duplicated
 	VxC_NoDeviceSetting          = 17;  // No device setting in the file
 	VxC_OpenSerialRegistryError  = 18;  // Failed to open Serial Device registry
	VxC_FileWriteError           = 19;  // Failed to write file
 	VxC_InvalidVersion           = 20;  // Should assign Version= 1.1 at first line
 	VxC_InvalidCommand           = 21;  // Invalid Command
 	VxC_InvalidOption            = 22;  // Invalid Option Name
 	VxC_InvalidValue             = 23;  // Invalid Value/data/setting
	VxC_InvalidStructSize        = 24;  // Invalid data struct size of SERVER_DATA
	VxC_InvalidDeviceName        = 25;  // Invalid Deivce Name in the registry

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

Function VxC_ImportConfigFile(
				pszConfigFile:PChar; 
				pLastErrorLine:PWord; 
				pwRebootRequired:PWord):Word; StdCall;

implementation


Function VxC_ImportConfigFile; external 'VxComCfg.DLL' name 'VxC_ImportConfigFile';


end.

