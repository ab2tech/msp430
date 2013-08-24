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

// MSP430 TLC5925 library

#include "tlc5925.h"

// Scans through all TLC5925 channels from high to low beginning with the
// specified channel for the specified number of scans
void tlc5925::channelScanDown(uint16_t scan_quantity)
{
  uint8_t i;
  uint16_t next_ch = start_ch;

  for ( ; scan_quantity > 0; scan_quantity--)
  {
    for (i=(TLC5925_NUM_CHANNELS + 1); i>0; i--)
    {
      write(next_ch);
      next_ch = rlc(next_ch);
      clock::delayMS(anim_delay);
    }
  }
}

// Scans through all TLC5925 channels from low to high beginning with the
// specified channel for the specified number of scans
void tlc5925::channelScanUp(uint16_t scan_quantity)
{
  uint8_t i;
  uint16_t next_ch = start_ch;

  for ( ; scan_quantity > 0; scan_quantity--)
  {
    for (i=(TLC5925_NUM_CHANNELS + 1); i>0; i--)
    {
      write(next_ch);
      next_ch = rrc(next_ch);
      clock::delayMS(anim_delay);
    }
  }
}

// Clear the TLC5925 by writing zeros
void tlc5925::clear(void)
{
  spi::risingEdge();
  spi::write(0);
  spi::write(0);
  shift_r::latch();
}

// Pulses the provided channel data for the specified pulse quantity
void tlc5925::flash(uint16_t pulse_quantity, uint16_t channel_data)
{
  if (oe != MSP_PIN_SIZE)
    outputDisable();
  else
    clear();
  write(channel_data);
  for ( ; pulse_quantity>0; pulse_quantity--)
  {
    if (oe != MSP_PIN_SIZE)
      outputEnable();
    else
      restore();
    clock::delayMS(anim_delay);
    if (oe != MSP_PIN_SIZE)
      outputDisable();
    else
      clear();
    clock::delayMS(anim_delay);
  }
}

// Rotates the specified TLC5925 channel data up (left shift with carry) the
// channels
void tlc5925::shiftDown(uint16_t shift_quantity, uint16_t channel_data)
{
  for ( ; shift_quantity>0; shift_quantity--)
  {
    write(channel_data);
    channel_data = rlc(channel_data);
    clock::delayMS(anim_delay);
  }
}

// Rotates the specified TLC5925 channel data up (right shift with carry) the
// channels
void tlc5925::shiftUp(uint16_t shift_quantity , uint16_t channel_data)
{
  for ( ; shift_quantity>0; shift_quantity--)
  {
    write(channel_data);
    channel_data = rrc(channel_data);
    clock::delayMS(anim_delay);
  }
}

// Writes channel data to TLC5925
void tlc5925::write(uint16_t channel_data)
{
  pres_channel_data = channel_data;

  spi::risingEdge();
  outputDisable();
  spi::write((pres_channel_data >> 8) & 0xFF);
  spi::write(pres_channel_data & 0xFF);
  shift_r::latch();
  outputEnable();
}

