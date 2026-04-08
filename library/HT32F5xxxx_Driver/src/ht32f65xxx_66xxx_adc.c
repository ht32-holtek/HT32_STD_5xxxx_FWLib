/*********************************************************************************************************//**
 * @file    ht32f65xxx_66xxx_adc.c
 * @version $Rev:: 9671         $
 * @date    $Date:: 2026-03-04 #$
 * @brief   This file provides all the ADC firmware functions.
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

/* Includes ------------------------------------------------------------------------------------------------*/
#include "ht32f65xxx_66xxx_adc.h"

/** @addtogroup HT32F5xxxx_Peripheral_Driver HT32F5xxxx Peripheral Driver
  * @{
  */

/** @defgroup ADC ADC
  * @brief ADC driver modules
  * @{
  */


/* Private constants ---------------------------------------------------------------------------------------*/
/** @defgroup ADC_Private_Define ADC private definitions
  * @{
  */
#define ADC_ENABLE_BIT       (0x00000080)

#if (LIBCFG_ADC1)
#define DUAL_MODE_MASK       (0x00000003)
#endif

#define ADC_SOFTWARE_RESET   (0x00000001)
#define LST_SEQ_SET          (0x0000001F)
#define TCR_SC_SET           (0x00000001)

#define HLST_SEQ_SET         (0x0000001F)
#define HTCR_SC_SET          (0x00000001)

#if (!LIBCFG_ADC_NO_OFFSET_REG)
#define OFR_ADOF_MASK        (0x00000FFF)
#define OFR_ADAL             (1 << 14)
#define OFR_ADOFE            (1 << 15)
#endif

#if (LIBCFG_ADC_MVDDA)
#define ADC_VREF_MVDDAEN     (0x00000100)
#endif

#if (LIBCFG_ADC_TEMP_SENSOR)
#define ADC_TEMP_SENSOR_ENABLE        (0x00000001)
#define ADC_TEMP_SENSOR_START_OPERATE (0x00000002)

#define ADC_POS_TSCAL        (0)
#define ADC_MASK_TSCAL       ((u32)0xFFF << ADC_POS_TSCAL)
#endif

#if (LIBCFG_ADC_FLEXIBLE_VREFVALR)
#define ADC_VREFVALR_KEY     (0x9C3A0000)
#endif

/**
  * @}
  */

/* Global functions ----------------------------------------------------------------------------------------*/
/** @addtogroup ADC_Exported_Functions ADC exported functions
  * @{
  */
/*********************************************************************************************************//**
 * @brief Deinitialize the HT_ADCn peripheral registers to their default reset values.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @retval None
 ************************************************************************************************************/
void ADC_DeInit(HT_ADC_TypeDef* HT_ADCn)
{
  RSTCU_PeripReset_TypeDef RSTCUReset = {{0}};

  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));

  if (HT_ADCn == HT_ADC0)
  {
    RSTCUReset.Bit.ADC0 = 1;
  }
  #if (LIBCFG_ADC1)
  else if (HT_ADCn == HT_ADC1)
  {
    RSTCUReset.Bit.ADC1 = 1;
  }
  #endif
  RSTCU_PeripReset(RSTCUReset, ENABLE);
}

/*********************************************************************************************************//**
 * @brief Reset ADC.
 * @param HT_ADCn: is the selected ADC from the ADC peripherals.
 * @retval None
 ************************************************************************************************************/
void ADC_Reset(HT_ADC_TypeDef* HT_ADCn)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));

  HT_ADCn->RST |= ADC_SOFTWARE_RESET;
}

/*********************************************************************************************************//**
 * @brief Enable or Disable the specified ADC.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param NewState: This parameter can be ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void ADC_Cmd(HT_ADC_TypeDef* HT_ADCn, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    HT_ADCn->CONV |= ADC_ENABLE_BIT;
  }
  else
  {
    HT_ADCn->CONV &= ~(ADC_ENABLE_BIT);
  }
}

#if (LIBCFG_ADC1)
/*********************************************************************************************************//**
 * @brief Configure the ADC dual mode.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals. (ADC1 only)
 * @param DUAL_X: ADC dual mode configuration.
 *   This parameter can be one of the following values:
 *     @arg DUAL_INDEPENDENT                : Independent mode (dual mode off).
 *     @arg DUAL_CASCADE_REGULAR            : Cascade mode in regular conversion.
 *     @arg DUAL_CASCADE_REGULAR_H_PRIORITY : Cascade mode in regular/high priority conversion.
 * @param HDelayTime: High priority ADC trigger delay.
 *   This parameter must be between 0x00 to 0xFF.
 * @param DelayTime: Regular ADC trigger delay.
 *   This parameter must be between 0x00 to 0xFF.
 * @retval None
 ************************************************************************************************************/
void ADC_DualModeConfig(HT_ADC_TypeDef* HT_ADCn, u32 DUAL_X, u8 HDelayTime, u8 DelayTime)
{
  u32 uTmpReg1, uTmpReg2;
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_DUAL_MODE(DUAL_X));

  uTmpReg1 = HT_ADCn->CFGR;
  uTmpReg2 = (DUAL_MODE_MASK | 0x00FFFF00);
  uTmpReg1 &= ~uTmpReg2;
  uTmpReg2 = (DUAL_X | HDelayTime << 16 | DelayTime << 8);
  uTmpReg1 |= uTmpReg2;

  HT_ADCn->CFGR = uTmpReg1;
}
#endif

/*********************************************************************************************************//**
 * @brief Configure conversion mode and length of list queue for regular group.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_MODE: ADC Cyclic Conversion Mode.
 *   This parameter can be one of the following values:
 *     @arg ONE_SHOT_MODE      :
 *     @arg CONTINUOUS_MODE    :
 *     @arg DISCONTINUOUS_MODE :
 * @param Length: must between 1 ~ 8
 * @param SubLength: must between 1 ~ 8, only valid for DISCONTINUOUS_MODE.
 * @retval None
 ************************************************************************************************************/
