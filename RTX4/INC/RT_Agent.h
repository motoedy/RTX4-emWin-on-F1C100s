/******************************************************************************/
/* RT_Agent.H: Real-Time Agent API v2.6                                       */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2006-2007 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#ifndef _RT_Agent_H_
#define _RT_Agent_H_


/*==============================================================================
   Real-Time Agent Defines
==============================================================================*/

#define RTA_SHORT_MSG     0         // Replace length by this to send a short message

#define __RTA_DEC         __weak    // Declaration specifiers

/*==============================================================================
   Real-Time Agent Types 
==============================================================================*/

/* Real-Time Agent command definitions */
typedef enum __RTA_COMMANDS {  

  /* Commands 0x00-0xF are reserved for
     internal use only */
  
  /* Commands 0x10-0xEF are reserved for
     future applications */

  /* Reserved for user custom applications */
  RTA_CMD_CUSTOM_0      = 0x70,
  RTA_CMD_CUSTOM_1      = 0x71,
  RTA_CMD_CUSTOM_2      = 0x72,
  RTA_CMD_CUSTOM_3      = 0x73,
  RTA_CMD_CUSTOM_4      = 0x74,
  RTA_CMD_CUSTOM_5      = 0x75,
  RTA_CMD_CUSTOM_6      = 0x76,
  RTA_CMD_CUSTOM_7      = 0x77,
  RTA_CMD_CUSTOM_8      = 0x78,
  RTA_CMD_CUSTOM_9      = 0x79,
  RTA_CMD_CUSTOM_10     = 0x7A,
  RTA_CMD_CUSTOM_11     = 0x7B,
  RTA_CMD_CUSTOM_12     = 0x7C,
  RTA_CMD_CUSTOM_13     = 0x7D,
  RTA_CMD_CUSTOM_14     = 0x7E,
  RTA_CMD_CUSTOM_15     = 0x7F,
} RTA_COMMANDS;

/* Real-Time Agent Status Errorcodes */
typedef enum __RTA_STATUS {
  RTA_OK                = 0,
  RTA_ERR_FAILED        = 1,
  RTA_ERR_DISABLED      = 2,
  RTA_ERR_NOT_INIT      = 3,
  RTA_ERR_BUF_OVERRUN   = 4,
  RTA_ERR_PARAM         = 5,
  RTA_ERR_TIMEOUT       = 6,
  RTA_ERR_FORMAT        = 7,
  RTA_ERR_CHECKSUM      = 8,
  RTA_ERR_UNKNOWN_CMD   = 9,
  RTA_ERR_LAST_CODE           
} RTA_STATUS;

/*  Real-Time Agent State Flags */
#define RTA_STATUS_INTIALIZED    0x00000001UL   /* Indicates RTA_Init() has been called */
#define RTA_STATUS_CONNECTED     0x00000002UL   /* Indicates the Real-Time Agent is connected to the uVision host */
#define RTA_STATUS_BUF_OVERRUN   0x00000004UL   /* Indicates the Real-Time Agent buffer is full and data is being discarded */


/*==============================================================================
   Real-Time Agent Exported Variables 
==============================================================================*/

extern void * __RTA_fpDbgIn;         /* Pointer (FILE *) to RTA Debug In stream  */
extern void * __RTA_fpDbgOut;        /* Pointer (FILE *) to RTA Debug Out stream */


/*==============================================================================
   Real-Time Agent User Functions 
==============================================================================*/

/*
 *  Initialise Real-Time Agent
 *    Parameters:     None
 *    Return Value:   0 = OK, 1 = Error
 */
extern RTA_STATUS RTA_Init (void);

/*
 *  Get the Real-Time Agent status
 *    Parameters:     None
 *    Return Value:  Real-Time Agent status flags
 */
__RTA_DEC extern unsigned int RTA_Status (void);

/*
 *  Send a message to the host via Real-Time Agent
 *    Parameters:     type:   type code of command/message
 *                    data:   pointer to message buffer to send
 *                    length: length of data message buffer in words,
 *                            or 0 for a short message
 *    Return Value:   0 = OK, 1 = Error
 */               
__RTA_DEC extern RTA_STATUS RTA_Msg (unsigned int type, unsigned int *data, unsigned int length);

