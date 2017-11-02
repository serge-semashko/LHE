/*
Project: FTP Client demo

Hardware: uPac-7186EX

Compiler: BC++ 3.1, 
          Turbo C++ 1.01(3.01) (free from http://cc.codegear.com/free/cpp)
          
Compile mode: large

Project: main.c
         FTP.c
         ..\lib\7186EL.LIB
         ..\lib\tcp_dm32.lib
[2010,03,05] by Kevin
[Aug,3,2011] by Nicholas
*/

#include <stdlib.h>
#include "..\..\lib\7186e.h"		
#include "..\..\lib\Tcpip32.h"
#include "FTP.h"

void main(void)
{
    int i,iRet;
    int iFTP_Scoket;
    int iFTP_Command;
    int iFTP_Result;
    int iFTP_Go;
    
    unsigned long lBeginTimeTicks;
    
//Step1. Initate the controller.
    InitLib();
    InstallCom1(115200L, 8, 0, 1);
    
//Step2. Initial the Ethernet adapter.
    iRet=NetStart();  
    if(iRet==NoError) 
        printCom1("Init Ethernet ok.\n\r");
    else
        printCom1("Init Ethernet error %d.\n\r",iRet);   
    
//Step3. Begin the loop function
    iFTP_Go=0;
	printCom1("Press 0~1 to execute FTP function.\n\r");
	printCom1("0: Disk A, 1: Disk B.\n\r");
    for(;;)
    {
        if(iFTP_Go==0)
        {
            //Press key 0~1 to execute FTP function.
            if(IsCom1())
            {
                iFTP_Command=ReadCom1()-'0';
                if(iFTP_Command<0) 
                {
                    RestoreCom1();
                    exit(1);
                }
				//user can define ftp site, port, id, password, timeout
                iFTP_Scoket=FTP_Connect("192.168.255.1",21,"id","password",1000);
				
				if(iFTP_Scoket>=0) 
                {
                    iFTP_Go=1;
                    lBeginTimeTicks=GetTimeTicks();
                }    
                printCom1("FTP socket=%d, iFTP_Command=%d\n\r",iFTP_Scoket,iFTP_Command);
            }
        }
        
        if(iFTP_Go)
        {
            switch(iFTP_Command)
            {
				//for example, ftp server: ftp://192.168.255.1/home/upload
                case 0:
					// home/upload:remote_path,   2010-0928-0.txt:remote_filename, 0: Disk A, ftp_clie.txt: local_filename
					iFTP_Result=FTP_SendFile("home/upload","2010-0928-0.txt",0,"ftp_clie.txt");
					break;
                case 1:
		            iFTP_Result=FTP_SendFile("home/upload","2010-0928-1.txt",1,"ftp_clie.c");
					break;		        
		        default:
		            break;
		    }
		    
		    if(iFTP_Result!=1) iFTP_Go=0;
            switch(iFTP_Result)
            {
                case 0:
                    printCom1("\n\r[Ok] Send File to FTP Server ok. Takes %ldms\n\r",GetTimeTicks()-lBeginTimeTicks);
                    break;
                case 1: //in the process of sending the file.
                    //printCom1("o");
                    break;
                case -1:
                    printCom1("\n\r[Error]Timeout!!!\n\r");
                    break;
                case -2:
                    printCom1("\n\r[Error]Sending failed!!!\n\r");
                    break;
                case -3:
                    printCom1("\n\r[Error]Due to another Ethernet communication reason!!!\n\r");
                    break;
                case -4: //FTP server not conneted!!!
                    printCom1("\n\r[Error] FTP server not connected.\n\r");
                    break;
                case -5: //Can not open file!!!
                    printCom1("\n\r[Error] Can not open file\n\r");
                    break;
                case 530: //Login incorrect!!!
                    printCom1("\n\r[Error] Login incorrect\n\r");
                    break;
                case 550: //Failed to change directory!!!
                    printCom1("\n\r[Error] Failed to change directory\n\r");
                    break;    
            }
        } //if(iFTP_Go)
    }//for(;;)
}    