void ADC_RegularGroupConfig(HT_ADC_TypeDef* HT_ADCn, u8 ADC_MODE, u8 Length, u8 SubLength)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_CONVERSION_MODE(ADC_MODE));
  Assert_Param(IS_ADC_REGULAR_LENGTH(Length));
  if (ADC_MODE == DISCONTINUOUS_MODE)
  {
    Assert_Param(IS_ADC_REGULAR_SUB_LENGTH(SubLength));
  }

  /* Config cyclic conversion mode and length of list queue and sub length for regular group                */
  HT_ADCn->CONV = ((u32)(SubLength - 1) << 16) | ((u32)(Length - 1) << 8) | ADC_MODE | (HT_ADCn->CONV & ADC_ENABLE_BIT);
}

/*********************************************************************************************************//**
 * @brief Configure conversion mode and length of list queue for high priority group.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_MODE: ADC Cyclic Conversion Mode.
 *   This parameter can be one of the following values:
 *     @arg ONE_SHOT_MODE      :
 *     @arg CONTINUOUS_MODE    :
 *     @arg DISCONTINUOUS_MODE :
 * @param Length: must between 1 ~ 12 (5 ~ 12 only for specific model)
 * @param SubLength: must between 1 ~ 12 (5 ~ 12 only for specific model)
 * @retval None
 ************************************************************************************************************/
void ADC_HPGroupConfig(HT_ADC_TypeDef* HT_ADCn, u8 ADC_MODE, u8 Length, u8 SubLength)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_CONVERSION_MODE(ADC_MODE));
  Assert_Param(IS_ADC_HP_LENGTH(Length));
  Assert_Param(IS_ADC_HP_SUB_LENGTH(SubLength));

  /* Config cyclic conversion mode and length of list queue and sub length for regular group                */
  HT_ADCn->HCONV = ((u32)(SubLength - 1) << 16) | ((u32)(Length - 1) << 8) | ADC_MODE;
}

/*********************************************************************************************************//**
 * @brief Configure the corresponding rank in the sequencer and the sampling time for the regular channel
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_CH_n: the ADC channel to configure
 *   This parameter can be one of the following values:
 *     @arg ADC_CH_n        : ADCn Channel m selected, n must between 0 ~ 7
 *     @arg ADC_CH_OPA0     : ADCn OPA0O selected
 *     @arg ADC_CH_OPA1     : ADCn OPA1O selected
 *     @arg ADCn_CH_PGA0O   : ADCn PGA0O selected, n must between 0 ~ 1
 *     @arg ADCn_CH_PGA1O   : ADCn PGA1O selected, n must between 0 ~ 1
 *     @arg ADCn_CH_PGA2O   : ADCn PGA2O selected, n must between 0 ~ 1
 *     @arg ADCn_CH_PGA3O   : ADCn PGA3O selected, n must between 0 ~ 1
 *     @arg ADCn_CH_PGA4O   : ADCn PGA4O selected, n must between 0 ~ 1
 *     @arg ADCn_CH_PGA5O   : ADCn PGA5O selected, n must between 0 ~ 1
 *     @arg ADCn_CH_VTS     : ADCn temperature sensor selected, n must between 0 ~ 1
 *     @arg ADC_CH_GND_VREF : ADCn GND VREF selected
 *     @arg ADC_CH_VDD_VREF : ADCn VDD VREF selected
 *     @arg ADC_CH_BANDGAP  : ADCn BANDGAP selected
 *     @arg ADC_CH_MVDDA    : ADCn MVDDA selected
 * @param Rank: The rank in the regular group sequencer.
 *   This parameter must be between 0 to 7.
 * @param SampleClock: Number of sampling clocks.
 *   This parameter must be between 0x00 to 0xFF.
 * @retval None
 ************************************************************************************************************/
void ADC_RegularChannelConfig(HT_ADC_TypeDef* HT_ADCn, u8 ADC_CH_n, u8 Rank, u8 SampleClock)
{
  u32 tmpreg1, tmpreg2;

  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_CHANNEL(ADC_CH_n));
  Assert_Param(IS_ADC_REGULAR_RANK(Rank));
  Assert_Param(IS_ADC_INPUT_SAMPLING_TIME(SampleClock));

  #if (HT32_LIB_LITE == 0)
  #if (LIBCFG_ADC_CH_CHECK_CHOOSE)
  if (IS_ADC_CH_CHOOSE(HT_ADCn, ADC_CH_n) == FALSE)
  {
    while(1){}; // The selected channel does not belong to the specified ADCn
  }
  ADC_CH_n = ADC_CH_REMOVE_MARK(ADC_CH_n);
  #endif
  #endif

  /* config sampling clock of correspond ADC input channel                                                  */
  #if (LIBCFG_ADC_STR_16_17)
  if (ADC_CH_n < 16)
  {
    HT_ADCn->STR[ADC_CH_n] = SampleClock;
  }
  else
  {
    HT_ADCn->STR16[ADC_CH_n - 16] = SampleClock;
  }
  #else
  HT_ADCn->STR[ADC_CH_n] = SampleClock;
  #endif

  /* Get the old register value                                                                             */
  tmpreg1 = HT_ADCn->LST[Rank >> 2];
  /* Calculate the mask to clear                                                                            */
  tmpreg2 = LST_SEQ_SET << (8 * (Rank & 0x3));
  /* Clear the old SEQx bits for the selected rank                                                          */
  tmpreg1 &= ~tmpreg2;
  /* Calculate the mask to set                                                                              */
  tmpreg2 = (u32)ADC_CH_n << (8 * (Rank & 0x3));
  /* Set the SEQx bits for the selected rank                                                                */
  tmpreg1 |= tmpreg2;
  /* Store the new register value                                                                           */
  HT_ADCn->LST[Rank >> 2] = tmpreg1;
}

