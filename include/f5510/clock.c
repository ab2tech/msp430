// Copyright (C) 2012, 2013
// AB2 Technologies
// Austin Beam, Alan Bullick

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// MSP430F5510 Clock Library
// Simplify MSP430F5510 clock initialization and frequency modification
#include "clock.h"

// Global variables to maintain the state of the DCO and ACLK
dco_freq_t  dco_freq;
aclk_freq_t aclk_freq;

// Return the current frequency of the DCO in terms of the definition structure
// established in the header file of this library
dco_freq_t clockGetDCOFreq()
{
  return dco_freq;
}

// Initialize the clock to the default settings
// DCO -> 25MHz
// ACLK -> REFO
void clockInit()
{
  clockSetDCO(DCO_F_25MHz);
  clockSetACLK(ACLK_F_REFO);
}

// Set ACLK to the aclk_freq_t value specified
void clockSetACLK(aclk_freq_t freq)
{
  aclk_freq = freq;
  switch (aclk_freq)
  {
    case ACLK_F_REFO:
      // Set ACLK = REFO
      UCSCTL4 |= SELA_2;
      break;
    //default:
      // ACLK = XT1CLK
  }
}

// Set the DCO frequency to the dco_freq_t value specified
void clockSetDCO(dco_freq_t freq)
{
  dco_freq = freq;
  switch (dco_freq)
  {
    case DCO_F_1MHz:
      // In this case, we're done
      return;
    case DCO_F_8MHz:
      // Set DCO FLL reference = REFO
      UCSCTL3 = SELREF_2;
      UCSCTL4 |= SELA_2;

      // Set lowest possible DCOx, MODx
      UCSCTL0 = 0x0000;

      // Disable the FLL control loop
      __bis_SR_register(SCG0);
      // Select DCO range 16MHz operation
      UCSCTL1 = DCORSEL_5;
      // Set DCO Multiplier for 8MHz
      // (N + 1) * FLLRef = Fdco
      // (249 + 1) * 32768 = 8MHz
      UCSCTL2 |= 249;
      
      // Enable the FLL control loop
      __bic_SR_register(SCG0);

      // Worst-case settling time for the DCO when the DCO range bits have been
      // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
      // UG for optimization.
      // 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
      __delay_cycles(250000);
      break;
    case DCO_F_12MHz:
      // Set DCO FLL reference = REFO
      UCSCTL3 = SELREF_2;
      UCSCTL4 |= SELA_2;

      // Set lowest possible DCOx, MODx
      UCSCTL0 = 0x0000;

      // Disable the FLL control loop
      __bis_SR_register(SCG0);
      // Select DCO range 24MHz operation
      UCSCTL1 = DCORSEL_5;
      // Set DCO Multiplier for 12MHz
      // (N + 1) * FLLRef = Fdco
      // (374 + 1) * 32768 = 12MHz
      UCSCTL2 = FLLD_1 + 374;
      
      // Enable the FLL control loop
      __bic_SR_register(SCG0);

      // Worst-case settling time for the DCO when the DCO range bits have been
      // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
      // UG for optimization.
      // 32 x 32 x 12 MHz / 32,768 Hz = 375000 = MCLK cycles for DCO to settle
      __delay_cycles(375000);
      break;
    case DCO_F_25MHz:
      // Increase Vcore setting to level3 to support fsystem=25MHz
      // NOTE: Change core voltage one level at a time..
      upVcore (0x01);
      upVcore (0x02);
      upVcore (0x03);
      
      // Set DCO FLL reference = REFO
      UCSCTL3 = SELREF_2;
      UCSCTL4 |= SELA_2;

      
      // Disable the FLL control loop
      __bis_SR_register(SCG0);
      // Set lowest possible DCOx, MODx
      UCSCTL0 = 0x0000;
      // Select DCO range 50MHz operation
      UCSCTL1 = DCORSEL_7;
      // Set DCO Multiplier for 25MHz
      // (N + 1) * FLLRef = Fdco
      // (762 + 1) * 32768 = 25MHz
      // Set FLL Div = fDCOCLK/2
      UCSCTL2 = FLLD_1 + 762;
      
      // Enable the FLL control loop
      __bic_SR_register(SCG0);

      // Worst-case settling time for the DCO when the DCO range bits have been
      // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
      // UG for optimization.
      // 32 x 32 x 25 MHz / 32,768 Hz ~ 780k MCLK cycles for DCO to settle
      __delay_cycles(782000);
      break;
    //default:
      // 1MHz
  }
  // Loop until XT1,XT2 & DCO stabilizes - In this case only DCO has to stabilize
  do
  {
    // Clear XT2,XT1,DCO fault flags
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
    // Clear fault flags                      
    SFRIFG1 &= ~OFIFG;
  } while (SFRIFG1&OFIFG); // Test oscillator fault flag 
}

// Raise the level of the chip's Vcore as needed to increase the DCO frequency
void clockUpVcore(uint16_t level)
{
  // Open PMM registers for write
  PMMCTL0_H = PMMPW_H;
  // Set SVS/SVM high side new level
  SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
  // Set SVM low side to new level
  SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
  // Wait till SVM is settled
  while ((PMMIFG & SVSMLDLYIFG) == 0);
  // Clear already set flags
  PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
  // Set VCore to new level
  PMMCTL0_L = PMMCOREV0 * level;
  // Wait till new level reached
  if ((PMMIFG & SVMLIFG))
    while ((PMMIFG & SVMLVLRIFG) == 0);
  // Set SVS/SVM low side to new level
  SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
  // Lock PMM registers for write access
  PMMCTL0_H = 0x00;
}
