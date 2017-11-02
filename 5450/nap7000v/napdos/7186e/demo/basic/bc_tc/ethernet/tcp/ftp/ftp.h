/******************************************************/
/*  FTP Client head file                              */
/*                                                    */
/******************************************************/

#ifdef __cplusplus
extern "C" {
#endif

int Ethernet_Init(void);
    /*
    If the program begins from main, 
    the Ethernet chip must be initialized first.
        
    Return:
         0: ok.
        -1: function "llip" error
        -2: function "Ninit" error
        -3: function "Portinit" error
    */
    
    
int FTP_Connect(unsigned char *cRemoteIP,unsigned int iRemoteTCPPort,char* cID, char* cPassword,long lConnect_Timeout);
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

int FTP_SendFile(char* remote_path,char* remote_filename,int local_disk, char* local_filename);
    /*
    *path: location on the FTP site, example: "/pub/"
    *remote_filename: the file name on the FTP server. can be long file name. 
                      example: "test-abc-20100125.txt"
                          
    local_disk: 0=diskA of MiniOS7
                1=diskB of MiniOS7
                2=diskA of 64 MB flash disk
                3=diskB of 64 MB flash disk
                4=disk of microSD
    
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


#ifdef __cplusplus
}
#endif