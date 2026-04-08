/*********************************************************************************************************//**
 * @file    CAN/CAN_Interactive_Demo/main.c
 * @version $Rev:: 9731         $
 * @date    $Date:: 2026-03-31 #$
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
#include "ht32_can_config.h"

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup CAN_Examples CAN
  * @{
  */

/** @addtogroup CAN_Interactive_Demo
  * @{
  */


/* Settings ------------------------------------------------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------------------------------------*/
void CAN_Configuration(void);
void CAN_MsgInit(void);
void CAN_MsgRx(void);
void CAN_MsgTx(char option);
CAN_LastErrorCode_TypeDef CAN_MainRoutine(void);
void Show_Menu(void);
void gTxMsg_Configuration(u32 Id, CAN_IdType_Enum IdType, CAN_FrameType_Enum FrameType);

/* Global variables ----------------------------------------------------------------------------------------*/
CAN_MSG_TypeDef gTxMsg;
CAN_MSG_TypeDef gRx1Msg;
CAN_MSG_TypeDef gRx2Msg;
CAN_MSG_TypeDef gRx3Msg;
CAN_MSG_TypeDef gRx4Msg;
CAN_MSG_TypeDef gRx5Msg;
CAN_MSG_TypeDef gRx6Msg;
CAN_MSG_TypeDef gRx7Msg;
CAN_MSG_TypeDef gRx8Msg;
CAN_MSG_TypeDef gRx9Msg;
CAN_MSG_TypeDef gRx10Msg;
CAN_MSG_TypeDef gRx11Msg;

u8 gTxDataBase = 0x00;

u8 gTxData[160];
u8 gRxData[160];

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
int main(void)
{
  u8 i;
  char option = '\0';
  for(i=0;i< 160;i++)
  {
    gTxData[i] = i;
  }
  RETARGET_Configuration();

  #if (HTCFG_CAN_CONF_CHECK_ENABLE == 1)
  CAN_Config0_Check();
  #endif
  CAN_Configuration();
  CAN_MsgInit();
  Show_Menu();
  while (1)
  {
    if (USART_GetFlagStatus(RETARGET_USART_PORT, USART_FLAG_RXDR) == SET)
    {
      option = USART_ReceiveData(RETARGET_USART_PORT);
      printf("%c\r\n",option);
    }
    CAN_MsgTx(option);
    CAN_MsgRx();
    if(option != '\0')
    {
      option = '\0';
      Show_Menu();
    }
    /* CAN_MainRoutine() is called in the CAN interrupt handler (CANx_IRQHandler).                          */
  }
}

/*********************************************************************************************************//**
  * @brief  Show Menu
  * @retval None
  ***********************************************************************************************************/
void Show_Menu(void)
{
  printf("\r\n====================== CAN Menu ======================\r\n");
  printf("1. Tx ID:0x540, CAN_EXT_ID, DATA, Fixed Data[0....7]\r\n");
  printf("2. Tx ID:0x541, CAN_STD_ID, DATA, Fixed Data[0....7]\r\n");
  printf("3. Tx ID:0x542, CAN_EXT_ID, DATA, Fixed Data[0....7]\r\n");
  printf("4. Tx ID:0x542, CAN_STD_ID, DATA, Fixed Data[0..159]\r\n");
  printf("5. Tx ID:0x543, CAN_EXT_ID, DATA, Send Sequence Number (auto-increment by 1)\r\n");
  printf("6. Tx ID:0x543, CAN_STD_ID, DATA, Send Sequence Number (auto-increment by 2)\r\n");
  printf("7. Tx ID:0x540, CAN_STD_ID, REMOTE\r\n");
  printf("8. Tx ID:0x540540, CAN_EXT_ID, REMOTE\r\n");
  printf("Select: \r\n");
}

/*********************************************************************************************************//**
  * @brief  Setting gTxMsg
  * @retval None
  ***********************************************************************************************************/
void gTxMsg_Configuration(u32 Id, CAN_IdType_Enum IdType, CAN_FrameType_Enum FrameType)
{
  gTxMsg.Id        = Id;
  gTxMsg.IdType    = IdType;
  gTxMsg.FrameType = FrameType;
}

