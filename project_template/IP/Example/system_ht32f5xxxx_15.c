/**************************************************************************//**
 * @file    library/Device/Holtek/HT32F5xxxx/Source/system_ht32f5xxxx_15.c
 * @brief   CMSIS Cortex-M0+ Device Peripheral Access Layer Source File
 *          for the Holtek HT32F5xxxx Device Series
 * @version $Rev:: 10081        $
 * @date    $Date:: 2026-10-14 #$
 *
 * @note
 * Copyright (C) Holtek Semiconductor Inc. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) supplies this software for Cortex-M processor-based
 * microcontrollers. This file can be freely distributed within development
 * tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

// Supported Device
// ========================================
//   HT32F53242, HT32F53252
//   HT32F53231, HT32F53241
//   HT32F64951

//#define USE_HT32F53242_52
//#define USE_HT32F53231_41
//#define USE_HT32F64951

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup HT32F5xxxx_system HT32F5xxxx System
  * @{
  */


#include "ht32f5xxxx_01.h"

/** @addtogroup HT32F5xxxx_System_Private_Defines
  * @{
  */
/*
//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
*/

/*--------------------- Clock Configuration ----------------------------------
//
//    <q1> Enable High Speed External Crystal Oscillator (HSE)
//          <i> Default HSE = DISABLE
//
//    <q3> Enable Low Speed External Crystal Oscillator (LSE)
//          <i> Default LSE = DISABLE
//
//    <e4> Enable PLL
//                <i> Default PLL = DISABLE
//                <i> PLL Out = (((HSE or HSI) / SRC_DIV) x NF2 ) / NO2
//        <o5>  PLL Clock Source
//                <0=> CK_HSE
//                <1=> CK_HSI
//                <i> Default PLL clock source = CK_HSI
//                <i> PLL source clock must be in the range of 4 MHz to 16 MHz
//        <o6>  PLL Clock Source Divider (SRC_DIV)
//                <0=> 1
//                <1=> 2
//                <i> PLL input clock = PLL Clock Source / (SRC_DIV)
//        <o7>  PLL Feedback Clock Divider (NF2): 1 ~ 16
//                <1-16:1>
//                <i> PLL feedback clock = PLL input clock x NF2
//                <i> PLL feedback clock must be in the range of 24 MHz to 60 MHz
//        <o8>  PLL Output Clock Divider (NO2)
//                <0=> 1
//                <1=> 2
//                <2=> 4
//                <3=> 8
//                <i> PLL output clock = PLL feedback clock / NO2
//                <i> PLL output clock must be in the range of 4 MHz to 60 MHz
//    </e>
//
//    <h> SystemCoreClock Configuration (CK_AHB)
//      <o9>    SystemCoreClock Source
//                <1=> CK_PLL
//                <2=> CK_HSE
//                <3=> CK_HSI
//                <6=> CK_LSE
//                <7=> CK_LSI
//                <i> Default SystemCoreClock source = CK_HSI
//      <o10>   SystemCoreClock Source Divider
//                <0=> 1
//                <1=> 2
//                <2=> 4
//                <3=> 8
//                <4=> 16
//                <5=> 32
//                <i> Default SystemCoreClock source divider = 1
//    </h>
//
//    <h> FLASH Configuration
//      <o11>   Wait state
//                <0=> 0 WS
//                <1=> 1 WS
//                <2=> 2 WS
//                <9=> AUTO
//                <i>   0 WS:  1 kHz <= CK_AHB <= 20 MHz
//                <i>   1 WS: 20 MHz <  CK_AHB <= 40 MHz
//                <i>   2 WS: 40 MHz <  CK_AHB <= 60 MHz
//      <q12>   Pre-fetch Buffer Enable
//                <i> Default pre-fetch buffer = ENABLE
//    </h>
*/

/* !!! NOTICE !!!
   HSI must keep turn on when doing the Flash operation (Erase/Program).
*/