/*
 *  Check is there is a character available in the terminal emulator buffer
 *    Parameters:      None
 *    Return Value:    0 = No character, 1 = Character available
 */
__RTA_DEC extern unsigned int RTA_TermIsKey (void);

/*
 *  Get a character from the RTA terminal. This function blocks until a
 *  character is available
 *    Parameters:      None
 *    Return Value:    Character read from terminal
 */
__RTA_DEC extern unsigned int RTA_TermGetChar (void);

/*
 *  Write a character to the RTA terminal. This function blocks until buffer 
 *  space is available if 'Block on Write' is enabled in th e Configuation 
 *  Wizard, otherwise it discards the character.
 *    Parameters:      ch:    Character to write to the terminal
 *    Return Value:    Character wrParitten to the terminal
 */
__RTA_DEC extern unsigned int RTA_TermPutChar (unsigned int ch);


/*
 *  For Polled Mode:
 *   The first two functions need to be called priodically from 
 *   a timer interrupt to send and receive Real-Time Agent data
 *   The second two functions needs to allow this timer interrupt
 *   to be enabled and disabled within the Real-Time Agent
 *
 *  For Atmel AT91SAM7Sx:
 *   These functions need to be called from the system interrupt when a
 *   COMMTX or COMMRX interrupt is received
 *
 *  For all other configurations:
 *   Not required
 *  
 */
extern void RTA_rx_word_ext (void);
extern void RTA_tx_word_ext (void);
extern void RTA_irq_enable_ext (void);
extern void RTA_irq_disable_ext (void);


/*==============================================================================
   Real-Time Agent User MACROs
==============================================================================*/            

/*
 * To use these macros in code, the following conditions must be satisfied:
 *   - Option 'Real-Time Agent --> Enable Real-Time Agent Terminal' = ENABLED
 *   - Option 'I/O Retargetting' = ENABLED
 *   - '#include <stdio.h>' must be included in the source file
 *
 * If the above conditions are not satisfied, and the RTA_printf, etc macros are
 * included, the code will hit the DAbt_Handler or SWI_Handler. To avoid this one 
 * of the following MACROs can be added to the compile line. This can be useful 
 * for disabling the Real-Time Agent in a release build:
 *
 *  #define RTA_TERM_MACROS_DISABLE
 *
 * If the return values of the MACROs are checked, use the following instead. The
 * macros will then all return 0
 * 
 *  #define RTA_TERM_MACROS_DISABLE_RETURNS
 *
 * NOTE: if using MicroLIB, both printf and RTA_printf redirects to the location  
 *       specified by STDIO routing in retarget.c
 */


#if defined(RTA_TERM_MACROS_DISABLE)

#define RTA_printf(...)   
#define RTA_fflush_out()  
#define RTA_scanf(...)    
#define RTA_fflush_in()   


#elif defined(RTA_TERM_MACROS_DISABLE_RETURNS)

#define RTA_printf(...)   0
#define RTA_fflush_out()  0
#define RTA_scanf(...)    0
#define RTA_fflush_in()   0

#else /* RTA_TERM_MACROS_DISABLE & RTA_TERM_MACROS_DISABLE_RETURNS not defined */

/*
 *  Print a formatted string to the RTA terminal
 *    Parameters:      as printf()
 *    Return Value:    as printf()
 */
#define RTA_printf(...)   fprintf((FILE *)__RTA_fpDbgOut, __VA_ARGS__)

/*
 *  Flush the RTA terminal output buffer
 *    Parameters:      as fflush()
 *    Return Value:    as fflush()
 */
#define RTA_fflush_out()  fflush((FILE *)__RTA_fpDbgOut)

/*
 *  Scan a formatted string from the RTA terminal
 *    Parameters:      as scanf()
 *    Return Value:    as scanf()
 */
#define RTA_scanf(...)    fscanf((FILE *)__RTA_fpDbgIn, __VA_ARGS__)

/*
 *  Flush the RTA terminal input buffer
 *    Parameters:      as fflush()
 *    Return Value:    as fflush()
 */
#define RTA_fflush_in()   fflush((FILE *)__RTA_fpDbgIn)

#endif /* RTA_TERM_MACROS_DISABLE & RTA_TERM_MACROS_DISABLE_RETURNS not defined */



#endif /* _RT_Agent_H_ */     



