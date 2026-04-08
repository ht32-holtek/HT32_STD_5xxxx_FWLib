/**
 @page I2C_NoStretch_Tester

 @verbatim
 * @file    I2C/I2C_NoStretch_Tester/readme.txt
 * @version V1.00
 * @date    2022-11-24
 * @brief   Description of I2C Master transmission and reception with a no-stretch Slave example.
 @endverbatim

@par Example Description:

This example demonstrates how to configure the I2C peripheral in Master mode and communicate with
an I2C Slave device that does not support clock stretching (No-Stretch mode).

This code is designed to work in conjunction with the "I2C_NoStretch_Slave" example.

Ensure that both the I2C master and slave are configured with the same device address and
communication parameters, including clock speed and addressing mode.

The transmission sequence is as follows:
1. Single Write:
     The master transmits a target register address followed by one data byte.
2. Single Read:
     The master transmits a target register address, then reads one data byte from that address.
     If the received value matches the expected data, LED1 is turned ON; otherwise, it is turned OFF.
3. Multi Write:
     The master transmits a target register address followed by three consecutive data bytes.
4. Multi Read:
     The master transmits a target register address, then reads three data bytes from that address.
     If all received bytes match the expected data, LED2 is turned ON; otherwise, it is turned OFF.

@par Directory Contents:

- I2C/I2C_NoStretch_Tester/main.c               Main program
- I2C/I2C_NoStretch_Tester/ht32fxxxx_it.c       Interrupt handlers

@par Hardware and Software Environment:

- Refer "ht32_board_config.h" for pin assignment.
- Remember to connect I2Cx to other I2Cy slave respectively.
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
