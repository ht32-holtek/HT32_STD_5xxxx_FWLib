/*********************************************************************************************************//**
 * @file    ADC/VREFP_Powered_by_Internal_VREF/main.c
 * @version $Rev:: 9599         $
 * @date    $Date:: 2025-12-04 #$
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

/** @addtogroup VREFP_Powered_by_Internal_VREF
  * @{
  */

/* !!! NOTICE !!!
   DO NOT supply the power externally on the VREFP pin (VREFP and MVDD must be opened on the Starter Kit).
   For example, the R8 of the HT32F66256 Starter Kit must be Remove. 
   For the Jx/Rx of other Starter Kit models, please check the schematics.
*/

/* Settings ------------------------------------------------------------------------------------------------*/
/* Internal VREF output level setting                                                                       */
#define ADC_RESOLUTION_MAX            (4095)          /* ADC 12-bit                                         */
#define ADC_VREF_LEVEL                ADC_VREF_2V0
#define VREF_VALUE_mV                 (2000)

/* Private function prototypes -----------------------------------------------------------------------------*/
void ADC_Configuration(void);

void ADC_MainRoutine(void);

/* Global variables ----------------------------------------------------------------------------------------*/
u32 gADC_Result[2];
volatile bool gADC_CycleEndOfConversion;

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
int main(void)
{
  RETARGET_Configuration();

  HT32F_DVB_LEDInit(HT_LED1);
  HT32F_DVB_LEDInit(HT_LED2);
  HT32F_DVB_LEDInit(HT_LED3);

  ADC_Configuration();

  ADC_Cmd(HT_ADC0, ENABLE);

  HT32F_DVB_LEDOn(HT_LED1);

  /* Software trigger ADC start                                                                             */
  ADC_SoftwareStartConvCmd(HT_ADC0, ENABLE);
  while (1)
  {
    ADC_MainRoutine();
  }
}

/*********************************************************************************************************//**
  * @brief  ADC main routine.
  * @retval None
  ***********************************************************************************************************/
void ADC_MainRoutine(void)
{
  if (gADC_CycleEndOfConversion)
  {
    #if (LIBCFG_ADC_MVDDA)
    u32 uMVDDAVoltage = (VREF_VALUE_mV * gADC_Result[0]) / ADC_RESOLUTION_MAX;
    #endif
    u32 uVRVoltage    = (VREF_VALUE_mV * gADC_Result[1]) / ADC_RESOLUTION_MAX;

    /* Show internal VREF/ADVREFP  voltage and ADC result                                                   */
    printf("VREF=   %4dmV  Value= %4d\n\r", VREF_VALUE_mV, ADC_RESOLUTION_MAX);
    
    /* Show VDDA, MVDDA, and ADC input (potentiometer) voltage and ADC result                               */
    #if (LIBCFG_ADC_MVDDA)
    printf("VDDA =  %4dmV\n\r",               uMVDDAVoltage*2);
    printf("MVDDA=  %4dmV  Value= %4d\n\r",   uMVDDAVoltage, gADC_Result[0]);
    #endif
    printf("VR=     %4dmV  Value= %4d\n\n\r", uVRVoltage   , gADC_Result[1]);

    gADC_CycleEndOfConversion = FALSE;
  }
}

/*********************************************************************************************************//**
  * @brief  ADC configuration.
  * @retval None
  ***********************************************************************************************************/
void ADC_Configuration(void)
{
  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.AFIO = 1;
    CKCUClock.Bit.ADC0 = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  /* Configure AFIO mode as ADC function                                                                    */
  AFIO_GPxConfig(HTCFG_VR_GPIO_ID, HTCFG_VR_AFIO_PIN, AFIO_FUN_ADC0);

  { /* ADC related settings                                                                                 */
    /* CK_ADC frequency is set to (CK_AHB / 64)                                                             */
    CKCU_SetADCnPrescaler(CKCU_ADCPRE_ADC0, CKCU_ADCPRE_DIV64);

    /* One Shot mode, sequence length = 2                                                                   */
    ADC_RegularGroupConfig(HT_ADC0, ONE_SHOT_MODE, 2, 0);

    /* ADC conversion time = (Sampling time + Latency) / CK_ADC = (1 + ADST + 13) / CK_ADC                  */
    /* Set ADST = 36, sampling time = 1 + ADST                                                              */
    #if (LIBCFG_ADC_SAMPLE_TIME_BY_CH)
      // The sampling time is set by the last parameter of the function "ADC_RegularChannelConfig()".
    #else
    ADC_SamplingTimeConfig(HT_ADC0, 36);
    #endif

    /* Set ADC conversion sequence as channel n                                                             */
    #if (LIBCFG_ADC_MVDDA)
    ADC_RegularChannelConfig(HT_ADC0, ADC_CH_MVDDA, 0, 0);
    #endif
    ADC_RegularChannelConfig(HT_ADC0, HTCFG_VR_ADC_CH, 1, 0);

    /* Set software as ADC trigger source                                                                   */
    ADC_RegularTrigConfig(HT_ADC0, ADC_TRIG_SOFTWARE);
  }

  /* Select the internal VREF level and enable it                                                    */
  ADC_VREFConfig(HT_ADC0, ADC_VREF_LEVEL);

  /* !!! NOTICE !!!
     VREF/VBG needs a minimum of 100 ms stable time (tSTABLE) after enable, please check the datasheet for the
     actual timing. You shall take care of a delay for the stable time according to the application.
  */
  ADC_VREFCmd(HT_ADC0, ENABLE);

  /* !!! NOTICE !!!
     DO NOT supply the power externally on the VREFP pin (VREFP and MVDD must be opened on the Starter Kit).
     For example, the R8 of the HT32F66256 Starter Kit must be Remove.
     For the Jx/Rx of other Starter Kit models, please check the schematics.
  */
  #if 0 // Please check the notice above, confirm the H/W setting is correct, and then turn on the code below.
  ADC_VREFOutputADVREFPCmd(HT_ADC0, ENABLE);
  #endif

  #if (LIBCFG_ADC_MVDDA)
  /* Enable MVDDA function                                                                                  */
  ADC_MVDDACmd(HT_ADC0, ENABLE);
  #endif

  /* Enable ADC single/cycle end of conversion interrupt                                                    */
  ADC_IntConfig(HT_ADC0, ADC_INT_SINGLE_EOC | ADC_INT_CYCLE_EOC, ENABLE);

  /* Enable the ADC interrupts                                                                              */
  NVIC_EnableIRQ(ADC0_IRQn);
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
