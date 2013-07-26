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
  for(scan_quantity; scan_quantity > 0; scan_quantity--)
  {
    for(i=17; i>0; i--)
    {
      write((uint16_t) next_ch);
      next_ch = (tlc5925_ch_t) ((start_ch>>1)|(start_ch<<15));
      clk->delayMS(anim_delay);
    }
  }
}

// Scans through all TLC5925 channels from low to high beginning with the
// specified channel for the specified number of scans
void tlc5925::channelScanUp(uint16_t scan_quantity)
{
  uint8_t i;
  uint16_t next_ch = start_ch;
  for(scan_quantity; scan_quantity > 0; scan_quantity--)
  {
    for(i=17; i>0; i--)
    {
      write((uint16_t) next_ch);
      next_ch = (tlc5925_ch_t) ((start_ch<<1)|(start_ch>>15));
      clk->delayMS(anim_delay);
    }
  }
}

// Pulses the provided channel data for the specified pulse quantity
void tlc5925::flash(uint16_t pulse_quantity, uint16_t channel_data)
{
  outputDisable();
  write(channel_data);
  for(pulse_quantity;pulse_quantity>0;pulse_quantity--)
  {
    outputEnable();
    clk->delayMS(anim_delay);
    outputDisable();
    clk->delayMS(anim_delay);
  }
}

// Toggles TLC5925 latch bit high
void tlc5925::latch(void)
{
  pinPulse(le);
}

// Sets TLC5925 output enable bit high (active low)
void tlc5925::outputDisable(void)
{
  pinOn(oe);
}

// Sets TLC5925 output enable bit low (active low)
void tlc5925::outputEnable(void)
{
  pinOff(oe);
}

// Rotates the specified TLC5925 channel data up (left shift with carry) the
// channels
void tlc5925::shiftDown(uint16_t shift_quantity, uint16_t channel_data)
{
  for(shift_quantity; shift_quantity>0; shift_quantity--)
  {
    write(channel_data);
    channel_data = rlc(channel_data);
    clk->delayMS(anim_delay);
  }
}

// Rotates the specified TLC5925 channel data up (left shift with carry) the
// channels
void tlc5925::shiftUp(uint16_t shift_quantity , uint16_t channel_data)
{
  for(shift_quantity; shift_quantity>0; shift_quantity--)
  {
    write(channel_data);
    channel_data = rrc(channel_data);
    clk->delayMS(anim_delay);
  }
}

// Writes channel data to TLC5925
void tlc5925::write(uint16_t channel_data)
{
  tlc_spi.risingEdge();
  outputDisable();
  tlc_spi.write((channel_data >> 8) & 0xFF);
  tlc_spi.write(channel_data & 0xFF);
  latch();
  outputEnable();
}

