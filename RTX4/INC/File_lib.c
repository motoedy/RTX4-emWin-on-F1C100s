/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    FILE_LIB.C
 *      Purpose: FlashFS System Configuration
 *      Rev.:    V4.74
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2014 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#pragma thumb
#pragma O3

/*----------------------------------------------------------------------------
 *      Library for File_Config.c
 *---------------------------------------------------------------------------*/

#ifdef __MICROLIB
 #error FlashFS does not work with MicroLIB
#endif

/* If MC1 drive not defined */
#ifndef MC1_EN
 #define MC1_EN   0
 #define MC1_DEF  0
#endif

/* If NAND1 drive not defined */
#ifndef NAND1_EN
 #define NAND1_EN  0
 #define NAND1_DEF 0
#endif

/* If USB1 drive not defined */
#ifndef USB1_EN
 #define USB1_EN  0
 #define USB1_DEF 0
#endif

/* Check number of enabled drives */
#define FS_NDRV (FL0_EN   +                 \
                 SF0_EN   +                 \
                 RAM0_EN  +                 \
                 MC0_EN   + MC1_EN  +       \
                 USB0_EN  + USB1_EN +       \
                 NAND0_EN + NAND1_EN)
#if (FS_NDRV == 0)
 #error all drives disabled in configuration
#endif

/* Count number of enabled FAT drives */
#define FS_FAT_NDRV (MC0_EN + MC1_EN + USB0_EN + USB1_EN + NAND0_EN + NAND1_EN)

/* Check Default drive settings */
#define __DEF  ((FL0_DEF   & FL0_EN)   +                          \
                (SF0_DEF   & SF0_EN)   +                          \
                (RAM0_DEF  & RAM0_EN)  +                          \
                (MC0_DEF   & MC0_EN)   + (MC1_DEF   & MC1_EN)  +  \
                (USB0_DEF  & USB0_EN)  + (USB1_DEF  & USB1_EN) +  \
                (NAND0_DEF & NAND0_EN) + (NAND1_DEF & NAND1_EN))
#if (__DEF == 0)
 #error default drive not specified
#elif (__DEF > 1)
 #error multiple default drives enabled
#endif

/* Check FS usage */
#define EFS_USE (FL0_EN + SF0_EN + RAM0_EN)
#define FAT_USE (MC0_EN + MC1_EN + USB0_EN + USB1_EN + NAND0_EN + NAND1_EN)

/* Check journal usage */
#define FSJ_USE (MC0_EN   * MC0_FSJ   + MC1_EN   * MC1_FSJ  +  \
                 USB0_EN  * USB0_FSJ  + USB1_EN  * USB1_FSJ +  \
                 NAND0_EN * NAND0_FSJ + NAND1_EN * NAND1_FSJ)

/* Reserved sector count */
#define FAT_SECT_RSVD 64

/* Make sure to resolve a weak stdlib symbol. */
#ifdef __RTX
 extern volatile int _stream_list_lock;
 #define STREAM_LIST_LOCK   _stream_list_lock;
#else
 #define STREAM_LIST_LOCK
#endif

/* Memory resources allocated by the Flash File System */
struct iob _iob[N_FILES];

/* Exported Defines to other modules */
U16 const _NFILE   = N_FILES;
U16 const _NDRV    = FS_NDRV;


/*----------------------------------------------------------------------------
 *  FAT Name Cache definition and configuration
 *---------------------------------------------------------------------------*/
#if (FAT_NAME_CACNT == 0)
FAT_NCACHE_CFG fat_ncache = {
  0, 0, 0, NULL
};
#else
 #if (FAT_NAME_RELOC == 1)
  #define __AT_FAT_NAME  __attribute__ ((at(FAT_NAME_CADR)))
 #else
  #define __AT_FAT_NAME
 #endif

/* FAT name cache max folder depth */
#define FAT_NAME_MAX_DEPTH 8

/* FAT name cache control block size and count */
#define FAT_NAME_CB_SZ (16)
#define FAT_NAME_CB_POOL_SZ     (FAT_NAME_CB_SZ * FS_FAT_NDRV)

/* FAT name cache folder depth control block size */
#define FAT_NAME_DEPTH_CB_SZ (8)
#define FAT_NAME_DEPTH_POOL_SZ  (FAT_NAME_DEPTH_CB_SZ * FAT_NAME_MAX_DEPTH * FS_FAT_NDRV)

/* FAT name cache pool size  */
#define FAT_NAME_LINK_SZ  (48)
#define FAT_NAME_LINK_POOL_SZ   (FAT_NAME_LINK_SZ * FAT_NAME_CACNT)