/*********************************************************************************************************//**
 * @brief Configure the corresponding rank in the sequencer and the sample time for the High Priority channel
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_CH_n: the ADC channel to configure
 *   This parameter can be one of the following values:
 *     @arg ADC_CH_n        : ADCn Channel m selected, n must between 0 ~ 7
 *     @arg ADC_CH_OPA0     : ADCn OPA0O selected
 *     @arg ADC_CH_OPA1     : ADCn OPA1O selected
 *     @arg ADCn_CH_PGA0O   : ADCn PGA0O selected, n must between 0 ~ 1
 *     @arg ADCn_CH_PGA1O   : ADCn PGA1O selected, n must between 0 ~ 1
 *     @arg ADCn_CH_PGA2O   : ADCn PGA2O selected, n must between 0 ~ 1
 *     @arg ADCn_CH_PGA3O   : ADCn PGA3O selected, n must between 0 ~ 1
 *     @arg ADCn_CH_PGA4O   : ADCn PGA4O selected, n must between 0 ~ 1
 *     @arg ADCn_CH_PGA5O   : ADCn PGA5O selected, n must between 0 ~ 1
 *     @arg ADCn_CH_VTS     : ADCn temperature sensor selected, n must between 0 ~ 1
 *     @arg ADC_CH_GND_VREF : ADCn GND VREF selected
 *     @arg ADC_CH_VDD_VREF : ADCn VDD VREF selected
 *     @arg ADC_CH_BANDGAP  : ADCn BANDGAP selected
 *     @arg ADC_CH_MVDDA    : ADCn MVDDA selected
 * @param Rank: The rank in the high priority group sequencer.
 *   This parameter must be between 0 to 11. (4 ~ 11 only for specific model)
 * @param SampleClock: Number of sampling clocks.
 *   This parameter must be between 0x00 to 0xFF.
 * @retval None
 ************************************************************************************************************/
void ADC_HPChannelConfig(HT_ADC_TypeDef* HT_ADCn, u8 ADC_CH_n, u8 Rank, u8 SampleClock)
{
  u32 tmpreg1, tmpreg2;

  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_CHANNEL(ADC_CH_n));
  Assert_Param(IS_ADC_HP_RANK(Rank));
  Assert_Param(IS_ADC_INPUT_SAMPLING_TIME(SampleClock));

  #if (HT32_LIB_LITE == 0)
  #if (LIBCFG_ADC_CH_CHECK_CHOOSE)
  if (IS_ADC_CH_CHOOSE(HT_ADCn, ADC_CH_n) == FALSE)
  {
    while(1){}; // The selected channel does not belong to the specified ADCn
  }
  ADC_CH_n = ADC_CH_REMOVE_MARK(ADC_CH_n);
  #endif
  #endif

  /* config sampling clock of correspond ADC input channel                                                  */
  #if (LIBCFG_ADC_STR_16_17)
  if (ADC_CH_n < 16)
  {
    HT_ADCn->STR[ADC_CH_n] = SampleClock;
  }
  else
  {
    HT_ADCn->STR16[ADC_CH_n - 16] = SampleClock;
  }
  #else
  HT_ADCn->STR[ADC_CH_n] = SampleClock;
  #endif
  
  /* Get the old register value                                                                             */
  #if (LIBCFG_ADC_HLST_0_2)
  tmpreg1 = HT_ADCn->HLST[Rank >> 2];
  #else
  tmpreg1 = HT_ADCn->HLST;
  #endif
  /* Calculate the mask to clear                                                                            */
  tmpreg2 = HLST_SEQ_SET << (8 * (Rank & 0x3));
  /* Clear the old SEQx bits for the selected rank                                                          */
  tmpreg1 &= ~tmpreg2;
  /* Calculate the mask to set                                                                              */
  tmpreg2 = (u32)ADC_CH_n << (8 * (Rank & 0x3));
  /* Set the SEQx bits for the selected rank                                                                */
  tmpreg1 |= tmpreg2;
  /* Store the new register value                                                                           */
  #if (LIBCFG_ADC_HLST_0_2)
  HT_ADCn->HLST[Rank >> 2] = tmpreg1;
  #else
  HT_ADCn->HLST = tmpreg1;
  #endif
}

/*********************************************************************************************************//**
 * @brief Configure the ADC trigger source for regular channels conversion.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_TRIG_x:
 *   This parameter can be one of the following values:
 *     @arg ADC_TRIG_SOFTWARE        : S/W trigger
 *     @arg ADC_TRIG_EXTI_n          : where n can be 0 ~ 15
 *     @arg ADC_TRIG_MCTMn_MEVx      : where n can be 0
 *     @arg ADC_TRIG_MCTM0_DMEVx     : where n can be 0*
 *     @arg ADC_TRIG_MCTM0_MEVxDEVz  : where n can be 0
 *     @arg ADC_TRIG_MCTMn_MTO       : where n can be 0
 *     @arg ADC_TRIG_MCTMn_CH0O      : where n can be 0
 *     @arg ADC_TRIG_MCTMn_CH1O      : where n can be 0
 *     @arg ADC_TRIG_MCTMn_CH2O      : where n can be 0
 *     @arg ADC_TRIG_MCTMn_CH3O      : where n can be 0
 *     @arg ADC_TRIG_MCTMn_CHmMEV    : where n can be 0, m can be 0 ~ 3
 *     @arg ADC_TRIG_MCTMn_CHmDEV    : where n can be 0, m can be 0 ~ 3
 *     @arg ADC_TRIG_MCTMn_CHmMDEV   : where n can be 0, m can be 0 ~ 3
 *     @arg ADC_TRIG_MCTMn_CHALLMEV  : where n can be 0
 *     @arg ADC_TRIG_MCTMn_CHALLDEV  : where n can be 0
 *     @arg ADC_TRIG_MCTMn_CHALLMDEV : where n can be 0
 *     @arg ADC_TRIG_BFTMn           : where n can be 0 ~ 1
 *     @arg ADC_TRIG_GPTMn_MTO       : where n can be 0
 *     @arg ADC_TRIG_GPTMn_CH0O      : where n can be 0
 *     @arg ADC_TRIG_GPTMn_CH1O      : where n can be 0
 *     @arg ADC_TRIG_GPTMn_CH2O      : where n can be 0
 *     @arg ADC_TRIG_GPTMn_CH3O      : where n can be 0
 *     @arg ADC_TRIG_CMPn            : where n can be 0 ~ 3
 * @retval None
 ************************************************************************************************************/
