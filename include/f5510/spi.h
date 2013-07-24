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

// Limitations: Only master out SPI is supported. Read should be trivial, with
// the only qualification being the configuration of an RX ISR

#pragma once
#define AB2_SPI

#include "msp/ab2.h"
#include <msp430f5510.h>
#include "pin_fw.h"

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
  spi(spi_usci_t spi);
  uint16_t getPrescaler(void);
  void pulseClk(void);
  void fallingEdge(void);
  void risingEdge(void);
  void setMaxPrescaler(void);
  void setPrescaler(uint16_t prescaler);
  void write(uint8_t byte);

private:
  void init(spi_usci_t spi);
  spi_usci_t spi_usci;
  static bool is_init[NUM_SPI_USCIs];
  static const msp_pin_t spi_pins[NUM_SPI_USCIs][NUM_SPI_PINS];
};