/* FAT name cache buffer size in bytes */
#define FAT_NAME_CACHE_SZ (FAT_NAME_CB_POOL_SZ    + \
                           FAT_NAME_DEPTH_POOL_SZ + \
                           FAT_NAME_LINK_POOL_SZ)

U32 fat_name_pool[FAT_NAME_CACHE_SZ/4] __AT_FAT_NAME;

FAT_NCACHE_CFG fat_ncache = {
  FAT_NAME_MAX_DEPTH,
  FS_FAT_NDRV,
  FAT_NAME_CACHE_SZ,
  fat_name_pool
};
#endif /* FAT_NAME_CACNT */

/*----------------------------------------------------------------------------
 *  Parallel Flash device FL0 configuration
 *---------------------------------------------------------------------------*/
#if (FL0_EN)
 extern EFS_DRV fl0_drv;

  /* Parallel Flash Device Configuration Table */
 #define  DFB(sz,adr)   { (U32)adr+FL0_BADR, ((U32)(adr+sz)-4)+FL0_BADR }
 #include FL0_HFILE

 static EFS_CFG fl0_cfg [FL_NSECT] = { FLASH_DEVICE };
 static EFS_VI  fl0_vol;
#endif

/*----------------------------------------------------------------------------
 *  SPI Flash device SF0 configuration
 *---------------------------------------------------------------------------*/
#if (SF0_EN)
 extern EFS_DRV sf0_drv;

 #define DSB(size,badr)   { (U32)badr, ((U32)(badr+size)-4) }
 #include SF0_HFILE

 /* Flash Device Configuration Table */
 static EFS_CFG sf0_cfg [SF_NSECT] = { SPI_FLASH_DEVICE };
 static EFS_VI  sf0_vol;
#endif

/*----------------------------------------------------------------------------
 *  RAM device RAM0 configuration
 *---------------------------------------------------------------------------*/
#if (RAM0_EN)
 #if (RAM0_RELOC == 1)
  #define __AT_RAM0  __attribute__ ((at(RAM0_BADR)))
 #else
  #define __AT_RAM0
 #endif
 
 #if ((RAM0_SIZE / RAM0_NSECT) < 64)
  #error "RAM drive sector size is set to low. It must be equal or higher than 64 bytes"
 #endif

 /* RAM Device data buffer */
 static U32 ram0_buf[(RAM0_SIZE/4)] __AT_RAM0;
 static EFS_VI ram0_vol;

 #define RBS        (RAM0_SIZE/4/RAM0_NSECT)
 #define DRB(n)     {(U32)&ram0_buf[(RBS*n)], (U32)&ram0_buf[(RBS*n+RBS-1)]}

 #pragma diag_suppress 1296
 /* Ram Device Configuration Table */
 static EFS_CFG ram0_cfg [RAM0_NSECT] = {
  DRB(0),  DRB(1),  DRB(2),  DRB(3),  DRB(4),  DRB(5),  DRB(6),  DRB(7),
 #if RAM0_NSECT > 8
  DRB(8),  DRB(9),  DRB(10), DRB(11), DRB(12), DRB(13), DRB(14), DRB(15),
 #endif
 #if RAM0_NSECT > 16
  DRB(16), DRB(17), DRB(18), DRB(19), DRB(20), DRB(21), DRB(22), DRB(23),
  DRB(24), DRB(25), DRB(26), DRB(27), DRB(28), DRB(29), DRB(30), DRB(31),
 #endif
 #if RAM0_NSECT > 32
  DRB(32), DRB(33), DRB(34), DRB(35), DRB(36), DRB(37), DRB(38), DRB(39),
  DRB(40), DRB(41), DRB(42), DRB(43), DRB(44), DRB(45), DRB(46), DRB(47),
  DRB(48), DRB(49), DRB(50), DRB(51), DRB(52), DRB(53), DRB(54), DRB(55),
  DRB(56), DRB(57), DRB(58), DRB(59), DRB(60), DRB(61), DRB(62), DRB(63),
 #endif
 #if RAM0_NSECT > 64
  DRB(64), DRB(65), DRB(66), DRB(67), DRB(68), DRB(69), DRB(70), DRB(71),
  DRB(72), DRB(73), DRB(74), DRB(75), DRB(76), DRB(77), DRB(78), DRB(79),
  DRB(80), DRB(81), DRB(82), DRB(83), DRB(84), DRB(85), DRB(86), DRB(87),
  DRB(88), DRB(89), DRB(90), DRB(91), DRB(92), DRB(93), DRB(94), DRB(95),
  DRB(96), DRB(97), DRB(98), DRB(99), DRB(100),DRB(101),DRB(102),DRB(103),
  DRB(104),DRB(105),DRB(106),DRB(107),DRB(108),DRB(109),DRB(110),DRB(111),
  DRB(112),DRB(113),DRB(114),DRB(115),DRB(116),DRB(117),DRB(118),DRB(119),
  DRB(120),DRB(121),DRB(122),DRB(123),DRB(124),DRB(125),DRB(126),DRB(127),
 #endif
 };
 #pragma diag_default 1296
