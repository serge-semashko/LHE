/*
FTP Implementation [2010,03,05] by Kevin

FTP Client                                      FTP Server
----------------------------------------------------------------
Socket[0] === (Connect) ======================> Command Port(21)
Socket[0] <== 220 Welcome ... ================= Command Port(21)
Socket[0] === USER id ========================> Command Port(21)
Socket[0] <== 331 Please specify the password = Command Port(21)
Socket[0] === PASS password ==================> Command Port(21)
Socket[0] <== 230 Login successfull =========== Command Port(21)
Socket[0] === CWD remote path ================> Command Port(21)
Socket[0] <== 250 Directory successfully changed. = Command Port(21)
Socket[0] === TYPE I =========================> Command Port(21)
Socket[0] <== 200 Switching to Binary mode. === Command Port(21)
Socket[0] === PASV ===========================> Command Port(21)
Socket[0] <== 227 Enter Passive Mode (ip1,ip2,ip3,ip4,port1,port2)  Command Port(21)

Socket[1] === (Connect) ======================> Data Port(port1*256+port2)

Socket[0] === STOR filename ==================> Command Port(21)
Socket[0] <== 150 or 125 Ok to send data ============= Command Port(21)

Socket[1] === (send data) ====================> Data Port(port1*256+port2)
Socket[1] === (Close) ========================> Data Port(port1*256+port2)

Socket[0] <== 226 File receive Ok ============= Command Port(21)
Socket[0] === QUIT ===========================> Command Port(21)
Socket[0] <== 221 Goodbye ===================== Command Port(21)
Socket[0] === (Close) ========================> Command Port(21)

Note: the implementation assumes that the format of all FTP responses 
      are like xxx Message, where xxx is a number
      
How to use ?
    Step1: Init the CPU:                calls InitLib();
    Step2: Init the Ethernet:           calls Ethernet_Init(); 
           //if the program begins from main, must call this.
           //if the program is based on the Xserver or MiniOS7 Framework,
           //then doesn't call the function.
                            
    Step3: Connect to the FTP server:   calls FTP_Connect(.....);
    Step4: Send the file:               calls FTP_SendFile(....); in a loop.
    
    Example:
        void main(void)
        {
            int ret;
            InitLib();
            Ethernet_Init();
            pc_ertfs_init();
            FTP_Connect("10.0.0.33",21,"id","password",1000);
            do
            {
                //send "source.txt" from Disk A to the FTP server as "target.txt". 
                ret=FTP_SendFile("/home/upload/","target.txt",0,"source.txt");
            }while(ret==1)
            if(ret==0)
            {
                //Send file ok.
            }
            else
            {
                //Error.
            }
        }
		
	[Aug,3,2011] by Nicholas
		Add command 125 to send data
*/

#include "..\..\lib\7186E.h"	
#include "..\..\lib\TCPIP32.h"
#include <stdio.h>

//Following 3 variables are defined in TCP/IP library.
extern unsigned long ACKDELAY;
extern int errno;
extern long ET_TOUT; /* basic timeout value */

//For FTP function
unsigned char gcFTP_IP_Address[16],gcFTP_ID[21],gcFTP_Password[21];
unsigned long glFTP_Connect_Timeout,glFTP_Send_TimeTick;
unsigned long glRemnant_size;
int giFTP_Socket=-1,giFTP_Data_Socket=-1,giSendFileOk;
int giReady2SendFile=0;

//for files in DiskA/B of MiniOS7
FILE_DATA far *gpFile_data;
unsigned long glFilePosition;
int giFileHandle;

