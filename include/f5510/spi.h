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

// MSP430F5510 SPI Library
// Simple SPI library for providing maximum re-use and ease of integration.
// Consider adding an ISR for receiving data if that is a significant use
// case for the given implementation.
// MSP430F5510 64-pin (RGC) or 80-pin BGA (ZQE) supported with definition of
// MSP430F5510_EXT flag

#pragma once
#define AB2_SPI

#include "msp/ab2.h"
#include <msp430f5510.h>
#include "pin_fw.h"
#include "usci.h"

#define DEFAULT_DUMMY_SPI_CHAR 0xFF
#define SPI_MIN_PRESCALER      1

// Define the various SPI USCIs available on the 5510
typedef enum
{
#ifdef MSP430F5510_EXT
  SPI_A0,
  SPI_B0,
#endif
  SPI_A1,
  SPI_B1,
  NUM_SPI_USCIs
} spi_usci_t;

// Define the various SPI pins for each USCI
typedef enum
{
  SPI_USCI_CLK,
  SPI_USCI_SOMI,
  SPI_USCI_SIMO,
  NUM_SPI_PINS
} spi_pin_t;

class spi
{
public:
  spi(spi_usci_t usci, bool io = false) : spi_usci(usci), io(io),
    dummy_char(DEFAULT_DUMMY_SPI_CHAR)
  {
    // Configure the base address depending on which USCI this is
    switch (spi_usci)
    {
#ifdef MSP430F5510_EXT
      case SPI_A0:
        spi_base_addr = USCI_A0_BASE;
        break;
      case SPI_B0:
        spi_base_addr = USCI_B0_BASE;
        break;
#endif
      case SPI_A1:
        spi_base_addr = USCI_A1_BASE;
        break;
      case SPI_B1:
        spi_base_addr = USCI_B1_BASE;
        break;
      default:
       _never_executed();
    }
    // Only initialize this USCI if it hasn't already been initialized
    // (useful if multiple libraries initialize the same SPI USCI)
    if (!is_init[spi_usci])
    {
      is_init[spi_usci] = true;
      init();
    }
    // Initialize the SOMI pin if I/O is enabled -- do this every time because
    // an I/O instance might get enabled after an output-only instance.
    if (io)
    {
      pinSelOn(spi_pins[spi_usci][SPI_USCI_SOMI]);
    }
  }

  void           cfgLSB(void);
  void           cfgMSB(void);
  void           disableSOMI(void);
  void           fallingEdge(void);
  uint16_t       getPrescaler(void);
  void           pulseClk(uint8_t times);
  void           readFrame(uint8_t *buf, uint16_t size);
  void           risingEdge(void);
  void           setPrescaler(uint16_t prescaler);
  uint8_t        write(uint8_t byte);
  void           writeFrame(uint8_t *buf, uint16_t size);

  // Put the state machine in reset
  void inline    enterReset(void) { on (UC_CTL1(spi_base_addr), UCSWRST); };
  // Release the state machine from reset
  void inline    exitReset(void)  { off(UC_CTL1(spi_base_addr), UCSWRST); };
  // Read a byte from the SPI slave
  uint8_t inline read(void) { return (write(dummy_char)); };
  // Set the dummy character to something other than default
  void inline    setDummyChar(uint8_t byte) { set(dummy_char, byte); };
  // Configure the minimum prescaler value (maximum frequency). For MSP430F55xx
  // devices, this is equal to the system frequency (prescaler == 1).
  void inline    setMinPrescaler(void) { setPrescaler(SPI_MIN_PRESCALER); };

  static const   msp_pin_t spi_pins[NUM_SPI_USCIs][NUM_SPI_PINS];
private:
  bool           io;

  void           init(void);

  uint8_t        dummy_char;

  static bool    is_init[NUM_SPI_USCIs];

  uint16_t       spi_base_addr;
  spi_usci_t     spi_usci;
};