#endif

/*----------------------------------------------------------------------------
 *  Memory Card device MC0 configuration
 *---------------------------------------------------------------------------*/
#if (MC0_EN)
 #if (MC0_RELOC == 1)
  #define __AT_MC0  __attribute__ ((at(MC0_CADR)))
 #else
  #define __AT_MC0
 #endif

 #if (MC0_FSJ == 1)
  static FSJOUR mc0_fsj;
 #endif

 /* MC0 Cache Buffer for Data and FAT Caching */
 static U32 mc0_cache[(MC0_CASZ+1) * 256 + MC0_FSJ * 128] __AT_MC0;
 static FAT_VI mc0_vol;

 #if (MC0_SPI == 0)
  /* SD Native bus mode */
  extern MCI_DRV mci0_drv;
  static MCI_DEV mci0_dev;
  #define MC0_INIT(m)      mci_Init (m,&mci0_dev)
  #define MC0_UNINIT(m)    mci_UnInit (m,&mci0_dev)
  #define MC0_RDSEC(s,b,c) mci_ReadSector (s,b,c,&mci0_dev)
  #define MC0_WRSEC(s,b,c) mci_WriteSector (s,b,c,&mci0_dev)
  #define MC0_RDINFO(i)    mci_ReadInfo (i,&mci0_dev)
  #define MC0_DEVCTRL(c,p) mci_DevCtrl (c,p, &mci0_dev)
 #else
  /* SPI bus mode */
  extern SPI_DRV spi0_drv;
  static MMC_DEV mmc0_dev;
  #define MC0_INIT(m)      mmc_Init (m,&mmc0_dev)
  #define MC0_UNINIT(m)    mmc_UnInit (m,&mmc0_dev)
  #define MC0_RDSEC(s,b,c) mmc_ReadSector (s,b,c,&mmc0_dev)
  #define MC0_WRSEC(s,b,c) mmc_WriteSector (s,b,c,&mmc0_dev)
  #define MC0_RDINFO(i)    mmc_ReadInfo (i,&mmc0_dev)
  #define MC0_DEVCTRL(c,p) mmc_DevCtrl (c,p,&mmc0_dev)
 #endif

  /* MC0 wrapper functions */
  static BOOL mc0_Init (U32 mode) {
    return (MC0_INIT (mode));
  }
  static BOOL mc0_UnInit (U32 mode) {
    return (MC0_UNINIT (mode));
  }
  static BOOL mc0_RdSect (U32 sect, U8 *buf, U32 cnt) {
    return (MC0_RDSEC (sect, buf, cnt));
  }
  static BOOL mc0_WrSect (U32 sect, U8 *buf, U32 cnt) {
    return (MC0_WRSEC (sect, buf, cnt));
  }
  static BOOL mc0_RdInfo (Media_INFO *info) {
    return (MC0_RDINFO (info));
  }
  static U32 mc0_DevCtrl (U32 code, void *p) {
    return (MC0_DEVCTRL (code, p));
  }

  /* MC0 FAT device driver */
  FAT_DRV mc0_drv = {
    mc0_Init,
    mc0_UnInit,
    mc0_RdSect,
    mc0_WrSect,
    mc0_RdInfo,
    mc0_DevCtrl
  };
#endif

/*----------------------------------------------------------------------------
 *  Memory Card device MC1 configuration
 *---------------------------------------------------------------------------*/
