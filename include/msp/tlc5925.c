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

// MSP430 TLC5925 Library
// Simple TLC5925 library for providing maximum re-use and ease of integration

#include "tlc5925.h"

// Scans through all TLC5925 channels from high to low beginning with the
// specified channel for the specified number of scans
tlc5925_ret_t tlc5925ChannelScanDown(tlc5925_t *tlc, uint16_t starting_channel,
                                     uint16_t scan_quantity)
{
  for(scan_quantity;scan_quantity>0;scan_quantity--)
  {
    uint8_t i = 17;
    for(i;i>0;i--)
    {
      tlc5925Write(tlc, starting_channel);
      starting_channel = (starting_channel>>1)|(starting_channel<<15);
      _delay_ms(ANIMATION_DELAY);
    }
  }
  return TLC5925_NO_ERR;
}

// Scans through all TLC5925 channels from low to high beginning with the
// specified channel for the specified number of scans
tlc5925_ret_t tlc5925ChannelScanUp(tlc5925_t *tlc, uint16_t starting_channel,
                                   uint16_t scan_quantity)
{
  for(scan_quantity;scan_quantity>0;scan_quantity--)
  {
    uint8_t i = 17;
    for(i;i>0;i--)
    {
      tlc5925Write(tlc, starting_channel);
      starting_channel = (starting_channel<<1)|(starting_channel>>15);
      _delay_ms(ANIMATION_DELAY);
    }
  }
  return TLC5925_NO_ERR;
}

// Pulses the provided channel data for the specified pulse quantity
tlc5925_ret_t tlc5925Pulse(tlc5925_t *tlc, uint16_t pulse_quantity, 
                           uint16_t channel_data)
{
  tlc5925OutputDisable(tlc);
  tlc5925Write(tlc, channel_data);
  for(pulse_quantity;pulse_quantity>0;pulse_quantity--)
  {
    tlc5925OutputEnable(tlc);
    _delay_ms(ANIMATION_DELAY);
    tlc5925OutputDisable(tlc);
    _delay_ms(ANIMATION_DELAY);
  }
  return TLC5925_NO_ERR;
}

// TLC5925 initialization of output and latch pins
tlc5925_ret_t tlc5925Init(tlc5925_t *tlc)
{
  // Don't re-initialize the TLC
  if (tlc->is_init == TRUE)
    return TLC5925_REINIT;

  // Initialize SPI for the TLC
  spi_ret_t spi_init_status = spiInit(tlc->usci);
  switch (spi_init_status)
  {
    case SPI_NO_ERR:
    case SPI_REINIT:
      break;
    case SPI_ERR:
      return TLC5925_ERR;
    case SPI_USCI_DNE:
      return TLC5925_PARAM_ERR;
    default:
      return TLC5925_ERR;
  }

  // Configure the TLC LE and OE pins
  pinOutput(tlc->le);
  pinOutput(tlc->oe);
  pinOff(tlc->le);
  tlc5925OutputDisable(tlc);

  tlc->is_init = TRUE;

  return TLC5925_NO_ERR;
}

// Toggles TLC5925 latch bit high
tlc5925_ret_t tlc5925Latch(tlc5925_t *tlc)
{
  pinOn(tlc->le);
  pinOff(tlc->le);
  return TLC5925_NO_ERR;
}

//  Sets TLC5925 output enable bit high (active low)
tlc5925_ret_t tlc5925OutputDisable(tlc5925_t *tlc)
{
  pinOn(tlc->oe);
  return TLC5925_NO_ERR;
}

//  Sets TLC5925 output enable bit low (active low)
tlc5925_ret_t tlc5925OutputEnable(tlc5925_t *tlc)
{
  pinOff(tlc->oe);
  return TLC5925_NO_ERR;
}
//  Rotates the specified TLC5925 channel data up (left shift with carry) the
//  channels
tlc5925_ret_t tlc5925ShiftDown(tlc5925_t *tlc, uint16_t shift_quantity,
                               uint16_t channel_data)
{
  for(shift_quantity;shift_quantity>0;shift_quantity--)
  {
    tlc5925Write(tlc,channel_data);
    channel_data = (channel_data>>1)|(channel_data<<15);
    _delay_ms(ANIMATION_DELAY);
  }
  return TLC5925_NO_ERR;
}

//  Rotates the specified TLC5925 channel data up (left shift with carry) the
//  channels
tlc5925_ret_t tlc5925ShiftUp(tlc5925_t *tlc, uint16_t shift_quantity, 
                             uint16_t channel_data)
{
  for(shift_quantity;shift_quantity>0;shift_quantity--)
  {
    tlc5925Write(tlc,channel_data);
    channel_data = (channel_data<<1)|(channel_data>>15);
    _delay_ms(ANIMATION_DELAY);
  }
  return TLC5925_NO_ERR;
}

// Writes channel data to TLC5925
tlc5925_ret_t tlc5925Write(tlc5925_t *tlc, uint16_t channel_data)
{
  spiRisingEdge(tlc->usci);
  tlc5925OutputDisable(tlc);
  spiWrite(tlc->usci,(channel_data >> 8) & 0xFF);
  spiWrite(tlc->usci,channel_data & 0xFF);
  tlc5925Latch(tlc);
  tlc5925OutputEnable(tlc);
  return TLC5925_NO_ERR;
}