/*********************************************************************************************************//**
  * @brief  CAN_MainRoutine will recover from bus-off state and return the Last Error Code (LEC).
  * @retval CAN_ErrorCode following values:
  *    - NO_ERROR    : No Error
  *    - STUFF_ERROR : Stuff Error
  *    - FORM_ERROR  : Form Error
  *    - ACK_ERROR   : Acknowledgment Error
  *    - BIT1_EROR   : Bit Recessive Error
  *    - BIT0_ERROR  : Bit Dominant Error
  *    - CRC_ERROR   : CRC Error
  *    - NO_CHANGE   : Software Set Error
  ***********************************************************************************************************/
CAN_LastErrorCode_TypeDef CAN_MainRoutine(void)
{
  if (CAN_GetFlagStatus(HTCFG_CAN_PORT, CAN_FLAG_BOFF))
  {
    /* Check if the CAN application is in bus-off state. If so, call the CAN_BusOffRecovery function to     */
    /* attempt recovery.                                                                                    */
    CAN_BusOffRecovery(HTCFG_CAN_PORT);

    /* Monitor bus-off (CAN_FLAG_BOFF).                                                                     */
    /* Example: printf("CAN_FLAG_BOFF: Bus-off detected, recovery initiated\r\n");                          */

    /* Wait until bus-off recovery sequence completes (129 bus idle periods detected).                      */
    while (CAN_GetFlagStatus(HTCFG_CAN_PORT, CAN_FLAG_BOFF) == SET){}
  }
  return CAN_GetLastErrorCode(HTCFG_CAN_PORT);
}

/*********************************************************************************************************//**
  * @brief  CAN_Configuration
  * @retval None
  ***********************************************************************************************************/
void CAN_Configuration(void)
{
  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.AFIO                          = 1;
    CKCUClock.Bit.HTCFG_CAN_IPN                 = 1;
    #if defined(HTCFG_CAN_STB_CONTROL)
    CKCUClock.Bit.HTCFG_CAN_STB_GPIO_CK         = 1;
    #endif
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  /* Configure AFIO as CAN mode                                                                             */
  AFIO_GPxConfig(HTCFG_CAN_TX_GPIO_ID, HTCFG_CAN_TX_AFIO_PIN, AFIO_FUN_CAN);
  AFIO_GPxConfig(HTCFG_CAN_RX_GPIO_ID, HTCFG_CAN_RX_AFIO_PIN, AFIO_FUN_CAN);
  #if defined(HTCFG_CAN_STB_CONTROL)
  AFIO_GPxConfig(HTCFG_CAN_STB_GPIO_ID, HTCFG_CAN_STB_AFIO_PIN, AFIO_FUN_GPIO);
  GPIO_WriteOutBits(HTCFG_CAN_STB_GPIO_PORT, HTCFG_CAN_STB_AFIO_PIN, RESET);
  GPIO_DirectionConfig(HTCFG_CAN_STB_GPIO_PORT, HTCFG_CAN_STB_AFIO_PIN, GPIO_DIR_OUT);
  #endif

  CKCU_SetPeripPrescaler(HTCFG_CAN_CKCU_PCLK, (CKCU_APBCLKPRE_TypeDef)HTCFG_CAN_CF0_CLK_DIV);

  {
    /* !!! NOTICE !!!
       Notice that the local variable (structure) did not have an initial value.
       Please confirm that there are no missing members in the parameter settings below in this function.
    */
    CAN_InitTypeDef CAN_InitStruct;
    CAN_InitStruct.CAN_Mode        = CAN_MODE_NORMAL;
    CAN_InitStruct.CAN_BRPrescaler = CAN_CONF0_PRESCALER;
    CAN_InitStruct.CAN_SJW         = CAN_CONF0_BIT_TIME_SJW;
    CAN_InitStruct.CAN_TSEG0       = CAN_CONF0_BIT_TIME_TSEG0;
    CAN_InitStruct.CAN_TSEG1       = CAN_CONF0_BIT_TIME_TSEG1;
    CAN_InitStruct.CAN_NART        = DISABLE;
    CAN_Init(HTCFG_CAN_PORT, &CAN_InitStruct);
  }
  /* Enable CAN interrupt of NVIC                                                                           */
  NVIC_EnableIRQ(HTCFG_CAN_IRQn);
}

/*********************************************************************************************************//**
  * @brief  CAN_MsgInit
  * @retval None
  ***********************************************************************************************************/
