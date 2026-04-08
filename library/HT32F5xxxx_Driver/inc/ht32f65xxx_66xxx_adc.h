/*********************************************************************************************************//**
 * @file    ht32f65xxx_66xxx_adc.h
 * @version $Rev:: 9671         $
 * @date    $Date:: 2026-03-04 #$
 * @brief   The header file of the ADC library.
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
#ifndef __HT32F65XXX_66XXX_ADC_H
#define __HT32F65XXX_66XXX_ADC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32.h"

/** @addtogroup HT32F5xxxx_Peripheral_Driver HT32F5xxxx Peripheral Driver
  * @{
  */

/** @addtogroup ADC
  * @{
  */

/* Exported types ------------------------------------------------------------------------------------------*/
/** @brief Definition of Temperature Sensor parameter Structure.
  * @{
  */
typedef struct
{
  u16  Advrefp_mV;               /* ADC reference voltage in millivolts */
  u16  TsData;                   /* Raw ADC conversion data of the temperature sensor. */
  s32  CalTempPoint_mC;          /* Calibration temperature point in milli-degree Celsius. */
} ADC_TempSensorParam_TypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------------------------------------*/
/** @defgroup ADC_Exported_Constants ADC exported constants
  * @{
  */
#if (LIBCFG_ADC1)  
#define IS_ADC(x)                               ((x == HT_ADC0) || (x == HT_ADC1))
#else
#define IS_ADC(x)                               (x == HT_ADC0)
#endif

#define ONE_SHOT_MODE                           (0x00000000)
#define CONTINUOUS_MODE                         (0x00000002)
#define DISCONTINUOUS_MODE                      (0x00000003)

#define IS_ADC_CONVERSION_MODE(REGULAR_MODE)    (((REGULAR_MODE) == ONE_SHOT_MODE)   || \
                                                 ((REGULAR_MODE) == CONTINUOUS_MODE) || \
                                                 ((REGULAR_MODE) == DISCONTINUOUS_MODE))

#define IS_ADC_HP_CONVERSION_MODE(HP_MODE)      (((HP_MODE) == ONE_SHOT_MODE)   || \
                                                 ((HP_MODE) == CONTINUOUS_MODE) || \
                                                 ((HP_MODE) == DISCONTINUOUS_MODE))

#if (LIBCFG_ADC1)
#define DUAL_INDEPENDENT                        (0x00000000)
#define DUAL_CASCADE_REGULAR                    (0x00000001)
#define DUAL_CASCADE_REGULAR_H_PRIORITY         (0x00000003)

#define IS_ADC_DUAL_MODE(DUAL_MODE)             (((DUAL_MODE) == DUAL_INDEPENDENT)   || \
                                                 ((DUAL_MODE) == DUAL_CASCADE_REGULAR) || \
                                                 ((DUAL_MODE) == DUAL_CASCADE_REGULAR_H_PRIORITY))
#endif

#if (LIBCFG_ADC_CH_CHECK_CHOOSE)
#define ADC0_CH_MARK                            (1 << 6)
#if (LIBCFG_ADC1)
#define ADC1_CH_MARK                            (1 << 7)
#else
#define ADC1_CH_MARK                            (0)
#endif
#define ADC_CH_MASK                             (ADC0_CH_MARK | ADC1_CH_MARK)

#define ADC_CH_REMOVE_MARK(CHANNEL)             (CHANNEL & (~ADC_CH_MASK))

#define IS_ADC0_CH_CHOOSE(HT_ADCn, CHANNEL)     (((HT_ADCn) == HT_ADC0) && (CHANNEL & ADC0_CH_MARK))
#if (LIBCFG_ADC1)
#define IS_ADC1_CH_CHOOSE(HT_ADCn, CHANNEL)     (((HT_ADCn) == HT_ADC1) && (CHANNEL & ADC1_CH_MARK))
#else
#define IS_ADC1_CH_CHOOSE(HT_ADCn, CHANNEL)     (0)
#endif
#define IS_ADC_CH_CHOOSE(HT_ADCn, CHANNEL)      (((CHANNEL & ADC_CH_MASK) == 0x00) || \
                                                 (IS_ADC0_CH_CHOOSE(HT_ADCn, CHANNEL)) || \
                                                 (IS_ADC1_CH_CHOOSE(HT_ADCn, CHANNEL)))
#endif

#if (LIBCFG_ADC_CH_65X_66X_V2)
#define ADC_CH_0                                (0)
#define ADC_CH_1                                (1)
#define ADC_CH_2                                (2)
#define ADC_CH_3                                (3)
#define ADC_CH_4                                (4)
#define ADC_CH_5                                (5)
#define ADC_CH_6                                (6)
#define ADC_CH_7                                (7)
#define ADC_CH_8                                (8)
#define ADC_CH_9                                (9)
#define ADC_CH_10                               (10)
#define ADC_CH_11                               (11)
#define ADC_CH_OPA0                             (12)
#define ADC_CH_GND_VREF                         (13)
#define ADC_CH_VDD_VREF                         (14)

#define IS_ADC_CHANNEL(CHANNEL)                 (((CHANNEL) == ADC_CH_0) || ((CHANNEL) == ADC_CH_1) || \
                                                 ((CHANNEL) == ADC_CH_2) || ((CHANNEL) == ADC_CH_3) || \
                                                 ((CHANNEL) == ADC_CH_4) || ((CHANNEL) == ADC_CH_5) || \
                                                 ((CHANNEL) == ADC_CH_6) || ((CHANNEL) == ADC_CH_7) || \
                                                 ((CHANNEL) == ADC_CH_8) || ((CHANNEL) == ADC_CH_9) || \
                                                 ((CHANNEL) == ADC_CH_10) || ((CHANNEL) == ADC_CH_11) || \
                                                 ((CHANNEL) == ADC_CH_OPA0) || \
                                                 ((CHANNEL) == ADC_CH_GND_VREF) || ((CHANNEL) == ADC_CH_VDD_VREF))

#define IS_ADC_INPUT_CHANNEL(CHANNEL)           (((CHANNEL) == ADC_CH_0) || ((CHANNEL) == ADC_CH_1) || \
                                                 ((CHANNEL) == ADC_CH_2) || ((CHANNEL) == ADC_CH_3) || \
                                                 ((CHANNEL) == ADC_CH_4) || ((CHANNEL) == ADC_CH_5) || \
                                                 ((CHANNEL) == ADC_CH_6) || ((CHANNEL) == ADC_CH_7) || \
                                                 ((CHANNEL) == ADC_CH_OPA0))
