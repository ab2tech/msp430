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

#include "spi.h"

static bool_t is_init[NUM_SPI_USCIs] = {FALSE, FALSE};

static const msp_pin_t spi_pins[NUM_SPI_USCIs][NUM_SPI_PINS] = {
  {
    // USCI_A0
    // CLK , SOMI, SIMO
       p1_4, p1_1, p1_2
  },
  {
    // USCI_B0
    // CLK , SOMI, SIMO
       p1_5, p1_6, p1_7
  }
};

spi_ret_t spiInit(spi_usci_t usci)
{
  // Only initialize this USCI if it hasn't already been initialized (useful if
  // multiple libraries initialize the same SPI USCI)
  if (!is_init[usci])
    is_init[usci] = TRUE;
  else
    return SPI_REINIT;

  // Which USCI are we initializing?
  switch (usci)
  {
    case SPI_A0:
      // Enable spi using the A0 Port SEL & SEL2 registers
      pinSelOn(spi_pins[SPI_A0][SPI_USCI_CLK]);
      pinSelOn(spi_pins[SPI_A0][SPI_USCI_SIMO]);
      pinSel2On(spi_pins[SPI_A0][SPI_USCI_CLK]);
      pinSel2On(spi_pins[SPI_A0][SPI_USCI_SIMO]);

      // Put state machine in reset
      on(UCA0CTL1, UCSWRST);

      // 3-pin, 8-bit SPI master
      // Clock polarity high, MSB
      on(UCA0CTL0, (UCMST | UCSYNC | UCCKPH | UCMSB));

      // SMCLK
      on(UCA0CTL1, UCSSEL_2);

      // Set the prescaler to max (2) initially
      spiSetMaxPrescaler(usci);

      // Initialize USCI state machine
      off(UCA0CTL1, UCSWRST);

    case SPI_B0:
      // Enable spi using the B0 Port SEL & SEL2 registers
      pinSelOn(spi_pins[SPI_B0][SPI_USCI_CLK]);
      pinSelOn(spi_pins[SPI_B0][SPI_USCI_SIMO]);
      pinSel2On(spi_pins[SPI_B0][SPI_USCI_CLK]);
      pinSel2On(spi_pins[SPI_B0][SPI_USCI_SIMO]);

      // Put state machine in reset
      on(UCB0CTL1, UCSWRST);

      // 3-pin, 8-bit SPI master
      // Clock polarity high, MSB
      on(UCB0CTL0, (UCMST | UCSYNC | UCCKPH | UCMSB));

      // SMCLK
      on(UCB0CTL1, UCSSEL_2);

      // Set the prescaler to max (2) initially
      spiSetMaxPrescaler(usci);

      // Initialize USCI state machine
      off(UCB0CTL1, UCSWRST);

    default:
      return SPI_USCI_DNE;
  }
  return SPI_NO_ERR;
}

uint16_t spiGetPrescaler(spi_usci_t usci)
{
  switch (usci)
  {
    case SPI_A0:
      return ((UCA0BR1 << BYTE_SIZE) | UCA0BR0);
    case SPI_B0:
      return ((UCB0BR1 << BYTE_SIZE) | UCB0BR0);
    default:
      return 0;
  }
}

spi_ret_t spiPulseClk(spi_usci_t usci)
{
  switch (usci)
  {
    case SPI_A0:
      // Need to first disable the SPI functionality of the pin
      pinSelOff(spi_pins[SPI_A0][SPI_USCI_CLK]);
      pinSel2Off(spi_pins[SPI_A0][SPI_USCI_CLK]);
      // Now make sure it's an output
      pinOutput(spi_pins[SPI_A0][SPI_USCI_CLK]);
      // Now pulse the pin
      pinPulse(spi_pins[SPI_A0][SPI_USCI_CLK]);
      // Now turn the SPI functionality back on
      pinSelOn(spi_pins[SPI_A0][SPI_USCI_CLK]);
      pinSel2On(spi_pins[SPI_A0][SPI_USCI_CLK]);
      break;
    case SPI_B0:
      // Need to first disable the SPI functionality of the pin
      pinSelOff(spi_pins[SPI_B0][SPI_USCI_CLK]);
      pinSel2Off(spi_pins[SPI_B0][SPI_USCI_CLK]);
      // Now make sure it's an output
      pinOutput(spi_pins[SPI_B0][SPI_USCI_CLK]);
      // Now pulse the pin
      pinPulse(spi_pins[SPI_B0][SPI_USCI_CLK]);
      // Now turn the SPI functionality back on
      pinSelOn(spi_pins[SPI_B0][SPI_USCI_CLK]);
      pinSel2On(spi_pins[SPI_B0][SPI_USCI_CLK]);
      break;
    default:
      return SPI_USCI_DNE;
  }
  return SPI_NO_ERR;
}

spi_ret_t spiFallingEdge(spi_usci_t usci)
{
  switch (usci)
  {
    case SPI_A0:
      off(UCA0CTL0, UCCKPH);
      break;
    case SPI_B0:
      off(UCB0CTL0, UCCKPH);
      break;
    default:
      return SPI_USCI_DNE;
  }
  return SPI_NO_ERR;
}

spi_ret_t spiRisingEdge(spi_usci_t usci)
{
  switch (usci)
  {
    case SPI_A0:
      on(UCA0CTL0, UCCKPH);
      break;
    case SPI_B0:
      on(UCB0CTL0, UCCKPH);
      break;
    default:
      return SPI_USCI_DNE;
  }
  return SPI_NO_ERR;
}

spi_ret_t spiSetMaxPrescaler(spi_usci_t usci)
{
  switch (usci)
  {
    case SPI_A0:
      // Set the prescaler to 2 (max)
      UCA0BR0 = 0x02;
      UCA0BR1 = 0;
      break;
    case SPI_B0:
      // Set the prescaler to 2 (max)
      UCB0BR0 = 0x02;
      UCB0BR1 = 0;
      break;
    default:
      return SPI_USCI_DNE;
   }
  return SPI_NO_ERR;
}

spi_ret_t spiSetPrescaler(spi_usci_t usci, uint16_t prescaler)
{
  switch (usci)
  {
    case SPI_A0:
      // Set the prescaler based on the prescaler input
      UCA0BR0 = (prescaler & 0x00FF);
      UCA0BR1 = ((prescaler >> BYTE_SIZE) & 0x00FF);
      break;
    case SPI_B0:
      // Set the prescaler based on the prescaler input
      UCB0BR0 = (prescaler & 0x00FF);
      UCB0BR1 = ((prescaler >> BYTE_SIZE) & 0x00FF);
      break;
    default:
      return SPI_USCI_DNE;
  }
  return SPI_NO_ERR;
}

spi_ret_t spiWrite(spi_usci_t usci, uint8_t byte)
{
  // Which USCI to write to?
  switch (usci)
  {
    case SPI_A0:
      UCA0TXBUF = byte;
      while ((UCA0STAT&UCBUSY));
      break;
    case SPI_B0:
      UCB0TXBUF = byte;
      while ((UCB0STAT&UCBUSY));
      break;
    default:
      return SPI_USCI_DNE;
  }
  return SPI_NO_ERR;
}