void ADC_RegularTrigConfig(HT_ADC_TypeDef* HT_ADCn, u32 ADC_TRIG_x)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_TRIG(ADC_TRIG_x));

  #if (HT32_LIB_LITE == 0)
  #if (LIBCFG_ADC_TRIG_CHECK_CHOOSE)
  if (IS_ADC_TRIG_CHOOSE(HT_ADCn, ADC_TRIG_x) == FALSE)
  {
    while(1){}; // The selected trigger does not belong to the specified ADCn
  }
  ADC_TRIG_x = ADC_TRIG_REMOVE_MARK(ADC_TRIG_x);
  #endif
  #endif

  /* Config external trigger conversion source of regular group                                             */
  HT_ADCn->TCR = ADC_TRIG_x & 0x0000001F;
  HT_ADCn->TSR = ADC_TRIG_x & (~0x0000001F);
}

/*********************************************************************************************************//**
 * @brief Configure the ADC trigger source for high priority channels conversion.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_TRIG_x:
 *   This parameter can be one of the following values:
 *     @arg ADC_TRIG_SOFTWARE        : S/W trigger
 *     @arg ADC_TRIG_EXTI_n          : where n can be 0 ~ 15
 *     @arg ADC_TRIG_MCTMn_MEVx      : where n can be 0
 *     @arg ADC_TRIG_MCTM0_DMEVx     : where n can be 0*
 *     @arg ADC_TRIG_MCTM0_MEVxDEVz  : where n can be 0
 *     @arg ADC_TRIG_MCTMn_MTO       : where n can be 0
 *     @arg ADC_TRIG_MCTMn_CH0O      : where n can be 0
 *     @arg ADC_TRIG_MCTMn_CH1O      : where n can be 0
 *     @arg ADC_TRIG_MCTMn_CH2O      : where n can be 0
 *     @arg ADC_TRIG_MCTMn_CH3O      : where n can be 0
 *     @arg ADC_TRIG_MCTMn_CHmMEV    : where n can be 0, m can be 0 ~ 3
 *     @arg ADC_TRIG_MCTMn_CHmDEV    : where n can be 0, m can be 0 ~ 3
 *     @arg ADC_TRIG_MCTMn_CHmMDEV   : where n can be 0, m can be 0 ~ 3
 *     @arg ADC_TRIG_MCTMn_CHALLMEV  : where n can be 0
 *     @arg ADC_TRIG_MCTMn_CHALLDEV  : where n can be 0
 *     @arg ADC_TRIG_MCTMn_CHALLMDEV : where n can be 0
 *     @arg ADC_TRIG_BFTMn           : where n can be 0 ~ 1
 *     @arg ADC_TRIG_GPTMn_MTO       : where n can be 0
 *     @arg ADC_TRIG_GPTMn_CH0O      : where n can be 0
 *     @arg ADC_TRIG_GPTMn_CH1O      : where n can be 0
 *     @arg ADC_TRIG_GPTMn_CH2O      : where n can be 0
 *     @arg ADC_TRIG_GPTMn_CH3O      : where n can be 0
 *     @arg ADC_TRIG_CMPn            : where n can be 0 ~ 3
 * @retval None
 ************************************************************************************************************/
void ADC_HPTrigConfig(HT_ADC_TypeDef* HT_ADCn, u32 ADC_TRIG_x)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_TRIG(ADC_TRIG_x));

  #if (HT32_LIB_LITE == 0)
  #if (LIBCFG_ADC_TRIG_CHECK_CHOOSE)
  if (IS_ADC_TRIG_CHOOSE(HT_ADCn, ADC_TRIG_x) == FALSE)
  {
    while(1){}; // The selected trigger does not belong to the specified ADCn
  }
  ADC_TRIG_x = ADC_TRIG_REMOVE_MARK(ADC_TRIG_x);
  #endif
  #endif

  HT_ADCn->HTCR = ADC_TRIG_x & 0x0000001F;
  HT_ADCn->HTSR = ADC_TRIG_x & (~0x0000001F);
}

#if (!LIBCFG_ADC_NO_OFFSET_REG)
/*********************************************************************************************************//**
 * @brief Configure the channel data alignment format.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_CH_n: the ADC channel to configure
 *   This parameter can be one of the following values:
 *     @arg ADC_CH_n        : ADC Channel x selected
 *     @arg ADC_CH_OPAn     : ADC channel for OPAn
 *     @arg ADC_CH_GND_VREF : ADC GND VREF selected
 *     @arg ADC_CH_VDD_VREF : ADC VDD VREF selected
 * @param ADC_ALIGN_x: ADC_ALIGN_RIGHT or ADC_ALIGN_LEFT
 * @retval None
 ************************************************************************************************************/
void ADC_ChannelDataAlign(HT_ADC_TypeDef* HT_ADCn, u8 ADC_CH_n, ADC_ALIGN_Enum ADC_ALIGN_x)
{
  u32 OFRValue;

  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_CHANNEL(ADC_CH_n));
  Assert_Param(IS_ADC_ALIGN(ADC_ALIGN_x));

  OFRValue = HT_ADCn->OFR[ADC_CH_n] & (~(OFR_ADAL));
  OFRValue |= ADC_ALIGN_x;

  HT_ADCn->OFR[ADC_CH_n] = OFRValue;
}

/*********************************************************************************************************//**
 * @brief Configure the offset value for channel offset cancellation.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_CH_n: the ADC channel to configure
 *   This parameter can be one of the following values:
 *     @arg ADC_CH_n        : ADC Channel x selected
 *     @arg ADC_CH_OPAn     : ADC channel for OPAn
 *     @arg ADC_CH_GND_VREF : ADC GND VREF selected
 *     @arg ADC_CH_VDD_VREF : ADC VDD VREF selected
 * @param OffsetValue: The offset value
 * @retval None
 ************************************************************************************************************/
void ADC_ChannelOffsetValue(HT_ADC_TypeDef* HT_ADCn, u8 ADC_CH_n, u16 OffsetValue)
{
  u32 OFRValue;

  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_CHANNEL(ADC_CH_n));
  Assert_Param(IS_ADC_OFFSET(OffsetValue));

  OFRValue = HT_ADCn->OFR[ADC_CH_n] & (~OFR_ADOF_MASK);
  OFRValue |= (OffsetValue & 0xFFF);
  HT_ADCn->OFR[ADC_CH_n] = OFRValue;
}

