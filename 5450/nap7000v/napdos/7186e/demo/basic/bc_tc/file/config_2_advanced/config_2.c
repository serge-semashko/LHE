/*
Config_2.c : Reads config file (text file)(advanced)

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.
          
Compile mode: large

Project: Config_2.c
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)

Detail description:
    In many applications, users need one text file to record some information.
    And the program can read it.
    You can use GetProFileInt, GetProFileStr to get specific information 
    in text files.
 
    The format of the text file (config.ini) must follow the rules:
    1. The item information you want to read must be positioned after '*'.
    2. The comment cann't contain any '*'.
    3. If the setting of KeyName is string type, you must add
       least one space character to the end of file(EOF).

    For example:
    =====================================
    config file sample for Config_2.c
    =====================================
    * baudreate=115200 ;integer type 
    * Debug=on         ;string type
    * slot0=I8024      ;string type
    =====================================
    comment...........
 
Hardware: i-7188/uPAC-7186/i-8000
 
Note: to run this demo, you must load one extra text file into i-7188/uPAC-7186/i-8000.

[Oct 30, 2008] by Liam
[03, June, 2010] by Nicholas
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "..\..\lib\7186e.h"   /* you must change this lib to suit the hardware that you want to use. */

unsigned long FSeek(FILE_DATA far *file_pointer, char cMark, unsigned long lStart, int iTh);
    /*
    Seeks the iTh(th) cMark from the start position 
    to the EOF(end of file).
    
    cMark: find the position next to the cMark.
    lStart: the start position to seek the cMark. 
            (0: start position of the file)
    iTh: seek the iTh(th) cMark.
    
    Return value:
        0: can't find the cMark
       >0: the offset position next to the cMark
    */
    
long GetProFileInt(FILE_DATA far *file_pointer, char* sKeyName, long lDefault);
    /*
    Gets integer of sKeyName, the sKeyName in text file must be after one '*'.
    If can't find the sKeyName, return the lDefault.
    If can't find the value of the sKeyName, return the lDefault. 
    */

float GetProFileFloat(FILE_DATA far *file_pointer, char* sKeyName, float fDefault);
    /*
    Gets float of sKeyName, the sKeyName in text file must be after one '*'.
    If can't find the sKeyName, return the fDefault.
    If can't find the value of the sKeyName, return the fDefault. 
    */  
    
int GetProFileStr(FILE_DATA far *file_pointer, char* sKeyName, char* sResult, char* sDefault);
    /*
    Gets string of sKeyName, the sKeyName in text file must be after one '*'.
    If can't find the sKeyName, sResult=sDefault.
    If can't find the string of the sKeyName, sResult=sDefault. 
    Return value: length of sResult

    I use GetProFileStr to replace GetProFileVal.
    float GetProFileVal(FILE_DATA far *file_pointer,char* sKeyName,float fDefault);
    Gets float value of sKeyName, the sKeyName in text file must be after one '*'.
    If can't find the sKeyName, return the fDefault.
    If can't find the value of the sKeyName, return the fDefault.
    */
    
void main(void)
{
    static FILE_DATA far *config;    
    float fValue,fDefault;
    long lValue,lDefault;
    char sString[40];
    int iLength;
    
    InitLib();
    
    // Opens one file by name
    config=GetFileInfoByName("config_2.ini"); 
    
    // Gets one integer
    lValue=GetProFileInt(config, "Baudrate", 9600);     // Default=9600
    Print("Baudrate=%ld\n\r", lValue);

    // Gets one string
    iLength=GetProFileStr(config, "ID", sString, "Error");      // Default=Error
    if(iLength>0)
        Print("ID=%s\n\r", sString);    
    
    // Gets one float value
    fValue=GetProFileFloat(config, "pi", 1.23);     // Default=1.23
    Print("pi=%f\n\r", fValue);
    
    // Gets one integer in Hex format
    iLength=GetProFileStr(config, "Hex_Value", sString, "FFFF");    // Default=FFFF
    if(iLength>0)
    {
        sscanf(sString, "%lx", &lValue); 
        Print("Hex_Value=%lx(Hex)\n\r", lValue);
    }
}