/* !!! NOTICE !!!
 * How to adjust the value of High Speed External oscillator (HSE)?
   The default value of HSE is define by "HSE_VALUE" in "ht32fxxxxx_nn.h".
   If your board uses a different HSE speed, please add a new compiler preprocessor
   C define, "HSE_VALUE=n000000" ("n" represents n MHz) in the toolchain/IDE,
   or edit the "HSE_VALUE" in the "ht32f5xxxx_conf.h" file.
   Take Keil MDK-ARM for instance, to set HSE as 16 MHz:
   "Option of Target -> C/C++ > Preprocessor Symbols"
      Define: USE_HT32_DRIVER, USE_HT32Fxxxxx_SK, USE_HT32Fxxxxx_xx, USE_MEM_HT32Fxxxxx, HSE_VALUE=16000000
                                                                                         ^^ Add "HSE_VALUE"
                                                                                            define as above.
*/
#define HSI_ENABLE        (1)     /*!< 0: DISABLE,  1: ENABLE                                               */
#define HSE_ENABLE        (1)     /*!< 0: DISABLE,  1: ENABLE                                               */
#define LSI_ENABLE        (1)     /*!< 0: DISABLE,  1: ENABLE                                               */
#define LSE_ENABLE        (0)     /*!< 0: DISABLE,  1: ENABLE                                               */
#define PLL_ENABLE        (1)     /*!< 0: DISABLE,  1: ENABLE                                               */
#define PLL_CLK_SRC       (0)     /*!< 0: HSE,      1: HSI                                                  */
#define PLL_CLK_SRC_DIV   (1)     /*!< 0: DIV1,     1: DIV2                                                 */
#define PLL_NF2_DIV       (15)    /*!< 1~16: DIV1~DIV16                                                     */
#define PLL_NO2_DIV       (0)     /*!< 0: DIV1,     1: DIV2,   2: DIV4,   3: DIV8                           */
#define HCLK_SRC          (1)     /*!< 0: PLL,      1: PLL,    2: HSE,    3: HSI     6: LSE,    7: LSI      */
#define HCLK_DIV          (0)     /*!< 0: DIV1,     1: DIV2,   2: DIV4,   3: DIV8,   4: DIV16,  5: DIV32    */
#define WAIT_STATE        (9)     /*!< 0: WS = 0,   1: WS = 1, 2: WS = 2,            9: AUTO                */
#define PRE_FETCH_ENABLE  (1)     /*!< 0: DISABLE,  1: ENABLE                                               */
#define DEINIT_ENABLE     (1)     /* Set 0 for reduce code size                                             */

/*----------------------------------------------------------------------------------------------------------*/
/* PLL Out = (((HSE or HSI) / (PLL_CLK_SRC_DIV + 1)) x PLL_NF2) / PLL_NO2                                   */
/*----------------------------------------------------------------------------------------------------------*/


/*--------------------- WDT Configuration ----------------------------------
//
//    <e0> Enable WDT Configuration
//      <o1> WDT Prescaler Selection
//           <0=> CK_WDT / 1
//           <1=> CK_WDT / 2
//           <2=> CK_WDT / 4
//           <3=> CK_WDT / 8
//           <4=> CK_WDT / 16
//           <5=> CK_WDT / 32
//           <6=> CK_WDT / 64
//           <7=> CK_WDT / 128
//      <o2> WDT Reload Value <1-4095:1>
//      <q3> Enable WDT Reset function
//      <o4> WDT Sleep Halt mode
//           <0=> No halt
//           <1=> Halt in DeepSleep1
//           <2=> Halt in Sleep & DeepSleep1
//   </e>
*/
#define WDT_ENABLE        (0)     /*!< 0: DISABLE,  1: ENABLE                                               */
#define WDT_PRESCALER     (5)     /*!< 0: 1/1, 1: 1/2, 2: 1/4, 3: 1/8, 4: 1/16, 5: 1/32, 6: 1/64, 7: 1/128  */
#define WDT_RELOAD        (2000)  /*!< 0 ~ 4095, 12 bit                                                     */
#define WDT_RESET_ENABLE  (1)     /*!< 0: No Reset, 1: Reset when WDT over flow                             */
#define WDT_SLEEP_HALT    (2)     /*!< 0: No halt,  1: Halt in DeepSleep1, 2: Halt in Sleep & DeepSleep1    */


/*--------------------- Load_Trim_Code Configuration -----------------------
//
//    <h> Load_Trim_Code Configuration
//          <i> Notice: This Configuration only support specific MCU (EX: HT32F64041, HT32F64141...)
//      <o0> Load_Trim_Code Retry Times Limit <1-255:1>
//      <o1> Enable Load_Trim_Code Error Handling
//          <0=> DISABLE
//          <1=> ENABLE
//          <i> Default LTC_ERRORHANDLING = DISABLE
//    </h>
*/
#define ERROR_COUNTER_LIMIT (5)   /*!< Error handling retry limit                                           */
#define LTC_ERRORHANDLING   (0)   /*!< 0: DISABLE,  1: ENABLE                                               */

/**
 * @brief Check HSI frequency
 */
#if (HSI_VALUE != 8000000UL)
  #error "CK_HSI clock issue: must be 8 MHz!"
#endif

/**
 * @brief Check HSE frequency
 */
#if ((HSE_VALUE < 4000000UL) || (HSE_VALUE > 16000000UL))
  #error "CK_HSE clock issue: must be in the range of 4 MHz to 16 MHz!"
#endif

/**
 * @brief Check LSI frequency
 */
#if (LSI_VALUE != 32000UL)
  #error "CK_LSI clock issue: must be 32 kHz!"
#endif

/**
 * @brief Check LSE frequency
 */
#if (LSE_VALUE != 32768UL)
  #error "CK_LSE clock issue: must be 32.768 kHz!"
#endif

/**
 * @brief CK_PLL definition
 */