/*********************************************************************************************************//**
 * @brief Enable or Disable the channel offset cancellation function.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_CH_n: the ADC channel to configure
 *   This parameter can be one of the following values:
 *     @arg ADC_CH_n        : ADC Channel x selected
 *     @arg ADC_CH_OPAn     : ADC channel for OPAn
 *     @arg ADC_CH_GND_VREF : ADC GND VREF selected
 *     @arg ADC_CH_VDD_VREF : ADC VDD VREF selected
 * @param NewState: ENABLE DISABLE
 * @retval None
 ************************************************************************************************************/
void ADC_ChannelOffsetCmd(HT_ADC_TypeDef* HT_ADCn, u8 ADC_CH_n, ControlStatus NewState)
{
  u32 OFRValue;

  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_CHANNEL(ADC_CH_n));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  OFRValue = HT_ADCn->OFR[ADC_CH_n] & (~(OFR_ADOFE));

  if (NewState == ENABLE)
  {
    OFRValue |= OFR_ADOFE;
  }

  HT_ADCn->OFR[ADC_CH_n] = OFRValue;
}
#endif

#if (LIBCFG_ADC_TRIG_DELAY)
/*********************************************************************************************************//**
 * @brief Configure the ADC Trigger Delay.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals. (ADC1 only)
  * @param HDelayTime: High priority ADC trigger delay.
 *   This parameter must be between 0x00 to 0xFF.
 * @param DelayTime: Regular ADC trigger delay.
 *   This parameter must be between 0x00 to 0xFF.
 * @retval None
 ************************************************************************************************************/
void ADC_TrigDelayConfig(HT_ADC_TypeDef* HT_ADCn, u8 HDelayTime, u8 DelayTime)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));

  HT_ADCn->CFGR = (HDelayTime << 16 | DelayTime << 8);
}
#endif

/*********************************************************************************************************//**
 * @brief Enable or Disable software start of the regular channel conversion of the selected ADC.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param NewState: This parameter can be ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void ADC_SoftwareStartConvCmd(HT_ADC_TypeDef* HT_ADCn, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  /* Start Conversion                                                                                       */
  if (NewState != DISABLE)
  {
    HT_ADCn->TSR |= TCR_SC_SET;
  }
  else
  {
    HT_ADCn->TSR &= ~TCR_SC_SET;
  }
}

/*********************************************************************************************************//**
 * @brief Enable or Disable software start of the high priority channel conversion of the selected ADC.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param NewState: This parameter can be ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void ADC_HPSoftwareStartConvCmd(HT_ADC_TypeDef* HT_ADCn, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  /* Start Conversion                                                                                       */
  if (NewState != DISABLE)
  {
    HT_ADCn->HTSR |= HTCR_SC_SET;
  }
  else
  {
    HT_ADCn->HTSR &= ~HTCR_SC_SET;
  }
}

/*********************************************************************************************************//**
 * @brief Return the result of ADC regular channel conversion.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_REGULAR_DATAn: where n can be 0 ~ 7
 * @retval The Value of data conversion.
 ************************************************************************************************************/
u16 ADC_GetConversionData(HT_ADC_TypeDef* HT_ADCn, u8 ADC_REGULAR_DATAn)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_REGULAR_DATA(ADC_REGULAR_DATAn));

  return ((u16)HT_ADCn->DR[ADC_REGULAR_DATAn]);
}

/*********************************************************************************************************//**
 * @brief Return the result of ADC high priority channel conversion.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_HP_DATAn: where x can be 0 ~ 11, (4 ~ 11 only for specific model)
 * @retval The Value of data conversion.
 ************************************************************************************************************/
u16 ADC_GetHPConversionData(HT_ADC_TypeDef* HT_ADCn, u8 ADC_HP_DATAn)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_HP_DATA(ADC_HP_DATAn));

  #if (LIBCFG_ADC_HDR_4_11)
  if (ADC_HP_DATAn < 4)
  {
    return ((u16)HT_ADCn->HDR[ADC_HP_DATAn]);
  }
  else
  {
    return ((u16)HT_ADCn->HDR4[ADC_HP_DATAn - 4]);
  }
  #else
  return ((u16)HT_ADCn->HDR[ADC_HP_DATAn]);
  #endif
}

/*********************************************************************************************************//**
 * @brief Enable or Disable the specified ADC interrupts.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_INT_x: Specify the ADC interrupt sources that is to be enabled or disabled.
 *   This parameter can be any combination of the following values:
 *     @arg ADC_INT_SINGLE_EOC        :
 *     @arg ADC_INT_SUB_GROUP_EOC     :
 *     @arg ADC_INT_CYCLE_EOC         :
 *     @arg ADC_INT_HP_SINGLE_EOC     :
 *     @arg ADC_INT_HP_SUB_GROUP_EOC  :
 *     @arg ADC_INT_HP_CYCLE_EOC      :
 *     @arg ADC_INT_DATA_OVERWRITE    :
 *     @arg ADC_INT_HP_DATA_OVERWRITE :
 *     @arg ADC_INT_AWD_LOWER         :
 *     @arg ADC_INT_AWD_UPPER         :
 * @param NewState: This parameter can be ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void ADC_IntConfig(HT_ADC_TypeDef* HT_ADCn, u32 ADC_INT_x, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_INT(ADC_INT_x));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    HT_ADCn->IER |= ADC_INT_x;
  }
  else
  {
    HT_ADCn->IER &= ~ADC_INT_x;
  }
}

/*********************************************************************************************************//**
 * @brief Check whether the specified ADC interrupt has occurred.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_INT_x: Specify the interrupt status to check.
 *   This parameter can be any combination of the following values:
 *     @arg ADC_INT_SINGLE_EOC        :
 *     @arg ADC_INT_SUB_GROUP_EOC     :
 *     @arg ADC_INT_CYCLE_EOC         :
 *     @arg ADC_INT_HP_SINGLE_EOC     :
 *     @arg ADC_INT_HP_SUB_GROUP_EOC  :
 *     @arg ADC_INT_HP_CYCLE_EOC      :
 *     @arg ADC_INT_DATA_OVERWRITE    :
 *     @arg ADC_INT_HP_DATA_OVERWRITE :
 *     @arg ADC_INT_AWD_LOWER         :
 *     @arg ADC_INT_AWD_UPPER         :
 * @retval SET or RESET
 ************************************************************************************************************/
