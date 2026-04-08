/*********************************************************************************************************//**
 * @file    I2C/I2C_NoStretch_Slave/main.c
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

/** @addtogroup I2C_Examples I2C
  * @{
  */

/** @addtogroup I2C_NoStretch_Slave
  * @{
  */


/* Private constants ---------------------------------------------------------------------------------------*/
#define I2C_SLAVE_ADDRESS               0x60
#define ClockSpeed                      1000000

#define REGISTAR_SIZE                   256

/* Private function prototypes -----------------------------------------------------------------------------*/
void I2C_Slave_Configuration(void);

void I2C_Slave_SetTransfer(u8 Register_Addr);
void I2C_Slave_TransferInit(void);

void I2C_Slave_SaveReceivedData(u8 Register_Addr);
void I2C_Slave_ReceiveInit(void);

bool CmpBuffer(u8 Register_Addr, u8* Buffer, u32 BufferLength);

/* Private variables ---------------------------------------------------------------------------------------*/
u8 I2C_Master_Setting_Addr = 0x20;
u8 I2C_Master_SingleData = 0x01;
u8 I2C_Master_MultiData[3] = {0xD1, 0xD2, 0xD3};

vu8 I2C_Slave_Register[REGISTAR_SIZE] = {0};
vu8 I2C_Slave_Rx_Buffer[128];

vu8 I2C_Slave_Tx_Addr;
vu8 I2C_Slave_Rx_Index;

vu32 IsI2CFinish = FALSE;
vu32 IsSlaveTxFinish = FALSE;

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
int main(void)
{
  HT32F_DVB_LEDInit(HT_LED1);
  HT32F_DVB_LEDInit(HT_LED2);

  I2C_Slave_Configuration();

  while (1)
  {
    if (I2C_GetFlagStatus(HTCFG_I2C_SLAVE_PORT, I2C_FLAG_OVR))
    {
      while(1){}; // Tx not ready data or Rx loss data during communication.
    }

    if (IsI2CFinish)
    {
      /* I2C Communication Finish                                                                           */
      if (IsSlaveTxFinish)
      {
        /* I2C is Transmit Finish                                                                           */
        I2C_Slave_TransferInit();
        IsSlaveTxFinish = FALSE;
      }
      else
      {
        /* I2C is Receive Finish                                                                            */
        if (I2C_Slave_Rx_Index > 1)
        {
          /* Save to I2C_Slave_Array Data                                                                   */
          I2C_Slave_SaveReceivedData(I2C_Slave_Rx_Buffer[0]);
        }
        else
        {
          /* Prepare the Data to be Transferred                                                             */
          I2C_Slave_SetTransfer(I2C_Slave_Rx_Buffer[0]);
        }
        I2C_Slave_ReceiveInit();
      }
      IsI2CFinish = FALSE;
    }

    if (CmpBuffer(I2C_Master_Setting_Addr, &I2C_Master_SingleData, sizeof(I2C_Master_SingleData)))
    {
      HT32F_DVB_LEDOn(HT_LED1);
      HT32F_DVB_LEDOff(HT_LED2);
    }
    else if (CmpBuffer(I2C_Master_Setting_Addr, I2C_Master_MultiData, sizeof(I2C_Master_MultiData)))
    {
      HT32F_DVB_LEDOff(HT_LED1);
      HT32F_DVB_LEDOn(HT_LED2);
    }
    else
    {
      HT32F_DVB_LEDOff(HT_LED1);
      HT32F_DVB_LEDOff(HT_LED2);
    }
  }
}

/*********************************************************************************************************//**
  * @brief  Configure the I2C.
  * @retval None
  ***********************************************************************************************************/
void I2C_Slave_Configuration(void)
{
  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    HTCFG_I2C_SLAVE_CLK(CKCUClock) = 1;
    CKCUClock.Bit.AFIO             = 1;
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

    I2C_InitStructure.I2C_GeneralCall = DISABLE;
    I2C_InitStructure.I2C_AddressingMode = I2C_ADDRESSING_7BIT;
    I2C_InitStructure.I2C_Acknowledge = ENABLE;
    I2C_InitStructure.I2C_OwnAddress = I2C_SLAVE_ADDRESS;
    I2C_InitStructure.I2C_Speed = 0;
    I2C_InitStructure.I2C_SpeedOffset = 0;
    #if (LIBCFG_I2C_NOSTRETCH)
    I2C_InitStructure.I2C_StretchMode = I2C_STRETCH_NO;
    #endif
    I2C_Init(HTCFG_I2C_SLAVE_PORT, &I2C_InitStructure);
  }
  /* Enable I2C Slave interrupts                                                                            */
  I2C_IntConfig(HTCFG_I2C_SLAVE_PORT, I2C_INT_ADRS | I2C_INT_RXDNE | I2C_INT_RXNACK | I2C_INT_STO, ENABLE);
  NVIC_EnableIRQ(HTCFG_I2C_Slave_IRQn);

  I2C_Cmd(HTCFG_I2C_SLAVE_PORT, ENABLE);
}

/*********************************************************************************************************//**
  * @brief  Configures the I2C Slave to transfer data.
  * @param  Register_Addr: 
  * @retval None
  ***********************************************************************************************************/
void I2C_Slave_SetTransfer(u8 Register_Addr)
{
  I2C_Slave_Tx_Addr = Register_Addr;
  I2C_IntConfig(HTCFG_I2C_SLAVE_PORT, I2C_INT_TXDE, ENABLE);
}

/*********************************************************************************************************//**
  * @brief  Initialize I2C Slave transfer.
  * @retval None
  ***********************************************************************************************************/
void I2C_Slave_TransferInit(void)
{
  I2C_IntConfig(HTCFG_I2C_SLAVE_PORT, I2C_INT_TXDE, DISABLE);      /* Disable TXDE Interrupt                */
  #if (LIBCFG_I2C_NOSTRETCH)
  I2C_TXDRReset(HTCFG_I2C_SLAVE_PORT);                             /* Clear TXDR Register                   */
  #endif
}

/*********************************************************************************************************//**
  * @brief  Saves I2C Slave receive data to the I2C_Slave_Register.
  * @param  Register_Addr: 
  * @retval None
  ***********************************************************************************************************/
void I2C_Slave_SaveReceivedData(u8 Register_Addr)
{
  u8 i;
  for (i = 0; i < I2C_Slave_Rx_Index ; i++)
  {
    I2C_Slave_Register[Register_Addr++] = I2C_Slave_Rx_Buffer[i + 1];
  }
}

/*********************************************************************************************************//**
  * @brief  Initialize I2C Slave receive.
  * @retval None
  ***********************************************************************************************************/
void I2C_Slave_ReceiveInit(void)
{
  I2C_Slave_Rx_Index = 0;                                    /* Clear Rx Index                              */
}

/*********************************************************************************************************//**
  * @brief  Compare buffer with I2C_Slave_Register.
  * @param  Register_Addr: I2C_Slave_Register's address.
  * @param  Buffer: buffers to be compared with I2C_Slave_Register.
  * @param  BufferLength: buffer's length
  * @retval None
  ***********************************************************************************************************/
bool CmpBuffer(u8 Register_Addr, u8* Buffer, u32 BufferLength)
{
  while (BufferLength--)
  {
    if (I2C_Slave_Register[Register_Addr] != *Buffer)
    {
      return FALSE;
    }

    Register_Addr++;
    Buffer++;
  }

  return TRUE;
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
