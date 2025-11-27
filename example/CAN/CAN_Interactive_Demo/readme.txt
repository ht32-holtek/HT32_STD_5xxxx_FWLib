/**
 @page CAN_CAN_Interactive_Demo

 @verbatim
 * @file    CAN/CAN_Interactive_Demo/readme.txt
 * @version V1.00
 * @date    2025-09-15
 * @brief   This example demonstrates interactive CAN transmit and receive functions.
 @endverbatim

@par Example Description:

This example shows how to configure the CAN peripheral and verify transmit/receive functions
through an interactive UART menu.
The program allows the user to select different CAN transmit modes (Standard/Extended ID, Data/Remote Frame,
fixed or sequential payloads). It also sets multiple receive filters and prints out received frames in real time.

@par Features in this example:

- **Transmit options via UART menu:**
  1. ID: 0x540, Extended ID, Data Frame, fixed 8-byte data {00..07}
  2. ID: 0x541, Standard ID, Data Frame, fixed 8-byte data {00..07}
  3. ID: 0x542, Extended ID, Data Frame, fixed 8-byte data {00..07}
  4. ID: 0x542, Standard ID, Data Frame, fixed 160-byte data {00..9F} (20 frames)
  5. ID: 0x543, Extended ID, Data Frame, sequential data (auto-increment)
  6. ID: 0x543, Standard ID, Data Frame, sequential data (auto-increment by 2)
  7. ID: 0x540, Standard ID, Remote Frame
  8. ID: 0x540540, Extended ID, Remote Frame

- **Receive filters configured for:**
  - ID: 0x540, Extended ID, Data Frame, FIFO depth 1 (1 * 8 = 8 byte).
  - ID: 0x541, Extended ID, Data Frame, FIFO depth 1 (1 * 8 = 8 byte).
  - ID: 0x542, Extended ID, Data Frame, FIFO depth 1 (1 * 8 = 8 byte).
  - ID: 0x543, Extended ID, Data Frame, FIFO depth 1 (1 * 8 = 8 byte).
  - ID: 0x540540, Extended ID, Data Frame, FIFO depth 1 (1 * 8 = 8 byte).
  - ID: 0x540, Standard ID, Data Frame, FIFO depth 1 (1 * 8 = 8 byte).
  - ID: 0x541, Standard ID, Data Frame, FIFO depth 1 (1 * 8 = 8 byte).
  - ID: 0x542, Standard ID, Data Frame, FIFO depth 20 (20 * 8 = 160 byte).
  - ID: 0x543, Standard ID, Data Frame, FIFO depth 1 (1 * 8 = 8 byte).
  - ID: 0x540, Standard ID, Remote Frame (Automatic reply), FIFO depth 1 (1 * 8 = 8 byte).
  - ID: 0x540540, Extended ID, Remote Frame (Automatic reply), FIFO depth 1 (1 * 8 = 8 byte).

- Total CAN Message Object usage:
  - Transmit: 1 (All transmissions are performed using the same Message Object)
  - Receive:  1 + 1 + 1 + 1 + 1 + 1 + 1 + 20 + 1 + 1 + 1 = 30
    Total used CAN Message Objects = 1 + 29 = 31
  Note: The HT32 CAN IP supports up to 32 Message Objects.

@par Directory Contents:

- CAN/Interactive_Demo/main.c               Main program
- CAN/Interactive_Demo/ht32fxxxxx_nn_it.c   Interrupt handlers
- CAN/Interactive_Demo/ht32_board_config.h  Board configuration file
- CAN/Interactive_Demo/ht32_can_config.h    CAN configuration file

@par Hardware and Software Environment:

- CAN bit rate: 500K
- This example can be run on the HT32 series development kit.
- Refer "ht32_board_config.h" for pin assignment.
- You need a terminal software such as TeraTerm (https://ttssh2.osdn.jp/).
- Connect a null-modem female/female RS232 cable between the UxART and PC serial port.
  Terminal configuration:
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - BaudRate = 115200 baud
  - Flow control: None

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