FlagStatus ADC_GetIntStatus(HT_ADC_TypeDef* HT_ADCn, u32 ADC_INT_x)
{
  FlagStatus Status;

  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_INT(ADC_INT_x));

  if ((HT_ADCn->ISR & ADC_INT_x) != RESET)
  {
    Status = SET;
  }
  else
  {
    Status = RESET;
  }

  return Status;
}

/*********************************************************************************************************//**
 * @brief Clear the ADC interrupt pending bits.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_INT_x: Specify the interrupt pending bits to be cleared.
 *   This parameter can be any combination of the following values:
 *     @arg ADC_INT_SINGLE_EOC        :
 *     @arg ADC_INT_SUB_GROUP_EOC     :
 *     @arg ADC_INT_CYCLE_EOC         :
 *     @arg ADC_INT_HP_SINGLE_EOC     :
 *     @arg ADC_INT_HP_SUB_GROUP_EOC  :
 *     @arg ADC_INT_HP_CYCLE_EOC      :
 *     @arg ADC_INT_DATA_OVERWRITE    :
 *     @arg ADC_INT_HP_DATA_OVERWRITE :
 *     @arg ADC_INT_AWD_LOWER         :
 *     @arg ADC_INT_AWD_UPPER         :
 * @retval None
 ************************************************************************************************************/
void ADC_ClearIntPendingBit(HT_ADC_TypeDef* HT_ADCn, u32 ADC_INT_x)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_INT(ADC_INT_x));

  HT_ADCn->ICLR = ADC_INT_x;
}

/*********************************************************************************************************//**
 * @brief Check whether the specified ADC flag has been set.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_FLAG_x: Specify the flag to check.
 *   This parameter can be any combination of the following values:
 *     @arg ADC_FLAG_SINGLE_EOC        :
 *     @arg ADC_FLAG_SUB_GROUP_EOC     :
 *     @arg ADC_FLAG_CYCLE_EOC         :
 *     @arg ADC_FLAG_HP_SINGLE_EOC     :
 *     @arg ADC_FLAG_HP_SUB_GROUP_EOC  :
 *     @arg ADC_FLAG_HP_CYCLE_EOC      :
 *     @arg ADC_FLAG_DATA_OVERWRITE    :
 *     @arg ADC_FLAG_HP_DATA_OVERWRITE :
 *     @arg ADC_FLAG_AWD_LOWER         :
 *     @arg ADC_FLAG_AWD_UPPER         :
 * @retval SET or RESET
 ************************************************************************************************************/
FlagStatus ADC_GetFlagStatus(HT_ADC_TypeDef* HT_ADCn, u32 ADC_FLAG_x)
{
  FlagStatus Status;

  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_FLAG(ADC_FLAG_x));

  if ((HT_ADCn->IRAW & ADC_FLAG_x) != RESET)
  {
    Status = SET;
  }
  else
  {
    Status = RESET;
  }

  return Status;
}

/*********************************************************************************************************//**
 * @brief Enable or Disable Lower/Upper threshold warning of the analog watchdog on single/all channels.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_AWD_x:
 *   This parameter can be any combination of the following values:
 *     @arg ADC_AWD_DISABLE            :
 *     @arg ADC_AWD_ALL_LOWER          :
 *     @arg ADC_AWD_ALL_UPPER          :
 *     @arg ADC_AWD_ALL_LOWER_UPPER    :
 *     @arg ADC_AWD_SINGLE_LOWER       :
 *     @arg ADC_AWD_SINGLE_UPPER       :
 *     @arg ADC_AWD_SINGLE_LOWER_UPPER :
 * @retval None
 ************************************************************************************************************/
void ADC_AWDConfig(HT_ADC_TypeDef* HT_ADCn, u32 ADC_AWD_x)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_AWD(ADC_AWD_x));

  HT_ADCn->WCR = (HT_ADCn->WCR & 0xFFFFFFF8) | ADC_AWD_x;
}

/*********************************************************************************************************//**
 * @brief Configure the analog watchdog that guards single channel.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_CH_n: where n must between 0 ~ 7
 * @retval None
 ************************************************************************************************************/
void ADC_AWDSingleChannelConfig(HT_ADC_TypeDef* HT_ADCn, u8 ADC_CH_n)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_INPUT_CHANNEL(ADC_CH_n));

  HT_ADCn->WCR = (HT_ADCn->WCR & 0xFFFFF0FF) | ((u32)ADC_CH_n << 8);
}

/*********************************************************************************************************//**
 * @brief Configure the high and low thresholds of the analog watchdog.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param UPPER: must between 0x0000 ~ 0x0FFF
 * @param LOWER: must between 0x0000 ~ 0x0FFF
 * @retval None
 ************************************************************************************************************/
void ADC_AWDThresholdsConfig(HT_ADC_TypeDef* HT_ADCn, u16 UPPER, u16 LOWER)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_THRESHOLD(UPPER));
  Assert_Param(IS_ADC_THRESHOLD(LOWER));

  HT_ADCn->LTR = LOWER;
  HT_ADCn->UTR = UPPER;
}

/*********************************************************************************************************//**
 * @brief Enable or Disable the specified PDMA request.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_PDMA_x: Specify the ADC PDMA request that is to be enabled or disabled.
 *   This parameter can be any combination of the following values:
 *     @arg ADC_PDMA_REGULAR_SINGLE   :
 *     @arg ADC_PDMA_REGULAR_SUBGROUP :
 *     @arg ADC_PDMA_REGULAR_CYCLE    :
 *     @arg ADC_PDMA_HP_SINGLE        :
 *     @arg ADC_PDMA_HP_SUBGROUP      :
 *     @arg ADC_PDMA_HP_CYCLE         :
 * @param NewState: This parameter can be ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void ADC_PDMAConfig(HT_ADC_TypeDef* HT_ADCn, u32 ADC_PDMA_x, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_PDMA(ADC_PDMA_x));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    HT_ADCn->PDMAR |= ADC_PDMA_x;
  }
  else
  {
    HT_ADCn->PDMAR &= ~ADC_PDMA_x;
  }
}

#if (LIBCFG_ADC_IVREF)
/*********************************************************************************************************//**
 * @brief Enable or Disable the VREF.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param NewState: This parameter can be ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void ADC_VREFCmd(HT_ADC_TypeDef* HT_ADCn, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    HT_ADCn->VREFCR |= 0x00000001;
  }
  else
  {
    HT_ADCn->VREFCR &= ~(0x00000001);
  }
}
#endif

#if (LIBCFG_ADC_IVREF_VOLTCONFIG)
/*********************************************************************************************************//**
 * @brief Configure the VREF output voltage.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_VREF_x:
 *   This parameter can be one of the following value:
 *     @arg ADC_VREF_2V0 :
 *     @arg ADC_VREF_2V5 :
 *     @arg ADC_VREF_2V7 :
 *     @arg ADC_VREF_3V0 :
 * @retval None
 ************************************************************************************************************/
