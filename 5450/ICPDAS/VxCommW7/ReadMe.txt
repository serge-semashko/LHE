**********************************************************************
**                                                                  **
** VxComm Driver                                                    **
**                                                                  **
** Update  : Nov.07, 2014                                           **
** Version : 2.12.11                                                **
**                                                                  **
**********************************************************************
** VxComm Utility : v2.12.11                                        **
** VxComm Driver  : v2.12.11   (Ynsernet.sys)                       **
**********************************************************************

A. Drivers & Platforms

   Note: You must use the correct driver depends on your platform.

   VxCommW7.exe : for 32- and 64-bit Windows XP/2003/2008/2012/Vista/7/8.

   Note: The VxComm driver for Windows NT 4.0 is no more update (Last 
         version is v2.11.04.) even that is still included in VxCommW7. 

   Note: The VxComm98 is no more update, and it supports 7188En series only.

B. Introduction

      The VxComm Driver is designed for ICP DAS serial device servers like 
   7188E/8000E/DS-700/PDS-700/PDS-800/iDS-700 series Programmable Device Server 
   (Internet/Ethernet Controller). It creates virtual COM port(s) under  
   Microsoft Windows XP/2003/2008/2012/Vista/7/8 platform and maps them to the 
   Ethernet port of these serial device servers.

      Commands and data  sending  to  the  virtual COM port(s)  will  be 
   redirected to the serial device servers' COM port(s). And the recieved 
   data from serial device servers' COM port(s) will also be redirected to 
   PC's virtual COM port(s). 

      The user's RS-232 client programs need only to change to the 
   different COM port to get the access of serial devices that are 
   allocated in the Internet or Ethernet network via the serial device servers.

      The VxComm Utility is the configuration program for the VxComm 
   Driver. Users can use the utility program to assign the IP address of 
   7188E/8000E/PDS, and assign it's COM port number on the PC. The max. COM 
   port number is up to COM256 on Windows.

C. Installation

   1. Install "VxComm Driver W7". (VxCommW7.exe)
   2. "VxComm Utility" will be automatically launched after the installation.
   3. Use the VxComm Utility to configure the VxComm Driver.
   4. Restart the driver by click menu item "Tools \ Restart driver".

Note: 
1. For more information, please refers to the VxComm Driver User's Manual.

