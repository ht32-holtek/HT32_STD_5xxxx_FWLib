/**
 @page CAN_Recv_REMOTE_ManualReply

 @verbatim
 * @file    CAN/Recv_REMOTE_ManualReply/main.c
 * @version V1.00
 * @date    2025-09-04
 * @brief   This example describes how to receive CAN remote frames and reply manually.
 @endverbatim

@par Example Description:

This example describes how to configure and use the CAN interface to receive remote frames from a
CAN bus, and reply with CAN data frames manually under software control.
When a remote frame with one of the two specific CAN IDs is received, the firmware checks the
reception and then transmits a corresponding data frame by calling 'CAN_Transmit()' explicitly.
The following CAN receive IDs are used in this example:
  - CAN ID    0x540 (Standard ID), FIFO depth 1 (1 * 8 = 8 byte).
    This ID uses a standard frame format and also ignores bits 2 and 3 of the ID.
    Therefore, it can receive messages with IDs 0x540, 0x544, 0x548, and 0x54C.
  - CAN ID 0x540540 (Extended ID), FIFO depth 1 (1 * 8 = 8 byte).
    This ID uses an extended frame format and has a filtering mask that ignores bits 2 and 3 of the ID.
    Therefore, it can receive messages with IDs 0x540540, 0x540544, 0x540548, and 0x54054C.

Additionally, the system allows the user to input a character via PC serial port. The received
character is used to fill the CAN message data buffer ('gTxData'), demonstrating real-time update
of CAN payload data. The next time a remote frame is received, the updated data will be transmitted.

@par Directory Contents:

- CAN/Recv_REMOTE_ManualReply/main.c               Main program
- CAN/Recv_REMOTE_ManualReply/ht32fxxxxx_nn_it.c   Interrupt handlers
- CAN/Recv_REMOTE_ManualReply/ht32_board_config.h  Board configuration file
- CAN/Recv_REMOTE_ManualReply/ht32_can_config.h    CAN configuration file

@par Hardware and Software Environment:

- Requires a CAN bus setup with nodes transmitting messages with the IDs specified above.
- This example shows the result via RS232.
- Connect a null-modem female/female RS232 cable between the COM1 DB9 connector and PC serial port.
  Hyperterminal configuration:
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - BaudRate = 115200 baud
  - flow control: None
- CAN bit rate: 500K

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
