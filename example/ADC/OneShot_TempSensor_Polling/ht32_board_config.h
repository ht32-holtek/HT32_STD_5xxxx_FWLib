/*********************************************************************************************************//**
 * @file    ADC/OneShot_TempSensor_Polling/ht32_board_config.h
 * @version $Rev:: 9600         $
 * @date    $Date:: 2025-12-04 #$
 * @brief   The header file of board configuration.
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
#ifndef __HT32_BOARD_CONFIG_H
#define __HT32_BOARD_CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif


/* Settings ------------------------------------------------------------------------------------------------*/
#if defined(USE_HT32F66256_DVB)
  #define HTCFG_ADC_IPN                           ADC0
  #define HTCFG_CK_ADC_DIV                        8
  #define HTCFG_TSCLK_DIV                         8
#endif

#define HTCFG_ADC_PORT                            STRCAT2(HT_,                 HTCFG_ADC_IPN)
#define HTCFG_ADC_AFIO_MODE                       STRCAT2(AFIO_FUN_,           HTCFG_ADC_IPN)
#define HTCFG_ADC_CKCU_ADCPRE                     STRCAT2(CKCU_ADCPRE_,        HTCFG_ADC_IPN)
#define HTCFG_ADC_CKCU_ADCPRE_DIV                 STRCAT2(CKCU_ADCPRE_DIV,     HTCFG_CK_ADC_DIV)

#define HTCFG_ADC_ADVREFP_MV                      3300      /* Assume ADVREFP = 3.3V                   */

#ifdef __cplusplus
}
#endif

#endif