#if (MC1_EN)
 #if (MC1_RELOC == 1)
  #define __AT_MC1  __attribute__ ((at(MC1_CADR)))
 #else
  #define __AT_MC1
 #endif

 #if (MC1_FSJ == 1)
  static FSJOUR mc1_fsj;
 #endif

 /* MC1 Cache Buffer for Data and FAT Caching */
 static U32 mc1_cache[(MC1_CASZ+1) * 256 + MC1_FSJ * 128] __AT_MC1;
 static FAT_VI mc1_vol;

 #if (MC1_SPI == 0)
  /* SD Native bus mode */
  extern MCI_DRV mci1_drv;
  static MCI_DEV mci1_dev;
  #define MC1_INIT(m)      mci_Init (m,&mci1_dev)
  #define MC1_UNINIT(m)    mci_UnInit (m,&mci1_dev)
  #define MC1_RDSEC(s,b,c) mci_ReadSector (s,b,c,&mci1_dev)
  #define MC1_WRSEC(s,b,c) mci_WriteSector (s,b,c,&mci1_dev)
  #define MC1_RDINFO(i)    mci_ReadInfo (i,&mci1_dev)
  #define MC1_DEVCTRL(c,p) mci_DevCtrl (c,p, &mci1_dev)
 #else
  /* SPI bus mode */
  extern SPI_DRV spi1_drv;
  static MMC_DEV mmc1_dev;
  #define MC1_INIT(m)      mmc_Init (m,&mmc1_dev)
  #define MC1_UNINIT(m)    mmc_UnInit (m,&mmc1_dev)
  #define MC1_RDSEC(s,b,c) mmc_ReadSector (s,b,c,&mmc1_dev)
  #define MC1_WRSEC(s,b,c) mmc_WriteSector (s,b,c,&mmc1_dev)
  #define MC1_RDINFO(i)    mmc_ReadInfo (i,&mmc1_dev)
  #define MC1_DEVCTRL(c,p) mmc_DevCtrl (c,p,&mmc1_dev)
 #endif

  /* MC1 wrapper functions */
  static BOOL mc1_Init (U32 mode) {
    return (MC1_INIT (mode));
  }
  static BOOL mc1_UnInit (U32 mode) {
    return (MC1_UNINIT (mode));
  }
  static BOOL mc1_RdSect (U32 sect, U8 *buf, U32 cnt) {
    return (MC1_RDSEC (sect, buf, cnt));
  }
  static BOOL mc1_WrSect (U32 sect, U8 *buf, U32 cnt) {
    return (MC1_WRSEC (sect, buf, cnt));
  }
  static BOOL mc1_RdInfo (Media_INFO *info) {
    return (MC1_RDINFO (info));
  }
  static U32 mc1_DevCtrl (U32 code, void *p) {
    return (MC1_DEVCTRL (code, p));
  }

  /* MC1 FAT device driver */
  FAT_DRV mc1_drv = {
    mc1_Init,
    mc1_UnInit,
    mc1_RdSect,
    mc1_WrSect,
    mc1_RdInfo,
    mc1_DevCtrl
  };
#endif

/*----------------------------------------------------------------------------
 *  USB Flash device USB0 configuration
 *---------------------------------------------------------------------------*/
#if (USB0_EN)
 extern FAT_DRV usb0_drv;

 #if (USB0_FSJ == 1)
  static FSJOUR usb0_fsj;
 #endif

 /* USB Cache Buffer for Data and FAT Caching */
 static U32 usb0_cache[(USB0_CASZ+1) * 256 + USB0_FSJ * 128];
 static FAT_VI usb0_vol;
#endif

/*----------------------------------------------------------------------------
 *  USB Flash device USB1 configuration
 *---------------------------------------------------------------------------*/
#if (USB1_EN)
 extern FAT_DRV usb1_drv;

 #if (USB1_FSJ == 1)
  static FSJOUR usb1_fsj;
 #endif

 /* USB Cache Buffer for Data and FAT Caching */
 static U32 usb1_cache[(USB1_CASZ+1) * 256 + USB1_FSJ * 128];
 static FAT_VI usb1_vol;
#endif

/*----------------------------------------------------------------------------
 *  NAND Flash device NAND0 configuration
 *---------------------------------------------------------------------------*/
