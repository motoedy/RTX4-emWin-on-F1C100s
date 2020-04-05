/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    FILE_CONFIG.H 
 *      Purpose: Common Definitions
 *      Rev.:    V4.74
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2014 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#ifndef __FILE_CONFIG_H__
#define __FILE_CONFIG_H__

#include <RTL.h>

/* FAT File Attributes */
#define ATTR_READ_ONLY   0x01
#define ATTR_HIDDEN      0x02
#define ATTR_SYSTEM      0x04
#define ATTR_VOLUME_ID   0x08
#define ATTR_DIRECTORY   0x10
#define ATTR_ARCHIVE     0x20

/* Device System Type attribute */
#define FS_DEF           0x01           /* Default Drive attribute           */
#define FS_EMB           0x02           /* Embedded File System (F:,S:,R:)   */
#define FS_FAT           0x04           /* FAT File System      (M:,N:,U:)   */
#define FS_IOC           0x08           /* IO Control allowed   (M:,N:)      */

/* SD/MMC Commands */
#define GO_IDLE_STATE    0              /* Reset card to idle state   MMC,SD */
#define SEND_OP_COND     1              /* Send Op.Cond. Register     MMC,-- */
#define ALL_SEND_CID     2              /* Send Card CID number       MMC,SD */
#define SET_REL_ADDR     3              /* Set Relative Address       MMC,SD */
#define SET_BUS_WIDTH    6              /* Set Bus Width 1bit/4bits   ---,SD */
#define MMC_SWITCH       6              /* Switch command             MMC,-- */
#define SELECT_CARD      7              /* Select/Deselect the Card   MMC,SD */
#define SEND_IF_COND     8              /* Send Interface Condition   ---,SD */
#define SEND_EXT_CSD     8              /* Send Extended CSD register MMC,-- */
#define SEND_CSD         9              /* Send Card Specific Data    MMC,SD */
#define SEND_CID         10             /* Send Card Identificat.Data MMC,SD */
#define STOP_TRANS       12             /* Stop Read or Write transm. MMC,SD */
#define SEND_STATUS      13             /* Send Status Resiger        MMC,SD */
#define SET_BLOCK_LEN    16             /* Set Block Length in bytes  MMC,SD */
#define READ_BLOCK       17             /* Read a Single Block        MMC,SD */
#define READ_MULT_BLOCK  18             /* Read Multiple Blocks       MMC,SD */
#define WRITE_BLOCK      24             /* Write a Block              MMC,SD */
#define WRITE_MULT_BLOCK 25             /* Write Multiple Blocks      MMC,SD */
#define SEND_APP_OP_COND 41             /* Send App. Op.Cond Register ---,SD */
#define APP_CMD          55             /* App.Specific Cmd follows   MMC,SD */
#define READ_OCR         58             /* Read OCR Register          MMC,SD */
#define CRC_ON_OFF       59             /* Turn CRC7 checking on/off  MMC,SD */

/* Driver DeviceCtrl Parameter Codes */
#define DC_CHKMEDIA      0              /* Check Media Status                */
#define DC_FORMAT        1              /* Format Media at low level (NAND)  */
#define DC_SERIAL        2              /* Return device serial number       */
#define DC_GETCID        3              /* Read Memory Card CID Register     */

/* Driver CheckMedia Return bit Definitions */
#define M_INSERTED       0x01           /* Media Detected                    */
#define M_PROTECTED      0x02           /* Media Write Protected             */
#define M_NOCHKMEDIA     0x80           /* Check Media Not implemented       */

/* Driver Init/Uninit Parameter Codes */
#define DM_IO            0              /* Init/Uninit IO peripherals        */
#define DM_MEDIA         1              /* Init/Uninit storage Media         */

/* MCI Expected Command Response Codes */
#define RESP_NONE        0              /* No Response                       */
#define RESP_SHORT       1              /* Short Response (4-bytes)          */
#define RESP_LONG        2              /* Long Response  (16-bytes)         */

/* MCI Bus Modes */
#define BUS_OPEN_DRAIN   0              /* Open Drain Card Bus mode          */
#define BUS_PUSH_PULL    1              /* Push-Pull Card Bus mode           */

/* Driver SetDma Parameter Codes */
#define DMA_READ         0              /* SetDma for block read             */ 
#define DMA_WRITE        1              /* SetDma for block write            */