int TCP_Connect(unsigned char *cRemoteIP,unsigned int iRemoteTCPPort,long lConnect_Timeout)
{
    //*cRemoteIP: example: "192.168.255.1"
    //iRemoteTCPPort: 0~65535
    //lConnect_Timeout: unit is 1 ms
    //Return: >=0  :ok, return socket number
    //         -1  :error, can not connect to the FTP server
    
    int iSocketNumber,iRet;
    unsigned long lStartTimeTick;
    int iTemp;
    
    struct sockaddr_in sktRemoteServer;  
    
    iSocketNumber=socket(PF_INET,SOCK_STREAM,0);   
    if(iSocketNumber>=0)
    {    
        YIELD();
        memset(&sktRemoteServer, 0, sizeof(sktRemoteServer)); /* bzero is a unix system call */
        sktRemoteServer.sin_family = AF_INET;          
        sktRemoteServer.sin_addr.s_addr = inet_addr(cRemoteIP);
        sktRemoteServer.sin_port = htons( iRemoteTCPPort );    
        YIELD();
        
        SOCKET_NOBLOCK(iSocketNumber);   /*Set Non-blocking mode*/
        ET_TOUT=600L;  //300ms, ET_TOUT has less than 800ms (WatchDog reboot time)
        iRet=connect(iSocketNumber, (struct sockaddr *)&sktRemoteServer, sizeof(sktRemoteServer));
        if(iRet) /* unsuccessful connection not 0 */
        {       
            lStartTimeTick=GetTimeTicks();
            
            if(errno==EINPROGRESS)
            {
                while((GetTimeTicks()-lStartTimeTick)<lConnect_Timeout)
                {
                    RefreshWDT();
                    YIELD();
                    if(SOCKET_ISOPEN(iSocketNumber)) 
                    {
                        ET_TOUT=300L;
                        iTemp=iSocketNumber;
                        break; 
                    }
                    else
                        iTemp=-1;
                }
                if(iTemp==-1)
                    closesocket(iSocketNumber);    
                iSocketNumber=iTemp;
            }
            else
            {
                closesocket(iSocketNumber);
                iSocketNumber=-1; 
            }
        }
    }
    else
        iSocketNumber=-1;

    YIELD();
    Delay(5);  //Must >=3 ms to make sure the following TCP/IP function works.
    
    if(iSocketNumber>=0)
    {
        /*#ifdef XSERVER
        SocketConnected++;   
        #endif*/
        return iSocketNumber;     
    }
    else
    {
        RefreshWDT(); 
        return -1;
    }
}      

int TCP_Receive(int iSocket,char *cInBuf,int iMaxLen,int iTimeout)
{
    int iLength;
    
    SOCKET_RXTOUT(iSocket, iTimeout); //set the receive timeout for a connection//
    iLength=recv(iSocket, cInBuf, iMaxLen, 0); //MSG_OOB
    SOCKET_BLOCK(iSocket);

    //Add 0 to the end of the received data.
    if(iLength>=0) cInBuf[iLength]=0;
    else cInBuf[0]=0;
        
    return iLength;
}

int FTP_Data_Close(void)
{
    // close the socket of FTP data only.
    if(giFTP_Data_Socket>=0)
    {
        closesocket(giFTP_Data_Socket);
        giFTP_Data_Socket=-1;
        YIELD();
        Delay(1);
        YIELD();
        Delay(1);
    }
    return NoError;
}

int FTP_Close(void)
{
    //close the sockets of FTP command and FTP data.
    if(giFTP_Socket>=0)
    {
        closesocket(giFTP_Socket);
        giFTP_Socket=-1;
        YIELD();
        Delay(1);
        YIELD();
        Delay(1);
    }
    if(giFTP_Data_Socket>=0)
    {
        closesocket(giFTP_Data_Socket);
        giFTP_Data_Socket=-1;
        YIELD();
        Delay(1);
        YIELD();
        Delay(1);
    }
    return NoError;
}

