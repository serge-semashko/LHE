Attribute VB_Name = "VxComCfg"
'*********************************************************************************
'      The Declare of VxComCfg.DLL for VxComm Driver/Utility
'*********************************************************************************

Global Const VxC_NoError = 0                    ' OK
Global Const VxC_DriverNotInstalled = 1         ' Open Driver Error
Global Const VxC_DriverInstalled = 2            ' Driver installed but not started
Global Const VxC_DriverRunning = 3              ' Driver installed and is running
Global Const VxC_OpenSCMError = 4               ' Failed to open SCM; 9x doesnot supports SCM
Global Const VxC_StartDriverError = 5           ' Failed to start driver
Global Const VxC_StopDriverError = 6            ' Failed to stop driver
Global Const VxC_TimeoutError = 7               ' Timeout occurred when starting/stopping driver
Global Const VxC_OpenDriverRegistryError = 8    ' Failed to open driver registry
Global Const VxC_ReadDriverRegistryError = 9    ' Failed to read from driver registry
Global Const VxC_WriteDriverRegistryError = 10  ' Failed to write into driver registry
Global Const VxC_FileOpenError = 11             ' Failed to open file
Global Const VxC_FileReadError = 12             ' Failed to read file
Global Const VxC_IPIsRequired = 13              ' IP command is missing in the file
Global Const VxC_InvalidParameter = 14          ' Invalid command or parameter line
Global Const VxC_COMPortNotAvailable = 15       ' Specified COM port already assigned by other device
Global Const VxC_COMPortDuplicated = 16         ' Assigned COM port is duplicated
Global Const VxC_NoDeviceSetting = 17           ' No device setting in the file
Global Const VxC_OpenSerialRegistryError = 18   ' Failed to open Serial Device registry
Global Const VxC_FileWriteError          = 19   ' Failed to write file
Global Const VxC_InvalidVersion          = 20   ' Should assign Version= 1.1 at first line
Global Const VxC_InvalidCommand          = 21   ' Invalid Command
Global Const VxC_InvalidOption           = 22   ' Invalid Option Name
Global Const VxC_InvalidValue            = 23   ' Invalid Value/data/setting
Global Const VxC_InvalidStructSize       = 24   ' Invalid data struct size of SERVER_DATA
Global Const VxC_InvalidDeviceName       = 25   ' Invalid Deivce Name in the registry

' ********************************************************************************
' VxC_ImportConfigFile
'
' Parameters :
'    pszConfigFile    : [Input]  The configuration file path and name.
'    pLastErrorLine   : [Output] Indicates which line in the file has error.
'    pwRebootRequired : [Output] Indicates this computer need or needn't to reboot
'                                after importing configuration file.
' Return :
'    Returns error code indicating something error, or returns 0 indicating no error.
'
' Notes :
'        In Windows NT/2K/XP, users needn't to reboot the computer if everything is
'    OK! But in Windows 9x/ME, users may need to reboot it after change the settings.
'
' ********************************************************************************

Declare Function VxC_ImportConfigFile Lib "VxComCfg.DLL" ( _
                 ByVal pszConfigFile As String, _
                 pLastErrorLine As Integer, _
                 pwRebootRequired As Integer _
                 ) As Integer
        
                              
                                 

