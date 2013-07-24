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

// Limitations: Only master out SPI is supported. Read should be trivial, with
// the only qualification being the configuration of an RX ISR

#include "spi.h"

bool spi::is_init[NUM_SPI_USCIs] = {false, false};

msp_pin_t spi::spi_pins[NUM_SPI_USCIs][NUM_SPI_PINS] = {
#ifdef MSP430F5510_EXT
  {
    // USCI_A0
    // CLK , SOMI, SIMO
       p2_7, p3_4, p3_3
  },
  {
    // USCI_B0
    // CLK , SOMI, SIMO
       p3_2, p3_1, p3_0
  },
#endif
  {
    // USCI_A1
    // CLK , SOMI, SIMO
       p4_0, p4_5, p4_4
  },
  {
    // USCI_B1
    // CLK , SOMI, SIMO
       p4_3, p4_2, p4_1
  }
};

void spi::init(spi_usci_t spi)
{
  spi_usci = spi;

  // Only initialize this USCI if it hasn't already been initialized (useful if
  // multiple libraries initialize the same SPI USCI)
  if (!is_init[spi_usci])
    is_init[spi_usci] = true;
  else
    return;

  // Which USCI are we initializing?
  switch (spi_usci)
  {
#ifdef MSP430F5510_EXT
    case SPI_A0:
      // Enable spi using the A0 Port SEL register
      pinSelOn(spi_pins[SPI_A0][SPI_USCI_CLK]);
      pinSelOn(spi_pins[SPI_A0][SPI_USCI_SIMO]);

      // Put state machine in reset
      on(UCA0CTL1, UCSWRST);

      // 3-pin, 8-bit SPI master
      // Clock polarity high, MSB
      on(UCA0CTL0, (UCMST | UCSYNC | UCCKPH | UCMSB));

      // SMCLK
      on(UCA0CTL1, UCSSEL_2);

      // Set the prescaler to max (2) initially
      setMaxPrescaler();

      // Initialize USCI state machine
      off(UCA0CTL1, UCSWRST);

      break;

    case SPI_B0:
      // Enable spi using the B0 Port SEL register
      pinSelOn(spi_pins[SPI_B0][SPI_USCI_CLK]);
      pinSelOn(spi_pins[SPI_B0][SPI_USCI_SIMO]);

      // Put state machine in reset
      on(UCB0CTL1, UCSWRST);

      // 3-pin, 8-bit SPI master
      // Clock polarity high, MSB
      on(UCB0CTL0, (UCMST | UCSYNC | UCCKPH | UCMSB));

      // SMCLK
      on(UCB0CTL1, UCSSEL_2);

      // Set the prescaler to max (2) initially
      setMaxPrescaler();

      // Initialize USCI state machine
      off(UCB0CTL1, UCSWRST);

      break;
#endif

    case SPI_A1:
      // Enable spi using the A1 Port SEL register
      pinSelOn(spi_pins[SPI_A1][SPI_USCI_CLK]);
      pinSelOn(spi_pins[SPI_A1][SPI_USCI_SIMO]);

      // Put state machine in reset
      on(UCA1CTL1, UCSWRST);

      // 3-pin, 8-bit SPI master
      // Clock polarity high, MSB
      on(UCA1CTL0, (UCMST | UCSYNC | UCCKPH | UCMSB));

      // SMCLK
      on(UCA1CTL1, UCSSEL_2);

      // Set the prescaler to max (2) initially
      setMaxPrescaler();

      // Initialize USCI state machine
      off(UCA1CTL1, UCSWRST);

      break;

    case SPI_B1:
      // Enable spi using the B1 Port SEL register
      pinSelOn(spi_pins[SPI_B1][SPI_USCI_CLK]);
      pinSelOn(spi_pins[SPI_B1][SPI_USCI_SIMO]);

      // Put state machine in reset
      on(UCB1CTL1, UCSWRST);

      // 3-pin, 8-bit SPI master
      // Clock polarity high, MSB
      on(UCB1CTL0, (UCMST | UCSYNC | UCCKPH | UCMSB));

      // SMCLK
      on(UCB1CTL1, UCSSEL_2);

      // Set the prescaler to max (2) initially
      setMaxPrescaler();

      // Initialize USCI state machine
      off(UCB1CTL1, UCSWRST);

      break;

    default:
      return; // uh-oh...
  }
}

uint16_t spi::getPrescaler(void)
{
  switch (spi_usci)
  {
#ifdef MSP430F5510_EXT
    case SPI_A0:
      return ((UCA0BR1 << BYTE_SIZE) | UCA0BR0);
    case SPI_B0:
      return ((UCB0BR1 << BYTE_SIZE) | UCB0BR0);
#endif
    case SPI_A1:
      return ((UCA1BR1 << BYTE_SIZE) | UCA1BR0);
    case SPI_B1:
      return ((UCB1BR1 << BYTE_SIZE) | UCB1BR0);
    default:
      return 0;
  }
}