#if (NAND0_EN)
 extern NAND_DRV nand0_drv;

 #if (NAND0_RELOC == 1)
  #define __AT_NAND0  __attribute__ ((at(NAND0_CADR)))
 #else
  #define __AT_NAND0
 #endif

 #if (NAND0_FSJ == 1)
  static FSJOUR nand0_fsj;
 #endif

 /* NAND Cache Buffer for FAT, Page and Block Caching */
 #define NAND0_CSZ   ((NAND0_CASZ+1) * 1024        + \
                      (NAND0_CAPG+2) * NAND0_PGSZ  + \
                      (NAND0_CABL+2) * NAND0_PGCNT)
 #define NAND0_FSJBUF (NAND0_FSJ     * 512)
 static U32 nand0_cache[NAND0_CSZ/4 + NAND0_FSJBUF/4] __AT_NAND0;

 static PAGE_CACHE nand0_capg[NAND0_CAPG+1];
 static BLOCK_CACHE nand0_cabl[NAND0_CABL+2];
 static U32 nand0_ttsn[NAND_TSN_SIZE(NAND0_BLCNT,NAND0_PGSZ)];
 static NAND_FTL_DEV nand0_dev;
 static FAT_VI nand0_vol;

 /* NAND Flash Configuration */
 static NAND_FTL_CFG nand0_cfg = {
   /* NAND Flash Geometry */
   { &nand0_dev.PgLay,
     NAND0_BLCNT,
     NAND0_PGCNT,
     NAND0_PGSZ,
     NAND_SEC_BLOCK (NAND0_PGCNT,NAND0_PGSZ),
     NAND_SEC_PAGE (NAND0_PGSZ),
     NAND_ADDR_CYCLES (NAND0_BLCNT,NAND0_PGCNT,NAND0_PGSZ),
     NAND0_SWECC,
     0
   },

   /* Block Translation Table Space */
   1,
   NAND_BTT_SIZE (NAND0_BLCNT),

   /* Data Space */
   NAND_BTT_SIZE (NAND0_BLCNT) + 1,
   NAND0_BLCNT - 1,

   /* Caching */
   NAND0_CABL + 2,
   NAND0_CAPG + 1,
   NAND_TSN_SIZE(NAND0_BLCNT,NAND0_PGSZ),

   /* Page buffer & Caches */
   (U8 *)&nand0_cache[(NAND0_CASZ+1)*256],
   &nand0_cabl[0],
   (U8 *)&nand0_cache[(NAND0_CASZ+1)*256+(NAND0_CAPG+2)*NAND0_PGSZ/4],
   &nand0_capg[0],
   (U8 *)&nand0_cache[(NAND0_CASZ+1)*256+NAND0_PGSZ/4],
   &nand0_ttsn[0]
 };

 /* NAND0 wrapper functions */
 static BOOL nand0_Init (U32 mode) {
   return (ftl_Init (mode, &nand0_dev));
 }
 static BOOL nand0_UnInit (U32 mode) {
   return (ftl_UnInit (mode, &nand0_dev));
 }
 static BOOL nand0_RdSect (U32 sect, U8 *buf, U32 cnt) {
   U32 retv = ftl_ReadSect (sect, buf, cnt, &nand0_dev);
   if (retv == RTV_NOERR) return (__TRUE);
   else                   return (__FALSE);
 }
 static BOOL nand0_WrSect (U32 sect, U8 *buf, U32 cnt) {
   U32 retv = ftl_WriteSect (sect, buf, cnt, &nand0_dev);
   if (retv == RTV_NOERR || retv == ERR_FTL_WARN) return (__TRUE);
   else                                           return (__FALSE);
 }
 static BOOL nand0_RdInfo (Media_INFO *info) {
   return (ftl_ReadInfo (info, &nand0_dev));
 }
 static U32 nand0_DevCtrl (U32 code, void *p) {
   if (code == DC_CHKMEDIA) return (M_NOCHKMEDIA);
   if (code == DC_FORMAT)   return (ftl_Format (&nand0_dev, p));
   return (0);
 }

 FAT_DRV nfat0_drv = {
   nand0_Init,
   nand0_UnInit,
   nand0_RdSect,
   nand0_WrSect,
   nand0_RdInfo,
   nand0_DevCtrl
 };
#endif

/*----------------------------------------------------------------------------
 *  NAND Flash device NAND1 configuration
 *---------------------------------------------------------------------------*/
