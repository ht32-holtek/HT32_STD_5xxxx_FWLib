/**
 @page I2C_NoStretch_Slave

 @verbatim
 * @file    I2C/I2C_NoStretch_Slave/readme.txt
 * @version V1.00
 * @date    2025-04-24
 * @brief   Description of I2C no-stretch Slave reception and transmission with I2C Master example.
 @endverbatim

@par Example Description:

This example demonstrates how to configure the I2C peripheral as a No-Stretch slave and handle
communication using interrupts.

This code is designed to work in conjunction with the "I2C_NoStretch_Tester" example.

Ensure the I2C master and slave use the same device address and timing parameters (clock speed,
addressing mode, etc.).

After I2C communication completes, the slave determines the next action based on the transfer direction:

In RX direction (Master Write or address phase of Read):
 - If the received length is greater than 1, it indicates a Master Write. The first byte is treated as 
   the target register address, and the remaining bytes are written sequentially to internal registers.
 - If only one byte is received, it indicates a Master Read. The byte specifies the target register,
   and the slave prepares the response data and enables the TXDE interrupt.

In TX direction (when Master Read ends with NACK), the slave disables the TXDE interrupt and clears
the TXDR register to complete the transmission.

At other times, the slave checks the register content. If it matches the Master Single Write data,
LED1 is turned on. If it matches the Master Multi Write data, LED2 is turned on.

@par Directory Contents:

- I2C/I2C_NoStretch_Slave/main.c               Main program
- I2C/I2C_NoStretch_Slave/ht32fxxxx_it.c       Interrupt handlers

@par Hardware and Software Environment:

- Refer "ht32_board_config.h" for pin assignment.
- Remember to connect I2Cx(Master) to I2Cy(Slave) respectively.
- This example shows the result by LED1 and LED2.
- This example can be run on HT32 Series development board.

@par Firmware Disclaimer Information

1. The customer hereby acknowledges and agrees that the program technical documentation, including the
   code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
   proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
   other intellectual property laws.

2. The customer hereby acknowledges and agrees that the program technical documentation, including the
   code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
   other than HOLTEK and the customer.

3. The program technical documentation, including the code, is provided "as is" and for customer reference
   only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
   the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
   the warranties of merchantability, satisfactory quality and fitness for a particular purpose.

 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 */
