/*********************************************************************************************************//**
 * @file    ADC/OneShot_TempSensor_Interrupt/main.c
 * @version $Rev:: 9705         $
 * @date    $Date:: 2026-03-18 #$
 * @brief   Main program.
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
#include "ht32.h"
#include "ht32_board.h"
#include "ht32_board_config.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup ADC_Examples ADC
  * @{
  */

/** @addtogroup OneShot_TempSensor_Interrupt
  * @{
  */

/* Private function prototypes -----------------------------------------------------------------------------*/
void ADC_TempSensor_Configuration(void);

/* Global variables ----------------------------------------------------------------------------------------*/
vu8 gADC_CycleEndOfConversion = FALSE;
extern s32 gAvgTemp;
extern bool bIsAvgTempReady;

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
int main(void)
{
  HT32F_DVB_LEDInit(HT_LED1);
  HT32F_DVB_LEDInit(HT_LED2);
  RETARGET_Configuration();           /* Retarget Related configuration                                     */
  ADC_TempSensor_Configuration();     /* ADC and Temperature Sensor configuration                           */
  ADC_Cmd(HTCFG_ADC_PORT, ENABLE);    /* ADC Enable                                                         */

  /* Trigger the Temperature Sensor to start preparing the voltage                                          */
  ADC_TempSensorStartOperateCmd(HTCFG_ADC_PORT, ENABLE);

  while (1)
  {
    if (gADC_CycleEndOfConversion)
    {
      /* Display the temperature result                                                                     */
      if(bIsAvgTempReady)
      {
        printf("Current Temperature = %d mC\r\n", gAvgTemp);
        bIsAvgTempReady = FALSE;
      }
      gADC_CycleEndOfConversion = FALSE;

      /* Trigger the Temperature Sensor to start preparing the voltage                                      */
      ADC_TempSensorStartOperateCmd(HTCFG_ADC_PORT, ENABLE);
    }
  }
}

/*********************************************************************************************************//**
  * @brief  ADC & Temperature configuration.
  * @retval None
  ***********************************************************************************************************/
void ADC_TempSensor_Configuration(void)
{
  /* Enable peripheral clock                                                                                */
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
  CKCUClock.Bit.AFIO = 1;
  CKCUClock.Bit.HTCFG_ADC_IPN = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* ADC related settings                                                                                   */
  /* CK_ADC frequency is set to (CK_AHB / HTCFG_ADC_CKCU_ADCPRE_DIV)                                        */
  CKCU_SetADCnPrescaler(HTCFG_ADC_CKCU_ADCPRE, HTCFG_ADC_CKCU_ADCPRE_DIV);

  /* One Shot mode, sequence length = 1                                                                     */
  ADC_RegularGroupConfig(HTCFG_ADC_PORT, ONE_SHOT_MODE, 1, 0);

  /* !!! NOTICE !!!
    According to the datasheet: tSTS = 10 us (ADC sampling time when reading the temperature sensor voltage)
    For Example:
      CK_AHB = 80 MHz
      CK_ADC = CK_AHB / 8 = 10 MHz
      Sampling time needs to meet tSRS = 10 us, so set ADST = 99.
      Sampling time = (sampling clock) / CK_ADC = (1 + 99) / 10 MHz = 10 us.
      ADC conversion time = (sampling clock + latency) / CK_ADC = (1 + 99 + 13) / 10 MHz = 11.3 us.
  */
  #if (LIBCFG_ADC_SAMPLE_TIME_BY_CH)
  // The sampling time is set by the last parameter of the function "ADC_RegularChannelConfig()".
  #else
  ADC_SamplingTimeConfig(HTCFG_ADC_PORT, SystemCoreClock / HTCFG_CK_ADC_DIV / 100000 - 1);
  #endif

  /* Set ADC conversion sequence as channel temperature sensor                                              */
  ADC_RegularChannelConfig(HTCFG_ADC_PORT, ADC0_CH_VTS, 0, SystemCoreClock / HTCFG_CK_ADC_DIV / 100000 - 1);

  /* Set ADC trigger source                                                                                 */
  ADC_RegularTrigConfig(HTCFG_ADC_PORT, ADC_TRIG_SOFTWARE);

  /* Eanble Temperature Sensor                                                                              */
  ADC_TempSensorCmd(HTCFG_ADC_PORT, ENABLE);

  /* !!! NOTICE !!!
    TSCLK shall be around 250 kHz
    For Example:
      ADCPCLK = 80 MHz
      HTCFG_TSCLK_DIV = 8
      TSCLK = (ADCPCLK / 2^HTCFG_TSCLK_DIV) = (80 MHz / 2^8) = 80 MHz / 256 = 312.5 kHz
  */
  ADC_TempSensorSetClockDivider(HTCFG_ADC_PORT, HTCFG_TSCLK_DIV);

  /* Enable ADC end of conversion interrupt                                                                 */
  ADC_IntConfig(HTCFG_ADC_PORT, ADC_INT_CYCLE_EOC, ENABLE);

  /* Enable Temperature Sensor ready interrupt                                                              */
  ADC_TempSensorIntConfig(HTCFG_ADC_PORT, TEMP_SENSOR_INT_READY, ENABLE);

  /* Enable the ADC interrupts                                                                              */
  NVIC_EnableIRQ(HTCFG_ADC_IRQn);
}

#if (HT32_LIB_DEBUG == 1)
/*********************************************************************************************************//**
  * @brief  Report both the error name of the source file and the source line number.
  * @param  filename: pointer to the source file name.
  * @param  uline: error line source number.
  * @retval None
  ***********************************************************************************************************/
void assert_error(u8* filename, u32 uline)
{
  /*
     This function is called by IP library that the invalid parameters has been passed to the library API.
     Debug message can be added here.
     Example: printf("Parameter Error: file %s on line %d\r\n", filename, uline);
  */

  while (1)
  {
  }
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