void ADC_VREFConfig(HT_ADC_TypeDef* HT_ADCn, u32 ADC_VREF_x)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_VREF_SEL(ADC_VREF_x));

  HT_ADCn->VREFCR = (HT_ADCn->VREFCR & ~(3ul << 4)) | (ADC_VREF_x);
}
#endif

#if (LIBCFG_ADC_VREFBUF)
/*********************************************************************************************************//**
 * @brief Enable or Disable the VREF output. When enable, the VREF provides a stable voltage output to the
          ADVREFP pin (ADC reference positive voltage).
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param NewState: This parameter can be ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void ADC_VREFOutputADVREFPCmd(HT_ADC_TypeDef* HT_ADCn, ControlStatus NewState)
{
  /* !!! NOTICE !!!
     The ADCREFP pin should not be connected to an external voltage when the VREF output is enabled.
  */

  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    HT_ADCn->VREFCR |= 0x00000002;
  }
  else
  {
    HT_ADCn->VREFCR &= ~(0x00000002);
  }
}
#endif

#if (LIBCFG_ADC_FLEXIBLE_VREFVALR)
/*********************************************************************************************************//**
 * @brief Set the internal voltage reference calibration value.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param VREFVALR : specify the internal voltage reference calibration value.
 *   This parameter must be a number between 0 and 0x7F
 * @retval None
 ************************************************************************************************************/
void ADC_SetVoltageReferenceValue(HT_ADC_TypeDef* HT_ADCn, u8 VREFVALR)
{
  u32 uVREFVALRTemp;

  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_ADC_VREFVALR(VREFVALR));

  /* Set the internal voltage reference calibration value                                                   */
  uVREFVALRTemp = HT_ADCn->VREFVALR & ADC_VREFVALR_MASK;

  /* Disable write protection                                                                               */
  HT_ADCn->VREFVALR = ADC_VREFVALR_KEY | uVREFVALRTemp;

  /* Set the internal voltage reference calibration value and write protection                              */
  HT_ADCn->VREFVALR = VREFVALR & ADC_VREFVALR_MASK;
}

/*********************************************************************************************************//**
 * @brief Get the internal voltage reference calibration value.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @retval The internal voltage reference calibration value between 0 and 0x7F
 ************************************************************************************************************/
u8 ADC_GetVoltageReferenceValue(HT_ADC_TypeDef* HT_ADCn)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));

  return (u8)(HT_ADCn->VREFVALR & ADC_VREFVALR_MASK);
}
#endif

#if (LIBCFG_ADC_MVDDA)
/*********************************************************************************************************//**
 * @brief Enable or Disable the power of MVDDA (VDDA/2)
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param NewState: This parameter can be ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void ADC_MVDDACmd(HT_ADC_TypeDef* HT_ADCn, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    HT_ADCn->VREFCR |= ADC_VREF_MVDDAEN;
  }
  else
  {
    HT_ADCn->VREFCR &= ~(ADC_VREF_MVDDAEN);
  }
}
#endif

#if (LIBCFG_ADC_TEMP_SENSOR)
/*********************************************************************************************************//**
 * @brief Enable or Disable the Temperature Sensor of specified ADC.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param NewState: This parameter can be ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void ADC_TempSensorCmd(HT_ADC_TypeDef* HT_ADCn, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  /* Enable Temperature Sensor                                                                              */
  if (NewState != DISABLE)
  {
    HT_ADCn->TSCR |= ADC_TEMP_SENSOR_ENABLE;
  }
  else
  {
    HT_ADCn->TSCR &= ~ADC_TEMP_SENSOR_ENABLE;
  }
}

/*********************************************************************************************************//**
 * @brief This function is used to set the Temperature Sensor clock divider.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param TempSensorClockDiv: specify the value of the TSCLK(PCK) divider as the clock source for the
 *        temperature sensor.
 * @retval None
 * @note  The TempSensorClockDiv range and optimal operation frequency, please refer to the electrical
          characteristics section of the device datasheet.
 ************************************************************************************************************/
void ADC_TempSensorSetClockDivider(HT_ADC_TypeDef* HT_ADCn, u8 TempSensorClockDiv)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_TEMP_SENSOR_DIV(TempSensorClockDiv));

  HT_ADCn->TSCR = (HT_ADCn->TSCR & (~0xF0UL)) | (TempSensorClockDiv << 4);
}

/*********************************************************************************************************//**
 * @brief This function is used to set the Temperature Sensor start to operate.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param NewState: This parameter can be ENABLE or DISABLE.
 * @retval None
 * @note  This control bit will automatically be reset by the hardware once the temperature sensor's process
          is complete.
 ************************************************************************************************************/
void ADC_TempSensorStartOperateCmd(HT_ADC_TypeDef* HT_ADCn, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  /* Start to operate                                                                                       */
  if (NewState != DISABLE)
  {
    HT_ADCn->TSCR |= ADC_TEMP_SENSOR_START_OPERATE;
  }
  else
  {
    HT_ADCn->TSCR &= ~ADC_TEMP_SENSOR_START_OPERATE;
  }
}

/*********************************************************************************************************//**
 * @brief Check whether the specified Temperature Sensor flag has been set.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param TempSensorFlag: Specify the flag to check.
 *   This parameter can be one of the following values:
 *     @arg TEMP_SENSOR_FLAG_READY : The temperature sensor status is ready to measure with ADC.
 *          Note:The TEMP_SENSOR_FLAG_READY is only keep the 256 clock periods of TSCLK then hardware auto
 *          time-out to reset.
 * @retval SET or RESET
 ************************************************************************************************************/
