// Copyright (C) [2014] [AB2 Technologies] [Austin Beam, Alan Bullick]
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

// MSP430 Seven Segment library
// Seven segment library for custom-designed AB2 seven segment configuration

#pragma once
#define AB2_SEVEN_SEG

#include "ab2.h"
// Steal our MSP include from pin_fw to make this a generic library
#include "../pin_fw.h"
#include "tlc5925.h"

// Segment definitions
typedef enum
{
  SEVEN_SEG_A0  = TLC5925_CH00,
  SEVEN_SEG_B0  = TLC5925_CH01,
  SEVEN_SEG_C0  = TLC5925_CH02,
  SEVEN_SEG_D0  = TLC5925_CH03,
  SEVEN_SEG_E0  = TLC5925_CH04,
  SEVEN_SEG_F0  = TLC5925_CH05,
  SEVEN_SEG_G0  = TLC5925_CH06,
  SEVEN_SEG_DP0 = TLC5925_CH07,
  SEVEN_SEG_A1  = TLC5925_CH08,
  SEVEN_SEG_B1  = TLC5925_CH09,
  SEVEN_SEG_C1  = TLC5925_CH10,
  SEVEN_SEG_D1  = TLC5925_CH11,
  SEVEN_SEG_E1  = TLC5925_CH12,
  SEVEN_SEG_F1  = TLC5925_CH13,
  SEVEN_SEG_G1  = TLC5925_CH14,
  SEVEN_SEG_DP1 = TLC5925_CH15
} seven_seg_t;

#define NUM_SEVEN_SEG_CHARS 16
#define SEVEN_SEG_DP_TOP    SEVEN_SEG_DP0
#define SEVEN_SEG_DP_BOT    SEVEN_SEG_DP1

class seven_seg : public tlc5925
{
public:
  seven_seg(msp_pin_t le, msp_pin_t oe = MSP_PIN_SIZE,
            uint16_t anim_delay = 100, spi_usci_t spi_usci = DEFAULT_SPI_USCI)
    : tlc5925(le, oe, anim_delay, TLC5925_CH00, spi_usci),
      blank0(false), blank1(false), dp_top(false), dp_bot(false) {};

  void blank(bool zero, bool one);
  void dp(bool top, bool bot);
  void num(uint8_t num, uint8_t base = 10, bool lead0 = false);

private:
  bool blank0;
  bool blank1;

  bool dp_top;
  bool dp_bot;

  static const uint8_t seven_seg_character[NUM_SEVEN_SEG_CHARS];
  uint16_t seven_seg_data;
};
