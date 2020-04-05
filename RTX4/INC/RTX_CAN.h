/*----------------------------------------------------------------------------
 *      RL-ARM - CAN
 *----------------------------------------------------------------------------
 *      Name:    RTX_CAN.h
 *      Purpose: Header for CAN Generic Layer Driver
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2014 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>                      /* RTX kernel functions & defines      */
#include "CAN_Cfg.h"                  /* CAN Configuration                   */


#ifndef __RTX_CAN_H
#define __RTX_CAN_H

#ifdef __cplusplus
extern "C"  {
#endif

#if (USE_CAN_CTRL4)
  #define CAN_CTRL_MAX_NUM      4
#elif (USE_CAN_CTRL3)
  #define CAN_CTRL_MAX_NUM      3
#elif (USE_CAN_CTRL2)
  #define CAN_CTRL_MAX_NUM      2
#elif (USE_CAN_CTRL1)
  #define CAN_CTRL_MAX_NUM      1
#else
  #error "No CAN Controller defined"
#endif

/* Definition of memory pool type, for CAN messages                          */
#define CAN_msgpool_declare(name,cnt)     U32 name[cnt*(sizeof(CAN_msg)/4) + 3]

/* Definition of mailbox array, for used controllers                         */
#define mbx_arr_declare(name,arr_num,cnt) U32 name[arr_num][4 + cnt]

/* CAN message object structure                                              */
typedef struct {
  U32 id;                 /* 29 bit identifier                               */
  U8  data[8];            /* Data field                                      */
  U8  len;                /* Length of data field in bytes                   */
  U8  ch;                 /* Object channel                                  */
  U8  format;             /* 0 - STANDARD,   1 - EXTENDED IDENTIFIER         */
  U8  type;               /* 0 - DATA FRAME, 1 - REMOTE FRAME                */
} CAN_msg;

/* Externaly declared memory pool for CAN messages, both transmit and receive*/
extern U32 CAN_mpool[CAN_CTRL_MAX_NUM*(CAN_No_SendObjects+CAN_No_ReceiveObjects)*(sizeof(CAN_msg)/4) + 3];

/* Externaly declared mailbox, for CAN transmit messages                     */
extern U32 MBX_tx_ctrl[CAN_CTRL_MAX_NUM][4 + CAN_No_SendObjects];

/* Externaly declared mailbox, for CAN receive messages                      */
extern U32 MBX_rx_ctrl[CAN_CTRL_MAX_NUM][4 + CAN_No_ReceiveObjects];

/* Semaphores used for protecting writing to CAN hardware                    */
extern OS_SEM wr_sem[CAN_CTRL_MAX_NUM];

/* Symbolic names for formats of CAN message                                 */
typedef enum {STANDARD_FORMAT = 0, EXTENDED_FORMAT} CAN_FORMAT;

/* Symbolic names for type of CAN message                                    */
typedef enum {DATA_FRAME      = 0, REMOTE_FRAME   } CAN_FRAME;

/* Definitions for filter_type                                               */
#define FORMAT_TYPE     (1 << 0)
#define FRAME_TYPE      (1 << 1)
#define STANDARD_TYPE   (0 << 0)
#define EXTENDED_TYPE   (1 << 0)
#define DATA_TYPE       (0 << 1)
#define REMOTE_TYPE     (1 << 1)

/* Error values that functions can return                                    */
typedef enum   
{ CAN_OK = 0,                       /* No error                              */
  CAN_NOT_IMPLEMENTED_ERROR,        /* Function has not been implemented     */
  CAN_MEM_POOL_INIT_ERROR,          /* Memory pool initialization error      */
  CAN_BAUDRATE_ERROR,               /* Baudrate was not set                  */
  CAN_TX_BUSY_ERROR,                /* Transmitting hardware busy            */
  CAN_OBJECTS_FULL_ERROR,           /* No more rx or tx objects available    */
  CAN_ALLOC_MEM_ERROR,              /* Unable to allocate memory from pool   */
  CAN_DEALLOC_MEM_ERROR,            /* Unable to deallocate memory           */
  CAN_TIMEOUT_ERROR,                /* Timeout expired                       */
  CAN_UNEXIST_CTRL_ERROR,           /* Controller does not exist             */
  CAN_UNEXIST_CH_ERROR,             /* Channel does not exist                */
} CAN_ERROR;

/* Functions defined CAN hardware driver (module CAN_chip.c)                 */
extern CAN_ERROR CAN_hw_testmode   (U32 ctrl, U32 testmode);
extern CAN_ERROR CAN_hw_setup      (U32 ctrl);
extern CAN_ERROR CAN_hw_init       (U32 ctrl, U32 baudrate);
extern CAN_ERROR CAN_hw_start      (U32 ctrl);
extern CAN_ERROR CAN_hw_tx_empty   (U32 ctrl);
extern CAN_ERROR CAN_hw_wr         (U32 ctrl, CAN_msg *msg);
extern CAN_ERROR CAN_hw_set        (U32 ctrl, CAN_msg *msg);
extern CAN_ERROR CAN_hw_rx_object  (U32 ctrl, U32 ch, U32 id, U32 object_para);
extern CAN_ERROR CAN_hw_tx_object  (U32 ctrl, U32 ch,         U32 object_para);

#if (__TARGET_ARCH_6S_M || __TARGET_ARCH_7_M || __TARGET_ARCH_7E_M)
extern CAN_ERROR            __CAN_hw_setup (U32 p, U32 ctrl) __SVC_0;
#define _CAN_hw_setup(ctrl) __CAN_hw_setup((U32)&CAN_hw_setup, ctrl);
#else
#define _CAN_hw_setup         CAN_hw_setup
#endif

/* Functions defined in module RTX_CAN.c                                     */
CAN_ERROR CAN_init                 (U32 ctrl, U32 baudrate);
CAN_ERROR CAN_start                (U32 ctrl);
CAN_ERROR CAN_send                 (U32 ctrl, CAN_msg *msg, U16 timeout);
CAN_ERROR CAN_request              (U32 ctrl, CAN_msg *msg, U16 timeout);
CAN_ERROR CAN_set                  (U32 ctrl, CAN_msg *msg, U16 timeout);
CAN_ERROR CAN_receive              (U32 ctrl, CAN_msg *msg, U16 timeout);
CAN_ERROR CAN_rx_object            (U32 ctrl, U32 ch, U32 id, U32 object_para);
CAN_ERROR CAN_tx_object            (U32 ctrl, U32 ch,         U32 object_para);

#ifdef __cplusplus
}
#endif

#endif /* __RTX_CAN_H */

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