#if (NAND1_EN)
 extern NAND_DRV nand1_drv;

 #if (NAND1_RELOC == 1)
  #define __AT_NAND1  __attribute__ ((at(NAND1_CADR)))
 #else
  #define __AT_NAND1
 #endif

 #if (NAND1_FSJ == 1)
  static FSJOUR nand1_fsj;
 #endif

 /* NAND Cache Buffer for FAT, Page and Block Caching */
 #define NAND1_CSZ   ((NAND1_CASZ+1) * 1024        + \
                      (NAND1_CAPG+2) * NAND1_PGSZ  + \
                      (NAND1_CABL+2) * NAND1_PGCNT)
 #define NAND1_FSJBUF (NAND1_FSJ     * 512)
 static U32 nand1_cache[NAND1_CSZ/4 + NAND1_FSJBUF/4] __AT_NAND1;

 static PAGE_CACHE nand1_capg[NAND1_CAPG+1];
 static BLOCK_CACHE nand1_cabl[NAND1_CABL+2];
 static U32 nand1_ttsn[NAND_TSN_SIZE(NAND1_BLCNT,NAND1_PGSZ)];
 static NAND_FTL_DEV nand1_dev;
 static FAT_VI nand1_vol;

 /* NAND Flash Configuration */
 static NAND_FTL_CFG nand1_cfg = {
   /* NAND Flash Geometry */
   { &nand1_dev.PgLay,
     NAND1_BLCNT,
     NAND1_PGCNT,
     NAND1_PGSZ,
     NAND_SEC_BLOCK (NAND1_PGCNT,NAND1_PGSZ),
     NAND_SEC_PAGE (NAND1_PGSZ),
     NAND_ADDR_CYCLES (NAND1_BLCNT,NAND1_PGCNT,NAND1_PGSZ),
     NAND1_SWECC,
     1
   },

   /* Block Translation Table Space */
   1,
   NAND_BTT_SIZE (NAND1_BLCNT),

   /* Data Space */
   NAND_BTT_SIZE (NAND1_BLCNT) + 1,
   NAND1_BLCNT - 1,

   /* Caching */
   NAND1_CABL + 2,
   NAND1_CAPG + 1,
   NAND_TSN_SIZE(NAND1_BLCNT,NAND1_PGSZ),

   /* Page buffer & Caches */
   (U8 *)&nand1_cache[(NAND1_CASZ+1)*256],
   &nand1_cabl[0],
   (U8 *)&nand1_cache[(NAND1_CASZ+1)*256+(NAND1_CAPG+2)*NAND1_PGSZ/4],
   &nand1_capg[0],
   (U8 *)&nand1_cache[(NAND1_CASZ+1)*256+NAND1_PGSZ/4],
   &nand1_ttsn[0]
 };

 /* NAND0 wrapper functions */
 static BOOL nand1_Init (U32 mode) {
   return (ftl_Init (mode, &nand1_dev));
 }
 static BOOL nand1_UnInit (U32 mode) {
   return (ftl_UnInit (mode, &nand1_dev));
 }
 static BOOL nand1_RdSect (U32 sect, U8 *buf, U32 cnt) {
   U32 retv = ftl_ReadSect (sect, buf, cnt, &nand1_dev);
   if (retv == RTV_NOERR) return (__TRUE);
   else                   return (__FALSE);
 }
 static BOOL nand1_WrSect (U32 sect, U8 *buf, U32 cnt) {
   U32 retv = ftl_WriteSect (sect, buf, cnt, &nand1_dev);
   if (retv == RTV_NOERR || retv == ERR_FTL_WARN) return (__TRUE);
   else                                           return (__FALSE);
 }
 static BOOL nand1_RdInfo (Media_INFO *info) {
   return (ftl_ReadInfo (info, &nand1_dev));
 }
 static U32 nand1_DevCtrl (U32 code, void *p) {
   if (code == DC_CHKMEDIA) return (M_NOCHKMEDIA);
   if (code == DC_FORMAT)   return (ftl_Format (&nand1_dev, p));
   return (0);
 }

 FAT_DRV nfat1_drv = {
   nand1_Init,
   nand1_UnInit,
   nand1_RdSect,
   nand1_WrSect,
   nand1_RdInfo,
   nand1_DevCtrl
 };
#endif

/*----------------------------------------------------------------------------
 *  An array of installed Device drivers
 *---------------------------------------------------------------------------*/
FS_DEV fs_DevPool[FS_NDRV] = {
#if (FL0_EN)
  { &fl0_vol,  "F0", FS_EMB | FL0_DEF  },
#endif
#if (SF0_EN)
  { &sf0_vol,  "S0", FS_EMB | SF0_DEF  },
#endif
#if (RAM0_EN)
  { &ram0_vol, "R0", FS_EMB | RAM0_DEF },
#endif
#if (MC0_EN)
  { &mc0_vol,  "M0", FS_FAT | MC0_DEF  | FS_IOC },
#endif
#if (MC1_EN)
  { &mc1_vol,  "M1", FS_FAT | MC1_DEF  | FS_IOC },
#endif
#if (USB0_EN)
  { &usb0_vol, "U0", FS_FAT | USB0_DEF },
#endif
#if (USB1_EN)
  { &usb1_vol, "U1", FS_FAT | USB1_DEF },
#endif
#if (NAND0_EN)
  { &nand0_vol,"N0", FS_FAT | NAND0_DEF| FS_IOC },
#endif
#if (NAND1_EN)
  { &nand1_vol,"N1", FS_FAT | NAND1_DEF| FS_IOC },
#endif
};

/*----------------------------------------------------------------------------
 *  Configure Flash File System
 *---------------------------------------------------------------------------*/