#elif (LIBCFG_ADC_CH_65X_66X_V3)
#define ADC_CH_0                                (0)
#define ADC_CH_1                                (1)
#define ADC_CH_2                                (2)
#define ADC_CH_3                                (3)
#define ADC_CH_4                                (4)
#define ADC_CH_5                                (5)
#define ADC_CH_6                                (6)
#define ADC_CH_7                                (7)
#define ADC_CH_8                                (8)
#define ADC_CH_9                                (9)
#define ADC_CH_10                               (10)
#define ADC_CH_11                               (11)
#define ADC_CH_OPA0                             (12)
#define ADC_CH_PGA0O                            (13)
#define ADC_CH_PGA1O                            (14)
#define ADC_CH_VDD_VREF                         (15)
#define ADC_CH_BANDGAP                          (16)
#define ADC_CH_MVDDA                            (17)

#define IS_ADC_CHANNEL(CHANNEL)                 (((CHANNEL) == ADC_CH_0) || ((CHANNEL) == ADC_CH_1) || \
                                                 ((CHANNEL) == ADC_CH_2) || ((CHANNEL) == ADC_CH_3) || \
                                                 ((CHANNEL) == ADC_CH_4) || ((CHANNEL) == ADC_CH_5) || \
                                                 ((CHANNEL) == ADC_CH_6) || ((CHANNEL) == ADC_CH_7) || \
                                                 ((CHANNEL) == ADC_CH_8) || ((CHANNEL) == ADC_CH_9) || \
                                                 ((CHANNEL) == ADC_CH_10) || ((CHANNEL) == ADC_CH_11) || \
                                                 ((CHANNEL) == ADC_CH_OPA0) || ((CHANNEL) == ADC_CH_PGA0O) || \
                                                 ((CHANNEL) == ADC_CH_PGA1O) || ((CHANNEL) == ADC_CH_VDD_VREF) || \
                                                 ((CHANNEL) == ADC_CH_BANDGAP) || ((CHANNEL) == ADC_CH_MVDDA))

#define IS_ADC_INPUT_CHANNEL(CHANNEL)           (((CHANNEL) == ADC_CH_0) || ((CHANNEL) == ADC_CH_1) || \
                                                 ((CHANNEL) == ADC_CH_2) || ((CHANNEL) == ADC_CH_3) || \
                                                 ((CHANNEL) == ADC_CH_4) || ((CHANNEL) == ADC_CH_5) || \
                                                 ((CHANNEL) == ADC_CH_6) || ((CHANNEL) == ADC_CH_7) || \
                                                 ((CHANNEL) == ADC_CH_8) || ((CHANNEL) == ADC_CH_9) || \
                                                 ((CHANNEL) == ADC_CH_10) || ((CHANNEL) == ADC_CH_11) || \
                                                 ((CHANNEL) == ADC_CH_OPA0) || ((CHANNEL) == ADC_CH_PGA0O) || \
                                                 ((CHANNEL) == ADC_CH_PGA1O))
#elif (LIBCFG_ADC_CH_65X_66X_V1)
#define ADC_CH_0                                (0)
#define ADC_CH_1                                (1)
#define ADC_CH_2                                (2)
#define ADC_CH_3                                (3)
#define ADC_CH_4                                (4)
#define ADC_CH_5                                (5)
#define ADC_CH_6                                (6)
#define ADC_CH_7                                (7)
#define ADC_CH_8                                (8)
#define ADC_CH_9                                (9)
#define ADC_CH_10                               (10)
#define ADC_CH_11                               (11)
#define ADC_CH_PGA0O                            (12)
#define ADC_CH_PGA1O                            (13)
#define ADC_CH_PGA2O                            (14)
#define ADC_CH_PGA3O                            (15)
#define ADC_CH_BANDGAP                          (16)
#define ADC_CH_MVDDA                            (17)

#define IS_ADC_CHANNEL(CHANNEL)                 (((CHANNEL) == ADC_CH_0) || ((CHANNEL) == ADC_CH_1) || \
                                                 ((CHANNEL) == ADC_CH_2) || ((CHANNEL) == ADC_CH_3) || \
                                                 ((CHANNEL) == ADC_CH_4) || ((CHANNEL) == ADC_CH_5) || \
                                                 ((CHANNEL) == ADC_CH_6) || ((CHANNEL) == ADC_CH_7) || \
                                                 ((CHANNEL) == ADC_CH_8) || ((CHANNEL) == ADC_CH_9) || \
                                                 ((CHANNEL) == ADC_CH_10) || ((CHANNEL) == ADC_CH_11) || \
                                                 ((CHANNEL) == ADC_CH_PGA0O) || ((CHANNEL) == ADC_CH_PGA1O) || \
                                                 ((CHANNEL) == ADC_CH_PGA2O) || ((CHANNEL) == ADC_CH_PGA3O) || \
                                                 ((CHANNEL) == ADC_CH_BANDGAP) || ((CHANNEL) == ADC_CH_MVDDA))

#define IS_ADC_INPUT_CHANNEL(CHANNEL)           (((CHANNEL) == ADC_CH_0) || ((CHANNEL) == ADC_CH_1) || \
                                                 ((CHANNEL) == ADC_CH_2) || ((CHANNEL) == ADC_CH_3) || \
                                                 ((CHANNEL) == ADC_CH_4) || ((CHANNEL) == ADC_CH_5) || \
                                                 ((CHANNEL) == ADC_CH_6) || ((CHANNEL) == ADC_CH_7) || \
                                                 ((CHANNEL) == ADC_CH_8) || ((CHANNEL) == ADC_CH_9) || \
                                                 ((CHANNEL) == ADC_CH_10) || ((CHANNEL) == ADC_CH_11) || \
                                                 ((CHANNEL) == ADC_CH_PGA0O) || ((CHANNEL) == ADC_CH_PGA1O) || \
                                                 ((CHANNEL) == ADC_CH_PGA2O) || ((CHANNEL) == ADC_CH_PGA3O))
#elif (LIBCFG_ADC_CH_65X_66X_V4) && (LIBCFG_ADC_CH_CHECK_CHOOSE)
#define ADC_CH_0                                (0)
#define ADC_CH_1                                (1)
#define ADC_CH_2                                (2)
#define ADC_CH_3                                (3)
#define ADC_CH_4                                (4)
#define ADC_CH_5                                (5)
#define ADC_CH_6                                (6)
#define ADC_CH_7                                (7)
#define ADC_CH_8                                (8)
#define ADC_CH_9                                (9)
#define ADC_CH_10                               (10)
#define ADC_CH_BANDGAP                          (16)
#define ADC_CH_VDD_VREF                         (17)

#define ADC0_CH_PGA0O                           (ADC0_CH_MARK | (11))
#define ADC0_CH_PGA1O                           (ADC0_CH_MARK | (12))
#define ADC0_CH_PGA2O                           (ADC0_CH_MARK | (13))
#define ADC0_CH_PGA3O                           (ADC0_CH_MARK | (14))
#define ADC0_CH_VTS                             (ADC0_CH_MARK | (15))