/*=====================================================================*/
/*====== Following functions are used to read config file.          ===*/
/*====== You can copy the functions to your own program.            ===*/
/*=====================================================================*/
unsigned long FSeek(FILE_DATA far *file_pointer, char cMark, unsigned long lStart, int iTh)
{
    /*
    Seeks the iTh(th) cMark from the start position 
    to the EOF(end of file).
    
    cMark: find the position next to the cMark.
    lStart: the start position to seek the cMark. 
            (0: start position of the file)
    iTh: seek the iTh(th) cMark.
    
    Return value:
        0: can't find the cMark
       >0: the offset position next to the cMark
    */
    
    char c;
    unsigned long i;
    int iOrder;
    
    i=lStart;
    iOrder=1;
    
    while(i<(file_pointer->size-1))
    {
        c=file_pointer->addr[i];
        if(c==cMark)
            if(iOrder==iTh)
                return ++i;     // return the position next to the cMark
            else
                iOrder++;
        i++;
    }
    return 0;
}

long GetProFileInt(FILE_DATA far *file_pointer, char* sKeyName, long lDefault)
{        
    /*
    If can't find the sKeyName, return the lDefault.
    If can't find the value of the sKeyName, return the lDefault.  
    */
    
    unsigned long lPointer=0;
    char sTemp[20];
    long lValue;
    int i;
    strlwr(sKeyName);
    
    // Find  out the Item
    while(lPointer<file_pointer->size)
    {
        lPointer=FSeek(file_pointer, '*', lPointer, 1);     // To find the 1st '*' from lPointer
        if(lPointer==0)     //' cannot find '*'
            return lDefault;
        sscanf(file_pointer->addr+lPointer, "%s", &sTemp);
        strlwr(sTemp);
		
        if( (strncmp(sKeyName,sTemp,strlen(sKeyName))) == 0) 
        {
            lPointer=FSeek(file_pointer, '=', lPointer,1);  // To find the 1st '=' from lPointer
            if(lPointer==0)     // cannot find '='
                return lDefault;
                
            sscanf(file_pointer->addr+lPointer, "%ld", &lValue);
            
            return lValue;
        }
    }
    
    // EOF
    return lDefault;
}

float GetProFileFloat(FILE_DATA far *file_pointer, char* sKeyName, float fDefault)
{      
    /*
    Gets float of sKeyName, the sKeyName in text file must be after one '*'.
    If can't find the sKeyName, return the fDefault.
    If can't find the value of the sKeyName, return the fDefault. 
    */    
    
    unsigned long lPointer=0;
    char sTemp[20];
    float fValue;
    
    strlwr(sKeyName);
    
    // Find  out the Item
    while(lPointer<file_pointer->size)
    {
        lPointer=FSeek(file_pointer, '*', lPointer, 1);    // To find the 1st '*' from lPointer
        if(lPointer==0)     // cannot find '*'
            return fDefault;
            
        sscanf(file_pointer->addr+lPointer, "%s", &sTemp);
        strlwr(sTemp);        
        
        if( (strncmp(sKeyName,sTemp,strlen(sKeyName))) == 0) 
        {
            lPointer=FSeek(file_pointer,'=',lPointer,1);    // To find the 1st '=' from lPointer
            if(lPointer==0)     // cannot find '='
                return fDefault;
                
            sscanf(file_pointer->addr+lPointer, "%f", &fValue);
            return fValue;
        }
    }
    
    // EOF
    return fDefault;
}

int GetProFileStr(FILE_DATA far *file_pointer, char* sKeyName, char* sResult, char* sDefault)
{
    /*
    If can't find the sKeyName, sResult=sDefault.
    If can't find the string of the sKeyName, sResult=sDefault.    
    Return value: length of sResult     
    */

    unsigned long lPointer=0;
    char sTemp[20];
    int i, iLen;

    strlwr(sKeyName);
    
    // Find  out the Item
    while(lPointer<file_pointer->size)
    {
        lPointer=FSeek(file_pointer, '*', lPointer,1);  // To find the 1st '*' from lPointer
        if(lPointer==0)     // cannot find '*'
        {
            strcpy(sResult, sDefault);
            return strlen(sDefault);
        }
        
        sscanf(file_pointer->addr+lPointer, "%s", &sTemp);
        strlwr(sTemp);
		
        if( (strncmp(sKeyName,sTemp,strlen(sKeyName))) == 0) 
        {
            lPointer=FSeek(file_pointer, '=', lPointer,1);  // To find the 1st '*' from lPointer
            if(lPointer==0)     // cannot find '='
            {
                strcpy(sResult,sDefault);
                return strlen(sResult);
            }
            
            sscanf(file_pointer->addr+lPointer,"%s",&sTemp);
            if(strlen(sTemp)==0)    // cannot find string
                strcpy(sResult,sDefault);
            else
                strcpy(sResult,sTemp);
            return strlen(sResult);
        }
    }
    
    // EOF
    strcpy(sResult,sDefault);
    return strlen(sResult);
}