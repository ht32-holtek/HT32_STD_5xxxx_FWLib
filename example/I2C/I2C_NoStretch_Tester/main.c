/*********************************************************************************************************//**
 * @file    I2C/I2C_NoStretch_Tester/main.c
 * @version $Rev:: 9565         $
 * @date    $Date:: 2025-11-18 #$
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

/** @addtogroup I2C_NoStretch_Tester
  * @{
  */


/* Private constants ---------------------------------------------------------------------------------------*/
#define I2C_MASTER_ADDRESS              0x0A
#define I2C_SLAVE_ADDRESS               0x60
#define ClockSpeed                      1000000

/* Private function prototypes -----------------------------------------------------------------------------*/
void I2C_Master_Configuration(void);

void I2C_Master_SingleWrite(void);
void I2C_Master_SingleRead(void);
void I2C_Master_MultiWrite(void);
void I2C_Master_MultiRead(void);

void I2C_Master_Transmit(u8 *Tx_Buffer, u8 Tx_Buffersize);
void I2C_Master_Receive(u8 *Rx_Buffer, u8 Rx_Buffersize);

bool CmpBuffer(u8* Buffer1, u8* Buffer2, u32 BufferLength);

static void __Delay(u32 count);

/* Private variables ---------------------------------------------------------------------------------------*/
u8 I2C_Master_Setting_Addr = 0x20;
u8 I2C_Master_SingleData = 0x01;
u8 I2C_Master_MultiData[3] = {0xD1, 0xD2, 0xD3};

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
int main(void)
{
  /* Initialize LED1 & LED2 on HT32 board                                                                   */
  HT32F_DVB_LEDInit(HT_LED1);
  HT32F_DVB_LEDInit(HT_LED2);

  /* I2C configuration                                                                                      */
  I2C_Master_Configuration();

  /* I2C communication                                                                                      */
  I2C_Master_SingleWrite();
  __Delay(200);
  I2C_Master_SingleRead();
  __Delay(200);

  I2C_Master_MultiWrite();
  __Delay(200);
  I2C_Master_MultiRead();
  __Delay(200);

  while (1);
}

/*********************************************************************************************************//**
  * @brief  Configure the I2C.
  * @retval None
  ***********************************************************************************************************/
void I2C_Master_Configuration(void)
{
  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    HTCFG_I2C_MASTER_CLK(CKCUClock) = 1;
    CKCUClock.Bit.AFIO              = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  /* Configure GPIO to I2C mode for Master                                                                  */
  AFIO_GPxConfig(HTCFG_I2C_MASTER_SCL_GPIO_ID, HTCFG_I2C_MASTER_SCL_AFIO_PIN, AFIO_FUN_I2C);
  AFIO_GPxConfig(HTCFG_I2C_MASTER_SDA_GPIO_ID, HTCFG_I2C_MASTER_SDA_AFIO_PIN, AFIO_FUN_I2C);

  { /* I2C Master configuration                                                                             */

    /* !!! NOTICE !!!
       Notice that the local variable (structure) did not have an initial value.
       Please confirm that there are no missing members in the parameter settings below in this function.
    */
    I2C_InitTypeDef  I2C_InitStructure;

    I2C_InitStructure.I2C_GeneralCall = DISABLE;
    I2C_InitStructure.I2C_AddressingMode = I2C_ADDRESSING_7BIT;
    I2C_InitStructure.I2C_Acknowledge = DISABLE;
    I2C_InitStructure.I2C_OwnAddress = I2C_MASTER_ADDRESS;
    I2C_InitStructure.I2C_Speed = ClockSpeed;
    I2C_InitStructure.I2C_SpeedOffset = 0;
    #if (LIBCFG_I2C_NOSTRETCH)
    I2C_InitStructure.I2C_StretchMode = I2C_STRETCH_YES;
    #endif
    I2C_Init(HTCFG_I2C_MASTER_PORT, &I2C_InitStructure);
  }

  /* Enable I2C                                                                                             */
  I2C_Cmd(HTCFG_I2C_MASTER_PORT, ENABLE);
}

/*********************************************************************************************************//**
  * @brief  I2C Master writes data to a slave register via single-write.
  * @retval None
  ***********************************************************************************************************/
