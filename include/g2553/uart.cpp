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

// UART putsf buffer size
#define UART_PUTSF_BUF_SIZE 256

// Fixed-point factor for fixed-point UART calculations
#define UART_FPF 1000
#define UCBRS_OFFSET 1
#define UCBRF_OFFSET 4

const msp_pin_t uart::uart_pins[NUM_UART_USCIs][NUM_UART_PINS] = {
  {
    // USCI_A0
    // RXD,  TXD
       p1_1, p1_2
  }
};

// Initialize the UART to the settings specified
void uart::init(brclk_src_t brclk_src,
                uint32_t brclk_freq,
                uint32_t br,
                bool ie,
                bool osample)
{
  // Shift the input frequency 3 places so we can do fixed-point math
  // with some resolution
  uint64_t freq_f = ((uint64_t)brclk_freq * UART_FPF); // fixed-point
  uint32_t n_f = (freq_f/((uint32_t)br));              // fixed-point
  uint16_t n = (n_f/UART_FPF)  ;                       // regular
  uint32_t intn_f = ((uint32_t)n*UART_FPF);            // fixed-point (rounded)

  // UCBRx
  uint16_t ucbr = n;
  // UCBRSx in UCXnMCTL
  uint8_t mctl = ((((n_f-intn_f)*8)/UART_FPF)<<UCBRS_OFFSET);

  if (osample)
  {
    if ((n/1000) < 16)
      return; // YIKES! -- for debugging purposes

    n_f /= 16; // fixed-point
    n = (n_f/UART_FPF); // regular
    intn_f = ((uint32_t)n*UART_FPF); // fixed-point (rounded)

    // UCBRx
    ucbr = n;
    // UCBRFx
    mctl = (((((n_f-intn_f)*16)/UART_FPF)<<UCBRF_OFFSET) | UCOS16);
  }

  // Enable the UART pins
  pinSelOn (uart_pins[uart_usci][UART_RXD]);
  pinSelOn (uart_pins[uart_usci][UART_TXD]);
  pinSel2On(uart_pins[uart_usci][UART_RXD]);
  pinSel2On(uart_pins[uart_usci][UART_TXD]);

  // Enable the USCI reset
  on(UC_CTL1(uart_base_addr), UCSWRST);

  // Configure the selected clock source
  // Clear any existing configuration
  off(UC_CTL1(uart_base_addr), (UCSSEL0 | UCSSEL1));
  // Set the clock source
  on (UC_CTL1(uart_base_addr), brclk_src);

  // Configure the prescaler and modulation
  // Set the lower byte of the prescaler
  set(UC_BR0(uart_base_addr), (ucbr & 0xFF));
  // Set the upper byte of the prescaler
  set(UC_BR1(uart_base_addr), ((ucbr>>BYTE_SIZE) && 0xFF));
  // Set the MCTL to match the calculations above
  set(UCA_MCTL(uart_base_addr), mctl);

  if (ie)
  {
    // Enable the USCI A0 RX Interrupt (ISR must be user implemented)
    on(UC_IE(uart_base_addr), UCA_RXIE(uart_usci));
  }
  // Disable the USCI reset
  off(UC_CTL1(uart_base_addr), UCSWRST);
}

// Block until a character is read on the UART
char uart::getc(void)
{
  // Blocking function until a character arrives.
  // Good for waiting on something like a return.
  while (!read(UC_IFG(uart_base_addr), UCA_RXIFG(uart_usci)));

  // Once there is a character to return, return it
  return UC_RXBUF(uart_base_addr);
}

// Put a character to UART
void uart::putc(char c)
{
  // Wait until the buffer is ready
  while (!read(UC_IFG(uart_base_addr), UCA_RXIFG(uart_usci)));

  // Write the character to the TX buffer
  set(UC_TXBUF(uart_base_addr), c);
}

// Put a null-terminated string to UART
void uart::puts(char *buf)
{
  while(*buf)
    putc(*buf++);
}

// Put a formatted string to UART
void uart::putsf(const char *fmt, ...)
{
  char buf[UART_PUTSF_BUF_SIZE];
  va_list valist;

  va_start(valist, fmt);
  vsnprintf(buf, sizeof(buf), fmt, valist);
  va_end(valist);

  puts(buf);
}

// Put a newline to UART
void uart::newLine()
{
  puts("\r\n");
}