FlagStatus ADC_TempSensorGetFlagStatus(HT_ADC_TypeDef* HT_ADCn, u32 TempSensorFlag)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_TEMP_SENSOR_FLAG(TempSensorFlag));

  if ((HT_ADCn->TSCR & TempSensorFlag) != RESET)
  {
    return SET;
  }
  else
  {
    return RESET;
  }
}

/*********************************************************************************************************//**
 * @brief Clear the specified Temperature Sensor flag.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param TempSensorFlag: Specify the flag to check.
 *   This parameter can be one of the following values:
 *     @arg TEMP_SENSOR_FLAG_READY : The temperature sensor status is ready to measure with ADC.
 *          Note:The TEMP_SENSOR_FLAG_READY is only keep the 256 clock periods of TSCLK then hardware auto
 *          time-out to reset.
 * @retval None
 ************************************************************************************************************/
void ADC_TempSensorClearFlag(HT_ADC_TypeDef* HT_ADCn, u32 TempSensorFlag)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_TEMP_SENSOR_FLAG(TempSensorFlag));

  HT_ADCn->TSCR &= ~TempSensorFlag;
}

/*********************************************************************************************************//**
 * @brief Get the calibration values of the Temperature Sensor.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @retval Return the temperature sensor calibration value.
 ************************************************************************************************************/
u16 ADC_TempSensorGetCalValue(HT_ADC_TypeDef* HT_ADCn)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));

  return HT_ADCn->TSCALR & ADC_MASK_TSCAL;
}

/*********************************************************************************************************//**
 * @brief Enable or Disable the specified Temperature Sensor interrupts.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param ADC_INT_TP_x: Specify the temperature sensor interrupt sources that is to be enabled or disabled.
 *   This parameter can be the following values:
 *     @arg TEMP_SENSOR_INT_READY        :
 * @param NewState: This parameter can be ENABLE or DISABLE.
 * @retval None
 ************************************************************************************************************/
void ADC_TempSensorIntConfig(HT_ADC_TypeDef* HT_ADCn, u32 ADC_INT_TP_x, ControlStatus NewState)
{
  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));
  Assert_Param(IS_TEMP_SENSOR_INT(ADC_INT_TP_x));
  Assert_Param(IS_CONTROL_STATUS(NewState));

  if (NewState != DISABLE)
  {
    HT_ADCn->TSCR |= ADC_INT_TP_x;
  }
  else
  {
    HT_ADCn->TSCR &= ~ADC_INT_TP_x;
  }
}

/*********************************************************************************************************//**
 * @brief Get the temperature. (Unit: 0.001 Celsius)
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @param tempPara:  pointer to a ADC_TempSensorParam_TypeDef structure.
 *   - Advrefp_mV: The reference voltage of ADVREFP in millivolts.
 *   - TsData: The digital value output by the temperature sensor converted by the ADC.
 *   - CalTempPoint_mC: The calibration temperature point in milli-degree Celsius
 * @param temperature_mC: the actual temperature and it is one thousandth degree Celsius.
 * @retval SUCCESS or ERROR
 ************************************************************************************************************/
ErrStatus ADC_TempSensorGetTemp(HT_ADC_TypeDef* HT_ADCn, ADC_TempSensorParam_TypeDef* tempPara ,s32* temperature_mC)
{
  u32 ts_calib_vol;   /* The temperature sensor calibration voltage value that is converted from TSCALR     */
  u32 ts_data_vol;    /* The actual temperature sensor output voltage value                                 */

  /* Check the parameters                                                                                   */
  Assert_Param(IS_ADC(HT_ADCn));

  if (tempPara->TsData > 0xFFF)
  {
    return ERROR;
  }

  if (tempPara->Advrefp_mV > LIBCFG_MAX_VREF_VOL)
  {
    return ERROR;
  }

  /* Calculate the temperature sensor calibration voltage value.                                            */
  /* Note: Multiplying the calculation result by 100 is to scale down the unit to convert to 10uV, avoiding */
  /*       floating-point arithmetic later on.                                                              */
  ts_calib_vol = (ADC_TempSensorGetCalValue(HT_ADCn) * LIBCFG_ADC_TEMP_SENSOR_CALIBRATION_ADVREFP * 100) >> 12;

  /* Calculate the actual temperature sensor output voltage value.                                          */
  /* Note: Multiplying the calculation result by 100 is to scale down the unit to convert to 10uV, avoiding */
  /*       floating-point arithmetic later on.                                                              */
  ts_data_vol  = (tempPara->TsData  * tempPara->Advrefp_mV * 100) >> 12;

  /* Calculate the actual temperature.                                                                      */
  *temperature_mC = (s32)(((s32)(ts_calib_vol - ts_data_vol) * 1000) / LIBCFG_ADC_TEMP_SENSOR_AVG_SLOPE +
                    tempPara->CalTempPoint_mC);
  return SUCCESS;
}

/*********************************************************************************************************//**
 * @brief Get the calibration temperature point of the Temperature Sensor.
 * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
 * @retval Return the temperature calibration reference temperature (Unit: 0.001 Celsius).
 * @note The calibration temperature point can be defined by user selection via the ADC_CAL_TEMP_POINT_SOURCE.
 *   - 0: A fixed default temperature is used.
 *     !!! NOTICE !!! The fixed default temperature is calibrated under specific production test conditions,
 *     which represent the environment of that batch only. The actual calibration temperature may slightly
 *     between production lots.
 *   - 1: The value is derived from the factory trim code.
 ************************************************************************************************************/
s32 ADC_TempSensorGetCalTempPoint(HT_ADC_TypeDef* HT_ADCn)
{
  /* Check the parameters                                                                                    */
  Assert_Param(IS_ADC(HT_ADCn));
  #if ADC_CAL_TEMP_POINT_SORUCE
  /* Get calibration temperature point from factory trim code                                                */
  /* To Do... */
   #error "Trim code feature is not yet implemented."
  #else
  /* Return fixed default calibration temperature point                                                      */
  HT_ADCn->TSCALR; /* Used only to avoid "unused parameter" compiler warning.                                */
  return  ADC_FIXED_CAL_TEMP_mC;
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