#if (PLL_ENABLE == 1)
  /* Get CK_VCO frequency                                                                                   */
  #if (PLL_CLK_SRC == 1)
    #if (HSI_ENABLE == 0)
      #error "CK_PLL clock source issue: HSI has not been enabled"
    #else
      #define __CK_VCO    ((HSI_VALUE >> PLL_CLK_SRC_DIV) * PLL_NF2_DIV)   /*!< Select HSI as PLL source    */
    #endif
  #else
    #if (HSE_ENABLE == 0)
      #error "CK_PLL clock source issue: HSE has not been enabled!"
    #else
      #define __CK_VCO    ((HSE_VALUE >> PLL_CLK_SRC_DIV) * PLL_NF2_DIV)  /*!< Select HSE as PLL source     */
    #endif
  #endif

  #define VCO_MIN 24000000UL
  #define VCO_MAX 60000000UL
  #define PLL_MIN  4000000UL
  #define PLL_MAX 60000000UL

  /* Check CK_VCO frequency                                                                                 */
  #if ((__CK_VCO < VCO_MIN) || (__CK_VCO > VCO_MAX))
    #error "CK_VCO clock issue: must be in the range!"
  #endif

  #define __CK_PLL    (__CK_VCO >> PLL_NO2_DIV)                 /*!< Get CK_PLL frequency                   */

  /* Check CK_PLL frequency                                                                                 */
  #if ((__CK_PLL < PLL_MIN) || (__CK_PLL > PLL_MAX))
    #error "CK_PLL clock issue: must be in the range!"
  #endif
#endif

/**
 * @brief CK_SYS definition
 */
#if (HCLK_SRC == 1)
  #if (PLL_ENABLE == 1)
    #define __CK_SYS    __CK_PLL              /*!< Select PLL as CK_SYS source                              */
  #else
    #error "CK_SYS clock source issue: PLL is not enable!"
  #endif
#elif (HCLK_SRC == 2)
  #if (HSE_ENABLE == 1)
    #define __CK_SYS    HSE_VALUE             /*!< Select HSE as CK_SYS source                              */
  #else
    #error "CK_SYS clock source issue: HSE is not enable!"
  #endif
#elif (HCLK_SRC == 3)
  #if (HSI_ENABLE == 1)
    #define __CK_SYS    HSI_VALUE             /*!< Select HSI as CK_SYS source                              */
  #else
    #error "CK_SYS clock source issue: HSI is not enable!"
  #endif
#elif (HCLK_SRC == 6)
  #if (LSE_ENABLE == 1)
    #define __CK_SYS    LSE_VALUE             /*!< Select LSE as CK_SYS source                              */
  #else
    #error "CK_SYS clock source issue: LSE is not enable!"
  #endif
#elif (HCLK_SRC == 7)
  #if (LSI_ENABLE == 1)
    #define __CK_SYS    LSI_VALUE             /*!< Select LSI as CK_SYS source                              */
  #else
    #error "CK_SYS clock source issue: LSI is not enable!"
  #endif
#else
  #error "CK_SYS clock source issue: No clock source is selected!"
#endif

/**
 * @brief CK_AHB definition
 */
#define __CK_AHB    (__CK_SYS >> HCLK_DIV)    /*!< Get CK_AHB frequency                                     */

#define CKAHB_MIN 1000UL
#define CKAHB_MAX 60000000UL
#define WS0_CLK   20000000UL
#define WS1_CLK   40000000UL

/* Check CK_AHB frequency                                                                                   */
#if ((__CK_AHB < CKAHB_MIN) || (__CK_AHB > CKAHB_MAX))
  #error "CK_AHB clock issue: must be in the range!"
#endif

/* Check FLASH wait-state setting                                                                           */
#if ((__CK_AHB > WS1_CLK) && (WAIT_STATE < 2) || \
     (__CK_AHB > WS0_CLK) && (WAIT_STATE < 1))
  #error "FLASH wait state configuration issue!"
#endif
/**
  * @}
  */

/** @addtogroup HT32F5xxxx_System_Private_Variables
  * @{
  */
__IO uint32_t SystemCoreClock = __CK_AHB;   /*!< SystemCoreClock = CK_AHB                                   */
/**
  * @}
  */

/** @addtogroup HT32Fxxxx_Load_Trim_Code
  * @{
  */
#if !defined(DISABLE_TRIMCODE) && defined(USE_HT32F64951)
  #define TRIM_CODE
#endif

#if defined(TRIM_CODE)
  __STATIC_FORCEINLINE void SystemLoadTrimCode(void);
#endif
/**
  * @}
  */

/** @addtogroup HT32F5xxxx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system.
  *         Initializes the system clocks and the embedded Flash.
  * @note   This function should be used after reset.
  * @retval None
  */
