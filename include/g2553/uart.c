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

#include "uart.h"

// Initialize the UART to our default settings
void uartInitDefaults()
{
  uartInit(BRSRC_SMCLK, BRCLK_16MHz, B9600);
}

// Initialize the UART to the settings specified
void uartInit(brclk_src_t brclk_src, brclk_freq_t brclk_freq, br_t br)
{
  // Enable the UART pins
  pinSelOn (UCA0RXD);
  pinSelOn (UCA0TXD);
  pinSel2On(UCA0RXD);
  pinSel2On(UCA0TXD);

  // Enable the USCI reset
  on(UCA0CTL1, UCSWRST);
  switch (brclk_src)
  {
    case BRSRC_ACLK:
      // Set the clock source
      UCA0CTL1 = UCSSEL0;
      // Go ahead and assume 32768 Hz (no other ACLK frequency is handled by
      // this library). Also assume B9600 since that's the highest supported
      // by BRCLK_32kHz.
      // BRCLK_32kHz 9600 Baud - UCBRx = 3
      UCA0BR0 = 0x3;
      // BRCLK_32kHz 9600 Baud - UCBRSx = 3; UCBRFx = 0
      UCA0MCTL = UCBRS_3;
      break;
    case BRSRC_SMCLK:
      UCA0CTL1 = UCSSEL1;

      // Now set the prescaler based on the selected baud rate
      switch (br)
      {
        case B9600:
          switch (brclk_freq)
          {
            case BRCLK_16MHz:
              // BRCLK_16MHz 9600 Baud - UCBRx = 1666
              UCA0BR0 = 0x82;
              UCA0BR1 = 0x6;
              // BRCLK_16MHz 9600 Baud - UCBRSx = 6; UCBRFx = 0
              UCA0MCTL = UCBRS_6;
              break;
            case BRCLK_12MHz:
              // BRCLK_12MHz 9600 Baud - UCBRx = 1250
              UCA0BR0 = 0xE2;
              UCA0BR1 = 0x4;
              // BRCLK_12MHz 9600 Baud - UCBRSx = 0; UCBRFx = 0
              UCA0MCTL = 0;
              break;
            case BRCLK_8MHz:
              // BRCLK_8MHz 9600 Baud - UCBRx = 833
              UCA0BR0 = 0x41;
              UCA0BR1 = 0x3;
              // BRCLK_8MHz 9600 Baud - UCBRSx = 2; UCBRFx = 0
              UCA0MCTL = UCBRS_2;
              break;
            case BRCLK_4MHz:
              // BRCLK_4MHz 9600 Baud - UCBRx = 416
              UCA0BR0 = 0xA0;
              UCA0BR1 = 0x1;
              // BRCLK_4MHz 9600 Baud - UCBRSx = 6; UCBRFx = 0
              UCA0MCTL = UCBRS_6;
              break;
            case BRCLK_1MHz:
              // BRCLK_1MHz 9600 Baud - UCBRx = 104
              UCA0BR0 = 0x68;
              UCA0BR1 = 0x0;
              // BRCLK_1MHz 9600 Baud - UCBRSx = 1; UCBRFx = 0
              UCA0MCTL = UCBRS_1;
              break;
          }
          break;
        case B115200:
          switch (brclk_freq)
          {
            case BRCLK_16MHz:
              // BRCLK_16MHz 115200 Baud - UCBRx = 138
              UCA0BR0 = 0x8A;
              UCA0BR1 = 0x0;
              // BRCLK_16MHz 115200 Baud - UCBRSx = 7; UCBRFx = 0
              UCA0MCTL = UCBRS_7;
              break;
            case BRCLK_12MHz:
              // BRCLK_12MHz 115200 Baud - UCBRx = 104
              UCA0BR0 = 0x68;
              UCA0BR1 = 0x0;
              // BRCLK_12MHz 115200 Baud - Calculator
              UCA0MCTL = 0x8;
              break;
            case BRCLK_8MHz:
              // BRCLK_8MHz 115200 Baud - UCBRx = 69
              UCA0BR0 = 0x45;
              UCA0BR1 = 0x0;
              // BRCLK_8MHz 115200 Baud - Calculator
              UCA0MCTL = 0xAA;
              break;
            case BRCLK_4MHz:
              // BRCLK_4MHz 115200 Baud - UCBRx = 34
              UCA0BR0 = 0x22;
              UCA0BR1 = 0x0;
              // BRCLK_4MHz 115200 Baud - Calculator
              UCA0MCTL = 0xDD;
              break;
            case BRCLK_1MHz:
              // BRCLK_1MHz 115200 Baud - UCBRx = 8
              UCA0BR0 = 0x8;
              UCA0BR1 = 0x0;
              // BRCLK_1MHz 115200 Baud - Calculator
              UCA0MCTL = 0x6D;
              break;
          }
          break;
      }
      break;
  }

  // Disable the USCI reset
  off(UCA0CTL1, UCSWRST);
  // Enable the USCI A0 RX Interrupt (ISR must be user implemented)
  on(IE2, UCA0RXIE);
}

// Block until a character is read on the UART
char uartGetc()
{
  // Blocking function until a character arrives.
  // Good for waiting on something like a return.
  while (!(IFG2 & UCA0RXIFG));

  // Once there is a character to return, return it
  return UCA0RXBUF;
}

// Put a character to UART
void uartPutc(char c)
{
  // Wait until the buffer is ready
  while (!(IFG2&UCA0TXIFG));

  // Write the character to the TX buffer
  UCA0TXBUF = c;
}

// Put a null-terminated string to UART
void uartPuts(char *buf)
{
  while(*buf)
    uartPutc(*buf++);
}

// Put a formatted string to UART 
void uartPutsf(const char *fmt, ...)
{
  char buf[UART_PUTSF_BUF_SIZE];
  va_list valist;

  va_start(valist, fmt);
  vsnprintf(buf, sizeof(buf), fmt, valist);
  va_end(valist);

  puts(buf);
}

// Put a newline to UART
void uartNewLine()
{
  uartPuts("\r\n");
}

