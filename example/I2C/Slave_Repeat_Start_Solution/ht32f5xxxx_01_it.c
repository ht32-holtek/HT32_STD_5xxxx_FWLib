/*********************************************************************************************************//**
 * @file    I2C/Slave_Repeat_Start_Solution/ht32f5xxxx_01_it.c
 * @version $Rev:: 9542         $
 * @date    $Date:: 2025-11-11 #$
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
#include "ht32_board.h"
#include "ht32_board_config.h"

extern void I2CSlave_Configuration(void);
extern u8 gI2C_Slave_Buffer_Tx[];
extern u8 gI2C_Slave_Buffer_Rx[];
extern vu8 gTx_Index;
extern vu8 gRx_Index;

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
 * @brief   This function handles I2Cn interrupt.
 * @retval  None
 ************************************************************************************************************/
void HTCFG_I2C_SLAVE_IRQHandler(void)
{
  u32 status = HTCFG_I2C_SLAVE_PORT->SR;

  if (status & (I2C_FLAG_BUSERR))
  {
    u32 reg0 = rw(HTCFG_I2C_SLAVE_SDA_AFIO_REG);
    u32 reg1 = rw(HTCFG_AFIO_TOGGLE_IO_AFIO_REG)                                   |
               (AFIO_FUN_I2C << HTCFG_AFIO_PIN_SHIFT(HTCFG_AFIO_TOGGLE_SCL_GPION)) |
               (AFIO_FUN_I2C << HTCFG_AFIO_PIN_SHIFT(HTCFG_AFIO_TOGGLE_SDA_GPION));

    HTCFG_I2C_SLAVE_PORT->SR = I2C_FLAG_BUSERR | I2C_FLAG_RXNACK;
    /* Toggle AFIO Mode to generate internal START                                                          */
    /*
      Step 1: Configure HTCFG_AFIO_TOGGLE_SCL and HTCFG_AFIO_TOGGLE_SDA to I2C mode,
              and that the internal I/O generates a  SDA/SCL high-levelsignal.
              - HTCFG_AFIO_TOGGLE_SCL : GPIO -> I2C.
              - HTCFG_AFIO_TOGGLE_SDA : GPIO -> I2C.
      Step 2: Configure HTCFG_I2C_SLAVE_SDA to GPIO mode so that the internal I/O generates a low-level 
              SDA signal, generating an internal I2C START signal.
              - HTCFG_I2C_SLAVE_SDA   : I2C -> GPIO.
      Step 3: Configure HTCFG_AFIO_TOGGLE_SDA to GPIO mode so that the internal I/O generates a low-level SDA
              signal, and then generate internal START signal.
              - HTCFG_AFIO_TOGGLE_SDA : I2C -> GPIO.
      Step 4: Configure HTCFG_AFIO_TOGGLE_SCL to GPIO mode.
              - HTCFG_I2C_SLAVE_SCA   : I2C -> GPIO.
      Step 5: Configure HTCFG_I2C_SLAVE_SDA back to I2C mode, and internally reconnect SDA to the real I2C bus
              through AFIO.
              - HTCFG_I2C_SLAVE_SDA   : GPIO -> I2C.
    */
    ww(HTCFG_AFIO_TOGGLE_IO_AFIO_REG, reg1);
    ww(HTCFG_I2C_SLAVE_SDA_AFIO_REG,  reg0 & ~(0xFUL << HTCFG_AFIO_PIN_SHIFT(HTCFG_I2C_SLAVE_SDA_GPION)));
    ww(HTCFG_AFIO_TOGGLE_IO_AFIO_REG, reg1 & ~(0xFUL << HTCFG_AFIO_PIN_SHIFT(HTCFG_AFIO_TOGGLE_SDA_GPION)));
    ww(HTCFG_AFIO_TOGGLE_IO_AFIO_REG, reg1 & ~(0xFUL << HTCFG_AFIO_PIN_SHIFT(HTCFG_AFIO_TOGGLE_SCL_GPION) |
                                               0xFUL << HTCFG_AFIO_PIN_SHIFT(HTCFG_AFIO_TOGGLE_SDA_GPION)));
    ww(HTCFG_I2C_SLAVE_SDA_AFIO_REG,  reg0);
  }

  if (status & I2C_FLAG_TXNRX)
  {
    if (status & I2C_FLAG_ADRS)
    {
      gTx_Index = 0;
      I2C_SendData(HTCFG_I2C_SLAVE_PORT, gI2C_Slave_Buffer_Tx[gTx_Index++]);
    }
    else if (status & I2C_FLAG_TXDE)
    {
      I2C_SendData(HTCFG_I2C_SLAVE_PORT, gI2C_Slave_Buffer_Tx[gTx_Index++]);
    }
  }
  else
  {
    if (status & I2C_FLAG_ADRS)
    {
      gRx_Index = 0;
    }
    else if ((status & I2C_FLAG_RXDNE))
    {
      gI2C_Slave_Buffer_Rx[gRx_Index++] = I2C_ReceiveData(HTCFG_I2C_SLAVE_PORT);
    }
  }

  if(status & I2C_FLAG_TOUTF)
  {
    I2C_DeInit(HTCFG_I2C_SLAVE_PORT);
    I2CSlave_Configuration();
  }

  if (status & I2C_FLAG_RXNACK)
  {
    I2C_ClearFlag(HTCFG_I2C_SLAVE_PORT, I2C_FLAG_RXNACK);
  }
}