void SystemInit(void)
{
#if (WDT_ENABLE == 1)
  HT_CKCU->APBCCR1 |= (0x1 << 4);
  HT_WDT->PR = 0x35CA;
  HT_WDT->MR0 = 0;
  HT_WDT->MR1 = ((HT_WDT->MR1 & 0xFFF) | (WDT_PRESCALER << 12));
  HT_WDT->MR0 = WDT_RELOAD | (WDT_RESET_ENABLE << 13) | (WDT_SLEEP_HALT << 14) | (0x1 << 16);
  HT_WDT->CR = 0x5FA00001;
#else
  #if (DEINIT_ENABLE == 1)
  HT_RSTCU->APBPRST1 = (1 << 4);
  #endif
#endif

  SetBit_BB((u32)(&HT_CKCU->APBCCR1), 6);                       /* enable VDD power domain register clock   */

  #if (DEINIT_ENABLE == 1)
  /* De-init the setting                                                                                    */
  HT_CKCU->AHBCCR &= ~(0x1 << 11);                              /* disable IP who may use PLL as source     */
  SetBit_BB((u32)(&HT_CKCU->GCCR), 11);                         /* enable HSI                               */
  while (!GetBit_BB((u32)(&HT_CKCU->GCSR), 3));                 /* wait for HSI ready                       */
  HT_CKCU->GCCR = ((HT_CKCU->GCCR & ~7UL) | 3UL);               /* select CK_SYS source                     */
  while ((HT_CKCU->CKST & 7UL) != 3UL);                         /* wait for clock switch complete           */
  HT_FLASH->CFCR = (((HT_FLASH->CFCR) & ~7UL) | 1UL);           /* set Wait State as 0 WS                   */
  HT_CKCU->AHBCFGR = 0;                                         /* set CK_AHB prescaler                     */
  ResetBit_BB((u32)(&HT_CKCU->GCCR), 9);                        /* disable PLL                              */
  SetBit_BB((u32)(&HT_CKCU->GCFGR), 8);                         /* select PLL source as HSI                 */
  #endif

  /* HSE initiation                                                                                         */
#if (HSE_ENABLE == 1)
  SetBit_BB((u32)(&HT_CKCU->GCCR), 10);                         /* enable HSE                               */
  while (!GetBit_BB((u32)(&HT_CKCU->GCSR), 2)){};               /* wait for HSE ready                       */
#endif

  /* LSE initiation                                                                                         */
#if (LSE_ENABLE == 1)
  do {
    SetBit_BB((u32)(&HT_RTC->CR), 3);                           /* enable LSE                               */
  } while (!GetBit_BB((u32)(&HT_RTC->CR), 3));
  while (!GetBit_BB((u32)(&HT_CKCU->GCSR), 4));                 /* wait for LSE ready                       */
#endif

  ResetBit_BB((u32)(&HT_CKCU->APBCCR1), 6);                     /* disable VDD power domain register clock  */

  /* LSI initiation                                                                                         */
#if (HCLK_SRC == 7)
  while (!GetBit_BB((u32)(&HT_CKCU->GCSR), 5)){};               /* wait for LSI ready                       */
#endif

  /* PLL initiation                                                                                         */
#if (PLL_ENABLE == 1)
  HT_CKCU->PLLCFGR = ((PLL_NF2_DIV & 0x0F) << 23) | (PLL_NO2_DIV << 21);  /* set PLL divider                */

  #if (PLL_CLK_SRC_DIV == 1)
    SetBit_BB((u32)(&HT_CKCU->PLLCFGR), 28);                    /* set PLL clock source divider             */
  #else
    ResetBit_BB((u32)(&HT_CKCU->PLLCFGR), 28);                  /* reset PLL clock source divider           */
  #endif

  #if (PLL_CLK_SRC == 0)
    ResetBit_BB((u32)(&HT_CKCU->GCFGR), 8);                     /* select PLL source as HSE                 */
  #else
    SetBit_BB((u32)(&HT_CKCU->GCFGR), 8);                       /* select PLL source as HSI                 */
  #endif

  SetBit_BB((u32)(&HT_CKCU->GCCR), 9);                          /* enable PLL                               */
  while (!GetBit_BB((u32)(&HT_CKCU->GCSR), 1)){};               /* wait for PLL ready                       */
#endif

  /* CK_AHB initiation                                                                                      */
#if (WAIT_STATE == 9)
  #if (__CK_AHB > WS1_CLK)
    HT_FLASH->CFCR = (((HT_FLASH->CFCR) & ~7UL) | 3UL);         /* auto-select wait state                   */
  #elif (__CK_AHB > WS0_CLK)
    HT_FLASH->CFCR = (((HT_FLASH->CFCR) & ~7UL) | 2UL);         /* auto-select wait state                   */
  #endif
#else
  HT_FLASH->CFCR = (((HT_FLASH->CFCR) & ~7UL) | (WAIT_STATE + 1));        /* manual wait state              */
#endif

  HT_CKCU->AHBCFGR = HCLK_DIV;                                  /* set CK_AHB prescaler                     */
  HT_CKCU->GCCR = ((HT_CKCU->GCCR & ~7UL) | HCLK_SRC);          /* select CK_SYS source                     */
  while ((HT_CKCU->CKST & 7UL) != HCLK_SRC);                    /* wait for clock switch complete           */

  /* Pre-fetch buffer configuration                                                                         */
#if (PRE_FETCH_ENABLE == 0)
  ResetBit_BB((u32)(&HT_FLASH->CFCR), 4);               /* 0: pre-fetch disable, 1: pre-fetch enable        */
#else
  SetBit_BB((u32)(&HT_FLASH->CFCR), 4);                 /* 0: pre-fetch disable, 1: pre-fetch enable        */
#endif

  /* HSE power down                                                                                         */
#if ((HSE_ENABLE == 0) && (HCLK_SRC != 2) && ((PLL_ENABLE == 0) || (PLL_CLK_SRC == 1)))
  ResetBit_BB((u32)(&HT_CKCU->GCCR), 10);
#endif

  /* HSI power down                                                                                         */
#if ((HSI_ENABLE == 0) && (HCLK_SRC != 3) && ((PLL_ENABLE == 0) || (PLL_CLK_SRC == 0)))
  ResetBit_BB((u32)(&HT_CKCU->GCCR), 11);
#endif

#if defined(TRIM_CODE)
  SystemLoadTrimCode();
#endif
}