/* NAND Configuration macros */
#define NAND_ADDR_CYCLES(b,p,s) (((b*p>65536) ? 3:2)+((s>528) ? 2:1))
#define NAND_BTT_SIZE(b)      ((b*3)/100)
#define NAND_SEC_PAGE(p)      (p/512)
#define NAND_SEC_BLOCK(b,p)   ((p/512)*b)
#define _DS_(b)               (b-(b*3)/100-1)
#define _EP_(p)               ((p/512)*128)
#define NAND_TSN_SIZE(b,p)    ((_DS_(b)+_EP_(p)-1)/_EP_(p))

/* NAND Flash Commands (ONFI V1.0 and V2.2 mandatory) */
#define NAND_CMD_READ1ST      0x00      /* Read 1st Cycle                    */
#define NAND_CMD_CHRDCOL1ST   0x05      /* Change Read Column 1st Cycle      */
#define NAND_CMD_PROG2ND      0x10      /* Page Program 2nd Cycle            */
#define NAND_CMD_READ2ND      0x30      /* Read 2nd Cycle                    */
#define NAND_CMD_ERASE1ST     0x60      /* Block erase 1st Cycle             */
#define NAND_CMD_STATUS       0x70      /* Read Status                       */
#define NAND_CMD_PROG1ST      0x80      /* Page Program 1st Cycle            */
#define NAND_CMD_CHWRCOL      0x85      /* Change Write Column               */
#define NAND_CMD_READID       0x90      /* Read ID                           */
#define NAND_CMD_ERASE2ND     0xD0      /* Block erase 2nd cycle             */
#define NAND_CMD_CHRDCOL2ND   0xE0      /* Change Read Column 2nd Cycle      */
#define NAND_CMD_PARAMPAGE    0xEC      /* Read Parameter Page               */
#define NAND_CMD_RESET        0xFF      /* Reset command                     */

/* NAND Status Flags Masks (ONFI V1.0 and V2.2) */
#define NAND_STAT_FAIL        0x01      /* Last command failed               */
#define NAND_STAT_FAILC       0x02      /* Command prior last failed         */
#define NAND_STAT_CSP         0x08      /* Command specific                  */
#define NAND_STAT_VSP         0x10      /* Vendor specific                   */
#define NAND_STAT_ARDY        0x20      /* Array operation in progress       */
#define NAND_STAT_RDY         0x40      /* LUN ready for another command     */
#define NAND_STAT_WP          0x80      /* Write protected                   */

/* NFTL ECC Status Codes */
#define ECC_NOERR       0               /* ECC passed, no errors detected    */
#define ECC_CORRECTED   1               /* ECC failed, data was corrected    */
#define ECC_UNCORRECTED 2               /* ECC failed, data was not corrected*/