void spi::pulseClk(void)
{
  switch (spi_usci)
  {
#ifdef MSP430F5510_EXT
    case SPI_A0:
      // Need to first disable the SPI functionality of the pin
      pinSelOff(spi_pins[SPI_A0][SPI_USCI_CLK]);
      // Now make sure it's an output
      pinOutput(spi_pins[SPI_A0][SPI_USCI_CLK]);
      // Now pulse the pin
      pinPulse(spi_pins[SPI_A0][SPI_USCI_CLK]);
      // Now turn the SPI functionality back on
      pinSelOn(spi_pins[SPI_A0][SPI_USCI_CLK]);
      break;
    case SPI_B0:
      // Need to first disable the SPI functionality of the pin
      pinSelOff(spi_pins[SPI_B0][SPI_USCI_CLK]);
      // Now make sure it's an output
      pinOutput(spi_pins[SPI_B0][SPI_USCI_CLK]);
      // Now pulse the pin
      pinPulse(spi_pins[SPI_B0][SPI_USCI_CLK]);
      // Now turn the SPI functionality back on
      pinSelOn(spi_pins[SPI_B0][SPI_USCI_CLK]);
      break;
#endif
    case SPI_A1:
      // Need to first disable the SPI functionality of the pin
      pinSelOff(spi_pins[SPI_A1][SPI_USCI_CLK]);
      // Now make sure it's an output
      pinOutput(spi_pins[SPI_A1][SPI_USCI_CLK]);
      // Now pulse the pin
      pinPulse(spi_pins[SPI_A1][SPI_USCI_CLK]);
      // Now turn the SPI functionality back on
      pinSelOn(spi_pins[SPI_A1][SPI_USCI_CLK]);
      break;
    case SPI_B1:
      // Need to first disable the SPI functionality of the pin
      pinSelOff(spi_pins[SPI_B1][SPI_USCI_CLK]);
      // Now make sure it's an output
      pinOutput(spi_pins[SPI_B1][SPI_USCI_CLK]);
      // Now pulse the pin
      pinPulse(spi_pins[SPI_B1][SPI_USCI_CLK]);
      // Now turn the SPI functionality back on
      pinSelOn(spi_pins[SPI_B1][SPI_USCI_CLK]);
      break;
    default:
      return; // this can't be good...
  }
}

void spi::fallingEdge(void)
{
  switch (spi_usci)
  {
#ifdef MSP430F5510_EXT
    case SPI_A0:
      off(UCA0CTL0, UCCKPH);
      break;
    case SPI_B0:
      off(UCB0CTL0, UCCKPH);
      break;
#endif
    case SPI_A1:
      off(UCA1CTL0, UCCKPH);
      break;
    case SPI_B1:
      off(UCB1CTL0, UCCKPH);
      break;
    default:
      return; // bad news
  }
}

void spi::risingEdge(void)
{
  switch (spi_usci)
  {
#ifdef MSP430F5510_EXT
    case SPI_A0:
      on(UCA0CTL0, UCCKPH);
      break;
    case SPI_B0:
      on(UCB0CTL0, UCCKPH);
      break;
#endif
    case SPI_A1:
      on(UCA1CTL0, UCCKPH);
      break;
    case SPI_B1:
      on(UCB1CTL0, UCCKPH);
      break;
    default:
      return; // crap
  }
}

void spi::setMaxPrescaler(void)
{
  switch (spi_usci)
  {
#ifdef MSP430F5510_EXT
    case SPI_A0:
      // Set the prescaler to 2 (max)
      set(UCA0BR0, 0x02);
      set(UCA0BR1, 0);
      break;
    case SPI_B0:
      // Set the prescaler to 2 (max)
      set(UCB0BR0, 0x02);
      set(UCB0BR1, 0);
      break;
#endif
    case SPI_A1:
      // Set the prescaler to 2 (max)
      set(UCA1BR0, 0x02);
      set(UCA1BR1, 0);
      break;
    case SPI_B1:
      // Set the prescaler to 2 (max)
      set(UCB1BR0, 0x02);
      set(UCB1BR1, 0);
      break;
    default:
      return; // for the very last time...
   }
}

void spi::setPrescaler(uint16_t prescaler)
{
  switch (spi_usci)
  {
#ifdef MSP430F5510_EXT
    case SPI_A0:
      // Set the prescaler based on the prescaler input
      set(UCA0BR0,  (prescaler & 0x00FF));
      set(UCA0BR1, ((prescaler >> BYTE_SIZE) & 0x00FF));
      break;
    case SPI_B0:
      // Set the prescaler based on the prescaler input
      set(UCB0BR0,  (prescaler & 0x00FF));
      set(UCB0BR1, ((prescaler >> BYTE_SIZE) & 0x00FF));
      break;
#endif
    case SPI_A1:
      // Set the prescaler based on the prescaler input
      set(UCA1BR0,  (prescaler & 0x00FF));
      set(UCA1BR1, ((prescaler >> BYTE_SIZE) & 0x00FF));
      break;
    case SPI_B1:
      // Set the prescaler based on the prescaler input
      set(UCB1BR0,  (prescaler & 0x00FF));
      set(UCB1BR1, ((prescaler >> BYTE_SIZE) & 0x00FF));
      break;
    default:
      return; // sheesh
  }
}

void spi::write(uint8_t byte)
{
  // Which USCI to write to?
  switch (spi_usci)
  {
#ifdef MSP430F5510_EXT
    case SPI_A0:
      UCA0TXBUF = byte;
      while ((UCA0STAT&UCBUSY));
      break;
    case SPI_B0:
      UCB0TXBUF = byte;
      while ((UCB0STAT&UCBUSY));
      break;
#endif
    case SPI_A1:
      UCA1TXBUF = byte;
      while ((UCA1STAT&UCBUSY));
      break;
    case SPI_B1:
      UCB1TXBUF = byte;
      while ((UCB1STAT&UCBUSY));
      break;
    default:
      return; // wrong
  }
}

