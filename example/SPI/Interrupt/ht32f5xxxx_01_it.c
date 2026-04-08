/*********************************************************************************************************//**
 * @file    SPI/Interrupt/ht32f5xxxx_01_it.c
 * @version $Rev:: 9671         $
 * @date    $Date:: 2026-03-04 #$
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

/** @addtogroup SPI_Examples SPI
  * @{
  */

/** @addtogroup SPI_Interrupt
  * @{
  */
/* Private constants ---------------------------------------------------------------------------------------*/
#define BufferSize        4

/* Private variables ---------------------------------------------------------------------------------------*/
extern vu8 SPI_Master_Buffer_Tx[];
extern vu8 SPI_Slave_Buffer_Tx[];
extern vu8 SPI_Master_Buffer_Rx[];
extern vu8 SPI_Slave_Buffer_Rx[];
extern vu8 SPI_Master_Tx_Index, SPI_Slave_Tx_Index, SPI_Master_Rx_Index, SPI_Slave_Rx_Index;

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
 * @brief   This function handles common SPI interrupt processing.
 * @retval  None
 ************************************************************************************************************/
__STATIC_INLINE void SPI_Process_Handler(HT_SPI_TypeDef* SPIx, 
                                        vu8* pRxBuffer, vu8* pRxIndex, 
                                        vu8* pTxBuffer, vu8* pTxIndex, 
                                        u32 uBufferSize)
{
  if (SPI_GetFlagStatus(SPIx, SPI_FLAG_RXBNE))
  {
    /* Read received data                                                                                   */
    pRxBuffer[(*pRxIndex)++] = SPI_ReceiveData(SPIx);
  }

  if (SPI_GetFlagStatus(SPIx, SPI_FLAG_TXBE))
  {
    if ((*pTxIndex) < uBufferSize)
    {
      /* Send SPI0 data                                                                                     */
      SPI_SendData(SPIx, pTxBuffer[(*pTxIndex)++]);
    }
    else
    {
      /* Disable SPI0 TXBE interrupt                                                                        */
      SPI_IntConfig(SPIx, SPI_INT_TXBE, DISABLE);
    }
  }
}

#if defined(LIBCFG_SPI_COMBINED_SPI01)
/*********************************************************************************************************//**
 * @brief   This function handles SPI interrupt.
 * @retval  None
 ************************************************************************************************************/
void SPI0_1_IRQHandler(void)
{
  SPI_Process_Handler(HTCFG_SPI_MASTER, 
                     SPI_Master_Buffer_Rx, &SPI_Master_Rx_Index, 
                     SPI_Master_Buffer_Tx, &SPI_Master_Tx_Index, 
                     BufferSize);

  SPI_Process_Handler(HTCFG_SPI_SLAVE, 
                     SPI_Slave_Buffer_Rx, &SPI_Slave_Rx_Index, 
                     SPI_Slave_Buffer_Tx, &SPI_Slave_Tx_Index, 
                     BufferSize);
}
#else
/*********************************************************************************************************//**
 * @brief   This function handles SPI Master interrupt.
 * @retval  None
 ************************************************************************************************************/
void HTCFG_SPI_MASTER_IRQHandler(void)
{
  SPI_Process_Handler(HTCFG_SPI_MASTER, 
                     SPI_Master_Buffer_Rx, &SPI_Master_Rx_Index, 
                     SPI_Master_Buffer_Tx, &SPI_Master_Tx_Index, 
                     BufferSize);
}

/*********************************************************************************************************//**
 * @brief   This function handles SPI Slave interrupt.
 * @retval  None
 ************************************************************************************************************/
void HTCFG_SPI_SLAVE_IRQHandler(void)
{
  SPI_Process_Handler(HTCFG_SPI_SLAVE, 
                     SPI_Slave_Buffer_Rx, &SPI_Slave_Rx_Index, 
                     SPI_Slave_Buffer_Tx, &SPI_Slave_Tx_Index, 
                     BufferSize);
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