#define _IS_ADC0_CHANNEL(CHANNEL)               (((CHANNEL) == ADC0_CH_PGA0O) || ((CHANNEL) == ADC0_CH_PGA1O) || \
                                                 ((CHANNEL) == ADC0_CH_PGA2O) || ((CHANNEL) == ADC0_CH_PGA3O) || \
                                                 ((CHANNEL) == ADC0_CH_VTS))

#define _IS_ADC0_INPUT_CHANNEL(CHANNEL)         (((CHANNEL) == ADC0_CH_PGA0O) || ((CHANNEL) == ADC0_CH_PGA1O) || \
                                                 ((CHANNEL) == ADC0_CH_PGA2O) || ((CHANNEL) == ADC0_CH_PGA3O))

#if (LIBCFG_ADC1)
#define ADC1_CH_11                              (ADC1_CH_MARK | (11))
#define ADC1_CH_PGA3O                           (ADC1_CH_MARK | (12))
#define ADC1_CH_PGA4O                           (ADC1_CH_MARK | (13))
#define ADC1_CH_PGA5O                           (ADC1_CH_MARK | (14))
#define ADC1_CH_PGA0O                           (ADC1_CH_MARK | (15))

#define _IS_ADC1_CHANNEL(CHANNEL)               (((CHANNEL) == ADC1_CH_11) || ((CHANNEL) == ADC1_CH_PGA3O) || \
                                                 ((CHANNEL) == ADC1_CH_PGA4O) || ((CHANNEL) == ADC1_CH_PGA5O) || \
                                                 ((CHANNEL) == ADC1_CH_PGA0O))

#define _IS_ADC1_INPUT_CHANNEL(CHANNEL)         (((CHANNEL) == ADC1_CH_11) || ((CHANNEL) == ADC1_CH_PGA3O) || \
                                                 ((CHANNEL) == ADC1_CH_PGA4O) || ((CHANNEL) == ADC1_CH_PGA5O) || \
                                                 ((CHANNEL) == ADC1_CH_PGA0O))
#else
#define _IS_ADC1_CHANNEL(CHANNEL)               (0)
#define _IS_ADC1_INPUT_CHANNEL(CHANNEL)         (0)
#endif

#define IS_ADC_CHANNEL(CHANNEL)                 (((CHANNEL) == ADC_CH_0) || ((CHANNEL) == ADC_CH_1) || \
                                                 ((CHANNEL) == ADC_CH_2) || ((CHANNEL) == ADC_CH_3) || \
                                                 ((CHANNEL) == ADC_CH_4) || ((CHANNEL) == ADC_CH_5) || \
                                                 ((CHANNEL) == ADC_CH_6) || ((CHANNEL) == ADC_CH_7) || \
                                                 ((CHANNEL) == ADC_CH_8) || ((CHANNEL) == ADC_CH_9) || \
                                                 ((CHANNEL) == ADC_CH_10) || ((CHANNEL) == ADC_CH_BANDGAP) || \
                                                 ((CHANNEL) == ADC_CH_VDD_VREF) || \
                                                 (_IS_ADC0_CHANNEL(CHANNEL)) || \
                                                 (_IS_ADC1_CHANNEL(CHANNEL)))
#define IS_ADC_INPUT_CHANNEL(CHANNEL)           (((CHANNEL) == ADC_CH_0) || ((CHANNEL) == ADC_CH_1) || \
                                                 ((CHANNEL) == ADC_CH_2) || ((CHANNEL) == ADC_CH_3) || \
                                                 ((CHANNEL) == ADC_CH_4) || ((CHANNEL) == ADC_CH_5) || \
                                                 ((CHANNEL) == ADC_CH_6) || ((CHANNEL) == ADC_CH_7) || \
                                                 ((CHANNEL) == ADC_CH_8) || ((CHANNEL) == ADC_CH_9) || \
                                                 ((CHANNEL) == ADC_CH_10) || \
                                                 (_IS_ADC0_INPUT_CHANNEL(CHANNEL)) || \
                                                 (_IS_ADC1_INPUT_CHANNEL(CHANNEL)))

#else
#define ADC_CH_0                                (0)
#define ADC_CH_1                                (1)
#define ADC_CH_2                                (2)
#define ADC_CH_3                                (3)
#define ADC_CH_4                                (4)
#define ADC_CH_5                                (5)
#define ADC_CH_6                                (6)
#define ADC_CH_7                                (7)
#define ADC_CH_OPA0                             (8)
#define ADC_CH_OPA1                             (9)
#define ADC_CH_GND_VREF                         (12)
#define ADC_CH_VDD_VREF                         (13)

#define IS_ADC_CHANNEL(CHANNEL)                 (((CHANNEL) == ADC_CH_0) || ((CHANNEL) == ADC_CH_1) || \
                                                 ((CHANNEL) == ADC_CH_2) || ((CHANNEL) == ADC_CH_3) || \
                                                 ((CHANNEL) == ADC_CH_4) || ((CHANNEL) == ADC_CH_5) || \
                                                 ((CHANNEL) == ADC_CH_6) || ((CHANNEL) == ADC_CH_7) || \
                                                 ((CHANNEL) == ADC_CH_OPA0) || ((CHANNEL) == ADC_CH_OPA1) || \
                                                 ((CHANNEL) == ADC_CH_GND_VREF) || ((CHANNEL) == ADC_CH_VDD_VREF))

#define IS_ADC_INPUT_CHANNEL(CHANNEL)           (((CHANNEL) == ADC_CH_0) || ((CHANNEL) == ADC_CH_1) || \
                                                 ((CHANNEL) == ADC_CH_2) || ((CHANNEL) == ADC_CH_3) || \
                                                 ((CHANNEL) == ADC_CH_4) || ((CHANNEL) == ADC_CH_5) || \
                                                 ((CHANNEL) == ADC_CH_6) || ((CHANNEL) == ADC_CH_7) || \
                                                 ((CHANNEL) == ADC_CH_OPA0) || ((CHANNEL) == ADC_CH_OPA1))
#endif

#if (LIBCFG_ADC_TRIG_CHECK_CHOOSE)
#define ADC0_TRIG_MARK                          (1 << 6)
#if (LIBCFG_ADC1)
#define ADC1_TRIG_MARK                          (1 << 7)
#else
#define ADC1_TRIG_MARK                          (0)
#endif
#define ADC_TRIG_MASK                           (ADC0_TRIG_MARK | ADC1_TRIG_MARK)

#define ADC_TRIG_REMOVE_MARK(TRIGGER)           (TRIGGER & (~ADC_TRIG_MASK))