int FTP_Connect(unsigned char *cRemoteIP,unsigned int iRemoteTCPPort,char* cID, char* cPassword,long lConnect_Timeout)
{
    /*
    *cRemoteIP: example: "192.168.255.1"
    iRemoteTCPPort: 0~65535
    *cID: example: "Admin", 20 bytes max.
    cPassword: example: "pass", 20 bytes max.
    lConnect_Timeout: unit is 1 ms
    
    Return: >=0  :ok, return socket number
             -1  :error, can not connect to the FTP server
             -2  :warning, in FTP processing. Doen't connect.
             
    Note: for anonymous login,
            *cID        ="anonymous" or "ftp"
            *cPassword  ="anonymous" or any string.
    */
    int iRet;
    
    if(giFTP_Socket<0)
    {
        strcpy(gcFTP_IP_Address,cRemoteIP);
        strcpy(gcFTP_ID,cID);
        strcpy(gcFTP_Password,cPassword);
        glFTP_Connect_Timeout=lConnect_Timeout;
        giFTP_Socket=TCP_Connect(gcFTP_IP_Address,iRemoteTCPPort,glFTP_Connect_Timeout);
        giFTP_Data_Socket=-1;
        giSendFileOk=0;
        return giFTP_Socket;
    }
    else
    {
        return -2;
    }
}       


