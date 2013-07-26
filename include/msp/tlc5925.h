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
#define BOLD_CURSOR                     (CH03|CH04)
#define BOTTOM_LEFT_QUADRANT            (CH03|CH04|CH05|CH06|CH07)
#define BOTTOM_RIGHT_QUADRANT           (CH00|CH01|CH02|CH03|CH15)
#define CH0_15                          (CH00|CH01|CH02|CH03|CH04|CH05|CH06| \
                                         CH07|CH08|CH09|CH10|CH11|CH12|CH13| \
                                         CH14|CH15)
#define CH5555                          (CH00|CH02|CH04|CH06|CH08|CH10|CH12| \
                                         CH14)
#define CHAAAA                          (CH01|CH03|CH05|CH07|CH09|CH11|CH13| \
                                         CH15)
#define CRASH_DUMMY_LEFT                (CH00|CH01|CH02|CH03|CH07|CH08|CH09| \
                                         CH10|CH11|CH15)
#define CRASH_DUMMY_RIGHT               (CH03|CH04|CH05|CH06|CH07|CH11|CH12| \
                                         CH13|CH14|CH15)
#define RIGHT_HALF                      (CH03|CH04|CH05|CH06|CH07|CH08|CH09| \
                                         CH10|CH11)
#define LEFT_HALF                       (CH11|CH12|CH13|CH14|CH15|CH00|CH01| \
                                         CH02|CH03)
// Channel definitions
typedef enum _tlc5925_ch_t
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
  tlc5925(clock *clk, msp_pin_t oe, msp_pin_t le, uint16_t anim_delay,
          tlc5925_ch_t start_ch = CH03, spi_usci_t spi_usci = SPI_B1)
    : clk(clk), oe(oe), le(le), anim_delay(anim_delay),
      start_ch(start_ch), tlc_spi(spi_usci) {
      // SPI was initialized using the initialization list...
      pinOutput(oe);
      pinOutput(le);
      pinOff(le);
      outputDisable();
  };
  void channelScanDown(uint16_t scan_quantity);
  void channelScanUp(uint16_t scan_quantity);
  void flash(uint16_t pulse_quantity, uint16_t channel_data);
  void latch(void);
  void outputDisable(void);
  void outputEnable(void);
  void shiftDown(uint16_t shift_quantity, uint16_t channel_data);
  void shiftUp(uint16_t shift_quantity, uint16_t channel_data);
  void write(uint16_t channel_data);
private:
  // Animation delay (loop/pulse)
  uint16_t     anim_delay;
  clock       *clk;
  msp_pin_t    oe;
  msp_pin_t    le;
  spi          tlc_spi;

  // Allows modification of effective channel 0 index
  tlc5925_ch_t start_ch;
};
