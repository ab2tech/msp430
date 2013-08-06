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

bool spi::is_init[NUM_SPI_USCIs] =
#ifdef MSP430F5510_EXT
  //  A0     B0     A1     B1
  {false, false, false, false};
#else
//    A1     B1
  {false, false};
#endif

const msp_pin_t spi::spi_pins[NUM_SPI_USCIs][NUM_SPI_PINS] = {
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

// Configure SPI for LSB-first transfers
void inline spi::cfgLSB(void)
{
  off(UC_CTL1(spi_base_addr), UCMSB);
}

// Configure SPI for MSB-first transfers
void inline spi::cfgMSB(void)
{
  on (UC_CTL1(spi_base_addr), UCMSB);
}

// Disable the SOMI pin if it's needed for something else
void inline spi::disableSOMI(void)
{
  pinSelOff(spi_pins[spi_usci][SPI_USCI_SOMI]);
}

// Disable the SIMO pin if it's needed for something else
void inline spi::disableSIMO(void)
{
  pinSelOff(spi_pins[spi_usci][SPI_USCI_SIMO]);
}

// Configure SPI for falling edge
void inline spi::fallingEdge(void)
{
  off(UC_CTL0(spi_base_addr), UCCKPH);
}

// Read a byte from the SPI slave
uint8_t inline spi::get(void)
{
  return (write(dummy_char));
}

void spi::getFrame(uint8_t *buf, uint16_t size)
{
  uint16_t i = 0;
  for (i=0; i<size; i++)
  {
    // Write the dummy char to the TXBUF
    set(UC_TXBUF(spi_base_addr), dummy_char);
    // Wait for the transaction to complete
    while (read(UC_IFG(spi_base_addr), UCRXIFG) == 0);
    // Store the received value in the buffer
    set(buf[i], UC_RXBUF(spi_base_addr));
  }
}

// Get the current SPI prescaler
uint16_t spi::getPrescaler(void)
{
  return (UC_BRW(spi_base_addr));
}

// Initialize SPI
void spi::init(void)
{
  // Enable spi using the SEL register for each pin
  pinSelOn(spi_pins[spi_usci][SPI_USCI_CLK]);
  pinSelOn(spi_pins[spi_usci][SPI_USCI_SIMO]);
  pinSelOn(spi_pins[spi_usci][SPI_USCI_SOMI]);

  // Put state machine in reset
  on(UC_CTL1(spi_base_addr), UCSWRST);

  // 3-pin, 8-bit SPI master
  // Clock polarity low, rising edge, MSB
  on(UC_CTL0(spi_base_addr), (UCMST | UCSYNC | UCCKPH | UCMSB));

  // SMCLK
  on(UC_CTL1(spi_base_addr), UCSSEL_2);

  // Set the prescaler to min (2) initially
  setMinPrescaler();

  // Initialize USCI state machine
  off(UC_CTL1(spi_base_addr), UCSWRST);
}

// Pulse the SPI CLK pin
void spi::pulseClk(uint8_t times)
{
  uint8_t i;

  // Need to first disable the SPI functionality of the pin
  pinSelOff(spi_pins[spi_usci][SPI_USCI_CLK]);
  // Now make sure it's an output
  pinOutput(spi_pins[spi_usci][SPI_USCI_CLK]);
  for (i=0; i<times; i++)
  {
    // Now pulse the pin
    pinPulse(spi_pins[spi_usci][SPI_USCI_CLK]);
  }
  // Now turn the SPI functionality back on
  pinSelOn(spi_pins[spi_usci][SPI_USCI_CLK]);
}

// Configure SPI for rising edge
void spi::risingEdge(void)
{
  on (UC_CTL0(spi_base_addr), UCCKPH);
}

// Set the dummy character to something other than default
void inline spi::setDummyChar(uint8_t byte)
{
  set(dummy_char, byte);
}

// Configure the minimum prescaler value (maximum frequency)
void inline spi::setMinPrescaler(void)
{
  set(UC_BRW(spi_base_addr), 0x02);
  // OR
  // setPrescaler(0x02);
}

// Set the SPI clock prescaler value
void spi::setPrescaler(uint16_t prescaler)
{
  set(UC_BRW(spi_base_addr), prescaler);
}

// Write a byte to SPI -- read return byte as well
uint8_t spi::write(uint8_t byte)
{
  // Write a byte to TXBUF
  set(UC_TXBUF(spi_base_addr), byte);
  // Wait for the transaction to complete
  while (read(UC_IFG(spi_base_addr), UCRXIFG) == 0);

  // Return any data that might have been returned by the slave
  return (UC_RXBUF(spi_base_addr));
}

// Write a series of bytes to SPI
void spi::writeFrame(uint8_t *buf, uint16_t size)
{
  uint16_t i = 0;
  volatile uint8_t tmpVar;

  // Send the buffer one byte at a time
  for (i=0; i<size; i++)
  {
    // Write the TX buffer with the i-th byte
    set(UC_TXBUF(spi_base_addr), buf[i]);

    // Wait for the TX buffer to be ready
    while (read(UC_IFG(spi_base_addr), UCTXIFG) == 0);
  }

  // Wait for the transaction to complete
  while (read(UC_STAT(spi_base_addr), UCBUSY) == 0);
  // Dummy read to clear the RX IFG flag
  set(tmpVar, UC_RXBUF(spi_base_addr));
}

