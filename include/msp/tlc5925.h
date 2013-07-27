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

// Channel preset definitions (channel 0 referenced)
#define TLC5925_BOLD_CURSOR            (CH00|CH01)
#define TLC5925_BOTTOM_LEFT_QUADRANT   (CH00|CH01|CH02|CH03|CH04)
#define TLC5925_BOTTOM_RIGHT_QUADRANT  (CH00|CH12|CH13|CH14|CH15)
#define TLC5925_CH0_15                 (CH00|CH01|CH02|CH03|CH04|CH05|CH06| \
                                        CH07|CH08|CH09|CH10|CH11|CH12|CH13| \
                                        CH14|CH15)
#define TLC5925_CH5555                 (CH00|CH02|CH04|CH06|CH08|CH10|CH12| \
                                        CH14)
#define TLC5925_CHAAAA                 (CH01|CH03|CH05|CH07|CH09|CH11|CH13| \
                                        CH15)
#define TLC5925_CRASH_DUMMY_LEFT       (CH00|CH04|CH05|CH06|CH07|CH08|CH12| \
                                        CH13|CH14|CH15)
#define TLC5925_CRASH_DUMMY_RIGHT      (CH00|CH01|CH02|CH03|CH04|CH08|CH09| \
                                        CH10|CH11|CH12)
#define TLC5925_RIGHT_HALF             (CH00|CH08|CH09|CH10|CH11|CH12|CH13| \
                                        CH14|CH15)
#define TLC5925_LEFT_HALF              (CH00|CH01|CH02|CH03|CH04|CH05|CH06| \
                                        CH07|CH08)

#define TLC5925_NUM_CHANNELS           16

// Channel definitions
typedef enum
{
  CH00 = BIT0,
  CH01 = BIT1,
  CH02 = BIT2,
  CH03 = BIT3,
  CH04 = BIT4,
  CH05 = BIT5,
  CH06 = BIT6,
  CH07 = BIT7,
  CH08 = BIT8,
  CH09 = BIT9,
  CH10 = BITA,
  CH11 = BITB,
  CH12 = BITC,
  CH13 = BITD,
  CH14 = BITE,
  CH15 = BITF
} tlc5925_ch_t;

// TLC5925 class declaration
class tlc5925
{
public:
  tlc5925(clock *clk, msp_pin_t le, msp_pin_t oe = MSP_PIN_SIZE,
          uint16_t anim_delay = 100, tlc5925_ch_t start_ch = CH00,
          spi_usci_t spi_usci = SPI_B1)
    : clk(clk), le(le), oe(oe), anim_delay(anim_delay),
      start_ch(start_ch), tlc_spi(spi_usci) {
      // SPI was initialized using the initialization list...
      if (oe != MSP_PIN_SIZE)
      {
        pinOutput(oe);
      }
      pinOutput(le);
      pinOff(le);
      outputDisable();
  };
  void channelScanDown(uint16_t scan_quantity);
  void channelScanUp(uint16_t scan_quantity);
  tlc5925_ch_t inline getStartCh(void);
  void flash(uint16_t pulse_quantity, uint16_t channel_data);
  void inline latch(void);
  void inline outputDisable(void);
  void inline outputEnable(void);
  void inline setStartCh(tlc5925_ch_t ch);
  void shiftDown(uint16_t shift_quantity, uint16_t channel_data);
  void shiftUp(uint16_t shift_quantity, uint16_t channel_data);
  void write(uint16_t channel_data);
private:
  // Animation delay (loop/pulse)
  uint16_t            anim_delay;
  clock              *clk;
  msp_pin_t           le;
  msp_pin_t           oe;
  spi                 tlc_spi;

  // Allows modification of effective channel 0 index
  tlc5925_ch_t        start_ch;
  uint16_t            pres_channel_data;
};
