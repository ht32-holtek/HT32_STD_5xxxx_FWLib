/*********************************************************************************************************//**
 * @file    CORDIC/Sin_PDMA/main.c
 * @version $Rev:: 9820         $
 * @date    $Date:: 2026-05-27 #$
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

/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup CORDIC_Examples CORDIC
  * @{
  */

/** @addtogroup Sin_PDMA
  * @{
  */


/* Private types -------------------------------------------------------------------------------------------*/
typedef enum {Fail = 0, Pass = !Fail} TestResult;

/* Private constants ---------------------------------------------------------------------------------------*/
#define ARRAY_SIZE        64U
#define DELTA             (s32)0x00001000       /* Max residual error for cos and sin, with 6 cycle precision:
                                                       2^-19 max residual error, ie 31-19=12 LSB, ie <0x1000*/

/* Private function prototypes -----------------------------------------------------------------------------*/
void PDMA_Configuration(void);
void CORDIC_Configuration(void);
void CORDIC_Test(void);
TestResult Check_Residual_Error(s32 VarA, s32 VarB, s32 MaxError);

/* Private variables ---------------------------------------------------------------------------------------*/
static s32 aAngles[ARRAY_SIZE] =
{
  0x00000000, 0x04000000, 0x08000000, 0x0C000000,
  0x10000000, 0x14000000, 0x18000000, 0x1C000000,
  0x20000000, 0x24000000, 0x28000000, 0x2C000000,
  0x30000000, 0x34000000, 0x38000000, 0x3C000000,
  0x40000000, 0x44000000, 0x48000000, 0x4C000000,
  0x50000000, 0x54000000, 0x58000000, 0x5C000000,
  0x60000000, 0x64000000, 0x68000000, 0x6C000000,
  0x70000000, 0x74000000, 0x78000000, 0x7C000000,
  0x80000000, 0x84000000, 0x88000000, 0x8C000000,
  0x90000000, 0x94000000, 0x98000000, 0x9C000000,
  0xA0000000, 0xA4000000, 0xA8000000, 0xAC000000,
  0xB0000000, 0xB4000000, 0xB8000000, 0xBC000000,
  0xC0000000, 0xC4000000, 0xC8000000, 0xCC000000,
  0xD0000000, 0xD4000000, 0xD8000000, 0xDC000000,
  0xE0000000, 0xE4000000, 0xE8000000, 0xEC000000,
  0xF0000000, 0xF4000000, 0xF8000000, 0xFC000000
};

/* Array of reference sines in Q1.31 format */
static s32 aRefSin[ARRAY_SIZE] =
{
  0x00000000, 0x0C8BD35E, 0x18F8B83C, 0x25280C5D,
  0x30FBC54D, 0x3C56BA70, 0x471CECE6, 0x5133CC94,
  0x5A827999, 0x62F201AC, 0x6A6D98A4, 0x70E2CBC6,
  0x7641AF3C, 0x7A7D055B, 0x7D8A5F3F, 0x7F62368F,
  0x80000000, 0x7F62368F, 0x7D8A5F3F, 0x7A7D055B,
  0x7641AF3C, 0x70E2CBC6, 0x6A6D98A4, 0x62F201AC,
  0x5A827999, 0x5133CC94, 0x471CECE6, 0x3C56BA70,
  0x30FBC54D, 0x25280C5D, 0x18F8B83C, 0x0C8BD35E,
  0x00000000, 0xF3742CA2, 0xE70747C4, 0xDAD7F3A3,
  0xCF043AB3, 0xC3A94590, 0xB8E3131A, 0xAECC336C,
  0xA57D8667, 0x9D0DFE54, 0x9592675C, 0x8F1D343A,
  0x89BE50C4, 0x8582FAA5, 0x8275A0C1, 0x809DC971,
  0x80000000, 0x809DC971, 0x8275A0C1, 0x8582FAA5,
  0x89BE50C4, 0x8F1D343A, 0x9592675C, 0x9D0DFE54,
  0xA57D8667, 0xAECC336C, 0xB8E3131A, 0xC3A94590,
  0xCF043AB3, 0xDAD7F3A3, 0xE70747C4, 0xF3742CA2
};

/* Array of calculated sines in Q1.31 format */
static s32 aCalculatedSin[ARRAY_SIZE * 2];

/* Global functions ----------------------------------------------------------------------------------------*/
/*********************************************************************************************************//**
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
int main(void)
{
  RETARGET_Configuration();

  /* Initialize LED1 & LED2 on HT32 board                                                                   */
  HT32F_DVB_LEDInit(HT_LED1);
  HT32F_DVB_LEDInit(HT_LED2);

  PDMA_Configuration();

  CORDIC_Configuration();                /* CORDIC configuration                                            */

  CORDIC_Test();

  while(1)
  {
  }
}

/*********************************************************************************************************//**
  * @brief  Configure the PDMA.
  * @retval None
  ***********************************************************************************************************/