/**
  * @brief  Update SystemCoreClock
  * @retval None
  */
void SystemCoreClockUpdate(void)
{
  u32 SystemCoreClockDiv = HT_CKCU->AHBCFGR & 7UL;
  u32 PllSourceClockDiv = (HT_CKCU->PLLCFGR >> 28) & 1UL;
  u32 PllFeedbackClockDiv = (((HT_CKCU->PLLCFGR >> 23) & 15UL) == 0) ? (16) : ((HT_CKCU->PLLCFGR >> 23) & 15UL);
  u32 PllOutputClockDiv = (HT_CKCU->PLLCFGR >> 21) & 3UL;
  u32 SystemCoreClockSrc = HT_CKCU->CKST & 7UL;

  /* Get system core clock according to global clock control & configuration registers                      */
  if (SystemCoreClockSrc == 1)
  {
    if (GetBit_BB((u32)(&HT_CKCU->PLLCR), 31))
    {
      PllFeedbackClockDiv = 1;
      PllOutputClockDiv = 0;
    }

    if (GetBit_BB((u32)(&HT_CKCU->GCFGR), 8))
    {
      SystemCoreClock = (((HSI_VALUE >> PllSourceClockDiv) * PllFeedbackClockDiv) >> PllOutputClockDiv) >> SystemCoreClockDiv;
    }
    else
    {
      SystemCoreClock = (((HSE_VALUE >> PllSourceClockDiv) * PllFeedbackClockDiv) >> PllOutputClockDiv) >> SystemCoreClockDiv;
    }
  }
  else if (SystemCoreClockSrc == 2)
  {
    SystemCoreClock = HSE_VALUE >> SystemCoreClockDiv;
  }
  else if (SystemCoreClockSrc == 3)
  {
    SystemCoreClock = HSI_VALUE >> SystemCoreClockDiv;
  }
  else if (SystemCoreClockSrc == 6)
  {
    SystemCoreClock = LSE_VALUE >> SystemCoreClockDiv;
  }
  else if (SystemCoreClockSrc == 7)
  {
    SystemCoreClock = LSI_VALUE >> SystemCoreClockDiv;
  }
}

#if defined(TRIM_CODE)
/* Trim Code definition                                                                                     */
#define TRIM_CODE_CKCU_APBCCR0_AFIO         0x00004000 /*!< CKCU AFIO IP Clock                              */

  #define TRIM_CODE_CKCU_APBCCR0_I2Cx       0x00000002 /*!< CKCU I2C IP Clock                               */
  #define TRIM_CODE_CKCU_AHBCCR_GPIOx       0x00040000 /*!< CKCU GPIO IP Clock                              */
  #define TRIM_CODE_RSTCU_APBPRST0_I2Cx     0x00000002 /*!< RSTCU I2C IP Reset                              */
  #define TRIM_CODE_RSTCU_AHBPRST_GPIOx     0x00000400 /*!< RSTCU GPIO IP Reset                             */
  #define TRIM_CODE_I2Cx                    HT_I2C1
  #define TRIM_CODE_GPIOx                   HT_GPIOC
  #define TRIM_CODE_GPxCFGR                 GPCCFGR
  #define TRIM_CODE_GPIOx_SCL_PIN           (4)
  #define TRIM_CODE_GPIOx_SDA_PIN           (5)

#define TRIM_CODE_AFIO_VALUE                (0x7UL << (4 * (TRIM_CODE_GPIOx_SCL_PIN % 8)) | 0x7UL << (4 * (TRIM_CODE_GPIOx_SDA_PIN % 8)))
#define TRIM_CODE_GPIOx_PIN                 ((1 << TRIM_CODE_GPIOx_SCL_PIN) | (1 << TRIM_CODE_GPIOx_SDA_PIN))

#define TRIM_CODE_SLAVE_ADDRESS             0x1F       /*!< AFE I2C Slave Address                           */
#define TRIM_CODE_REG_ADDRESS               0x12       /*!< AFE Register Address                            */

#define TRIM_CODE_TX_LENGTH                 (3)
#define TRIM_CODE_RX_LENGTH                 TRIM_CODE_TX_LENGTH - 1

#define TRIM_CODE_ISINK_MASK                0x3F
#define TRIM_CODE_LIRC_MASK                 0xFF

/* CKCU                                                                                                     */
#define CKCU_AHBCCR_DEFAULT                 (0x00000065)
#define CKCU_APBCCR0_DEFAULT                (0x00000000)

/* I2C                                                                                                      */
#define I2C_WRITE                           (0x00000000)
#define I2C_READ                            (0x00000400)

