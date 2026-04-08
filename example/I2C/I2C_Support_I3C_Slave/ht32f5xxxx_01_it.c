/*********************************************************************************************************//**
 * @file    I2C/I2C_Support_I3C_Slave/ht32f5xxxx_01_it.c
 * @version $Rev:: 9602         $
 * @date    $Date:: 2025-12-05 #$
 * @brief   This file provides all interrupt service routine.
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
#include "ht32_board_config.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup I2C_Examples I2C
  * @{
  */

/** @addtogroup I2C_Support_I3C_Slave
  * @{
  */


/* Private constants ---------------------------------------------------------------------------------------*/
#define I2C1_SLAVE_ADDRESS     0x60
#define BufferSize             4

/* Private variables ---------------------------------------------------------------------------------------*/
extern vu8 I2C_Slave_Register[];
extern vu8 I2C_Slave_Rx_Buffer[];

extern vu8 I2C_Slave_Tx_Addr;
extern vu8 I2C_Slave_Rx_Index;

extern vu32 IsI2CFinish;
extern vu32 IsSlaveTxFinish;

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
 * @brief   This function handles NMI exception.
 * @retval  None
 ************************************************************************************************************/
void NMI_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles Hard Fault exception.
 * @retval  None
 ************************************************************************************************************/
void HardFault_Handler(void)
{
  #if 1

  static vu32 gIsContinue = 0;
  /*--------------------------------------------------------------------------------------------------------*/
  /* For development FW, MCU run into the while loop when the hardfault occurred.                           */
  /* 1. Stack Checking                                                                                      */
  /*    When a hard fault exception occurs, MCU push following register into the stack (main or process     */
  /*    stack). Confirm R13(SP) value in the Register Window and typing it to the Memory Windows, you can   */
  /*    check following register, especially the PC value (indicate the last instruction before hard fault).*/
  /*    SP + 0x00    0x04    0x08    0x0C    0x10    0x14    0x18    0x1C                                   */
  /*           R0      R1      R2      R3     R12      LR      PC    xPSR                                   */
  while (gIsContinue == 0)
  {
  }
  /* 2. Step Out to Find the Clue                                                                           */
  /*    Change the variable "gIsContinue" to any other value than zero in a Local or Watch Window, then     */
  /*    step out the HardFault_Handler to reach the first instruction after the instruction which caused    */
  /*    the hard fault.                                                                                     */
  /*--------------------------------------------------------------------------------------------------------*/

  #else

  /*--------------------------------------------------------------------------------------------------------*/
  /* For production FW, you shall consider to reboot the system when hardfault occurred.                    */
  /*--------------------------------------------------------------------------------------------------------*/
  NVIC_SystemReset();

  #endif
}

/*********************************************************************************************************//**
 * @brief   This function handles SVCall exception.
 * @retval  None
 ************************************************************************************************************/
void SVC_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles PendSVC exception.
 * @retval  None
 ************************************************************************************************************/
void PendSV_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles SysTick Handler.
 * @retval  None
 ************************************************************************************************************/
void SysTick_Handler(void)
{
}

/*********************************************************************************************************//**
 * @brief   This function handles I2C Slave interrupt.
 * @retval  None
 ************************************************************************************************************/
void HTCFG_I2C_Slave_IRQHandler(void)
{
  u32 I2C_Slave_ISR = I2C_ReadRegister(HTCFG_I2C_SLAVE_PORT, I2C_REGISTER_SR);

  if (I2C_Slave_ISR & I2C_FLAG_RXDNE)
  {
    if (I2C_Slave_Rx_Index < 128)
    {
      /* Receive data sent from Master                                                                      */
      I2C_Slave_Rx_Buffer[I2C_Slave_Rx_Index ++] = I2C_ReceiveData(HTCFG_I2C_SLAVE_PORT);
    }
  }

  if (I2C_Slave_ISR & I2C_FLAG_STO)
  {
    IsI2CFinish = TRUE;
  }

  if ((I2C_Slave_ISR & I2C_FLAG_TXDE) && (I2C_ReadRegister(HTCFG_I2C_SLAVE_PORT, I2C_REGISTER_IER) & I2C_INT_TXDE))
  {
    I2C_SendData(HTCFG_I2C_SLAVE_PORT, I2C_Slave_Register[I2C_Slave_Tx_Addr++]);
  }

  if (I2C_Slave_ISR & I2C_FLAG_RXNACK)
  {
    /* Clear RXNACK flag                                                                                    */
    I2C_ClearFlag(HTCFG_I2C_SLAVE_PORT, I2C_FLAG_RXNACK);
    /* Set Slave TX END flag                                                                                */
    IsSlaveTxFinish = TRUE;
  }

  #if 0
  /* Receive or Transmit Happen Overflow                                                                    */
  if (I2C_Slave_Rx_Index >= 128)
  {
    while(1){};
  }
  #endif
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
