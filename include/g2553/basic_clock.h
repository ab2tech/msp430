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

// MSP430G2553 Clock Library
// Simplify MSP430G2553 clock initialization and frequency modification

#pragma once
#define AB2_BASIC_CLOCK

#include <msp430g2553.h>
#include "msp/ab2.h"

// Define the default divisions supported by the various clock signals
// If a clock signal doesn't support all of these dividers or supports
// additional dividers, they should be defined independently
typedef enum
{
  CLK_DIV_1 = 0, // 00b
  CLK_DIV_2 = 1, // 01b
  CLK_DIV_4 = 2, // 10b
  CLK_DIV_8 = 3  // 11b
} clk_div_t;

// Define the ACLK sources supported by the clock module
typedef enum
{
  // 32.768kHz external crystal (LFXT1)
  // This is explicit because no other frequencies are supported.
  ACLK_SRC_32kHz = LFXT1S_0, // 0x00 # 00b

  // Internal very low frequency oscillator (~12kHz)
  ACLK_SRC_VLO   = LFXT1S_2, // 0x20 # 10b

  // External digital source, 10kHz-50kHz
  ACLK_SRC_DIG   = LFXT1S_3  // 0x30 # 11b
} aclk_src_t;

// Define the pre-calibrated DCO frequencies supported by the clock module
typedef enum
{
  DCO_F_1MHz  = (uint16_t)(&CALDCO_1MHZ),
  DCO_F_8MHz  = (uint16_t)(&CALDCO_8MHZ),
  DCO_F_12MHz = (uint16_t)(&CALDCO_12MHZ),
  DCO_F_16MHz = (uint16_t)(&CALDCO_16MHZ),
} dco_freq_t;

// Define the MCLK sources supported by the clock module
typedef enum
{
  MCLK_SRC_DCO  = SELM_0, // 0x00 | 0x01 # 00b | 01b
  // Not technically sourced by ACLK, but by LFXT1 or VLO
  MCLK_SRC_ACLK = SELM_3, // 0x80 | 0xC0 # 10b | 11b
} mclk_src_t;

// Define the SMCLK sources supported by the clock module
typedef enum
{
  SMCLK_SRC_DCO  = 0,   // 0x00 # 0b
  SMCLK_SRC_XTAL = SELS // 0x08 # 1b
} smclk_src_t;

// Define the settings for XT1 capacitance as seen by the LFXT1 crystal when
// XTS=0. If LFXT1Sx=11, XCAPx should be 00.
typedef enum
{
  XCAP_1pF    = XCAP_0, // 0x00 # 00b
  XCAP_6pF    = XCAP_1, // 0x04 # 01b
  XCAP_10pF   = XCAP_2, // 0x08 # 10b
  XCAP_12_5pF = XCAP_3  // 0x0C # 11b
} xcap_t;

class basic_clock
{
public:
  basic_clock(dco_freq_t freq = DCO_F_16MHz)
  {
    setDCO(freq);
    cfgACLK();
    cfgMCLK();
    cfgSMCLK();
  }

  // Default the XCAP configuration to 1pF (reset default). The 'cap' paramater
  // is only meaningful if the ACLK source is to be the 32kHz crystal. Otherwise,
  // it will be ignored.
  static void cfgACLK(aclk_src_t src = ACLK_SRC_VLO,
                      clk_div_t  div = CLK_DIV_1,
                      xcap_t     cap = XCAP_1pF);
  static void cfgMCLK(mclk_src_t src = MCLK_SRC_DCO,
                      clk_div_t  div = CLK_DIV_1);
  static void cfgSMCLK(smclk_src_t src = SMCLK_SRC_DCO,
                        clk_div_t  div = CLK_DIV_1);
  static void setDCO(dco_freq_t freq = DCO_F_16MHz);
};