void PDMA_Configuration(void)
{
  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
    CKCUClock.Bit.PDMA  = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  {
    /* !!! NOTICE !!!
       Notice that the local variable (structure) did not have an initial value.
       Please confirm that there are no missing members in the parameter settings below in this function.
    */
    PDMACH_InitTypeDef PDMACH_InitStructure;

    /* CORDIC Write PDMA Channel configuration                                                              */
    PDMACH_InitStructure.PDMACH_SrcAddr = (u32) &aAngles;
    PDMACH_InitStructure.PDMACH_DstAddr = (u32) &HT_CORDIC->WDATA;
    PDMACH_InitStructure.PDMACH_BlkCnt = ARRAY_SIZE;
    PDMACH_InitStructure.PDMACH_BlkLen = 1;
    PDMACH_InitStructure.PDMACH_DataSize = WIDTH_32BIT;
    PDMACH_InitStructure.PDMACH_Priority = H_PRIO;
    PDMACH_InitStructure.PDMACH_AdrMod = SRC_ADR_LIN_INC | DST_ADR_FIX;
    PDMA_Config(PDMA_CORDIC_WR, &PDMACH_InitStructure);

    /* CORDIC Read PDMA Channel configuration                                                               */
    PDMACH_InitStructure.PDMACH_SrcAddr = (u32) &HT_CORDIC->RDATA;
    PDMACH_InitStructure.PDMACH_DstAddr = (u32) &aCalculatedSin;
    PDMACH_InitStructure.PDMACH_BlkCnt = ARRAY_SIZE; // Need to check NRES bit, one output data or two output data
    PDMACH_InitStructure.PDMACH_Priority = M_PRIO;
    PDMACH_InitStructure.PDMACH_AdrMod = SRC_ADR_FIX | DST_ADR_LIN_INC;
    PDMA_Config(PDMA_CORDIC_RD, &PDMACH_InitStructure);
  }

  /* Enable CORDIC Write GE & TC interrupt                                                                  */
  PDMA_IntConfig(PDMA_CORDIC_WR, PDMA_INT_GE | PDMA_INT_TC, ENABLE);

  /* Enable CORDIC Read GE & TC interrupt                                                                   */
  PDMA_IntConfig(PDMA_CORDIC_RD, PDMA_INT_GE | PDMA_INT_TC, ENABLE);

  /* Enable the corresponding PDMA channel                                                                  */
  PDMA_EnaCmd(PDMA_CORDIC_WR, ENABLE);
  PDMA_EnaCmd(PDMA_CORDIC_RD, ENABLE);
}

/*********************************************************************************************************//**
  * @brief  Configure the CORDIC.
  * @retval None
  ***********************************************************************************************************/
void CORDIC_Configuration(void)
{
  { /* Enable peripheral clock                                                                              */
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
    CKCUClock.Bit.CORDIC = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);
  }

  { /* CORDIC configuration                                                                                 */

    /* !!! NOTICE !!!
       Notice that the local variable (structure) did not have an initial value.
       Please confirm that there are no missing members in the parameter settings below in this function.
    */
    CORDIC_InitTypeDef CORDIC_InitStructure;
    CORDIC_InitStructure.Function  = CORDIC_FUNCTION_SINE;     /* SINE function */
    CORDIC_InitStructure.Precision = CORDIC_PRECISION_6CYCLES; /* max precision for q1.31 cosine */
    CORDIC_InitStructure.Scale     = CORDIC_SCALE_0;           /* no scale */
    CORDIC_InitStructure.NbWrite   = CORDIC_NBWRITE_1;         /* One input data: angle. Second input data (modulus) is 1 after cordic reset */
    CORDIC_InitStructure.NbRead    = CORDIC_NBREAD_1;          /* One output data: sine */
    CORDIC_InitStructure.InSize    = CORDIC_INSIZE_32BITS;     /* q1.31 format for input data */
    CORDIC_InitStructure.OutSize   = CORDIC_OUTSIZE_32BITS;    /* q1.31 format for output data */

    CORDIC_Init(&CORDIC_InitStructure);
  }
}

/*********************************************************************************************************//**
  * @brief  CORDIC Test.
  * @retval None
  ***********************************************************************************************************/
void CORDIC_Test(void)
{
  u8 i = 0;
  u8 err = 0;

  /* Enable CORDIC PDMA WEN and REN request                                                               */
  CORDIC_PDMACmd(CORDIC_DMA_WEN | CORDIC_DMA_REN, ENABLE);

  while (!PDMA_GetFlagStatus(PDMA_CORDIC_RD, PDMA_FLAG_TC));

  for (i = 0; i < ARRAY_SIZE; i++)
  {
    if (Check_Residual_Error(aCalculatedSin[i], aRefSin[i], DELTA) == Fail)
    {
      err++;
    }
  }

  if (err != 0)
  {
    HT32F_DVB_LEDOn(HT_LED2);

    printf("Result: Unexpected result.\r\n");
    printf("Check residual error on aCalculatedSin vs. aRefSin fail.\r\n");
  }
  else
  {
    HT32F_DVB_LEDOn(HT_LED1);

    printf("Result: Success.\r\n");
    printf("\r\n");
  }

  #if 1
  {
    s32 diff = 0;
    for (i = 0; i < ARRAY_SIZE; i++)
    {
      diff = aCalculatedSin[i] - aRefSin[i];
      if (diff < 0)
      {
        diff = -diff;
      }
      printf("aCalculatedSin[%03d] = 0x%08X, aRefSin[%03d] = 0x%08X, Diff = 0x%08X\r\n", i, aCalculatedSin[i], i, aRefSin[i], diff);
    }
  }
  #endif
}

/*********************************************************************************************************//**
  * @brief  Check delta between two values is below threshold.
  * @param  VarA: First input variable
  * @param  VarB: Second input variable
  * @param  MaxError: Maximum delta allowed between VarA and VarB
  * @retval Pass or Fail
  ***********************************************************************************************************/
TestResult Check_Residual_Error(s32 VarA, s32 VarB, s32 MaxError)
{
  if ((VarA - VarB) >= 0)
  {
    if ((VarA - VarB) > MaxError)
    {
      return Fail;
    }
  }
  else
  {
    if ((VarB - VarA) > MaxError)
    {
      return Fail;
    }
  }

  return Pass;
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
