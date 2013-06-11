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
#include "clock.h"

// Global variables to maintain the state of the DCO, ACLK, MCLK, and SMCLK
dco_freq_t  dco_freq;

aclk_src_t  aclk_src;
clk_div_t   aclk_div;
mclk_src_t  mclk_src;
clk_div_t   mclk_div;
smclk_src_t smclk_src;
clk_div_t   smclk_div;

xcap_t      xcap;
bool_t      xt2_enabled;
xt2_range_t xt2_range;

// Initialize the clock to our default settings
// These defaults may differ from MSP default settings
// ACLK  -> VLO / 1
// DCO   -> 16MHz
// MCLK  -> DCO / 1
// SMCLK -> DCO / 1
void clockInitDefaults()
{
  clockSetACLK(ACLK_SRC_VLO, CLK_DIV_1);
  clockSetDCO(DCO_F_16MHz);
  clockSetMCLK(MCLK_SRC_DCO, CLK_DIV_1);
  clockSetSMCLK(SMCLK_SRC_DCO, CLK_DIV_1);
}

void clockInit(aclk_src_t asrc, clk_div_t adiv,
               dco_freq_t dcof,
               mclk_src_t msrc, clk_div_t mdiv,
               smclk_src_t ssrc, clk_div_t sdiv)
{
  clockSetACLK(asrc, adiv);
  clockSetDCO(dcof);
  clockSetMCLK(msrc, mdiv);
  clockSetSMCLK(ssrc, sdiv);
}

// Set the ACLK source and divider
void clockSetACLK(aclk_src_t src, clk_div_t div)
{
  aclk_src = src;
  aclk_div = div;
  switch (src)
  {
    case ACLK_SRC_VLO:
      // Disable XTS to set VLO as the ACLK
      off(BCSCTL1, XTS);
      // Set VLOCLK  ACLK
      off(BCSCTL3, LFXT1S1);
      on (BCSCTL3, LFXT1S0);
      break;
    case ACLK_SRC_1MHz:
      // Enable XTS
      on (BCSCTL1, XTS);
      // Disable the XCaps when XTS=1
      clockSetXT1Caps(XCAP_OFF);
      // 00 for 400kHz-1MHz external crystal
      off(BCSCTL3, LFXT1S1);
      off(BCSCTL3, LFXT1S0);
      break;
    case ACLK_SRC_3MHz:
      // Enable XTS
      on (BCSCTL1, XTS);
      // Disable the XCaps when XTS=1
      clockSetXT1Caps(XCAP_OFF);
      // 01 for 1MHz-3MHz external crystal
      off(BCSCTL3, LFXT1S1);
      on (BCSCTL3, LFXT1S0);
      break;
    case ACLK_SRC_16MHz:
      // Enable XTS
      on (BCSCTL1, XTS);
      // Disable the XCaps when XTS=1
      clockSetXT1Caps(XCAP_OFF);
      // 10 for 3MHz-16MHz external crystal
      on (BCSCTL3, LFXT1S1);
      off(BCSCTL3, LFXT1S0);
      break;
    case ACLK_SRC_DIG:
      // XTS is a don't care. However, we need to disable the XCAPs when using
      // an external digital clock source
      clockSetXT1Caps(XCAP_OFF);
      // 11 for digital external clock source
      on (BCSCTL3, LFXT1S1);
      on (BCSCTL3, LFXT1S0);
      break;
    case ACLK_SRC_32kHz:
      // Disable XTS to set 32kHz external crystal as the ACLK
      off(BCSCTL1, XTS);
      // 00 for 32.768kHz external crystal
      off(BCSCTL3, LFXT1S1);
      off(BCSCTL3, LFXT1S0);
      break;
  }

  switch (div)
  {
    case CLK_DIV_8: // 11
      on (BCSCTL1, DIVA1);
      on (BCSCTL1, DIVA0);
      break;
    case CLK_DIV_4: // 10
      off(BCSCTL1, DIVA1);
      on (BCSCTL1, DIVA0);
      break;
    case CLK_DIV_2: // 01
      on (BCSCTL1, DIVA1);
      off(BCSCTL1, DIVA0);
      break;
    case CLK_DIV_1: // 00
      off(BCSCTL1, DIVA1);
      off(BCSCTL1, DIVA0);
      break;
  }
}

