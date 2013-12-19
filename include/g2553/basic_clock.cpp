// Copyright (C) [2013, 2013] [AB2 Technologies] [Austin Beam, Alan Bullick]
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

// MSP430G2553 Basic Clock Library
// Simplify MSP430G2553 clock initialization with as minimal a footprint
// as possible
#include "basic_clock.h"

// Bit offsets for Basic Clock Module+ dividers
#define DIVA_OFFSET 4
#define DIVM_OFFSET 4
#define DIVS_OFFSET 1

// Define the maximum value for a 2-bit divider
#define DIV_MAX CLK_DIV_8

// Define the XIN/XOUT pins
#define XIN_PIN  p2_6
#define XOUT_PIN p2_7

// Set the DCO frequency
void basic_clock::setDCO(dco_freq_t freq)
{
  // dco_freq_t are the addresses for each calibrated DCO data in flash
  // freq is the address corresponding to CALDCO of the desired frequency
  uint8_t caldco = REG8(freq);
  // freq+1 is the address corresponding to CALBC1 of the desired frequency
  uint8_t calbc1 = REG8(freq+1);

  set(BCSCTL1, calbc1);
  set(DCOCTL,  caldco);
}

// On higher-end MSP430 devices, cfgCLK can provide a common API for each clock
// configuration (since each configuration is identical), but there are too
// many deltas on the G2553 between each clock to make this practical. Too many
// cycles would be wasted cross-checking everything. Thus, we'll stick with
// discrete configurations on this device for each ACLK, MCLK, and SMCLK.

// Another thing to keep in mind on value-line devices is the loose definition
// of ACLK. Here, we try to be as consistent as possible to avoid confusion,
// but there really is no ACLK as would be defined on a higher-end device.
// Basically ACLK is synonymous with LFXT1 and LFXT1 can coincidentally be
// sourced by VLO. Silly, but it is what it is.

// Set the ACLK source and divider
void basic_clock::cfgACLK(aclk_src_t src = ACLK_SRC_VLO,
                          clk_div_t  div = CLK_DIV_1,
                          xcap_t     cap = XCAP_1pF)
{
  if (src != ACLK_SRC_VLO)
  {
    // Enable XIN and XOUT
    pinInput(XIN_PIN);
    pinSelOn(XIN_PIN);
    pinOutput(XOUT_PIN);
    pinSelOn(XOUT_PIN);
    // Clear the XCAP bits
    off(BCSCTL3, XCAP_3);
  }

  if (src == ACLK_SRC_32kHz)
  {
    // Configure XCAP
    on (BCSCTL3, cap);
  }

  // Clear the source bits
  off(BCSCTL3, LFXT1S_3);
  // Configure the source
  on (BCSCTL3, src);

  // Clear the divider bits
  off(BCSCTL1, (DIV_MAX << ACLK_DIV_OFFSET));
  // Configure the divider
  on (BCSCTL1, (div << ACLK_DIV_OFFSET));

  // Unless the clock source is VLO, need to let the oscillator stabilize
  if (src != ACLK_SRC_VLO)
  {
    do
    {
      // Clear the XT1 fault flag
      off(BCSCTL3, LFXT1OF);
      // Clear the OFIFG fault flag
      off(IFG1, OFIFG);
    } while (read(BCSCTL3, LFXT1OF));
  }

}

// Set the MCLK source and divider
void basic_clock::cfgMCLK(mclk_src_t src = MCLK_SRC_DCO,
                          clk_div_t  div = CLK_DIV_1)
{
  // Clear the source bits
  off(BCSCTL2, SELM_3);
  // Configure the source
  on (BCSCTL2, src);

  // Clear the divider bits
  off(BCSCTL2, (DIV_MAX << MCLK_DIV_OFFSET));
  // Configure the divider
  on (BCSCTL2, (div << MCLK_DIV_OFFSET));
}

// Set the SMCLK source and divider
void basic_clock::cfgSMCLK(smclk_src_t src = SMCLK_SRC_DCO,
                           clk_div_t   div = CLK_DIV_1)
{
  // Clear the source bit
  off(BCSCTL2, SELS);
  // Configure the source
  on (BCSCTL2, src);

  // Clear the divider bits
  off(BCSCTL2, (DIV_MAX << SMCLK_DIV_OFFSET));
  // Configure the divider
  on (BCSCTL2, (div << SMCLK_DIV_OFFSET));
}