#ifdef __cplusplus
extern "C"  {
#endif

/* NFTL Error Codes */
typedef enum {
  RTV_NOERR = 0,                        /* No error                          */
  ERR_INVALID_PARAM,                    /* Invalid parameter                 */

  ERR_FTL_ECC,                          /* ECC error on last page read       */
  ERR_FTL_WARN,                         /* Warning, data not reliable        */
  ERR_FTL_BB,                           /* Bad block                         */
  ERR_FTL_LBN,                          /* Wrong LBN                         */
  ERR_FTL_ALLOC,                        /* Block allocation error            */
  ERR_FTL_RANGE,                        /* BN to high                        */
  ERR_FTL_UNFOR,                        /* NAND is unformatted               */
  ERR_FTL_NOT_FOUND,                    /* Item not found                    */
  ERR_FTL_NOT_FOUND_EOB,                /* LSN not found, end of block       */
  ERR_FTL_FATAL,                        /* Fatal, undefined error            */

  ERR_ECC_COR,                          /* ECC corrected the data            */
  ERR_ECC_UNCOR,                        /* ECC could not correct the data    */

  ERR_NAND_PROG,                        /* Programming failed (flash status) */
  ERR_NAND_ERASE,                       /* Erase verify failed (flash status)*/
  ERR_NAND_HW_TOUT,                     /* NAND hardware timeout             */
  ERR_NAND_DMA_TOUT,                    /* DMA timeout                       */
  ERR_NAND_UNSUPPORTED,                 /* Functionality not supported       */
} NAND_FTL_ERROR;

#pragma push
#pragma anon_unions

/* File Control Block structure */
typedef struct iob {
  void *vol;                            /* Volume Information [FAT,EF]       */
  U16  flags;                           /* File status flags                 */
  U32  fsize;                           /* File Size                         */
  U32  fcsz;                            /* Current File Size                 */
  U32  fpos;                            /* FAT File Position Indicator       */
  
  union {
    struct {                            /* Embedded Flash variables          */
      U16 fileID;                       /* File Identification Number        */
      U16 fblock;                       /* Current Flash Block index         */
      U16 fidx;                         /* Current File Block index          */
      U16 nfidx;                        /* Next File Block index             */
      U32 fbot;                         /* Flash Block free space bottom     */
      U32 ftop;                         /* Flash Block free space top        */
    } efs;
    struct {                            /* FAT variables                     */
      U32 first_clus;                   /* First data cluster                */
      U32 current_clus;                 /* Current data cluster              */
      U32 short_ent_clus;               /* SFN: Short entry cluster number   */
      U16 short_ent_offs;               /* SFN: Short entry sector offset    */
      U8  current_sect;                 /* Current data sector               */
      U8  rsvd;                         /* Reserved value */
    } fat;
  };
} IOB;
#pragma pop

/* Flash Device Configuration */
typedef struct DevConf {
  U32 bStart;                           /* Flash Block Start address         */
  U32 bEnd;                             /* Flash Block End address (bsize-4) */
} const EFS_CFG;

/* FAT Volume Information struct */
typedef struct fatinfo {
  U32 BootRecSec;                       /* Boot Record Sector Offset         */
  U8  FatType;                          /* File System Fat type.             */
  U8  NumOfFat;                         /* Number of Fat tables              */
  U8  SecPerClus;                       /* Number of Sectors per Cluster     */
  U16 FatSize;                          /* Number of Sectors per FAT table   */
  U16 RsvdSecCnt;                       /* Number of Reserved Sectors        */
  U32 DskSize;                          /* Disk Size Sector Count            */
  U32 DataSecCnt;                       /* Number of Data Sectors            */
  U16 RootEntCnt;                       /* Maximum Root Directory entries    */
  U16 BytesPerSec;                      /* Sector Size in bytes              */
  U32 DataClusCnt;                      /* Data Cluster Count                */
  U16 RootDirAddr;                      /* Root Dir First Sector             */
  U16 RootSecCnt;                       /* Number of Sectors for Root dir    */
  U32 ClusSize;                         /* Cluster Size in bytes             */
  U16 EntsPerClus;                      /* Number of entries per cluster     */
                                        /* Added fields for FAT32            */
  U16 FAT32_ExtFlags;                   /* FAT extended flags                */
  U32 FAT32_RootClus;                   /* Root directory first cluster      */
  U16 FAT32_FSInfo;                     /* FSInfo structure sector number    */
  U16 FAT32_BkBootSec;                  /* Boot Record copy sector number    */
} FATINFO;

/* FAT Sector Caching structure */
typedef struct fcache {
  U32 sect;                             /* Cached FAT sector number          */
  U8  *buf;                             /* FAT sector cache buffer           */
  BIT dirty;                            /* FAT table content modified        */
  U8  cfat;                             /* Current FAT                       */
} FCACHE;

/* Data Sector Caching structure */
typedef struct dcache {
  U32 sect;                             /* Working Data sector number        */
  U32 csect;                            /* Data Cache starting sector number */
  U8  *buf;                             /* Working Data sector buffer        */
  U8  *cbuf;                            /* Data Cache sector buffer          */
  U8  nwr;                              /* Number of buffered write sectors  */
  U8  nrd;                              /* Number of cached read sectors     */
} DCACHE;

/* NFTL Block Index Cache */
typedef struct {
  U16 pbn;                              /* Physical block number             */
  U16 lbn;                              /* Logical block number              */
  U8  typ;                              /* Type of block in cache            */
  U8  nextPg;                           /* Next page to be indexed           */
  U8  *pgIdx;                           /* Index buffer                      */
} BLOCK_CACHE;

/* NFTL Page Cache */
typedef struct {
  U32 row;                              /* Row address of the page in cache  */
  U8 *buf;                              /* Cached page buffer                */
} PAGE_CACHE;

/* NAND FTL Buffer Pointers Structure */
typedef struct {
  U32 CacheBS;                          /* Block slot for block caching      */
  U32 CachePS;                          /* Page slot for page caching        */
  U16 CachedBlocks;                     /* Number of indexed blocks          */
  U16 CachedPages;                      /* Number of cached pages            */
  BLOCK_CACHE *Block;                   /* Block indexing cache info struct  */
  PAGE_CACHE  *Page;                    /* Page data cache info structure    */
} NAND_FTL_CACHE;

/* NAND Page Layout Configuration Structure */
typedef struct {
  U8  Pos_LSN;                          /* LSN position                      */
  U8  Pos_COR;                          /* Data in page corrupted marker     */
  U8  Pos_BBM;                          /* Bad Block marker position         */
  U8  Pos_ECC;                          /* First byte of ECC                 */
  U16 SectInc;                          /* Column increment till next sector */
  U16 SpareOfs;                         /* Spare area offset from begining   */
                                        /*  of the page                      */
  U16 SpareInc;                         /* Column increment till next spare  */  
} NAND_PG_LAY;

/* NAND Flash Geometry and Layout */
typedef struct _NAND_DRV_CFG {
  NAND_PG_LAY *PgLay;                   /* Page Layout Definitions           */
  U16 NumBlocks;                        /* Number of blocks per device       */
  U16 NumPages;                         /* Number of pages per block         */
  U16 PageSize;                         /* Page size                         */
  U16 SectorsPerBlock;                  /* Number of sectors per block       */
  U8  SectorsPerPage;                   /* Number of sectors per page        */
  U8  AddrCycles;                       /* Device address cycles             */
  U8  SwEccEn;                          /* Software ECC enabled              */
  U8  DrvInst;                          /* Driver Instance definition        */
} const NAND_DRV_CFG;

/* NAND Configuration structure */
typedef struct {
  /* NAND Flash Geometry */
  struct _NAND_DRV_CFG DrvCfg;
  /* Block Translation Table Space */
  U16 BttStartBn;                       /* First Physical Block              */
  U16 BttEndBn;                         /* Last Physical Block               */
  /* Data Space */
  U16 DataStartBn;                      /* First Physical Block              */
  U16 DataEndBn;                        /* Last Physical Block               */
  /* Buffer Sizes */  
  U16 NumCacheBlocks;                   /* Number of indexed data blocks     */
  U16 NumCachePages;                    /* Number of cached data pages       */
  U16 TsnTableSize;                     /* Translation table cache size      */ 
  /* Page buffer & Caches */
  U8  *PgBuf;                           /* Page data buffer                  */
  BLOCK_CACHE *BlockCache;              /* Block indexing cache info struct  */
  U8  *BlockCacheBuf;                   /* Block indexing cache buffer       */
  PAGE_CACHE  *PageCache;               /* Page data cache info structure    */
  U8  *PageCacheBuf;                    /* Page data cache buffer            */
  U32 *TsnTable;                        /* Translation table cache buffer    */
} const NAND_FTL_CFG;

/* Device driver mapping type */
typedef struct {
  void *dcb;                            /* Device Control block              */
  char id[3];                           /* Drive Name Identification         */
  U8   attr;                            /* File System attributes            */
} const FS_DEV;

/* Media information */
typedef struct {
  U32  block_cnt;                       /* Total number of blocks            */
  U16  read_blen;                       /* Read block length                 */
  U16  write_blen;                      /* Write block length                */
} Media_INFO;

/* Memory card CID register */
typedef struct {
  U32   PSN;                            /* Product serial number             */
  U16   OID;                            /* OEM/Application ID                */
  U8    MID;                            /* Manufacturer ID                   */
  U8    PRV;                            /* Product revision                  */
  U16   MDT;                            /* Manufacturing date                */
  U8    PNM[6];                         /* Product name                      */
} CID_REG;

/* FAT Name Cache Configuration */
typedef struct fat_nca_cfg {
  U8   MaxPathDepth;                    /* Maximum path depth                */
  U8   ControlBlockCnt;                 /* Number of control blocks          */
  U32  NameMemPoolSize;                 /* Name cache memory pool size       */
  U32 *NameMemPool;                     /* Name cache memory pool            */
} const FAT_NCACHE_CFG;

/* Embedded Flash Device driver */
typedef struct {
  BOOL (*Init)        (U32 adr, U32 clk);
  BOOL (*UnInit)      (void);
  BOOL (*ReadData)    (U32 adr, U32 sz, U8 *buf); /* Optional, =NULL for     */
                                        /* memory-mapped Parallel Flash      */
  BOOL (*ProgramPage) (U32 adr, U32 sz, U8 *buf);
  BOOL (*EraseSector) (U32 adr);
  BOOL (*EraseChip)   (void);           /* Optional, NULL if not existing    */
} const EFS_DRV;

/* NAND Flash Device driver */
typedef struct {
  U32  (*Init)        (NAND_DRV_CFG *cfg);
  U32  (*UnInit)      (NAND_DRV_CFG *cfg);
  U32  (*PageRead)    (U32 row, U8 *buf, NAND_DRV_CFG *cfg);
  U32  (*PageWrite)   (U32 row, U8 *buf, NAND_DRV_CFG *cfg);
  U32  (*BlockErase)  (U32 row, NAND_DRV_CFG *cfg);
} const NAND_DRV;

/* SPI Device driver */
typedef struct {
  BOOL (*Init)        (void);
  BOOL (*UnInit)      (void);
  U8   (*Send)        (U8 outb);
  BOOL (*SendBuf)     (U8 *buf, U32 sz);
  BOOL (*RecBuf)      (U8 *buf, U32 sz);
  BOOL (*BusSpeed)    (U32 kbaud);
  BOOL (*SetSS)       (U32 ss);
  U32  (*CheckMedia)  (void);           /* Optional, NULL if not existing    */
} const SPI_DRV;

/* MCI Device driver */
typedef struct {
  BOOL (*Init)        (void);
  BOOL (*UnInit)      (void);
  void (*Delay)       (U32 us);
  BOOL (*BusMode)     (U32 mode);
  BOOL (*BusWidth)    (U32 width);
  BOOL (*BusSpeed)    (U32 kbaud);
  BOOL (*Command)     (U8  cmd, U32 arg, U32 resp_type, U32 *rp);
  BOOL (*ReadBlock)   (U32 bl, U8 *buf, U32 cnt);
  BOOL (*WriteBlock)  (U32 bl, U8 *buf, U32 cnt);
  BOOL (*SetDma)      (U32 mode, U8 *buf, U32 cnt); /* Optional, =NULL for   */
                                        /* local DMA or non DMA drivers      */ 
  U32  (*CheckMedia)  (void);           /* Optional, NULL if not existing    */
} const MCI_DRV;

/* FAT Layer Device driver */
typedef struct {
  BOOL (*Init)        (U32 mode);
  BOOL (*UnInit)      (U32 mode);
  BOOL (*ReadSect)    (U32 sect, U8 *buf, U32 cnt);
  BOOL (*WriteSect)   (U32 sect, U8 *buf, U32 cnt);
  BOOL (*ReadInfo)    (Media_INFO *cfg);
  U32  (*DeviceCtrl)  (U32 code, void *p);
} const FAT_DRV;

/* MMC Device Control block */
typedef struct {
  U8    CardType;                       /* Memory Card Type                  */
  U8    TranSpeed;                      /* Maximum data transfer rate        */
  U8    CmdClass;                       /* Card command class                */
  U32   SerNum;                         /* Serial Number of Memory Card      */
  SPI_DRV *spi;                         /* Registered SPI driver             */
} MMC_DEV;

/* MCI Device Control block */
typedef struct {
  U8    CardType;                       /* Memory Card Type                  */
  U8    TranSpeed;                      /* Maximum data transfer rate        */
  U8    Property;                       /* Device properties                 */
  U16   CardRCA;                        /* Relative Card Address             */
  U32   SerNum;                         /* Serial Number of Memory Card      */
  U32   SectorCount;                    /* Total number of device sectors    */
  U8   *ExtCsd;                         /* Extended CSD buffer pointer       */
  MCI_DRV *drv;                         /* Registered MCI driver             */
} MCI_DEV;

/* NAND Device Control block */
typedef struct {
  NAND_PG_LAY PgLay;                    /* Page Layout definition            */
  U8  PageSectors;                      /* Number of sectors/page 1,4,8, ... */
  U8  SPP;                              /* Number of sectors/page            */
                                        /*  (2^n, n = SPP)                   */
  U8  PPB;                              /* Number of pages/block             */
                                        /*  (2^n, n = PPB)                   */
  U8  SPB;                              /* Number of sectors/block           */
                                        /*  (2^n, n = SPB)                   */
  U8  EPS;                              /* Entries/table sector              */
                                        /*  (2^n, n = EPS)                   */
  U8  LastECC;                          /* ECC status after last page read   */
  U16 LastDBN;                          /* Last allocated phy. block         */
                                        /*  in data area                     */
  U16 LastTBN;                          /* Last allocated phy. block         */
                                        /*  in table area                    */
  U32 CurrLBN;                          /* Current logical block number      */
  U32 GcLBN;                            /* Current logical block number used */
                                        /*  by forced GC                     */
  U16 PbnQ[3];                          /* Empty block queue                 */
  U16 BadBlockCnt;                      /* Bad Block Counter                 */
  U16 NumDataBlocks;                    /* Number of data blocks reported to */
                                        /*  file system                      */
  U16 Status;                           /* FTL Status Flags                  */
  U8  *PgBuf;                           /* Page data buffer                  */
  U32 *TsnTable;                        /* Translation table cache buffer    */
  U16 TsnTableSize;                     /* Translation table cache size      */
  NAND_FTL_CACHE Ca;                    /* FTL cache info structure          */
  NAND_FTL_CFG  *Cfg;                   /* FTL configuration structure       */
  NAND_DRV      *Drv;                   /* NAND device driver control block  */
} NAND_FTL_DEV;

/* File System Journal Control block */
typedef struct fsjour {
  FAT_DRV *drv;                         /* Memory device driver              */
  U8 *buf;                              /* Journal buffer                    */
  U32 FirstSect;                        /* First sector of journal           */
  U32 JournSect;                        /* Number of sectors available       */
  U32 TrId;                             /* Transaction identifier            */
  U32 TrSect;                           /* Transaction sector                */
  U8 State;                             /* Journal state                     */
  U8 Status;                            /* Journal status                    */
} FSJOUR;

/* FAT Volume Info Control block */
typedef struct {
  U32     Status;                       /* Volume Status                     */
  FAT_DRV *drv;                         /* Registered FAT Device Driver      */
  U32     *CaBuf;                       /* Cache Buffer (FAT + Data)         */
  U32     CaSize;                       /* Cache Buffer size                 */
  U16     RsvdS;                        /* Reserved sectors used by journal  */
  FSJOUR *fsj;                          /* File System Journal               */
  FATINFO cfg;                          /* FAT Volume configuration          */
  FCACHE  fat;                          /* FAT table cache control           */
  DCACHE  ca;                           /* Data cache control                */
  U32     free_clus_cnt;                /* FAT32: Number of free clusters    */
  U32     free_clus;                    /* FAT32: First free cluster         */
  U32     elink_id;                     /* Name cache drive id               */
} FAT_VI;

/* Embedded Flash Volume Info Control block */
typedef struct {
  U32     Status;                       /* Volume Status                     */
  EFS_DRV *drv;                         /* Registered Device Driver          */
  U32     BaseAdr;                      /* Device Base Address               */
  U32     InitVal;                      /* Value for empty flash             */
  U32     NumSect;                      /* Number of Flash Sectors           */
  U32     Size;                         /* Device Size                       */
  EFS_CFG *DevCfg;                      /* Device Configuration              */
  U32     CpuClk;                       /* Cpu Clock Frequency               */
  U16     TopID;                        /* Top used FileID                   */
} EFS_VI;

/* File_Lib.c module */
extern void  fs_config (void);

/* fs_sys.c module */
extern int  __sys_open (const char *fname, int openmode);
extern int  __sys_close (int handle);
extern int  __sys_read (int handle, U8 *buf, U32 len);
extern int  __sys_write (int handle, const U8 *buf, U32 len);
extern int  __sys_ensure (int handle);
extern int  __sys_seek (int handle, U32 pos);
extern long __sys_flen (int handle);

/* Macros for compatibility with old version of 'retarget.c' */
#define __fopen     __sys_open
#define __fclose    __sys_close
#define __read      __sys_read
#define __write     __sys_write
#define __setfpos   __sys_seek
#define __flushbuf  __sys_ensure
#define __get_flen  __sys_flen

/* FAT exported functions */
extern int  fat_init  (FAT_VI *vi);
extern int  fat_open  (IOB *fcb, const char *name, int openmode);
extern int  fat_close (IOB *fcb);
extern int  fat_read  (IOB *fcb, U8 *buf, U32 len);
extern int  fat_write (IOB *fcb, const U8 *buf, U32 len);
extern int  fat_seek  (IOB *fcb, U32 pos);
extern int  fat_delete(const char *fn, FAT_VI *vi);
extern int  fat_ffind (const char *fn, FINFO *info, FAT_VI *vi);
extern int  fat_rename(const char *path, const char *newname, FAT_VI *vi);
extern U64  fat_free  (FAT_VI *vi);
extern int  fat_format(FAT_VI *vi, const char *param);

extern BOOL fat_jour_init (FAT_VI *vi);
extern BOOL fat_jour_prep (FAT_VI *vi);

/* fs_journal.c module */
extern BOOL fsj_init      (FSJOUR *fsj, FAT_DRV *drv);
extern BOOL fsj_set_space (FSJOUR *fsj, U32 start_sect, U32 cnt);
extern BOOL fsj_write     (FSJOUR *fsj, U32 sect, U32 rsec, U8 *buf);

/* fs_ioc.c module */
extern FAT_VI *ioc_getcb (const char *drive);
extern int  ioc_init (FAT_VI *vi);
extern int  ioc_uninit (FAT_VI *vi);
extern BOOL ioc_read_info (Media_INFO *info, FAT_VI *vi);
extern BOOL ioc_read_sect (U32 sect, U8 *buf, U32 cnt, FAT_VI *vi);
extern BOOL ioc_write_sect (U32 sect, U8 *buf, U32 cnt, FAT_VI *vi);

/* fs_mmc.c module */
extern BOOL mmc_Init (U32 mode, MMC_DEV *mmc);
extern BOOL mmc_UnInit (U32 mode, MMC_DEV *mmc);
extern BOOL mmc_ReadSector (U32 sect, U8 *buf, U32 cnt, MMC_DEV *mmc);
extern BOOL mmc_WriteSector (U32 sect, U8 *buf, U32 cnt, MMC_DEV *mmc);
extern BOOL mmc_ReadInfo (Media_INFO *info, MMC_DEV *mmc);
extern U32  mmc_DevCtrl (U32 code, void *p, MMC_DEV *mmc);

/* fs_mci.c module */
extern BOOL mci_Init (U32 mode, MCI_DEV *mci);
extern BOOL mci_UnInit (U32 mode, MCI_DEV *mci);
extern BOOL mci_ReadSector (U32 sect, U8 *buf, U32 cnt, MCI_DEV *mci);
extern BOOL mci_WriteSector (U32 sect, U8 *buf, U32 cnt, MCI_DEV *mci);
extern BOOL mci_ReadInfo (Media_INFO *info, MCI_DEV *mci);
extern U32  mci_DevCtrl (U32 code, void *p, MCI_DEV *mci);

/* fs_nftl.c module */
extern BOOL ftl_Init (U32 mode, NAND_FTL_DEV *ftl);
extern BOOL ftl_UnInit (U32 mode, NAND_FTL_DEV *ftl);
extern U32  ftl_ReadSect (U32 lsn, U8 *buf, U32 cnt, NAND_FTL_DEV *ftl);
extern U32  ftl_WriteSect(U32 lsn, U8 *buf, U32 cnt, NAND_FTL_DEV *ftl);
extern BOOL ftl_Format (NAND_FTL_DEV *ftl, void *par);
extern BOOL ftl_ReadInfo (Media_INFO *info, NAND_FTL_DEV *ftl);

/* fs_efs.c module */
extern BOOL efs_rename (const char *filename, IOB *fcb);
extern BOOL efs_create (const char *fname, IOB *fcb);
extern BOOL efs_delete (IOB *fcb);
extern U32  efs_free (EFS_VI *vi);
extern BOOL efs_format (EFS_VI *vi);
extern BOOL efs_ffind (FINFO *info, IOB *fcb);
extern BOOL efs_wclose (IOB *fcb);
extern U32  efs_read (IOB *fcb, U8 *buf, U32 len);
extern BOOL efs_write (IOB *fcb, const U8 *buf, U32 len);
extern BOOL efs_ensure (IOB *fcb);
extern BOOL efs_seek (IOB *fcb, U32 pos);
extern void efs_fsize (IOB *fcb);
extern BOOL efs_find (const char *name, IOB *fcb);
extern int  efs_analyse (EFS_VI *vi);
extern BOOL efs_check (EFS_VI *vi);
extern BOOL efs_defrag (IOB *fcb);

/* fs_time.c module */
extern U32  fs_get_time (void);
extern U32  fs_get_date (void);

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

#endif