#define IS_ADC0_TRIG_CHOOSE(HT_ADCn, TRIGGER)   ((HT_ADCn == HT_ADC0) && (TRIGGER & ADC0_TRIG_MARK))
#if (LIBCFG_ADC1)
#define IS_ADC1_TRIG_CHOOSE(HT_ADCn, TRIGGER)   ((HT_ADCn == HT_ADC1) && (TRIGGER & ADC1_TRIG_MARK))
#else
#define IS_ADC1_TRIG_CHOOSE(HT_ADCn, TRIGGER)   (0)
#endif

#define IS_ADC_TRIG_CHOOSE(HT_ADCn, TRIGGER)    (((TRIGGER & ADC_TRIG_MASK) == 0x00000000)  || \
                                                 (IS_ADC0_CH_CHOOSE(HT_ADCn, TRIGGER)) || \
                                                 (IS_ADC1_CH_CHOOSE(HT_ADCn, TRIGGER)))
#endif

#define ADC_TRIG_SOFTWARE                       (1UL << 0)

#define ADC_TRIG_MCTM0_MTO                      ((1UL << 2) | (0UL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH0O                     ((1UL << 2) | (1UL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH1O                     ((1UL << 2) | (2UL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH2O                     ((1UL << 2) | (3UL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH3O                     ((1UL << 2) | (4UL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH0MEV                   ((1UL << 2) | (0x18UL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH0DEV                   ((1UL << 2) | (0x28UL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH0MDEV                  ((1UL << 2) | (0x38UL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH1MEV                   ((1UL << 2) | (0x19UL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH1DEV                   ((1UL << 2) | (0x29UL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH1MDEV                  ((1UL << 2) | (0x39UL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH2MEV                   ((1UL << 2) | (0x1AUL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH2DEV                   ((1UL << 2) | (0x2AUL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH2MDEV                  ((1UL << 2) | (0x3AUL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH3MEV                   ((1UL << 2) | (0x1BUL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH3DEV                   ((1UL << 2) | (0x2BUL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH3MDEV                  ((1UL << 2) | (0x3BUL << 24) | (0UL << 16))

#define IS_ADC_TRIG_MCTM0_CH(REGTRIG)           (((REGTRIG) == ADC_TRIG_MCTM0_MTO)     || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM0_CH0O)    || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM0_CH1O)    || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM0_CH2O)    || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM0_CH3O)    || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM0_CH0MEV)  || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM0_CH0DEV)  || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM0_CH0MDEV) || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM0_CH1MEV)  || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM0_CH1DEV)  || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM0_CH1MDEV) || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM0_CH2MEV)  || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM0_CH2DEV)  || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM0_CH2MDEV) || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM0_CH3MEV)  || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM0_CH3DEV)  || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM0_CH3MDEV))

#if (LIBCFG_MCTM1)
#define ADC_TRIG_MCTM1_MTO                      ((1UL << 2) | (0UL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH0O                     ((1UL << 2) | (1UL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH1O                     ((1UL << 2) | (2UL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH2O                     ((1UL << 2) | (3UL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH3O                     ((1UL << 2) | (4UL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH0MEV                   ((1UL << 2) | (0x18UL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH0DEV                   ((1UL << 2) | (0x28UL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH0MDEV                  ((1UL << 2) | (0x38UL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH1MEV                   ((1UL << 2) | (0x19UL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH1DEV                   ((1UL << 2) | (0x29UL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH1MDEV                  ((1UL << 2) | (0x39UL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH2MEV                   ((1UL << 2) | (0x1AUL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH2DEV                   ((1UL << 2) | (0x2AUL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH2MDEV                  ((1UL << 2) | (0x3AUL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH3MEV                   ((1UL << 2) | (0x1BUL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH3DEV                   ((1UL << 2) | (0x2BUL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH3MDEV                  ((1UL << 2) | (0x3BUL << 24) | (1UL << 16))

#define IS_ADC_TRIG_MCTM1_CH(REGTRIG)           (((REGTRIG) == ADC_TRIG_MCTM1_MTO)     || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM1_CH0O)    || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM1_CH1O)    || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM1_CH2O)    || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM1_CH3O)    || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM1_CH0MEV)  || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM1_CH0DEV)  || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM1_CH0MDEV) || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM1_CH1MEV)  || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM1_CH1DEV)  || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM1_CH1MDEV) || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM1_CH2MEV)  || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM1_CH2DEV)  || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM1_CH2MDEV) || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM1_CH3MEV)  || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM1_CH3DEV)  || \
                                                 ((REGTRIG) == ADC_TRIG_MCTM1_CH3MDEV))
#else
#define IS_ADC_TRIG_MCTM1_CH(REGTRIG)           (0)
#endif

#define IS_ADC_TRIG_MCTM0_CH_EXPANSION(REGTRIG) (0)
#define IS_ADC_TRIG_MCTM1_CH_EXPANSION(REGTRIG) (0)

#if (LIBCFG_ADC_TRIG_SRC_V2)
#define ADC_TRIG_MCTM0_CHALLMEV                 ((1UL << 2) | (0x1CUL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CHALLDEV                 ((1UL << 2) | (0x2CUL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CHALLMDEV                ((1UL << 2) | (0x3CUL << 24) | (0UL << 16))

#undef  IS_ADC_TRIG_MCTM0_CH_EXPANSION
#define IS_ADC_TRIG_MCTM0_CH_EXPANSION(REGTRIG) ((REGTRIG == ADC_TRIG_MCTM0_CHALLMEV) || \
                                                 (REGTRIG == ADC_TRIG_MCTM0_CHALLDEV) || \
                                                 (REGTRIG == ADC_TRIG_MCTM0_CHALLMDEV))
#elif (LIBCFG_ADC_TRIG_SRC_V3)
#define ADC_TRIG_MCTM0_CHALLMEV                 ((1UL << 2) | (0x1CUL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CHALLDEV                 ((1UL << 2) | (0x2CUL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CHALLMDEV                ((1UL << 2) | (0x3CUL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH0_2MEV                 ((1UL << 2) | (0x1DUL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH0_2DEV                 ((1UL << 2) | (0x2DUL << 24) | (0UL << 16))
#define ADC_TRIG_MCTM0_CH0_2MDEV                ((1UL << 2) | (0x3DUL << 24) | (0UL << 16))

