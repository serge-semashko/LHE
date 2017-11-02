#define NoError     0
#define TimeOut     -5
#define ValError    2
#define DeviceError 3
#define CheckError  4
#define NoInit	    5
#define BadPage     6
#define UsedPage    7
#define CrcError    8

#ifdef __cplusplus
extern "C" {
#endif

extern int X600_CheckValidBlock;
/*
Default value is 1.
When X600_CheckValidBlock is set to 1 before call X600_Init(), X600_Init() will
check the valid block and make a invalid block table.
If X600_CheckValidBlock is set to 0, X600_Init() will not check the valid block.
If you want use the function to check the valid block, please DO NOT write data
to the spared data address 6 of page 0 & 1 in any block.
*/

extern int X600_CheckWriteData;	
/*
Default value is 0.
set X600_CheckWriteData=1, 
X600_Erase_FLASH_Block() will do blank check after erase the block.
All other write function will read back the data and compare it.
*/
int  X600_Init(void);
/*
 For 4M flash return 4,
 for 8M flash return 8,
 for 16M flash return 16,
 for 32M flash return 32,
 for 64M flash return 64,
 for 128M flash return 128,
 others(or NO flash) return 0
*/

void X600_Close(void);
/* [2005/10/14] add. 
 At the end of the program please call X600_Close() to free the dynamic buffer. 
*/

int  X600_Reset_FLASH(void);

#define X600_WRITE_DISABLE	0	/* [2005/05/20] */
#define X600_WRITE_ENABLE	1	/* [2005/05/20] */

void X600_WP_FLASH(char wp);
/*
  wp=X600_WRITE_DISABLE(0) --> write protect
  wp=X600_WRITE_ENABLE(1) --> write enable
*/
unsigned char X600_Read_FLASH_Status(void);
/* definition of status read :
   BIT 0: 0=erase/program OK, 1=erase/program Error
   BIT 1: 0
   BIT 2: 0
   BIT 3: 0
   BIT 4: 0
   BIT 5: 0
   BIT 6: 0=device busy, 1=device ready
   BIT 7: 0=write protected, 1=not protected
*/

int X600_Erase_FLASH_Block(unsigned block);
/*
 block range: 0~(MaxBlock-1)
*/

int X600_CheckBlank(unsigned block);
/*
 block range: 0~(MaxBlock-1)
*/

int X600_Write_Page(unsigned block,unsigned page, void far *buf1, void far *buf2);
int X600_Read_Page(unsigned block,unsigned page, void far *buf1, void far *buf2);
/*
  block: 0~(MaxBlock-1)
  page: 0~(PageNoPerBlock-1)
  buf1 size=512 bytes(PageDataSize)
  buf2 size=16 bytes(PageSpareDataSize)
*/

extern int  PageDataSize,PageSpareDataSize,PageNoPerBlock;
/*	     		  4M(X600) 8M(X601/X560) 16M(X602)
PageDataSize		= 512	   512        512
PageSpareDataSize	= 16	   16         16
PageNoPerBlock		= 16	   16         32	
*/

extern unsigned MaxBlock;
/*
  for 4M:  MaxBlock=512
  for 8M:  MaxBlock=1024
  for 16M: MaxBlock=1024
  for 32M: MaxBlock=2048
  for 64M: MaxBlock=4096
  for 128M: MaxBlock=8192
*/

/*
FLASH size=MaxBlock * PageNoPerBlock * (PageDataSize+PageSpareDataSize)
*/
typedef struct {
	int addr:9;
	int page:4;
	int block:10;
	int reserve:9;
}FLASH_ADDR;	/* for 4/8M type only */

/*
 [2005/05/20] add
*/

#define  X600_Write_Page_1(blk,page,buf)  X600_Write_Page_1n(blk,page,buf,PageDataSize) 
/*
int X600_Write_Page_1(unsigned block,unsigned page, unsigned char far *buf);

	X600_Write_Page_1() write data to main data area(size=512 bytes) 
*/

int X600_Write_Page_1n(unsigned block,unsigned page, void far *buf,int size);
/*
	X600_Write_Page_1n() write data to main data area,size<=512 bytes. 
*/

int X600_Write_Page_2n(unsigned block,unsigned page, void far *buf,int size);
/*
	X600_Write_Page_2n() write data to spare data area,size<=16 bytes) 
*/

#define  X600_Raed_Page_1(blk,page,buf)  X600_Read_Page_1n(blk,page,buf,PageDataSize) 
/*
int X600_Read_Page_1(unsigned block,unsigned page, unsigned char far *buf);
	X600_Read_Page_1() read data from main data area(size=512 bytes) 
*/

int X600_Read_Page_1n(unsigned block,unsigned page, void far *buf,int size);
/*
	X600_Read_Page_1n() read data from main data area,size <= 512 bytes 
*/

int X600_Read_Page_2n(unsigned block,unsigned page, void far *buf,int size);
/*
	X600_Read_Page_2n() read data from spare data area,size<=16 bytes) 
*/
int X600_Read_Page_nByte(unsigned block,unsigned page,unsigned StartAddr,void far *buf,unsigned size);
int X600_Write_Page_nByte(unsigned block,unsigned page,unsigned StartAddr,void far *buf,unsigned size);

/*
  block: 0~(MaxBlock-1)
  page: 0~(PageNoPerBlock-1)
*/

void X600_MarkInvalidBlock(unsigned block);
/*
	X600_MarkInvalidBlock() will write 0 to the block at page 0 & 1 
	address 517(the spare area address 5) .
	
	When the flash is erase/write many times, it may can not be erase anymore,
	the program must call  X600_MarkInvalidBlock() to set the block to invalid.
	and don't use that block anymore.
	
	X600_Init() will check all blocks and create a invalid block table. 
	(version after [2005/05/20] will support this function.) 
*/
int X600_IsBlockValid(unsigned block);
//extern unsigned char X600_InvalidTable[1024];
extern unsigned char *X600_InvalidTable;
extern int X600_InvalidNo;

unsigned X600_GetFirstBlock(void);
/*
  return the first valid block number. 
*/

/*=================================================================*/
/* 
  for X600 File System
*/
#define MAX_FILE_HANDLE		10

typedef struct {
	int F_OpenMode; 
	FILE_DATA F_fdata;
	unsigned F_StartBlock;
	unsigned F_StartPage;
	long F_CurPtr;
	unsigned F_CurBlock;
	unsigned F_CurPage;
	unsigned F_CurPageIdx;
	char *F_tmpbuf; 
} X600Fs_DATA;

extern X600Fs_DATA X600Fs_Fdata[MAX_FILE_HANDLE];

int X600Fs_Init(void);
int X600Fs_Stop(void);
int X600Fs_GetFileNo(unsigned long *freesize);
int X600Fs_OpenFile(char *fname, char *mode);
int X600Fs_CloseFile(int fh);
long X600Fs_GetFileLength(int fh);
unsigned X600Fs_ReadFile(int fh,char *buf,unsigned size);
int X600Fs_WriteFile(int fh,void *data,int no);

int X600Fs_GetFileFirst(char *fname,FILE_DATA *fdata);
int X600Fs_GetFileNext(FILE_DATA *fdata);

int X600Fs_DeleteAllFile(void);
int X600Fs_DeleteFile(char *fname);

#ifdef __cplusplus
 }
#endif
