/*********************************************************************************************************//**
 * @file    I2C/Slave_Repeat_Start_Solution/main.c
 * @version $Rev:: 9539         $
 * @date    $Date:: 2025-11-10 #$
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

/** @addtogroup I2C_Examples I2C
  * @{
  */

/** @addtogroup I2C_Slave_Repeat_Start_Solution
  * @{
  */


/* Private constants ---------------------------------------------------------------------------------------*/
#define I2C_SLAVE_ADDRESS      0x60
#define BufferSize             5

/* Private variables ---------------------------------------------------------------------------------------*/
u8 gI2C_Slave_Buffer_Tx[BufferSize] = {0x1, 0x2, 0x3, 0x4, 0x5};
u8 gI2C_Slave_Buffer_Rx[BufferSize] = {0x00};
vu8 gTx_Index = 0;
vu8 gRx_Index = 0;

/* Global functions ----------------------------------------------------------------------------------------*/
void I2CSlave_Configuration(void);

/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
int main(void)
{
  I2CSlave_Configuration();

  while (1);
}

/*********************************************************************************************************//**
  * @brief  Configure the I2C.
  * @retval None
  ***********************************************************************************************************/
void I2CSlave_Configuration(void)
{
  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.AFIO                     = 1;
    CKCUClock.Bit.HTCFG_AFIO_TOGGLE_IO_CK  = 1;
    CKCUClock.Bit.HTCFG_I2C_SLAVE_CK       = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  /* Configure AFIO as I2C function                                                                         */
  AFIO_GPxConfig(HTCFG_I2C_SLAVE_SCL_GPIO_ID, HTCFG_I2C_SLAVE_SCL_AFIO_PIN, AFIO_FUN_I2C);
  AFIO_GPxConfig(HTCFG_I2C_SLAVE_SDA_GPIO_ID, HTCFG_I2C_SLAVE_SDA_AFIO_PIN, AFIO_FUN_I2C);

  { /* I2C Slave configuration                                                                              */
    /* !!! NOTICE !!!
       Notice that the local variable (structure) did not have an initial value.
       Please confirm that there are no missing members in the parameter settings below in this function.
    */
    I2C_InitTypeDef  I2C_InitStructure;
    I2C_InitStructure.I2C_GeneralCall    = DISABLE;
    I2C_InitStructure.I2C_AddressingMode = I2C_ADDRESSING_7BIT;
    I2C_InitStructure.I2C_Acknowledge    = ENABLE;
    I2C_InitStructure.I2C_OwnAddress     = I2C_SLAVE_ADDRESS;
    I2C_InitStructure.I2C_Speed          = 0;
    I2C_InitStructure.I2C_SpeedOffset    = 0;
    I2C_Init(HTCFG_I2C_SLAVE_PORT, &I2C_InitStructure);
  }

  I2C_SetTimeOutPrescaler(HTCFG_I2C_SLAVE_PORT, I2C_PRESCALER_128);
  I2C_SetTimeOutValue(HTCFG_I2C_SLAVE_PORT, 2000);
  I2C_TimeOutCmd(HTCFG_I2C_SLAVE_PORT, ENABLE);

  /* Enable I2C Slave interrupts                                                                            */
  I2C_IntConfig(HTCFG_I2C_SLAVE_PORT, I2C_INT_ADRS | I2C_INT_RXDNE | I2C_INT_TXDE | I2C_INT_RXNACK | I2C_INT_BUSERR | I2C_INT_TOUT, ENABLE);
  NVIC_EnableIRQ(HTCFG_I2C_SLAVE_IRQn);

  /* !!! NOTICE !!!
     Must be the highest priority interrupt, and other active interrupts must have priority > 0.
  */
  NVIC_SetPriority(HTCFG_I2C_SLAVE_IRQn, 0);
  I2C_Cmd(HTCFG_I2C_SLAVE_PORT, ENABLE);

  /* I2C ACK START Bus Error Solution IO                                                                    */
  AFIO_GPxConfig(HTCFG_AFIO_TOGGLE_IO_ID, HTCFG_AFIO_TOGGLE_SCL_AFIO_PIN | HTCFG_AFIO_TOGGLE_SDA_AFIO_PIN, AFIO_FUN_GPIO);
  GPIO_PullResistorConfig(HTCFG_AFIO_TOGGLE_IO_PORT, HTCFG_AFIO_TOGGLE_SCL_GPIO_PIN | HTCFG_AFIO_TOGGLE_SDA_GPIO_PIN, GPIO_PR_UP);
  GPIO_DirectionConfig(HTCFG_AFIO_TOGGLE_IO_PORT, HTCFG_AFIO_TOGGLE_SCL_GPIO_PIN | HTCFG_AFIO_TOGGLE_SDA_GPIO_PIN, GPIO_DIR_IN);
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