#undef  IS_ADC_TRIG_MCTM0_CH_EXPANSION
#define IS_ADC_TRIG_MCTM0_CH_EXPANSION(REGTRIG) ((REGTRIG == ADC_TRIG_MCTM0_CHALLMEV)  || \
                                                 (REGTRIG == ADC_TRIG_MCTM0_CHALLDEV)  || \
                                                 (REGTRIG == ADC_TRIG_MCTM0_CHALLMDEV) || \
                                                 (REGTRIG == ADC_TRIG_MCTM0_CH0_2MEV)  || \
                                                 (REGTRIG == ADC_TRIG_MCTM0_CH0_2DEV)  || \
                                                 (REGTRIG == ADC_TRIG_MCTM0_CH0_2MDEV))
#if (LIBCFG_MCTM1)
#define ADC_TRIG_MCTM1_CHALLMEV                 ((1UL << 2) | (0x1CUL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CHALLDEV                 ((1UL << 2) | (0x2CUL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CHALLMDEV                ((1UL << 2) | (0x3CUL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH0_2MEV                 ((1UL << 2) | (0x1DUL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH0_2DEV                 ((1UL << 2) | (0x2DUL << 24) | (1UL << 16))
#define ADC_TRIG_MCTM1_CH0_2MDEV                ((1UL << 2) | (0x3DUL << 24) | (1UL << 16))

#undef  IS_ADC_TRIG_MCTM1_CH_EXPANSION
#define IS_ADC_TRIG_MCTM1_CH_EXPANSION(REGTRIG) ((REGTRIG == ADC_TRIG_MCTM1_CHALLMEV)  || \
                                                 (REGTRIG == ADC_TRIG_MCTM1_CHALLDEV)  || \
                                                 (REGTRIG == ADC_TRIG_MCTM1_CHALLMDEV) || \
                                                 (REGTRIG == ADC_TRIG_MCTM1_CH0_2MEV)  || \
                                                 (REGTRIG == ADC_TRIG_MCTM1_CH0_2DEV)  || \
                                                 (REGTRIG == ADC_TRIG_MCTM1_CH0_2MDEV))
#endif
#endif

#if (LIBCFG_ADC_TRIG_CHECK_CHOOSE)
#if defined(USE_HT32F66256)
#define ADC_TRIG_CMP0                           (ADC0_TRIG_MARK | (1UL << 4) | (0UL << 20))
#define ADC_TRIG_CMP1                           (ADC0_TRIG_MARK | (1UL << 4) | (1UL << 20))
#define ADC_TRIG_CMP2                           (ADC1_TRIG_MARK | (1UL << 4) | (0UL << 20))
#define IS_ADC_TRIG_CMP2(REGTRIG)               ((REGTRIG == ADC_TRIG_CMP2))
#define ADC_TRIG_CMP3                           (ADC1_TRIG_MARK | (1UL << 4) | (1UL << 20))
#define IS_ADC_TRIG_CMP3(REGTRIG)               ((REGTRIG == ADC_TRIG_CMP3))
#endif
#else
#define ADC_TRIG_CMP0                           ((1UL << 4) | (0UL << 20))
#define ADC_TRIG_CMP1                           ((1UL << 4) | (1UL << 20))
#if (LIBCFG_CMP2)
#define ADC_TRIG_CMP2                           ((1UL << 4) | (2UL << 20))
#define IS_ADC_TRIG_CMP2(REGTRIG)               ((REGTRIG == ADC_TRIG_CMP2))
#else
#define IS_ADC_TRIG_CMP2(REGTRIG)               (0)
#endif
#if (LIBCFG_CMP3)
#define ADC_TRIG_CMP3                           ((1UL << 4) | (3UL << 20))
#define IS_ADC_TRIG_CMP3(REGTRIG)               ((REGTRIG == ADC_TRIG_CMP3))
#else
#define IS_ADC_TRIG_CMP3(REGTRIG)               (0)
#endif
#endif

#define ADC_TRIG_BFTM0                          ((1UL << 3) | (0UL << 22) | (0UL << 19))
#define ADC_TRIG_BFTM1                          ((1UL << 3) | (0UL << 22) | (1UL << 19))

#if (LIBCFG_MCTM1)
#define ADC_TRIG_GPTM0_MTO                      ((1UL << 2) | (0UL << 24) | (2UL << 16))
#define ADC_TRIG_GPTM0_CH0O                     ((1UL << 2) | (1UL << 24) | (2UL << 16))
#define ADC_TRIG_GPTM0_CH1O                     ((1UL << 2) | (2UL << 24) | (2UL << 16))
#define ADC_TRIG_GPTM0_CH2O                     ((1UL << 2) | (3UL << 24) | (2UL << 16))
#define ADC_TRIG_GPTM0_CH3O                     ((1UL << 2) | (4UL << 24) | (2UL << 16))
#if (LIBCFG_GPTM1)
#define ADC_TRIG_GPTM1_MTO                      ((1UL << 2) | (0UL << 24) | (3UL << 16))
#define ADC_TRIG_GPTM1_CH0O                     ((1UL << 2) | (1UL << 24) | (3UL << 16))
#define ADC_TRIG_GPTM1_CH1O                     ((1UL << 2) | (2UL << 24) | (3UL << 16))
#define ADC_TRIG_GPTM1_CH2O                     ((1UL << 2) | (3UL << 24) | (3UL << 16))
#define ADC_TRIG_GPTM1_CH3O                     ((1UL << 2) | (4UL << 24) | (3UL << 16))
#endif
#else
#define ADC_TRIG_GPTM0_MTO                      ((1UL << 2) | (0UL << 24) | (1UL << 16))
#define ADC_TRIG_GPTM0_CH0O                     ((1UL << 2) | (1UL << 24) | (1UL << 16))
#define ADC_TRIG_GPTM0_CH1O                     ((1UL << 2) | (2UL << 24) | (1UL << 16))
#define ADC_TRIG_GPTM0_CH2O                     ((1UL << 2) | (3UL << 24) | (1UL << 16))
#define ADC_TRIG_GPTM0_CH3O                     ((1UL << 2) | (4UL << 24) | (1UL << 16))
#endif
#define IS_ADC_TRIG_GPTM0_CH(REGTRIG)           (((REGTRIG) == ADC_TRIG_GPTM0_MTO)          || \
                                                 ((REGTRIG) == ADC_TRIG_GPTM0_CH0O)         || \
                                                 ((REGTRIG) == ADC_TRIG_GPTM0_CH1O)         || \
                                                 ((REGTRIG) == ADC_TRIG_GPTM0_CH2O)         || \
                                                 ((REGTRIG) == ADC_TRIG_GPTM0_CH3O))
#if (LIBCFG_GPTM1)
#define IS_ADC_TRIG_GPTM1_CH(REGTRIG)           (((REGTRIG) == ADC_TRIG_GPTM1_MTO)          || \
                                                 ((REGTRIG) == ADC_TRIG_GPTM1_CH0O)         || \
                                                 ((REGTRIG) == ADC_TRIG_GPTM1_CH1O)         || \
                                                 ((REGTRIG) == ADC_TRIG_GPTM1_CH2O)         || \
                                                 ((REGTRIG) == ADC_TRIG_GPTM1_CH3O))
