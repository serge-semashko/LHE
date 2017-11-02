/*
config_1.c: Reads information from text files(basic)

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          MSC 6.0,
          MSVC 1.52.
          
Compile mode: large

Project: config_1.c
         ..\..\Lib\(8000e.Lib, 7188el.Lib or 7186el.Lib)

Detail description:
    In many applications, users need one text file to record some information.
    And the program can read it.
    You can use FSeek to get specific information in text files. 

    The format of the text file (config_1.ini) must follow the rules:
    1. The item information you want to read must be positioned after '*'.
    2. The comment cann't contain any '*'.
    3. KeyName and '=' must be seperated by least one space character.
    4. If the setting of KeyName is string type, you must add least one space 
       character to the end of file(EOF).

        For example:
        =================================================
        config file sample for reading config_1.ini
        =================================================
        Star Item1  Item2  Item3  Item4   Comment        
        =================================================
        *    1st    19200  on     1.25    ;comment1      
        *    2nd    9600   off    -1.3   ;comment2      
        *    3th    2400   off    19.34   ;comment3      
        = end of settings =                              
        =================================================
        comment...........                               
    
Hardware: i-7188/uPAC-7186/i-8000
     
Note: To run this demo, you must load one extra text file into i-7188/uPAC-7186/i-8000.

[Oct 30, 2008] by Liam
*/

#include <stdlib.h>
#include <stdio.h>
#include "..\..\lib\7186e.h"

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
    
void main(void)
{
    static FILE_DATA far *config;
    int i;
    char sItem1[20];
    char sItem2[20];
    char sItem3[20];
    char sItem4[20];
    char sTemp[40];
      
    unsigned long lPointer=0;
    int iTh;    // iThe: to find information from ?th * in the text file
    
    InitLib();
    
    config=GetFileInfoByName("config_1.ini");   // Opens one file by name
    
    while(lPointer<config->size) 
    {
        lPointer=FSeek(config, '*', lPointer, 1);
        if(lPointer)    // cannot find '*'
        {
            sscanf(config->addr+lPointer, "%s %s %s %s", &sItem1, &sItem2, &sItem3, &sItem4); 
            sprintf(sTemp, "Item1=%s Item2=%s Item3=%s Item4=%s", sItem1, sItem2, sItem3, sItem4);
            Print("%s\n\r", sTemp);
        }   
        else
            break;
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
            {    
                iTh++;
                return ++i; /* return the position next to the cMark. */
            }
            else
                iOrder++;
        i++;
    }
    return 0;
}