// Set the DCO frequency
void clockSetDCO(dco_freq_t freq)
{
  dco_freq = freq;
  switch (freq)
  {
    case DCO_F_16MHz:
      BCSCTL1 = CALBC1_16MHZ;
      DCOCTL  = CALDCO_16MHZ;
      break;
    case DCO_F_12MHz:
      BCSCTL1 = CALBC1_12MHZ;
      DCOCTL  = CALDCO_12MHZ;
      break;
    case DCO_F_8MHz:
      BCSCTL1 = CALBC1_8MHZ;
      DCOCTL  = CALDCO_8MHZ;
      break;
    case DCO_F_1MHz:
      BCSCTL1 = CALBC1_1MHZ;
      DCOCTL  = CALDCO_1MHZ;
      break;
  }
}

// Set the MCLK source and divider
void clockSetMCLK(mclk_src_t src, clk_div_t div)
{
  mclk_src = src;
  mclk_div = div;
  switch (src)
  {
    case MCLK_SRC_ACLK: // 11
      on (BCSCTL2, SELM1);
      on (BCSCTL2, SELM0);
      break;
    case MCLK_SRC_XT2:  // 10 - falls back to ACLK source if XT2 not present
      on (BCSCTL2, SELM1);
      off(BCSCTL2, SELM0);
      break;
    case MCLK_SRC_DCO:  // 00 | 01
      off(BCSCTL2, SELM1);
      off(BCSCTL2, SELM0);
      break;
  }

  switch (div)
  {
    case CLK_DIV_8: // 11
      on (BCSCTL2, DIVM1);
      on (BCSCTL2, DIVM0);
      break;
    case CLK_DIV_4: // 10
      off(BCSCTL2, DIVM1);
      on (BCSCTL2, DIVM0);
      break;
    case CLK_DIV_2: // 01
      on (BCSCTL2, DIVM1);
      off(BCSCTL2, DIVM0);
      break;
    case CLK_DIV_1: // 00
      off(BCSCTL2, DIVM1);
      off(BCSCTL2, DIVM0);
      break;
  }
}

// Set the SMCLK source and divider
void clockSetSMCLK(smclk_src_t src, clk_div_t div)
{
  smclk_src = src;
  smclk_div = div;
  switch (src)
  {
    case SMCLK_SRC_XTAL: // 1
      on (BCSCTL2, SELS);
      break;
    case SMCLK_SRC_DCO:  // 0
      off(BCSCTL2, SELS);
      break;
  }

  switch (div)
  {
    case CLK_DIV_8: // 11
      on (BCSCTL2, DIVS1);
      on (BCSCTL2, DIVS0);
      break;
    case CLK_DIV_4: // 10
      off(BCSCTL2, DIVS1);
      on (BCSCTL2, DIVS0);
      break;
    case CLK_DIV_2: // 01
      on (BCSCTL2, DIVS1);
      off(BCSCTL2, DIVS0);
      break;
    case CLK_DIV_1: // 00
      off(BCSCTL2, DIVS1);
      off(BCSCTL2, DIVS0);
      break;
  }
}

// Set the XT1 capacitance value
void clockSetXT1Caps(xcap_t cap)
{
  xcap = cap;
  switch(cap)
  {
    case XCAP_12_5pF: // 11
      on (BCSCTL3, XCAP1);
      on (BCSCTL3, XCAP0);
      break;
    case XCAP_10pF:   // 10
      on (BCSCTL3, XCAP1);
      off(BCSCTL3, XCAP0);
      break;
    case XCAP_6pF:    // 01
      off(BCSCTL3, XCAP1);
      on (BCSCTL3, XCAP0);
      break;
    case XCAP_1pF:    // 00
    case XCAP_OFF:    // 00
      off(BCSCTL3, XCAP1);
      off(BCSCTL3, XCAP0);
      break;
  }
}
// Enable XT2 and set its frequency range
void clockSetXT2(xt2_range_t range)
{
  xt2_enabled = TRUE;
  // Enable XT2
  off(BCSCTL1, XT2OFF);

  xt2_range = range;
  // Configure the XT2 crystal/oscillator frequency range
  switch (range)
  {
    case XT2_F_DIG:   // 11
      on (BCSCTL3, XT2S1);
      on (BCSCTL3, XT2S0);
      break;
    case XT2_F_16MHz: // 10
      on (BCSCTL3, XT2S1);
      off(BCSCTL3, XT2S0);
      break;
    case XT2_F_3MHz:  // 01
      off(BCSCTL3, XT2S1);
      on (BCSCTL3, XT2S0);
      break;
    case XT2_F_1MHz:  // 00
      off(BCSCTL3, XT2S1);
      off(BCSCTL3, XT2S0);
      break;
  }
}

// Return the fault status for LFXT1
inline uint8_t clockGetLFXT1Fault()
{
  return (BCSCTL3 | LFXT1OF);
}

// Return the fault status for XT2
inline uint8_t clockGetXT2Fault()
{
  return (BCSCTL3 | XT2OF);
}