void CAN_MsgInit(void)
{
  u8 i, data[8];
  gRx1Msg.Id        = 0x540;
  gRx1Msg.IdMask    = 0x1FFFFFFF;
  gRx1Msg.IdType    = CAN_EXT_ID;
  gRx1Msg.FrameType = CAN_DATA_FRAME;
  CAN_SetRxMsg(HTCFG_CAN_PORT, &gRx1Msg, 1);

  gRx2Msg.Id        = 0x541;
  gRx2Msg.IdMask    = 0x1FFFFFFF;
  gRx2Msg.IdType    = CAN_EXT_ID;
  gRx2Msg.FrameType = CAN_DATA_FRAME;
  CAN_SetRxMsg(HTCFG_CAN_PORT, &gRx2Msg, 1);

  gRx3Msg.Id        = 0x542;
  gRx3Msg.IdMask    = 0x1FFFFFFF;
  gRx3Msg.IdType    = CAN_EXT_ID;
  gRx3Msg.FrameType = CAN_DATA_FRAME;
  CAN_SetRxMsg(HTCFG_CAN_PORT, &gRx3Msg, 1);

  gRx4Msg.Id        = 0x543;
  gRx4Msg.IdMask    = 0x1FFFFFFF;
  gRx4Msg.IdType    = CAN_EXT_ID;
  gRx4Msg.FrameType = CAN_DATA_FRAME;
  CAN_SetRxMsg(HTCFG_CAN_PORT, &gRx4Msg, 1);

  gRx5Msg.Id        = 0x540540;
  gRx5Msg.IdMask    = 0x1FFFFFFF;
  gRx5Msg.IdType    = CAN_EXT_ID;
  gRx5Msg.FrameType = CAN_DATA_FRAME;
  CAN_SetRxMsg(HTCFG_CAN_PORT, &gRx5Msg, 1);

  gRx6Msg.Id        = 0x540;
  gRx6Msg.IdMask    = 0x7FF;
  gRx6Msg.IdType    = CAN_STD_ID;
  gRx6Msg.FrameType = CAN_DATA_FRAME;
  CAN_SetRxMsg(HTCFG_CAN_PORT, &gRx6Msg, 1);

  gRx7Msg.Id        = 0x541;
  gRx7Msg.IdMask    = 0x7FF;
  gRx7Msg.IdType    = CAN_STD_ID;
  gRx7Msg.FrameType = CAN_DATA_FRAME;
  CAN_SetRxMsg(HTCFG_CAN_PORT, &gRx7Msg, 1);

  gRx8Msg.Id        = 0x542;
  gRx8Msg.IdMask    = 0x7FF;
  gRx8Msg.IdType    = CAN_STD_ID;
  gRx8Msg.FrameType = CAN_DATA_FRAME;
  CAN_SetRxMsg(HTCFG_CAN_PORT, &gRx8Msg, 20);

  gRx9Msg.Id        = 0x543;
  gRx9Msg.IdMask    = 0x1FFFFFFF;
  gRx9Msg.IdType    = CAN_STD_ID;
  gRx9Msg.FrameType = CAN_DATA_FRAME;
  CAN_SetRxMsg(HTCFG_CAN_PORT, &gRx9Msg, 1);


  gRx10Msg.Id        = 0x540;
  gRx10Msg.IdMask    = 0x7FF;
  gRx10Msg.IdType    = CAN_STD_ID;
  gRx10Msg.FrameType = CAN_REMOTE_FRAME;
  CAN_SetRxMsg(HTCFG_CAN_PORT, &gRx10Msg, 1);
  CAN_RemoteFrameAutoReplyCmd(HTCFG_CAN_PORT, &gRx10Msg, ENABLE);
  for (i = 0; i < 8; i++)
  {
    data[i] = 0x11;
  }
  CAN_UpdateTxMsgData(HTCFG_CAN_PORT, &gRx10Msg, data, sizeof(data));

  gRx11Msg.Id        = 0x540540;
  gRx11Msg.IdMask    = 0x1FFFFFFF;
  gRx11Msg.IdType    = CAN_EXT_ID;
  gRx11Msg.FrameType = CAN_REMOTE_FRAME;
  CAN_SetRxMsg(HTCFG_CAN_PORT, &gRx11Msg, 1);
  CAN_RemoteFrameAutoReplyCmd(HTCFG_CAN_PORT, &gRx11Msg, ENABLE);
  for (i = 0; i < 8; i++)
  {
    data[i] = 0x22;
  }
  CAN_UpdateTxMsgData(HTCFG_CAN_PORT, &gRx11Msg, data, sizeof(data));
}

