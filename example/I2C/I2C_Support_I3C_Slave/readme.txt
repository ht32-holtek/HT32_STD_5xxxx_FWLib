/**
 @page I2C_Support_I3C_Slave

 @verbatim
 * @file    I2C/I2C_Support_I3C_Slave/readme.txt
 * @version V1.00
 * @date    2022-11-24
 * @brief   Description of I2C slave support I3C example.
 @endverbatim

@par Example Description:

This example describes how to configure an I2C to support I3C-compatible communication as a slave device.

The I2C is configured with the following parameters:
- 7-bit addressing mode
- Slave mode
- Clock stretching disabled (No Stretch)
- Sequential filter enabled to reject glitches shorter than the configured threshold

@par Directory Contents:

- I2C/I2C_Support_I3C_Slave/main.c               Main program
- I2C/I2C_Support_I3C_Slave/ht32fxxxx_it.c       Interrupt handlers

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