void fs_config (void) {

#if (FL0_EN)
  fl0_vol.BaseAdr = FL0_BADR;
  fl0_vol.InitVal = (U32)0x1010101 * FL0_INITV; 
  fl0_vol.NumSect = sizeof(fl0_cfg)/sizeof(EFS_CFG);
  fl0_vol.Size    = FL0_SIZE;
  fl0_vol.DevCfg  = fl0_cfg;
  fl0_vol.drv     = &fl0_drv;
  fl0_vol.CpuClk  = CPU_CLK;
#endif

#if (SF0_EN)
  sf0_vol.BaseAdr = 0;
  sf0_vol.InitVal = (U32)0x1010101 * SF0_INITV; 
  sf0_vol.NumSect = sizeof(sf0_cfg)/sizeof(EFS_CFG);
  sf0_vol.Size    = SF0_SIZE;
  sf0_vol.DevCfg  = sf0_cfg;
  sf0_vol.drv     = &sf0_drv;
  sf0_vol.CpuClk  = CPU_CLK;
#endif

#if (RAM0_EN)
  ram0_vol.BaseAdr= (U32)ram0_buf;
  ram0_vol.InitVal= 0;
  ram0_vol.NumSect= RAM0_NSECT;
  ram0_vol.Size   = RAM0_SIZE;
  ram0_vol.DevCfg = ram0_cfg;
  ram0_vol.drv    = NULL;
#endif

#if (MC0_EN)
  mc0_vol.CaSize  = MC0_CASZ * 2;
  mc0_vol.CaBuf   = mc0_cache;
  mc0_vol.drv     = &mc0_drv;
 #if (MC0_SPI == 0)
  /* Register MCI driver for SD-Native mode */
  mci0_dev.drv    = &mci0_drv;
  mci0_dev.ExtCsd = (U8 *)mc0_cache;
 #else
  /* Register SPI driver for SPI mode */
  mmc0_dev.spi    = &spi0_drv;
 #endif

 #if (MC0_FSJ)
  /* Register file system journal */
  mc0_fsj.buf     = (U8 *)&mc0_cache[(MC0_CASZ+1) * 256];
  mc0_vol.fsj     = &mc0_fsj;
  mc0_vol.RsvdS   = FAT_SECT_RSVD;
 #else
  mc0_vol.fsj     = NULL;
  mc0_vol.RsvdS   = 0;
 #endif

#endif  

#if (MC1_EN)
  mc1_vol.CaSize  = MC1_CASZ * 2;
  mc1_vol.CaBuf   = mc1_cache;
  mc1_vol.drv     = &mc1_drv;
 #if (MC1_SPI == 0)
  /* Register MCI driver for SD-Native mode */
  mci1_dev.drv    = &mci1_drv;
  mci1_dev.ExtCsd = (U8 *)mc1_cache;
 #else
  /* Register SPI driver for SPI mode */
  mmc1_dev.spi    = &spi1_drv;
 #endif

 #if (MC1_FSJ)
  /* Register file system journal */
  mc1_fsj.buf     = (U8 *)&mc1_cache[(MC1_CASZ+1) * 256];
  mc1_vol.fsj     = &mc1_fsj;
  mc1_vol.RsvdS   = FAT_SECT_RSVD;
 #else
  mc1_vol.fsj     = NULL;
  mc1_vol.RsvdS   = 0;
 #endif
#endif  

#if (USB0_EN)
  usb0_vol.CaSize = USB0_CASZ * 2;
  usb0_vol.CaBuf  = usb0_cache;
  usb0_vol.drv    = &usb0_drv;
 
 #if (USB0_FSJ)
  /* Register file system journal */
  usb0_fsj.buf     = (U8 *)&usb0_cache[(USB0_CASZ+1) * 256];
  usb0_vol.fsj     = &usb0_fsj;
  usb0_vol.RsvdS   = FAT_SECT_RSVD;
 #else
  usb0_vol.fsj     = NULL;
  usb0_vol.RsvdS   = 0;
 #endif
#endif  

#if (USB1_EN)
  usb1_vol.CaSize = USB1_CASZ * 2;
  usb1_vol.CaBuf  = usb1_cache;
  usb1_vol.drv    = &usb1_drv;

 #if (USB1_FSJ)
  /* Register file system journal */
  usb1_fsj.buf     = (U8 *)&usb1_cache[(USB1_CASZ+1) * 256];
  usb1_vol.fsj     = &usb1_fsj;
  usb1_vol.RsvdS   = FAT_SECT_RSVD;
 #else
  usb1_vol.fsj     = NULL;
  usb1_vol.RsvdS   = 0;
 #endif
#endif  

#if (NAND0_EN)
  nand0_vol.CaSize= NAND0_CASZ * 2;
  nand0_vol.CaBuf = nand0_cache;
  nand0_vol.drv   = &nfat0_drv;
  nand0_dev.Cfg   = &nand0_cfg;
  nand0_dev.Drv   = &nand0_drv;

 #if (NAND0_FSJ)
  /* Register file system journal */
  nand0_fsj.buf   = (U8 *)&nand0_cache[NAND0_CSZ/4];
  nand0_vol.fsj   = &nand0_fsj;
  nand0_vol.RsvdS = FAT_SECT_RSVD;
 #else
  nand0_vol.fsj   = NULL;
  nand0_vol.RsvdS   = 0;
 #endif
#endif

#if (NAND1_EN)
  nand1_vol.CaSize= NAND1_CASZ * 2;
  nand1_vol.CaBuf = nand1_cache;
  nand1_vol.drv   = &nfat1_drv;
  nand1_dev.Cfg   = &nand1_cfg;
  nand1_dev.Drv   = &nand1_drv;

 #if (NAND1_FSJ)
  /* Register file system journal */
  nand1_fsj.buf   = (U8 *)&nand1_cache[NAND1_CSZ/4];
  nand1_vol.fsj   = &nand1_fsj;
  nand1_vol.RsvdS = FAT_SECT_RSVD;
 #else
  nand1_vol.fsj   = NULL;
  nand1_vol.RsvdS   = 0;
 #endif
#endif

  STREAM_LIST_LOCK;
}

