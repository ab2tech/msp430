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

#define UART_PUTSF_BUF_SIZE 256

// Define some common brclk_freq values
typedef enum
{
  BRCLK_32kHz, BRCLK_1MHz, BRCLK_4MHz, BRCLK_8MHz, BRCLK_12MHz,
  BRCLK_16MHz
} brclk_freq_t;

// Define the brclk_src options
typedef enum
{
  BRSRC_ACLK, BRSRC_SMCLK
} brclk_src_t;

// Define the supported baud rates
typedef enum
{
  B9600, B115200
} br_t;

#define UCA0RXD p1_1
#define UCA0TXD p1_2

void uartInitDefaults(void);
void uartInit(brclk_src_t brclk_src, brclk_freq_t brclk_freq, br_t br);
char uartGetc(void);
void uartPutc(char c);
void uartPuts(char *buf);
void uartPutsf(const char *fmt, ...);
void uartNewLine(void);

