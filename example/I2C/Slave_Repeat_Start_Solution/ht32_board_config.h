/*********************************************************************************************************//**
 * @file    I2C/Slave_Repeat_Start_Solution/ht32_board_config.h
 * @version $Rev:: 9723         $
 * @date    $Date:: 2026-03-25 #$
 * @brief   The header file of board configuration.
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
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32_BOARD_CONFIG_H
#define __HT32_BOARD_CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Settings ------------------------------------------------------------------------------------------------*/
#if defined(USE_HT32F50230_SK)
  #define _HTCFG_SLAVE_I2CX                 I2C0

  #define _HTCFG_I2C_SLAVE_SCL_GPIOX        B
  #define  HTCFG_I2C_SLAVE_SCL_GPION        15
  #define _HTCFG_I2C_SLAVE_SDA_GPIOX        C
  #define  HTCFG_I2C_SLAVE_SDA_GPION        0

  #define _HTCFG_AFIO_TOGGLE_IO_GPIOX       A
  #define  HTCFG_AFIO_TOGGLE_SCL_GPION      14
  #define  HTCFG_AFIO_TOGGLE_SDA_GPION      15
#endif

#if defined(USE_HT32F61152_DVB)
  #define _HTCFG_SLAVE_I2CX                 I2C0

  #define _HTCFG_I2C_SLAVE_SCL_GPIOX        B
  #define  HTCFG_I2C_SLAVE_SCL_GPION        15
  #define _HTCFG_I2C_SLAVE_SDA_GPIOX        C
  #define  HTCFG_I2C_SLAVE_SDA_GPION        0

  #define _HTCFG_AFIO_TOGGLE_IO_GPIOX       A
  #define  HTCFG_AFIO_TOGGLE_SCL_GPION      6
  #define  HTCFG_AFIO_TOGGLE_SDA_GPION      7
#endif

#define HTCFG_AFIO_TOGGLE_IO_CK             STRCAT2(P,               _HTCFG_AFIO_TOGGLE_IO_GPIOX)
#define HTCFG_AFIO_TOGGLE_IO_ID             STRCAT2(GPIO_P,          _HTCFG_AFIO_TOGGLE_IO_GPIOX)
#define HTCFG_AFIO_TOGGLE_IO_PORT           STRCAT2(HT_GPIO,         _HTCFG_AFIO_TOGGLE_IO_GPIOX)
#define HTCFG_AFIO_TOGGLE_SCL_AFIO_PIN      STRCAT2(AFIO_PIN_,       HTCFG_AFIO_TOGGLE_SCL_GPION)
#define HTCFG_AFIO_TOGGLE_SCL_GPIO_PIN      STRCAT2(GPIO_PIN_,       HTCFG_AFIO_TOGGLE_SCL_GPION)
#define HTCFG_AFIO_TOGGLE_SDA_AFIO_PIN      STRCAT2(AFIO_PIN_,       HTCFG_AFIO_TOGGLE_SDA_GPION)
#define HTCFG_AFIO_TOGGLE_SDA_GPIO_PIN      STRCAT2(GPIO_PIN_,       HTCFG_AFIO_TOGGLE_SDA_GPION)

#define HTCFG_I2C_SLAVE_SCL_GPIO_ID         STRCAT2(GPIO_P,           _HTCFG_I2C_SLAVE_SCL_GPIOX)
#define HTCFG_I2C_SLAVE_SDA_GPIO_ID         STRCAT2(GPIO_P,           _HTCFG_I2C_SLAVE_SDA_GPIOX)
#define HTCFG_I2C_SLAVE_SCL_AFIO_PIN        STRCAT2(GPIO_PIN_,         HTCFG_I2C_SLAVE_SCL_GPION)
#define HTCFG_I2C_SLAVE_SDA_AFIO_PIN        STRCAT2(GPIO_PIN_,         HTCFG_I2C_SLAVE_SDA_GPION)

#define HTCFG_I2C_SLAVE_CK                  _HTCFG_SLAVE_I2CX
#define HTCFG_I2C_SLAVE_PORT                STRCAT2(HT_,                       _HTCFG_SLAVE_I2CX)
#define HTCFG_I2C_SLAVE_IRQn                STRCAT2(_HTCFG_SLAVE_I2CX,                     _IRQn)
#define HTCFG_I2C_SLAVE_IRQHandler          STRCAT2(_HTCFG_SLAVE_I2CX,               _IRQHandler)

#define HTCFG_I2C_SLAVE_SDA_AFIO_REG        HTCFG_GET_AFIO_REG(_HTCFG_I2C_SLAVE_SDA_GPIOX, HTCFG_I2C_SLAVE_SDA_GPION)
#define HTCFG_AFIO_TOGGLE_IO_AFIO_REG       HTCFG_GET_AFIO_REG(_HTCFG_AFIO_TOGGLE_IO_GPIOX, HTCFG_AFIO_TOGGLE_SCL_GPION)

/*
  Address calculation explanation
  1. HT_AFIO_BASE + 0x20:
    Base offset of AFIO configuration registers.
  2. ((STRCAT2(0x, GPIOX)) - 0xA) << 3:
    Converts GPIO port letter (A~E) into a numeric index.
    Each GPIO port occupies 8 bytes in the AFIO register map:
    Port A=0, B=8, C=16, D=24, E=32.
  3. ((GPION) > 7 ? 1 : 0) << 2:
    Determines whether the pin belongs to the low or high group.
    Pin >7  : high register (H), offset +4 bytes.
    Pin <=7 : low register  (L), no additional offset.
  --------------------------------------------------
  Example:
  GPIOX = C, GPION = 0
   - STRCAT2(0x, C) = 0xC
   - (0xC - 0xA) = 2
   - (2 << 3) = 0x10
   - ((0 > 7) ? 1 : 0) << 2 = 0
  Resulting address = HT_AFIO_BASE + 0x20 + 0x10 + 0 = HT_AFIO_BASE + 0x30

  This address corresponds to: HT_AFIO->GPCCFGR[0]
  (the AFIO configuration register for GPIOC low pins, controlling GPIOC Pin 0~7 alternate functions)
*/
#define HTCFG_GET_AFIO_REG(GPIOX, GPION) (HT_AFIO_BASE + 0x20 + (((STRCAT2(0x, GPIOX)) - 0xA) << 3) + (((GPION) > 7 ? 1 : 0) << 2))

/*
  General macro: calculate AFIO pin bit shift
  Input: pin number GPION (0~15)
  If pin > 7, belongs to high register, subtract 8 to locate correct bit position
  Each pin occupies 4 bits, so shift left by 2
 */
#define HTCFG_AFIO_PIN_SHIFT(GPION) (((GPION) > 7 ? ((GPION) - 8) : (GPION)) << 2)

#ifdef __cplusplus
}
#endif

#endif