void I2C_Master_SingleWrite(void)
{
  u8 I2C_Master_Tx_Buffer[sizeof(I2C_Master_Setting_Addr) + sizeof(I2C_Master_SingleData)];

  /* Setting I2C Master Transmitter Data.                                                                   */
  I2C_Master_Tx_Buffer[0] = I2C_Master_Setting_Addr;           // Setting Target Register Address
  I2C_Master_Tx_Buffer[1] = I2C_Master_SingleData;             // Setting Target Single Register Data

  /* Start Transmitter.                                                                                     */
  I2C_Master_Transmit(I2C_Master_Tx_Buffer, sizeof(I2C_Master_Tx_Buffer));
}

/*********************************************************************************************************//**
  * @brief  I2C Master reads data from a slave register via single-read.
  * @retval None
  ***********************************************************************************************************/
void I2C_Master_SingleRead(void)
{
  u8 I2C_Master_Tx_Buffer[1];
  u8 I2C_Master_Rx_Buffer[1];

  /* Setting I2C Master Transmitter Data.                                                                   */
  I2C_Master_Tx_Buffer[0] = I2C_Master_Setting_Addr;           // Setting Target Register Address

  /* Start Transmitter.                                                                                     */
  I2C_Master_Transmit(I2C_Master_Tx_Buffer, sizeof(I2C_Master_Tx_Buffer));

  __Delay(100);                                                // Waiting for the Slave to prepare data

  /* Start Receiver.                                                                                        */
  I2C_Master_Receive(I2C_Master_Rx_Buffer, sizeof(I2C_Master_Rx_Buffer));

  /* Check on validity of received data on I2Cx.                                                            */
  if (CmpBuffer(I2C_Master_Rx_Buffer, &I2C_Master_SingleData, sizeof(I2C_Master_Rx_Buffer)))
  {
    HT32F_DVB_LEDOn(HT_LED1);
  }
  else
  {
    HT32F_DVB_LEDOff(HT_LED1);
  }
}

/*********************************************************************************************************//**
  * @brief  I2C Master write multi data to multiple slave registers via multi-write.
  * @retval None
  ***********************************************************************************************************/
void I2C_Master_MultiWrite(void)
{
  u8 i;
  u8 I2C_Master_Tx_Buffer[sizeof(I2C_Master_Setting_Addr) + sizeof(I2C_Master_MultiData)];

  /* Setting I2C Master Transmitter Data.                                                                   */
  I2C_Master_Tx_Buffer[0] = I2C_Master_Setting_Addr;           // Setting Target Register Address
  for (i = 0; i < sizeof(I2C_Master_MultiData); i++)
  {
    I2C_Master_Tx_Buffer[i + 1] = I2C_Master_MultiData[i];     // Setting Target Multi Register Datas
  }

  /* Start Transmitter.                                                                                     */
  I2C_Master_Transmit(I2C_Master_Tx_Buffer, sizeof(I2C_Master_Tx_Buffer));
}

/*********************************************************************************************************//**
  * @brief  I2C Master read multi data to multiple slave registers via multi-read.
  * @retval None
  ***********************************************************************************************************/
void I2C_Master_MultiRead(void)
{
  u8 I2C_Master_Tx_Buffer[1];
  u8 I2C_Master_Rx_Buffer[3];

  /* Setting I2C Master Transmitter Data.                                                                   */
  I2C_Master_Tx_Buffer[0] = I2C_Master_Setting_Addr;           // Setting Target Register Address

  /* Start Transmitter.                                                                                     */
  I2C_Master_Transmit(I2C_Master_Tx_Buffer, sizeof(I2C_Master_Tx_Buffer));

  __Delay(100);                                                // Waiting for the Slave to prepare data

  /* Start Receiver.                                                                                        */
  I2C_Master_Receive(I2C_Master_Rx_Buffer, sizeof(I2C_Master_Rx_Buffer));

  /* Check on validity of received data on I2Cx.                                                            */
  if (CmpBuffer(I2C_Master_Rx_Buffer, I2C_Master_MultiData, sizeof(I2C_Master_Rx_Buffer)))
  {
    HT32F_DVB_LEDOn(HT_LED2);
  }
  else
  {
    HT32F_DVB_LEDOff(HT_LED2);
  }
}

/*********************************************************************************************************//**
  * @brief  I2C Master transfers data to another device.
  * @param  Tx_Buffer: Pointer to the data buffer to be transmitted.
  * @param  Tx_Buffersize: Number of bytes to transmit from the buffer.
  * @retval None
  ***********************************************************************************************************/