#define I2C_SEND_ADDR(add_dir)              (TRIM_CODE_I2Cx->TAR = (add_dir))
#define I2C_SEND(data)                      (TRIM_CODE_I2Cx->DR  = (data))
#define I2C_RECV()                          (TRIM_CODE_I2Cx->DR)

#define I2C_STOP()                          (TRIM_CODE_I2Cx->CR  =  0xA)
#define I2C_ACK_EN()                        (TRIM_CODE_I2Cx->CR  =  0x9)
#define I2C_ACK_DIS()                       (TRIM_CODE_I2Cx->CR  =  0x8)

#define I2C_STATUS_TX_EMPTY                 ((u32)0x003A0000)
#define I2C_STATUS_RX_NOT_EMPTY             ((u32)0x00190000)
#define I2C_STATUS_STOP_FINISH              ((u32)0x00000002)
#define I2C_STATUS_RX_NACK                  ((u32)0x00000200)

#define I2C_CONFIG_CLOCK_SPEED              (100000)
#define I2C_CONFIG_SHPG                     (((__CK_AHB * 9)  / (20 * I2C_CONFIG_CLOCK_SPEED)) - 6)
#define I2C_CONFIG_SLPG                     (((__CK_AHB * 11) / (20 * I2C_CONFIG_CLOCK_SPEED)) - 6)
#define I2C_CONFIG_ENABLE                   0x00000008

#define I2C_WRITE_ADDR                      (TRIM_CODE_SLAVE_ADDRESS | I2C_WRITE)
#define I2C_READ_ADDR                       (TRIM_CODE_SLAVE_ADDRESS | I2C_READ)

/* SysTick                                                                                                  */
#if (LTC_ERRORHANDLING == 1)
  #define TICK_CTRL_DEFAULT                 0x00000000
  #define TICK_LOAD_DEFAULT                 0x00FFFFFF
  #define TICK_OFFSET                       (17)
  #define TICK_US                           (__CK_AHB / 1000000)
  #define US2TICK(us)                       (us * TICK_US - TICK_OFFSET)

  #define SET_TICK_TIMEOUT(cnt)             {SysTick->LOAD = cnt; \
                                             SysTick->VAL  = 0; \
                                             SysTick->CTRL;}
  #define IS_TICK_TIMEOUT()                 (SysTick->CTRL != 0x5)
#endif

#if (LTC_ERRORHANDLING == 1)
  /* Error Handling                                                                                         */
  #define ERROR_STATUS_SLAVE_HOLD_BUS       (1 << 0)
  #define ERROR_STATUS_TX_EMPTY_NACK        (1 << 1)
  #define ERROR_STATUS_TX_EMPTY_TIMEOUT     (1 << 2)
  #define ERROR_STATUS_SEND_STOP_TIMEOUT    (1 << 3)
  #define ERROR_STATUS_RX_NOT_EMPTY_TIMEOUT (1 << 4)
  #define ERROR_STATUS_TRIM_CODE_FAILED     (1 << 5)
#endif

#if (LTC_ERRORHANDLING == 1)
  #define TICK_I2C_BUS_BUSY_TIMEOUT         (30000)  /*!< Wait about 30 ms                                  */
  #define TICK_I2C_RTX_EMPTY_TIMEOUT        (450)    /*!< Wait about 5-byte time (5 * 9 bits)               */
  #define TICK_I2C_SEND_STOP_TIMEOUT        (180)    /*!< Wait about 2-byte time                            */
#endif

#if (LTC_ERRORHANDLING == 1)
u32 Error_Status;
/* Wait                                                                                                     */
/**
  * @brief  Wait until specified I2C status TX flag is set or timeout/error occurs
  */
static void WAIT_RTX(u32 flag, u32 timeout_err)
{
  SET_TICK_TIMEOUT(US2TICK(TICK_I2C_RTX_EMPTY_TIMEOUT));
  while ((TRIM_CODE_I2Cx->SR & flag) != flag)
  {
    if (TRIM_CODE_I2Cx->SR & I2C_STATUS_RX_NACK)
    {
      Error_Status = ERROR_STATUS_TX_EMPTY_NACK;
      break;
    }
    if (IS_TICK_TIMEOUT())
    {
      Error_Status = timeout_err;
      break;
    }
  }
}

/**
  * @brief  Wait until I2C STOP condition is completed
  */
static void WAIT_STOP(void)
{
  SET_TICK_TIMEOUT(US2TICK(TICK_I2C_SEND_STOP_TIMEOUT));
  while (TRIM_CODE_I2Cx->CR & I2C_STATUS_STOP_FINISH)
  {
    if (IS_TICK_TIMEOUT())
    {
      Error_Status = ERROR_STATUS_SEND_STOP_TIMEOUT;
      break;
    }
  }
}

#define WAIT_TX()          WAIT_RTX(I2C_STATUS_TX_EMPTY, ERROR_STATUS_TX_EMPTY_TIMEOUT)
#define WAIT_RX()          WAIT_RTX(I2C_STATUS_RX_NOT_EMPTY, ERROR_STATUS_RX_NOT_EMPTY_TIMEOUT)