int FTP_SendFile(char* remote_path,char* remote_filename,int local_disk, char* local_filename)
{
    /*
    *path: location on the FTP site, example: "/pub/"
    *remote_filename: the file name on the FTP server. can be long file name. 
                      example: "test-abc-20100125.txt"
                          
    local_disk: 0=diskA of MiniOS7
                1=diskB of MiniOS7
                2=diskA of 64 MB flash disk
                3=diskB of 64 MB flash disk
    
    *local_filename: the file name on the local disk. must be 8.3 format. 
                      example: "test-abc.txt"
    
    return 0: send file ok.
           1: in the process of sending the file.
          -1: Timeout
          -2: Send file to FTP server failed.
          -3: Due to another Ethernet communication reason.
          -4: FTP server not conneted.
          -5: Can not open file
          530: Login incorrect.
          550: Failed to change dircetory.
    */
    
    char sCommand[40],sResponse[80],sData[1460];
    int iReceive_Length;
    int iComma,i,iPort1,iPort2,iRet;
    
    int iSend_size;
    
    if(giFTP_Socket>=0)
    {
        //The time interval between two TCP/IP actions must >1 ms.
        if((GetTimeTicks()-glFTP_Send_TimeTick)> 1)  
        {
            //Loop to send file.
            if(giFTP_Data_Socket>=0 && giReady2SendFile)
            {
                if(local_disk==0 || local_disk==1)
                {
                    glFilePosition=gpFile_data->size-glRemnant_size;
                }    
                if(glRemnant_size>1460)
                {
                    iSend_size=1460;
                    glRemnant_size-=1460;
                }
                else
                {
                    iSend_size=glRemnant_size;
                    glRemnant_size=0;
                }
                
                if(local_disk==0 || local_disk==1)
                {
                    iRet=send(giFTP_Data_Socket,gpFile_data->addr+glFilePosition,iSend_size,0); //MSG_OOB
                    if(iRet>=0) glFTP_Send_TimeTick=GetTimeTicks();
                }             
                if(glRemnant_size==0)
                {
                    giReady2SendFile=0;
                    FTP_Data_Close();
                }
            }
                        
            iReceive_Length=TCP_Receive(giFTP_Socket,sResponse,80-1,0); //wait for 0 ms is enough
            
            if(iReceive_Length>=0)
            {
                glFTP_Send_TimeTick=GetTimeTicks();
                if(!memcmp(sResponse,"220",3)) //220 Welcome ...
                {
                    sprintf(sCommand,"USER %s\r\n",gcFTP_ID);
                    iRet= send(giFTP_Socket,sCommand,strlen(sCommand),0); //MSG_OOB
                }
                else if(!memcmp(sResponse,"331",3)) //331 Please specify the password
                {
                    //The time to pass the identification is much longer than other commands.
                    //SO, increase the timeout wheile waiting for passing identification.
                    glFTP_Connect_Timeout+=3000;
                    sprintf(sCommand,"PASS %s\r\n",gcFTP_Password);
                    iRet= send(giFTP_Socket,sCommand,strlen(sCommand),0); //MSG_OOB
                }
                else if(!memcmp(sResponse,"230",3)) //230 Login successfull
                {
                    glFTP_Connect_Timeout-=3000;
                    sprintf(sCommand,"CWD %s\r\n",remote_path);
                    iRet= send(giFTP_Socket,sCommand,strlen(sCommand),0); //MSG_OOB
                }
                else if(!memcmp(sResponse,"250",3)) //250 Directory successfully changed.
                {
                    strcpy(sCommand,"TYPE I\r\n");
                    iRet= send(giFTP_Socket,sCommand,strlen(sCommand),0); //MSG_OOB
                }
                else if(!memcmp(sResponse,"200",3)) //200 Switching to Binary mode.
                {
                    strcpy(sCommand,"PASV\r\n");
                    iRet= send(giFTP_Socket,sCommand,strlen(sCommand),0); //MSG_OOB
                }
                else if(!memcmp(sResponse,"227",3)) //227 Enter Passive Mode (ip1,ip2,ip3,ip4,port1,port2)
                {
                    iComma=0;
                    for(i=0;i<80;i++)
                    {
                        if(sResponse[i]==',') iComma++;
                        if(iComma>=4) break;
                    }
                    sscanf(sResponse+i+1,"%d,%d",&iPort1,&iPort2);
                    if(giFTP_Data_Socket<0)
                    {
                        giFTP_Data_Socket=TCP_Connect(gcFTP_IP_Address,(iPort1<<8)+iPort2,glFTP_Connect_Timeout);
                    }
                    
                    if(giFTP_Data_Socket>=0)
                    {
                        //Prepare to send file.
                        if(local_disk==0 || local_disk==1)
                        {
                            gpFile_data=(FILE_DATA far *) GetFileInfoByName_AB(local_disk,local_filename);
                            if(gpFile_data!=NULL)
                            {
                                glRemnant_size=gpFile_data->size;
                            }
                            else
                            {
                                FTP_Close();
                                return -5; //Can not open file
                            }
                        }                        
                        
                        sprintf(sCommand,"STOR %s\r\n",remote_filename);
                        iRet= send(giFTP_Socket,sCommand,strlen(sCommand),0); //MSG_OOB
                    }    
                    else
                    {
                        strcpy(sCommand,"QUIT\r\n");
                        iRet= send(giFTP_Socket,sCommand,strlen(sCommand),0); //MSG_OOB
                    }
                }
                else if(!memcmp(sResponse,"150",3)|| !memcmp(sResponse,"125",3)) //150 or 125 Ok to send data
                {
                    giReady2SendFile=1;
                }
                else if(!memcmp(sResponse,"226",3)) //226 File receive Ok
                {
                    giSendFileOk=1;
                    strcpy(sCommand,"QUIT\r\n");
                    iRet= send(giFTP_Socket,sCommand,strlen(sCommand),0); //MSG_OOB
                }
                else if(!memcmp(sResponse,"221",3)) //221 Goodbye
                {
                    FTP_Close();
                    if(giSendFileOk)   
                        return NoError;  //return NoError¡C
                    else
                        return -2; //Send file to FTP server failed.
                }
                else// if(!memcmp(sResponse,"530",3)) //530 Login incorrect
                {
                    sscanf(sResponse,"%d",&iRet);
                    FTP_Close();
                    return iRet;
                }
                    
                return 1; //in the process of sending the file.  
            }
            //Due to timeout.
            else if(errno==ETIMEDOUT) //Detect whether the error of recv is timeout.
            {
                if((GetTimeTicks()-glFTP_Send_TimeTick)>glFTP_Connect_Timeout)
                {
                    FTP_Close();
                    return -1; //Timeout
                }                                                   
                else
                {
                    return 1; //in the process of sending the file.
                }
            }                               
            else 
            {
                FTP_Close();
                return -3; //Due to another Ethernet communication reason.
            }
        }
        return 1; //in the process of sending the file. 
    }
    else
        return -4; //FTP server not conneted.
}

