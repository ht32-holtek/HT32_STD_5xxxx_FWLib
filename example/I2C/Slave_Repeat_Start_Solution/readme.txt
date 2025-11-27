/**
 @page I2C_Slave_Repeat_Start_Solution

 @verbatim
 * @file    I2C/Slave_Repeat_Start_Solution/readme.txt
 * @version V1.00
 * @date    2025-11-05
 * @brief   Description of I2C Slave_Repeat_Start_Solution example.
 @endverbatim

@par Example Description:

This example describes how to handle the reception of a START signal at a specific timing and how to process
the corresponding handler after a timeout occurs.

Repeat START Error Description:

When the I2C slave receives a repeated START (repeat START) during the ACK phase (before the ACK bit is
generated or while the ACK signal is incomplete),
a Bus Error occurs. In this condition, I2C slave cannot send an ACK for the subsequent Address Frame of that
Repeat START. Even if the Bus Error Flag is cleared immediately,
the I2C slave can only resume ACK responses after detecting the next valid START condition.

Internal START Trigger Mechanism:

To allow communication to continue after a Bus Error caused by a repeat START,
I2C slave must detect another START condition before the Address frame is transmitted.
This can be achieved by toggling the SDA AFIO together with two unused I/O pins that also support I2C
functionality,
thereby generating an internal START condition that is only visible to I2C slave and allowing the I2C module to
re-enter a valid communication state.

Timeout Handling:

When a timeout event is detected, perform an I2C IP reset and reconfigure the relevant parameters to restore
normal communication.

@par Directory Contents:

- I2C/Slave_Repeat_Start_Solution/main.c               Main program
- I2C/Slave_Repeat_Start_Solution/ht32fxxxx_it.c       Interrupt handlers

@par Hardware and Software Environment:

- Refer "ht32_board_config.h" for pin assignment.
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