#else
#define IS_ADC_TRIG_GPTM1_CH(REGTRIG)           (0)
#endif

#define ADC_TRIG_EXTI_0                         ((1UL << 1) | ( 0UL << 8))
#define ADC_TRIG_EXTI_1                         ((1UL << 1) | ( 1UL << 8))
#define ADC_TRIG_EXTI_2                         ((1UL << 1) | ( 2UL << 8))
#define ADC_TRIG_EXTI_3                         ((1UL << 1) | ( 3UL << 8))
#define ADC_TRIG_EXTI_4                         ((1UL << 1) | ( 4UL << 8))
#define ADC_TRIG_EXTI_5                         ((1UL << 1) | ( 5UL << 8))
#define ADC_TRIG_EXTI_6                         ((1UL << 1) | ( 6UL << 8))
#define ADC_TRIG_EXTI_7                         ((1UL << 1) | ( 7UL << 8))
#if (LIBCFG_EXTI_8CH)
#define IS_ADC_TRIG_EXTI8_15(REGTRIG)           (0)
#else
#define ADC_TRIG_EXTI_8                         ((1UL << 1) | ( 8UL << 8))
#define ADC_TRIG_EXTI_9                         ((1UL << 1) | ( 9UL << 8))
#define ADC_TRIG_EXTI_10                        ((1UL << 1) | (10UL << 8))
#define ADC_TRIG_EXTI_11                        ((1UL << 1) | (11UL << 8))
#define ADC_TRIG_EXTI_12                        ((1UL << 1) | (12UL << 8))
#define ADC_TRIG_EXTI_13                        ((1UL << 1) | (13UL << 8))
#define ADC_TRIG_EXTI_14                        ((1UL << 1) | (14UL << 8))
#define ADC_TRIG_EXTI_15                        ((1UL << 1) | (15UL << 8))
#define IS_ADC_TRIG_EXTI8_15(REGTRIG)           ((REGTRIG) == ADC_TRIG_EXTI_8)              || \
                                                ((REGTRIG) == ADC_TRIG_EXTI_9)              || \
                                                ((REGTRIG) == ADC_TRIG_EXTI_10)             || \
                                                ((REGTRIG) == ADC_TRIG_EXTI_11)             || \
                                                ((REGTRIG) == ADC_TRIG_EXTI_12)             || \
                                                ((REGTRIG) == ADC_TRIG_EXTI_13)             || \
                                                ((REGTRIG) == ADC_TRIG_EXTI_14)             || \
                                                ((REGTRIG) == ADC_TRIG_EXTI_15)
#endif

#define IS_ADC_TRIG(REGTRIG)                    ((IS_ADC_TRIG_GPTM0_CH(REGTRIG))            || \
                                                 (IS_ADC_TRIG_GPTM1_CH(REGTRIG))            || \
                                                 (IS_ADC_TRIG_MCTM0_CH(REGTRIG))            || \
                                                 (IS_ADC_TRIG_MCTM1_CH(REGTRIG))            || \
                                                 (IS_ADC_TRIG_MCTM0_CH_EXPANSION(REGTRIG))  || \
                                                 (IS_ADC_TRIG_MCTM1_CH_EXPANSION(REGTRIG))  || \
                                                 ((REGTRIG) == ADC_TRIG_BFTM0)              || \
                                                 ((REGTRIG) == ADC_TRIG_BFTM1)              || \
                                                 ((REGTRIG) == ADC_TRIG_CMP0)               || \
                                                 ((REGTRIG) == ADC_TRIG_CMP1)               || \
                                                 (IS_ADC_TRIG_CMP2(REGTRIG))                || \
                                                 (IS_ADC_TRIG_CMP3(REGTRIG))                || \
                                                 ((REGTRIG) == ADC_TRIG_EXTI_0)             || \
                                                 ((REGTRIG) == ADC_TRIG_EXTI_1)             || \
                                                 ((REGTRIG) == ADC_TRIG_EXTI_2)             || \
                                                 ((REGTRIG) == ADC_TRIG_EXTI_3)             || \
                                                 ((REGTRIG) == ADC_TRIG_EXTI_4)             || \
                                                 ((REGTRIG) == ADC_TRIG_EXTI_5)             || \
                                                 ((REGTRIG) == ADC_TRIG_EXTI_6)             || \
                                                 ((REGTRIG) == ADC_TRIG_EXTI_7)             || \
                                                 (IS_ADC_TRIG_EXTI8_15(REGTRIG))            || \
                                                 ((REGTRIG) == ADC_TRIG_SOFTWARE))

#define ADC_INT_SINGLE_EOC                      (0x00000001)
#define ADC_INT_SUB_GROUP_EOC                   (0x00000002)
#define ADC_INT_CYCLE_EOC                       (0x00000004)
#define ADC_INT_HP_SINGLE_EOC                   (0x00000100)
#define ADC_INT_HP_SUB_GROUP_EOC                (0x00000200)
#define ADC_INT_HP_CYCLE_EOC                    (0x00000400)
#define ADC_INT_AWD_LOWER                       (0x00010000)
#define ADC_INT_AWD_UPPER                       (0x00020000)
#define ADC_INT_DATA_OVERWRITE                  (0x01000000)
#define ADC_INT_HP_DATA_OVERWRITE               (0x02000000)

#define IS_ADC_INT(INT)                         ((((INT) & 0xFCFCF8F8) == 0) && ((INT) != 0))

#define ADC_FLAG_SINGLE_EOC                     (0x00000001)
#define ADC_FLAG_SUB_GROUP_EOC                  (0x00000002)
#define ADC_FLAG_CYCLE_EOC                      (0x00000004)
#define ADC_FLAG_HP_SINGLE_EOC                  (0x00000100)
#define ADC_FLAG_HP_SUB_GROUP_EOC               (0x00000200)
#define ADC_FLAG_HP_CYCLE_EOC                   (0x00000400)
#define ADC_FLAG_AWD_LOWER                      (0x00010000)
#define ADC_FLAG_AWD_UPPER                      (0x00020000)
#define ADC_FLAG_DATA_OVERWRITE                 (0x01000000)
#define ADC_FLAG_HP_DATA_OVERWRITE              (0x02000000)

#define IS_ADC_FLAG(FLAG)                       ((((FLAG) & 0xFCFCF8F8) == 0) && ((FLAG) != 0))


