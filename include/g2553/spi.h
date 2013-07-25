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

// MSP430G2553 SPI Library
// Simple SPI library for providing maximum re-use and ease of integration

// Limitations: Only master out SPI is supported. Read should be trivial, with
// the only qualification being the configuration of an RX ISR

#pragma once
#define AB2_SPI

#include "msp/ab2.h"
#include <msp430g2553.h>
#include "pin_fw.h"

// Define a return type for SPI functions with no meaningful return value
// (return status)
typedef enum
{
  SPI_NO_ERR,   // No error
  SPI_REINIT,   // SPI USCI has already been initialized
  SPI_USCI_DNE, // USCI does not exist
  SPI_ERR       // Error
} spi_ret_t;

// Define the various SPI USCIs available on the 2553
typedef enum
{
  SPI_A0,
  SPI_B0,
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

spi_ret_t spiInit(spi_usci_t usci);
uint16_t  spiGetPrescaler(spi_usci_t usci);
spi_ret_t spiPulseClk(spi_usci_t usci);
spi_ret_t spiFallingEdge(spi_usci_t usci);
spi_ret_t spiRisingEdge(spi_usci_t usci);
spi_ret_t spiSetMaxPrescaler(spi_usci_t usci);
spi_ret_t spiSetPrescaler(spi_usci_t usci, uint16_t prescaler);
spi_ret_t spiWrite(spi_usci_t usci, uint8_t byte);

