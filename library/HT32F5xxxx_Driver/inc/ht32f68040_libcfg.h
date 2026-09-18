/*********************************************************************************************************//**
 * @file    ht32f68040_libcfg.h
 * @version $Rev:: 9912         $
 * @date    $Date:: 2026-07-09 #$
 * @brief   The library configuration file.
 *************************************************************************************************************
* @attention
*
* Firmware Disclaimer Information
*
* 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
*    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
*    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
*    other intellectual property laws.
*
* 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
*    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
*    other than HOLTEK and the customer.
*
* 3. The program technical documentation, including the code, is provided "as is" and for customer reference
*    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
*    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
*    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
*
* <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32F68040_LIBCFG_H
#define __HT32F68040_LIBCFG_H

/* Settings ------------------------------------------------------------------------------------------------*/

#if !defined(USE_MEM_HT32F68040)
#define USE_MEM_HT32F68040
#endif

#define LIBCFG_MAX_SPEED                  (64000000)

#define LIBCFG_FLASH_PAGESIZE             (1024)

#ifdef USE_MEM_HT32F68040
  #define LIBCFG_FLASH_SIZE               (LIBCFG_FLASH_PAGESIZE * 63)
  #define LIBCFG_RAM_SIZE                 (1024 * 8)
  #define LIBCFG_CHIPNAME                 (0x68040)
#endif


#define LIBCFG_BFTM1                      (1)
#define LIBCFG_CMP                        (1)
#define LIBCFG_CKCU_NO_LPCR               (1)
#define LIBCFG_CKCU_NO_INTERRUPT_REG      (1)
#define LIBCFG_CRC                        (1)
#define LIBCFG_DIV                        (1)
#define LIBCFG_GPIOC                      (1)
#define LIBCFG_GPIOD                      (1)
#define LIBCFG_GPIOF                      (1)
#define LIBCFG_GPTM_GIRQ                  (1)
#define LIBCFG_I2C_NOSTRETCH              (1)
#define LIBCFG_PDMA                       (1)
#define LIBCFG_PGA                        (1)
#define LIBCFG_PGA1                       (1)
#define LIBCFG_SCTM0                      (1)
#define LIBCFG_SCTM1                      (1)

#endif