#define ADC_REGULAR_DATA0                       (0)
#define ADC_REGULAR_DATA1                       (1)
#define ADC_REGULAR_DATA2                       (2)
#define ADC_REGULAR_DATA3                       (3)
#define ADC_REGULAR_DATA4                       (4)
#define ADC_REGULAR_DATA5                       (5)
#define ADC_REGULAR_DATA6                       (6)
#define ADC_REGULAR_DATA7                       (7)

#define IS_ADC_REGULAR_DATA(DATA)               ((DATA) < 8)


#define ADC_HP_DATA0                            (0)
#define ADC_HP_DATA1                            (1)
#define ADC_HP_DATA2                            (2)
#define ADC_HP_DATA3                            (3)
#if (LIBCFG_ADC_HDR_4_11)
#define ADC_HP_DATA4                            (4)
#define ADC_HP_DATA5                            (5)
#define ADC_HP_DATA6                            (6)
#define ADC_HP_DATA7                            (7)
#define ADC_HP_DATA8                            (8)
#define ADC_HP_DATA9                            (9)
#define ADC_HP_DATA10                           (10)
#define ADC_HP_DATA11                           (11)
#endif

#if (LIBCFG_ADC_HDR_4_11)
#define IS_ADC_HP_DATA(DATA)                    ((DATA) < 12)
#else
#define IS_ADC_HP_DATA(DATA)                    ((DATA) < 4)
#endif


#define ADC_AWD_DISABLE                         (u8)0x00
#define ADC_AWD_ALL_LOWER                       (u8)0x05
#define ADC_AWD_ALL_UPPER                       (u8)0x06
#define ADC_AWD_ALL_LOWER_UPPER                 (u8)0x07
#define ADC_AWD_SINGLE_LOWER                    (u8)0x01
#define ADC_AWD_SINGLE_UPPER                    (u8)0x02
#define ADC_AWD_SINGLE_LOWER_UPPER              (u8)0x03

#define IS_ADC_AWD(AWD)                         (((AWD) == ADC_AWD_DISABLE)         || \
                                                 ((AWD) == ADC_AWD_ALL_LOWER)       || \
                                                 ((AWD) == ADC_AWD_ALL_UPPER)       || \
                                                 ((AWD) == ADC_AWD_ALL_LOWER_UPPER) || \
                                                 ((AWD) == ADC_AWD_SINGLE_LOWER)    || \
                                                 ((AWD) == ADC_AWD_SINGLE_UPPER)    || \
                                                 ((AWD) == ADC_AWD_SINGLE_LOWER_UPPER))

#define IS_ADC_THRESHOLD(THRESHOLD)             ((THRESHOLD) < 4096)

#define ADC_PDMA_REGULAR_SINGLE                 (0x00000001)
#define ADC_PDMA_REGULAR_SUBGROUP               (0x00000002)
#define ADC_PDMA_REGULAR_CYCLE                  (0x00000004)

#define ADC_PDMA_HP_SINGLE                      (0x00000100)
#define ADC_PDMA_HP_SUBGROUP                    (0x00000200)
#define ADC_PDMA_HP_CYCLE                       (0x00000400)

#define IS_ADC_PDMA(PDMA)                       (((PDMA) == ADC_PDMA_REGULAR_SINGLE)   || \
                                                 ((PDMA) == ADC_PDMA_REGULAR_SUBGROUP) || \
                                                 ((PDMA) == ADC_PDMA_REGULAR_CYCLE)    || \
                                                 ((PDMA) == ADC_PDMA_HP_SINGLE)        || \
                                                 ((PDMA) == ADC_PDMA_HP_SUBGROUP)      || \
                                                 ((PDMA) == ADC_PDMA_HP_CYCLE))


#define IS_ADC_INPUT_SAMPLING_TIME(TIME)        ((TIME) <= 255)

#if (!LIBCFG_ADC_NO_OFFSET_REG)
#define IS_ADC_OFFSET(OFFSET)                   ((OFFSET) < 4096)
#endif

#define IS_ADC_REGULAR_RANK(RANK)               ((RANK) < 8)

#if (LIBCFG_ADC_HDR_4_11)
#define IS_ADC_HP_RANK(RANK)                    ((RANK) < 12)
#else
#define IS_ADC_HP_RANK(RANK)                    ((RANK) < 4)
#endif

#define IS_ADC_REGULAR_LENGTH(LENGTH)           (((LENGTH) >= 1) && ((LENGTH) <= 8))
#define IS_ADC_REGULAR_SUB_LENGTH(SUB_LENGTH)   (((SUB_LENGTH) >= 1) && ((SUB_LENGTH) <= 8))

#if (LIBCFG_ADC_HCONV_LENGTH_V2)
#define IS_ADC_HP_LENGTH(LENGTH)                (((LENGTH) >= 1) && ((LENGTH) <= 8))
#define IS_ADC_HP_SUB_LENGTH(SUB_LENGTH)        (((SUB_LENGTH) >= 1) && ((SUB_LENGTH) <= 8))
#else
#define IS_ADC_HP_LENGTH(LENGTH)                (((LENGTH) >= 1) && ((LENGTH) <= 4))
#define IS_ADC_HP_SUB_LENGTH(SUB_LENGTH)        (((SUB_LENGTH) >= 1) && ((SUB_LENGTH) <= 4))
#endif

#if (LIBCFG_ADC_TRIG_DELAY)
#define IS_ADC_TRIG_DELAY(DELAY)                ((DELAY) < 256)
#endif

#if (!LIBCFG_ADC_NO_OFFSET_REG)
typedef enum
{
  ADC_ALIGN_RIGHT = (0 << 14),
  ADC_ALIGN_LEFT  = (1 << 14),
} ADC_ALIGN_Enum;

#define IS_ADC_ALIGN(ALIGN)                     (((ALIGN) == ADC_ALIGN_RIGHT) || ((ALIGN) == ADC_ALIGN_LEFT))
#endif

#if (LIBCFG_ADC_IVREF_VOLTCONFIG)
#define ADC_VREF_2V0                            (0ul << 4)
#define ADC_VREF_2V5                            (1ul << 4)
#define ADC_VREF_2V7                            (2ul << 4)
#define ADC_VREF_3V0                            (3ul << 4)

#define IS_ADC_VREF_SEL(SEL)                    ((SEL == ADC_VREF_2V0) || \
                                                 (SEL == ADC_VREF_2V5) || \
                                                 (SEL == ADC_VREF_2V7) || \
                                                 (SEL == ADC_VREF_3V0))
#endif

#if (LIBCFG_ADC_FLEXIBLE_VREFVALR)
#define ADC_VREFVALR_MASK                       (0x7F)
#define IS_ADC_VREFVALR(VAL)                    ((VAL) <= ADC_VREFVALR_MASK)
#endif

#if (LIBCFG_ADC_TEMP_SENSOR)
#define IS_TEMP_SENSOR_DIV(DIVx)                 ((DIVx) < 16)

