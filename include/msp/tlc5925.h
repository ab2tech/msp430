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
// Simple TLC5925 library for providing maximum re-use and ease of integration

#pragma once
#define AB2_TLC5925

#include "ab2.h"
// Steal our MSP include from pin_fw to make this a generic library
#include "../pin_fw.h"
#include "../spi.h"
#include "../clock.h"
#include "shift_r.h"

// Channel definitions
typedef enum
{
  TLC5925_CH00 = BIT0,
  TLC5925_CH01 = BIT1,
  TLC5925_CH02 = BIT2,
  TLC5925_CH03 = BIT3,
  TLC5925_CH04 = BIT4,
  TLC5925_CH05 = BIT5,
  TLC5925_CH06 = BIT6,
  TLC5925_CH07 = BIT7,
  TLC5925_CH08 = BIT8,
  TLC5925_CH09 = BIT9,
  TLC5925_CH10 = BITA,
  TLC5925_CH11 = BITB,
  TLC5925_CH12 = BITC,
  TLC5925_CH13 = BITD,
  TLC5925_CH14 = BITE,
  TLC5925_CH15 = BITF
} tlc5925_ch_t;

// Channel preset definitions (channel 0 referenced)
#define TLC5925_BOLD_CURSOR            (TLC5925_CH00|TLC5925_CH01)
#define TLC5925_BOTTOM_LEFT_QUADRANT   (TLC5925_CH00|TLC5925_CH01|TLC5925_CH02|\
                                        TLC5925_CH03|TLC5925_CH04)
#define TLC5925_BOTTOM_RIGHT_QUADRANT  (TLC5925_CH00|TLC5925_CH12|TLC5925_CH13|\
                                        TLC5925_CH14|TLC5925_CH15)
#define TLC5925_CH0_15                 (TLC5925_CH00|TLC5925_CH01|TLC5925_CH02|\
                                        TLC5925_CH03|TLC5925_CH04|TLC5925_CH05|\
                                        TLC5925_CH06|TLC5925_CH07|TLC5925_CH08|\
                                        TLC5925_CH09|TLC5925_CH10|TLC5925_CH11|\
                                        TLC5925_CH12|TLC5925_CH13|TLC5925_CH14|\
                                        TLC5925_CH15)
#define TLC5925_CH5555                 (TLC5925_CH00|TLC5925_CH02|TLC5925_CH04|\
                                        TLC5925_CH06|TLC5925_CH08|TLC5925_CH10|\
                                        TLC5925_CH12|TLC5925_CH14)
#define TLC5925_CHAAAA                 (TLC5925_CH01|TLC5925_CH03|TLC5925_CH05|\
                                        TLC5925_CH07|TLC5925_CH09|TLC5925_CH11|\
                                        TLC5925_CH13|TLC5925_CH15)
#define TLC5925_CRASH_DUMMY_LEFT       (TLC5925_CH00|TLC5925_CH04|TLC5925_CH05|\
                                        TLC5925_CH06|TLC5925_CH07|TLC5925_CH08|\
                                        TLC5925_CH12|TLC5925_CH13|TLC5925_CH14|\
                                        TLC5925_CH15)
#define TLC5925_CRASH_DUMMY_RIGHT      (TLC5925_CH00|TLC5925_CH01|TLC5925_CH02|\
                                        TLC5925_CH03|TLC5925_CH04|TLC5925_CH08|\
                                        TLC5925_CH09|TLC5925_CH10|TLC5925_CH11|\
                                        TLC5925_CH12)
#define TLC5925_RIGHT_HALF             (TLC5925_CH00|TLC5925_CH08|TLC5925_CH09|\
                                        TLC5925_CH10|TLC5925_CH11|TLC5925_CH12|\
                                        TLC5925_CH13|TLC5925_CH14|TLC5925_CH15)
#define TLC5925_LEFT_HALF              (TLC5925_CH00|TLC5925_CH01|TLC5925_CH02|\
                                        TLC5925_CH03|TLC5925_CH04|TLC5925_CH05|\
                                        TLC5925_CH06|TLC5925_CH07|TLC5925_CH08)

#define TLC5925_NUM_CHANNELS           16

#define TLC5925_ANIM_DELAY             100

// TLC5925 class declaration
class tlc5925 : public shift_r
{
public:
  tlc5925(msp_pin_t le, msp_pin_t oe = MSP_PIN_SIZE,
          uint16_t anim_delay = 100, tlc5925_ch_t start_ch = TLC5925_CH00,
          spi_usci_t spi_usci = DEFAULT_SPI_USCI)
    : anim_delay(anim_delay), start_ch(start_ch), shift_r(spi_usci, le, oe) {};

  using shift_r::clear;
  using shift_r::restore;
  using shift_r::write;

  void channelScanDown(uint16_t scan_quantity);
  void channelScanUp(uint16_t scan_quantity);
  void clear(void);
  void flash(uint16_t pulse_quantity, uint16_t channel_data);
  void shiftDown(uint16_t shift_quantity, uint16_t channel_data);
  void shiftUp(uint16_t shift_quantity, uint16_t channel_data);
  void write(uint16_t channel_data);

  // Logical AND with existing channel data written to the TLC
  void inline andWrite(uint16_t channel_data) {
    write((pres_channel_data & channel_data)); };
  // Retrieve the instance start_ch value
  tlc5925_ch_t inline getStartCh(void) { return start_ch; };
  // Logical OR with existing channel data written to the TLC
  void inline orWrite(uint16_t channel_data) {
    write((pres_channel_data | channel_data)); };
  // Restores pres_channel_data to the TLC
  void inline restore(void) { write(pres_channel_data); };
  // Re-configures the animation delay
  void inline setAnimDelay(uint16_t delay) { set(anim_delay, delay); };
  // Re-configure start channel
  void inline setStartCh(tlc5925_ch_t ch)  { set(start_ch, ch); };
  // Override the shift_r write function with one that calls the tlc5925 write
  // function in case an 8 bit value is attempted to be written to the TLC
  void inline write(uint8_t channel_data) { write((uint16_t) channel_data); };
private:
  // Animation delay (loop/pulse)
  uint16_t            anim_delay;

  // Allows modification of effective channel 0 index
  tlc5925_ch_t        start_ch;
protected:
  uint16_t            pres_channel_data;
};
