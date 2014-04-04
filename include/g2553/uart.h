// Copyright (C) [2012, 2013] [AB2 Technologies] [Austin Beam, Alan Bullick]
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

// MSP430G2553 UART Library
// Simplify use of the MSP430G2553 UART

// Notes:
// The MSP430G2553 only has one hardware UART, so this library is pretty simple.
//
// Example usage:
// #include <msp430g2553.h>
// #include <msp430g2553/uart.h>
//
// int main (void)
// {
//   char c;
//
//   // A new UART @ 16MHz with SMCLK
//   uart myUART = uart(SMCLK, BRCLK_16MHz, B9600);
//
//   myUART.puts("Some string to UART...");
// }
//
// #pragma vector=USCIAB0RX_VECTOR
// __interrupt void my_rx_isr(void)
// {
//   char c = UCA0RXBUF;
//   // do something with character c...
// }

#pragma once
#define AB2_UART

#include <stdarg.h>
#include <stdio.h>
#include <msp430g2553.h>
#include "msp/ab2.h"
#include "pin_fw.h"
#include "usci.h"

// Define the brclk_src options
typedef enum
{
  BRSRC_ACLK = UCSSEL_1,
  BRSRC_SMCLK = UCSSEL_2
} brclk_src_t;

typedef enum
{
  UART_A0 = 0,
  NUM_UART_USCIs
} uart_usci_t;

typedef enum
{
  UART_RXD = 0,
  UART_TXD = 1,
  NUM_UART_PINS
} uart_pin_t;

class uart
{
public:
  uart(uart_usci_t usci = UART_A0,
       brclk_src_t brclk_src = BRSRC_SMCLK,
       uint32_t brclk_freq = F_16MHz,
       uint32_t br = 9600,
       bool ie = true,
       bool osample = false) : uart_usci(usci) {

    switch (uart_usci)
    {
      case UART_A0:
      case NUM_UART_USCIs:
        uart_base_addr = USCI_A0_BASE;
        break;
      default:
        _never_executed();
    }

    init(brclk_src, brclk_freq, br, ie, osample);
  }
  void init(brclk_src_t brclk_src, uint32_t brclk_freq, uint32_t br, bool ie, bool osample);
  char getc(void);
  void putc(char c);
  void puts(char *buf);
  void putsf(const char *fmt, ...);
  void newLine(void);
private:
  uart_usci_t uart_usci;
  uint16_t    uart_base_addr;
  static const msp_pin_t uart_pins[NUM_UART_USCIs][NUM_UART_PINS];
};