/*********************************************************************************************************//**
  * @brief  CAN_MsgRx
  * @retval None
  ***********************************************************************************************************/
void CAN_MsgRx(void)
{
  u32 i, j, dataLength;
  CAN_MSG_TypeDef *rxMsgs[] = { &gRx1Msg, &gRx2Msg, &gRx3Msg, &gRx4Msg, &gRx5Msg, &gRx6Msg, &gRx7Msg, &gRx8Msg, &gRx9Msg, &gRx10Msg, &gRx11Msg};
  u8 msgCount = 11;

  for (j = 0; j < msgCount; j++)
  {
    if (CAN_Receive(HTCFG_CAN_PORT, rxMsgs[j], gRxData, &dataLength) == MSG_RX_FINISH)
    {
      printf("\r\nRx(==>), ID 0x%06X, ID Type %d, Frame Type %d, length: %d, Data: ",rxMsgs[j]->Id, rxMsgs[j]->IdType, rxMsgs[j]->FrameType, dataLength);
      for (i = 0; i < dataLength; i++) printf("%02X ", gRxData[i]);
      printf("\r\n");
    }
  }
}

/*********************************************************************************************************//**
  * @brief  CAN_MsgTx
  * @retval None
  ***********************************************************************************************************/
void CAN_MsgTx(char option)
{
  u8 len, i, pTxSerialData[8];
  u8 *pTxData;
  pTxData = gTxData;
  switch(option)
  {
    case '1':
      gTxMsg_Configuration(0x540, CAN_EXT_ID, CAN_DATA_FRAME);
      len = 8;
      break;

    case '2':
      gTxMsg_Configuration(0x541, CAN_STD_ID, CAN_DATA_FRAME);
      len = 8;
      break;

    case '3':
      gTxMsg_Configuration(0x542, CAN_EXT_ID, CAN_DATA_FRAME);
      len = 8;
      break;

    case '4':
      gTxMsg_Configuration(0x542, CAN_STD_ID, CAN_DATA_FRAME);
      len = 8 * 20;
      break;

    case '5':
      gTxMsg_Configuration(0x543, CAN_EXT_ID, CAN_DATA_FRAME);
      len = 8;
      for(i = 0; i < len; i++)
      {
        pTxSerialData[i] = gTxDataBase + i;
      }
      gTxDataBase = pTxSerialData[7];
      pTxData = pTxSerialData;
      break;

    case '6':
      gTxMsg_Configuration(0x543, CAN_STD_ID, CAN_DATA_FRAME);
      len = 8;
      for(i = 0; i < len; i++)
      {
        pTxSerialData[i] = gTxDataBase + i * 2;
      }
      gTxDataBase = pTxSerialData[7];
      pTxData = pTxSerialData;
      break;

    case '7':
      gTxMsg_Configuration(0x540, CAN_STD_ID, CAN_REMOTE_FRAME);
      len = 0;
      pTxData = NULL;
      break;

    case '8':
      gTxMsg_Configuration(0x540540, CAN_EXT_ID, CAN_REMOTE_FRAME);
      len = 0;
      pTxData = NULL;
      break;

    default :
      option = '\0';
      break;
  }

  if(option > '\0')
  {
    u8 index = 0;
    do
    {
      u8 dlc = 8;
      if(pTxData == NULL)
      {
        dlc = 0;
      }

      while (CAN_TransmitStatus(HTCFG_CAN_PORT, &gTxMsg) == 0){}  /* Waiting tx Msg idle                    */
      CAN_Transmit(HTCFG_CAN_PORT, &gTxMsg, &pTxData[index], dlc);
      printf("\r\nTx(==>), ID 0x%06X, ID Type %d, Frame Type %d, length: %d, Data: ", gTxMsg.Id, gTxMsg.IdType, gTxMsg.FrameType, dlc);
      for (i = 0; i < dlc; i++)
      {
        printf("%02X ", pTxData[index+i]);
      }
      printf("\r\n");
      index += 8;
    }while(index < len);
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
