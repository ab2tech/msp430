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
// Simple SPI library for providing maximum re-use and ease of integration
// MSP430F5510 64-pin (RGC) or 80-pin BGA (ZQE) supported with definition of
// MSP430F5510_EXT flag

#pragma once
#define AB2_SPI

#include "msp/ab2.h"
#include <msp430f5510.h>
#include "pin_fw.h"
#include "usci.h"

#define DEFAULT_DUMMY_SPI_CHAR 0xFF

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
  spi(spi_usci_t usci) : spi_usci(usci), dummy_char(DEFAULT_DUMMY_SPI_CHAR) {
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
      //default: ...asplode
    }
    // Only initialize this USCI if it hasn't already been initialized
    // (useful if multiple libraries initialize the same SPI USCI)
    if (!is_init[spi_usci])
    {
      is_init[spi_usci] = true;
      init();
    }
  }

  void inline      cfgLSB(void);
  void inline      cfgMSB(void);
  void inline      disableSIMO(void);
  void inline      disableSOMI(void);
  void inline      enterReset(void);
  void inline      exitReset(void);
  void inline      fallingEdge(void);
  uint8_t          get(void);
  void             getFrame(uint8_t *buf, uint16_t size);
  uint16_t inline  getPrescaler(void);
  void             pulseClk(uint8_t times);
  void             risingEdge(void);
  void inline      setDummyChar(uint8_t byte);
  void inline      setMinPrescaler(void);
  void             setPrescaler(uint16_t prescaler);
  uint8_t          write(uint8_t byte);
  void             writeFrame(uint8_t *buf, uint16_t size);

private:
  void             init(void);

  uint8_t          dummy_char;

  static bool      is_init[NUM_SPI_USCIs];
  static const     msp_pin_t spi_pins[NUM_SPI_USCIs][NUM_SPI_PINS];

  uint16_t         spi_base_addr;
  spi_usci_t       spi_usci;
};