#define ERR_CONTINUE()     if (Error_Status) continue;     
#define ERR_BREAK()        if (Error_Status) break;  
#else
  #define WAIT_TX()        while ((TRIM_CODE_I2Cx->SR & I2C_STATUS_TX_EMPTY) != I2C_STATUS_TX_EMPTY) {}
  #define WAIT_RX()        while ((TRIM_CODE_I2Cx->SR & I2C_STATUS_RX_NOT_EMPTY) != I2C_STATUS_RX_NOT_EMPTY) {}
  #define WAIT_STOP()      while (TRIM_CODE_I2Cx->CR & I2C_STATUS_STOP_FINISH) {}

  #define ERR_CONTINUE(...)  
  #define ERR_BREAK(...)
#endif

/**
  * @brief  Load Trim Code
  * @note   This function uses AFIO, GPIOx, I2Cn, and SysTick peripherals.
            After communication, it restores peripheral enable configuration and SysTick configuration,
            resets I2Cn, and returns all involved I/O pins to their default configuration.
  * @retval None
  */
__STATIC_FORCEINLINE void SystemLoadTrimCode(void)
{
  #if (LTC_ERRORHANDLING == 0)
  u32 ok;                                                    /* Simple verification flag                    */
  #endif

  u32 Retry = ERROR_COUNTER_LIMIT;
  u32 tx[TRIM_CODE_TX_LENGTH];                               /* TX buffer: [RegAddr, ISINK, LIRC]           */

  /* Prepare Trim Code Data                                                                                 */
  u32 trim = HT_FLASH->CID[3];                               /* Read trim value from FMC                    */
  tx[0] = TRIM_CODE_REG_ADDRESS;                             /* Target register address                     */
  tx[1] = (trim >> 8) & TRIM_CODE_ISINK_MASK;                /* Extract ISINK calibration                   */
  tx[2] = trim & TRIM_CODE_LIRC_MASK;                        /* Extract LIRC calibration                    */

  #if (LTC_ERRORHANDLING == 1)
  /* Initialize SysTick for timeout mechanism                                                               */
  SysTick->CTRL  = 0x4;                                      /* SysTick clock source for AHB clock          */
  SysTick->LOAD  = TICK_LOAD_DEFAULT;                        /* Set SysTick reload value to default         */
  SysTick->VAL   = 0;                                        /* Clear SysTick counter value to 0            */
  SysTick->CTRL |= 0x1;                                      /* Enable SysTick counter                      */
  SysTick->CTRL;                                             /* Fix the timing issue for timing function    */
  #endif

  /* Enable required peripheral clocks                                                                      */
  #if (LTC_ERRORHANDLING == 1)
  HT_CKCU->AHBCCR  = CKCU_AHBCCR_DEFAULT | TRIM_CODE_CKCU_AHBCCR_GPIOx;           /* Enable GPIOx           */
  #endif
  HT_CKCU->APBCCR0 = (TRIM_CODE_CKCU_APBCCR0_AFIO | TRIM_CODE_CKCU_APBCCR0_I2Cx); /* Enable AFIO + I2Cx     */

  /* Configure GPIO pins to I2C function                                                                    */
  #if (TRIM_CODE_GPIOx_SCL_PIN < 8) || (TRIM_CODE_GPIOx_SDA_PIN < 8)
  HT_AFIO->TRIM_CODE_GPxCFGR[0] = TRIM_CODE_AFIO_VALUE;
  #endif
  #if (TRIM_CODE_GPIOx_SCL_PIN > 8) || (TRIM_CODE_GPIOx_SDA_PIN > 8)
  HT_AFIO->TRIM_CODE_GPxCFGR[1] = TRIM_CODE_AFIO_VALUE;
  #endif

  #if (LTC_ERRORHANDLING == 1)
  /* Check if I2C bus is free                                                                               */
  TRIM_CODE_GPIOx->INER  |=  TRIM_CODE_GPIOx_PIN;
  #endif

  /* Retry loop                                                                                             */
  do
  {
    #if (LTC_ERRORHANDLING == 1)
    Error_Status = 0;
    #else
    ok = 1;
    #endif

    /* Initialize I2C peripheral                                                                            */
    HT_RSTCU->APBPRST0 = TRIM_CODE_RSTCU_APBPRST0_I2Cx;      /* Reset I2Cx                                  */
    while (HT_RSTCU->APBPRST0){};                            /* Wait reset done                             */

    TRIM_CODE_I2Cx->SHPGR = I2C_CONFIG_SHPG;                 /* SCL high period                             */
    TRIM_CODE_I2Cx->SLPGR = I2C_CONFIG_SLPG;                 /* SCL low period                              */
    TRIM_CODE_I2Cx->CR    = I2C_CONFIG_ENABLE;               /* Enable I2C master                           */

    #if (LTC_ERRORHANDLING == 1)
    /* Check I2C Bus Status                                                                                 */
    SET_TICK_TIMEOUT(US2TICK(TICK_I2C_BUS_BUSY_TIMEOUT));
    while ((TRIM_CODE_GPIOx->DINR & TRIM_CODE_GPIOx_PIN) != TRIM_CODE_GPIOx_PIN)
    {
      if (IS_TICK_TIMEOUT())
      {
        Error_Status = ERROR_STATUS_SLAVE_HOLD_BUS;
        break;
      }
    }
    if (Error_Status)
    {
      Retry = 0;                                             /* Fatal error -> stop retry                   */
      break;
    }
    #endif

    /* I2C WRITE: send trim code                                                                            */
    {
      u32 *p = tx;
      u32 Tx_Length = TRIM_CODE_TX_LENGTH;

      I2C_SEND_ADDR(I2C_WRITE_ADDR);                         /* Send slave address (write)                  */

      do
      {
        WAIT_TX();
        ERR_BREAK();
        I2C_SEND(*p++);                                      /* Send byte                                   */
      } while (--Tx_Length);

      ERR_CONTINUE();
      WAIT_TX();
      ERR_CONTINUE();

      I2C_STOP();                                            /* Generate STOP condition                     */
      WAIT_STOP();
      ERR_CONTINUE();
    }

    /* I2C VERIFY: read back and compare                                                                    */
    {
      u32 *p = tx;
      u32 Rx_Length = TRIM_CODE_RX_LENGTH;
      /* Set register address                                                                               */
      I2C_SEND_ADDR(I2C_WRITE_ADDR);                         /* Send slave address (write)                  */
      WAIT_TX();
      ERR_CONTINUE();

      I2C_SEND(*p++);                                        /* Send register address                       */
      WAIT_TX();
      ERR_CONTINUE();

      /* Read back data                                                                                     */
      I2C_SEND_ADDR(I2C_READ_ADDR);                          /* Send slave address (read)                   */
      I2C_ACK_EN();

      do
      {
        if (Rx_Length == 1) I2C_ACK_DIS();                   /* Last byte -> NACK                           */
        WAIT_RX();
        ERR_BREAK();

        /* Compare received data                                                                            */
        if (I2C_RECV() != *p++)
        {
          #if (LTC_ERRORHANDLING == 1)
          Error_Status = ERROR_STATUS_TRIM_CODE_FAILED;
          #else
          ok = 0;
          #endif
        }
      } while (--Rx_Length);
      ERR_CONTINUE();

      #if (LTC_ERRORHANDLING == 0)
      if (!ok) continue;
      #endif

      I2C_STOP();
      WAIT_STOP();
      ERR_CONTINUE();
    }

    break;                                                   /* Success                                     */
  } while (--Retry);

  /* Fatal failure handling                                                                                 */
  if (!Retry)
  {
    while (1)
    {
      #if (LTC_ERRORHANDLING == 1)
      /* !!! Notice !!!
        Check the "Error_Status" bits to identify the Load Trim Code failure reason:
          bit0: Slave Hold Bus - Slave does not respond within timeout period.
          bit1: Slave Receive NACK - Slave did not acknowledge the transmitted data/address.
          bit2: Master TX_EMPTY timeout - Master waiting for TX buffer empty event timed out.
          bit3: Master TX_STOP timeout - Master waiting for STOP condition completion timed out.
          bit4: Master RX_NOT_EMPTY timeout - Master waiting for RX buffer not empty event timed out.
          bit5: Multiple comparisons of Trim Code values failed.
      */
      #else
      /* !!! Notice !!!
        Multiple comparisons of Trim Code values failed.
      */
      #endif
    }
  }

  /* Restore system state                                                                                   */
  #if (TRIM_CODE_GPIOx_SCL_PIN < 8) || (TRIM_CODE_GPIOx_SDA_PIN < 8)
  HT_AFIO->TRIM_CODE_GPxCFGR[0] = 0;
  #endif
  #if (TRIM_CODE_GPIOx_SCL_PIN > 8) || (TRIM_CODE_GPIOx_SDA_PIN > 8)
  HT_AFIO->TRIM_CODE_GPxCFGR[1] = 0;
  #endif

  #if (LTC_ERRORHANDLING == 1)
  /* Disable SysTick                                                                                        */
  SysTick->CTRL = TICK_CTRL_DEFAULT;
  #endif

  #if 0
    /* !!! Notice !!!
      Set to 1 to restore other IPs used during Trim Code transmission after completion.
    */
    #if (LTC_ERRORHANDLING == 1)
    /* Reset GPIO                                                                                           */
    HT_RSTCU->AHBPRST  = TRIM_CODE_RSTCU_AHBPRST_GPIOx;
    while (HT_RSTCU->AHBPRST){};
    #endif
    /* Reset I2C                                                                                            */
    HT_RSTCU->APBPRST0 = TRIM_CODE_RSTCU_APBPRST0_I2Cx;
    while (HT_RSTCU->APBPRST0){};

    /* Restore CKCU                                                                                         */
    #if (LTC_ERRORHANDLING == 1)
    HT_CKCU->AHBCCR  = CKCU_AHBCCR_DEFAULT;
    #endif
    HT_CKCU->APBCCR0 = CKCU_APBCCR0_DEFAULT;
  #endif
}
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

/******************* (C) COPYRIGHT Holtek Semiconductor Inc. *****END OF FILE***                            */