#define TEMP_SENSOR_FLAG_READY                   (0x00000004)
#define IS_TEMP_SENSOR_FLAG(TSFLAG)              (TSFLAG == TEMP_SENSOR_FLAG_READY)

#define TEMP_SENSOR_INT_READY                    (0x00000008)
#define IS_TEMP_SENSOR_INT(TSINT)                (TSINT == TEMP_SENSOR_INT_READY)
#endif

/**
  * @}
  */

/* Exported functions --------------------------------------------------------------------------------------*/
/** @defgroup ADC_Exported_Functions ADC exported functions
  * @{
  */
void ADC_DeInit(HT_ADC_TypeDef* HT_ADCn);
void ADC_Reset(HT_ADC_TypeDef* HT_ADCn);
void ADC_Cmd(HT_ADC_TypeDef* HT_ADCn, ControlStatus NewState);
#if (LIBCFG_ADC1)
void ADC_DualModeConfig(HT_ADC_TypeDef* HT_ADCn, u32 DUAL_X, u8 HDelayTime, u8 DelayTime);
#endif

void ADC_RegularChannelConfig(HT_ADC_TypeDef* HT_ADCn, u8 ADC_CH_n, u8 Rank, u8 SampleClock);
void ADC_RegularGroupConfig(HT_ADC_TypeDef* HT_ADCn, u8 ADC_MODE, u8 Length, u8 SubLength);
void ADC_RegularTrigConfig(HT_ADC_TypeDef* HT_ADCn, u32 ADC_TRIG_x);

void ADC_HPChannelConfig(HT_ADC_TypeDef* HT_ADCn, u8 ADC_CH_n, u8 Rank, u8 SampleClock);
void ADC_HPGroupConfig(HT_ADC_TypeDef* HT_ADCn, u8 ADC_MODE, u8 Length, u8 SubLength);
void ADC_HPTrigConfig(HT_ADC_TypeDef* HT_ADCn, u32 ADC_TRIG_x);

#if (!LIBCFG_ADC_NO_OFFSET_REG)
void ADC_ChannelDataAlign(HT_ADC_TypeDef* HT_ADCn, u8 ADC_CH_n, ADC_ALIGN_Enum ADC_ALIGN_x);
void ADC_ChannelOffsetValue(HT_ADC_TypeDef* HT_ADCn, u8 ADC_CH_n, u16 OffsetValue);
void ADC_ChannelOffsetCmd(HT_ADC_TypeDef* HT_ADCn, u8 ADC_CH_n, ControlStatus NewState);
#endif

#if (LIBCFG_ADC_TRIG_DELAY)
void ADC_TrigDelayConfig(HT_ADC_TypeDef* HT_ADCn, u8 HDelayTime, u8 DelayTime);
#endif

void ADC_SoftwareStartConvCmd(HT_ADC_TypeDef* HT_ADCn, ControlStatus NewState);
void ADC_HPSoftwareStartConvCmd(HT_ADC_TypeDef* HT_ADCn, ControlStatus NewState);

u16 ADC_GetConversionData(HT_ADC_TypeDef* HT_ADCn, u8 ADC_REGULAR_DATAn);
u16 ADC_GetHPConversionData(HT_ADC_TypeDef* HT_ADCn, u8 ADC_HP_DATAn);

void ADC_IntConfig(HT_ADC_TypeDef* HT_ADCn, u32 ADC_INT_x, ControlStatus NewState);
FlagStatus ADC_GetIntStatus(HT_ADC_TypeDef* HT_ADCn, u32 ADC_INT_x);
void ADC_ClearIntPendingBit(HT_ADC_TypeDef* HT_ADCn, u32 ADC_INT_x);
FlagStatus ADC_GetFlagStatus(HT_ADC_TypeDef* HT_ADCn, u32 ADC_FLAG_x);

void ADC_AWDConfig(HT_ADC_TypeDef* HT_ADCn, u32 ADC_AWD_x);
void ADC_AWDSingleChannelConfig(HT_ADC_TypeDef* HT_ADCn, u8 ADC_CH_n);
void ADC_AWDThresholdsConfig(HT_ADC_TypeDef* HT_ADCn, u16 UPPER, u16 LOWER);

void ADC_PDMAConfig(HT_ADC_TypeDef* HT_ADCn, u32 ADC_PDMA_x, ControlStatus NewState);

#if (LIBCFG_ADC_IVREF)
void ADC_VREFCmd(HT_ADC_TypeDef* HT_ADCn, ControlStatus NewState);
#endif

#if (LIBCFG_ADC_IVREF_VOLTCONFIG)
void ADC_VREFConfig(HT_ADC_TypeDef* HT_ADCn, u32 ADC_VREF_x);
#endif

#if (LIBCFG_ADC_VREFBUF)
void ADC_VREFOutputADVREFPCmd(HT_ADC_TypeDef* HT_ADCn, ControlStatus NewState);
#endif

#if (LIBCFG_ADC_FLEXIBLE_VREFVALR)
void ADC_SetVoltageReferenceValue(HT_ADC_TypeDef* HT_ADCn, u8 VREFVALR);
u8 ADC_GetVoltageReferenceValue(HT_ADC_TypeDef* HT_ADCn);
#endif

#if (LIBCFG_ADC_MVDDA)
void ADC_MVDDACmd(HT_ADC_TypeDef* HT_ADCn, ControlStatus NewState);
#endif

#if (LIBCFG_ADC_TEMP_SENSOR)
void ADC_TempSensorCmd(HT_ADC_TypeDef* HT_ADCn, ControlStatus NewState);
void ADC_TempSensorSetClockDivider(HT_ADC_TypeDef* HT_ADCn, u8 TempSensorClockDiv);
void ADC_TempSensorStartOperateCmd(HT_ADC_TypeDef* HT_ADCn, ControlStatus NewState);
FlagStatus ADC_TempSensorGetFlagStatus(HT_ADC_TypeDef* HT_ADCn, u32 TempSensorFlag);
void ADC_TempSensorClearFlag(HT_ADC_TypeDef* HT_ADCn, u32 TempSensorFlag);
u16 ADC_TempSensorGetCalValue(HT_ADC_TypeDef* HT_ADCn);
void ADC_TempSensorIntConfig(HT_ADC_TypeDef* HT_ADCn, u32 ADC_INT_TP_x, ControlStatus NewState);
ErrStatus ADC_TempSensorGetTemp(HT_ADC_TypeDef* HT_ADCn, ADC_TempSensorParam_TypeDef* tempPara ,s32* temperature_mC);
s32 ADC_TempSensorGetCalTempPoint(HT_ADC_TypeDef* HT_ADCn);
#endif
/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