#if (EFS_USE == 0)
 /* Empty functions to reduce image size when EFS not used */
 BOOL efs_rename (const char *filename, IOB *fcb) { return (__FALSE); }
 BOOL efs_create (const char *fname, IOB *fcb)    { return (__FALSE); }
 BOOL efs_delete (IOB *fcb)                       { return (__FALSE); }
 U32  efs_free (EFS_VI *vi)                       { return (0); }
 BOOL efs_format (EFS_VI *vi)                     { return (__FALSE); }
 BOOL efs_ffind (FINFO *info, IOB *fcb)           { return (__FALSE); }
 BOOL efs_wclose (IOB *fcb)                       { return (__FALSE); }
 U32  efs_read (IOB *fcb, U8 *buf, U32 len)       { return (0); }
 BOOL efs_write (IOB *fcb, const U8 *buf, U32 len){ return (__FALSE); }
 BOOL efs_ensure (IOB *fcb)                       { return (__FALSE); }
 BOOL efs_seek (IOB *fcb, U32 pos)                { return (__FALSE); }
 void efs_fsize (IOB *fcb)                        { ; }
 BOOL efs_find (const char *name, IOB *fcb)       { return (__FALSE); }
 int  efs_analyse (EFS_VI *vi)                    { return (0); }
 BOOL efs_check (EFS_VI *vi)                      { return (__FALSE); }
 BOOL efs_defrag (IOB *fcb)                       { return (__FALSE); }
#endif

#if (FAT_USE == 0)
 /* Empty functions to reduce image size when FAT not used */
 int fat_init (FAT_VI *vi)                              { return (-1); }
 int fat_open (IOB *fh, const char *path, int openmode) { return (-1); }
 int fat_close (IOB *fcb)                               { return (-1); }
 int fat_read (IOB *fcb, U8 *buf, U32 len)              { return (-1); }
 int fat_write (IOB *fcb, const U8 *buf, U32 len)       { return (-1); }
 int fat_seek (IOB *fcb, U32 pos)                       { return (-1); }
 int fat_delete (const char *fn, FAT_VI *vol)           { return (-1); }
 int fat_rename (const char *old, const char *newn, FAT_VI *vol) {
                                                          return (-1); }
 U64 fat_free (FAT_VI *vi)                              { return (0) ; }
 int fat_format (FAT_VI *vi, const char *opt)           { return (-1); }
 int fat_ffind  (const char *fn, FINFO *info, FAT_VI *vol) {
                                                          return (-1); }
 BOOL fat_jour_init (FAT_VI *vi)                  { return (__TRUE);  }
 BOOL fat_path_valid (const char *path, IOB *fcb) { return (__FALSE); }
#else
 #if (FSJ_USE == 0)
  /* Empty functions to reduce image size when journal not used */
  BOOL fat_jour_init (FAT_VI *vi)                  { return (__TRUE);  }
  BOOL fat_jour_prep (FAT_VI *vi)                  { return (__TRUE);  }
  BOOL fsj_set_space (FSJOUR *fsj, U32 start_sect, U32 cnt) {
                                                     return (__FALSE); }
  BOOL fsj_write     (FSJOUR *fsj, U32 sect, U32 cnt, U8 *buf) {
                                                     return (__FALSE); }
  BOOL fsj_init      (FSJOUR *fsj, FAT_DRV *drv)   { return (__FALSE); }
 #endif
#endif

#if (FAT_NAME_CACNT == 0)
 U32   elink_init      (void)                    { return (1);    }
 U32   elink_init_path (const char *drvLet)      { return (0xFF); }
 U32   elink_flush     (const char *drvLet)      { return (1);    }
 U32   elink_cmd       (U32 i, U32 j)            { return (0);    }
 U32   elink_insert    (void *p, U32 i, U32 j)   { return (1);    }
 U32   elink_delete    (void *p, U32 i)          { return (0);    }
 U32   elink_scan      (U32 i, void **pp, U32 j) { return (1);    }
#endif

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