void I2C_Master_Transmit(u8 *Tx_Buffer, u8 Tx_Buffersize)
{
  u8 Tx_Index = 0;
  /*--------------------------------------------------------------------------------------------------------*/
  /* Master Transmitter                                                                                     */
  /*--------------------------------------------------------------------------------------------------------*/
  /* Send I2C START & I2C slave address for write                                                           */
  I2C_TargetAddressConfig(HTCFG_I2C_MASTER_PORT, I2C_SLAVE_ADDRESS, I2C_MASTER_WRITE);

  /* Check on Master Transmitter STA condition and clear it                                                 */
  while (!I2C_CheckStatus(HTCFG_I2C_MASTER_PORT, I2C_MASTER_SEND_START));

  /* Check on Master Transmitter ADRS condition and clear it                                                */
  while (!I2C_CheckStatus(HTCFG_I2C_MASTER_PORT, I2C_MASTER_TRANSMITTER_MODE));
  /* Send data                                                                                              */
  while (Tx_Index < Tx_Buffersize)
  {
    /* Check on Master Transmitter TXDE condition                                                           */
    while (!I2C_CheckStatus(HTCFG_I2C_MASTER_PORT, I2C_MASTER_TX_EMPTY));
    /* Master Send I2C data                                                                                 */
    I2C_SendData(HTCFG_I2C_MASTER_PORT, Tx_Buffer[Tx_Index ++]);
  }
  /* Send I2C STOP condition                                                                                */
  I2C_GenerateSTOP(HTCFG_I2C_MASTER_PORT);
  /*wait for BUSBUSY become idle                                                                            */
  while (I2C_ReadRegister(HTCFG_I2C_MASTER_PORT, I2C_REGISTER_SR) & 0x80000);
}

/*********************************************************************************************************//**
  * @brief  I2C Master receives data to another device.
  * @param  Rx_Buffer: Pointer to the buffer where received data will be stored.
  * @param  Rx_Buffersize: Size of the buffer and number of bytes to receive.
  * @retval None
  ***********************************************************************************************************/
void I2C_Master_Receive(u8 *Rx_Buffer, u8 Rx_Buffersize)
{
  u8 Rx_Index = 0;
  /*--------------------------------------------------------------------------------------------------------*/
  /* Master Receiver                                                                                        */
  /*--------------------------------------------------------------------------------------------------------*/
  /* Send I2C START & I2C slave address for read                                                            */
  I2C_TargetAddressConfig(HTCFG_I2C_MASTER_PORT, I2C_SLAVE_ADDRESS, I2C_MASTER_READ);

  /* Check on Master Transmitter STA condition and clear it                                                 */
  while (!I2C_CheckStatus(HTCFG_I2C_MASTER_PORT, I2C_MASTER_SEND_START));

  /* Check on Master Transmitter ADRS condition and clear it                                                */
  while (!I2C_CheckStatus(HTCFG_I2C_MASTER_PORT, I2C_MASTER_RECEIVER_MODE));

  I2C_AckCmd(HTCFG_I2C_MASTER_PORT, ENABLE);
  /* Receive data                                                                                           */
  while (Rx_Index < Rx_Buffersize)
  {
    if (Rx_Index == (Rx_Buffersize - 1))
    {
      I2C_AckCmd(HTCFG_I2C_MASTER_PORT, DISABLE);
    }
    /* Check on Slave Receiver RXDNE condition                                                              */
    while (!I2C_CheckStatus(HTCFG_I2C_MASTER_PORT, I2C_MASTER_RX_NOT_EMPTY));
    /* Store received data on I2C1                                                                          */
    Rx_Buffer[Rx_Index ++] = I2C_ReceiveData(HTCFG_I2C_MASTER_PORT);
  }
  /* Send I2C STOP condition                                                                                */
  I2C_GenerateSTOP(HTCFG_I2C_MASTER_PORT);
  /*wait for BUSBUSY become idle                                                                            */
  while (I2C_ReadRegister(HTCFG_I2C_MASTER_PORT, I2C_REGISTER_SR) & 0x80000);
}


/*********************************************************************************************************//**
  * @brief  Compare two buffers.
  * @param  Buffer1, Buffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval None
  ***********************************************************************************************************/
bool CmpBuffer(u8* Buffer1, u8* Buffer2, u32 BufferLength)
{
  while (BufferLength--)
  {
    if (*Buffer1 != *Buffer2)
    {
      return FALSE;
    }

    Buffer1++;
    Buffer2++;
  }

  return TRUE;
}

/* Private functions ---------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  delay function
  * @param  count: delay count for loop
  * @retval None
  ***********************************************************************************************************/
static void __Delay(u32 count)
{
  while (count--)
  {
    __NOP(); // Prevent delay loop be optimized
  }
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
