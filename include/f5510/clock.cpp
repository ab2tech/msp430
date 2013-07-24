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

// MSP430F5510 Clock Library
// Simplify MSP430F5510 clock initialization and frequency modification
#include "clock.h"

// Static class variable initialization
uint32_t clock::uptime_count = 0;
uint16_t clock::uptime_interval = 0;
uint16_t clock::uptime_interval_count = 0;

// Configure CLK sel and divider -> ACLK, MCLK, SMCLK
clk_ret_t clock::cfgCLK(clk_t clk, clk_sel_t sel, clk_div_t div)
{
  bool uptime_reconfigure = false;
  clk_ret_t status = CLK_NO_ERR;

  switch (clk)
  {
    case CLK_ACLK:
      switch (sel)
      {
        case CLK_SEL_XT1:
          if (!xt1_is_enabled)
          {
            sel = CLK_SEL_REFO;
            status = CLK_SEL_NOT_INITED;
          }
          break;
        case CLK_SEL_XT2:
          if (!xt2_is_enabled)
          {
            sel = CLK_SEL_REFO;
            status = CLK_SEL_NOT_INITED;
          }
          break;
      }
      break;
    case CLK_MCLK:
      switch (sel)
      {
        case CLK_SEL_XT1:
          if (!xt1_is_enabled)
          {
            sel = CLK_SEL_DCODIV;
            status = CLK_SEL_NOT_INITED;
          }
          break;
        case CLK_SEL_XT2:
          if (!xt2_is_enabled)
          {
            sel = CLK_SEL_DCODIV;
            status = CLK_SEL_NOT_INITED;
          }
          break;
      }
      break;
    case CLK_SMCLK:
      switch (sel)
      {
        case CLK_SEL_XT1:
          if (!xt1_is_enabled)
          {
            sel = CLK_SEL_DCODIV;
            status = CLK_SEL_NOT_INITED;
          }
          break;
        case CLK_SEL_XT2:
          if (!xt2_is_enabled)
          {
            sel = CLK_SEL_DCODIV;
            status = CLK_SEL_NOT_INITED;
          }
          break;
      }
      break;
  }

  // Disable uptime counting while we modify the clock configuration if this
  // is the WDT clock
  if (uptime_is_enabled && (getUpTimeClk() == clk))
  {
    disableWDT();
    uptime_reconfigure = true;
  }
  // Disable the existing source
  UCSCTL4 &= ~(CLK_DIVSEL_MAX << clk);
  // Configure the new source
  UCSCTL4 |= (sel << clk);

  // Disable the existing divider
  UCSCTL5 &= ~(CLK_DIVSEL_MAX << clk);
  // Configure the new divider
  UCSCTL5 |= (div << clk);
  // Re-configure uptime if we just disabled it
  if (uptime_reconfigure)
    cfgUpTime();

  return status;
}

// Configure the values used to establish delay functions that are integrated
// into the clock library
void clock::cfgDelay(void)
{
  // Calculate the number of ticks in a ms
  ticks_in_a_ms = (sys_freq / CLK_MS_IN_S);

  // Configure the clock's timerA for up mode and assign SMCLK as its source
  set(tactl(timer), (TASSEL__SMCLK | MC__UP | TACLR));
  // Make sure the timerA in use does not have its interrupt enabled (we only
  // want to use the IFG, don't need an ISR)
  off(tacctl(timer), CCIE);
  // Disable the timerA
  set(taccr(timer), 0);
}

// Set the DCO frequency to the sys_freq_t value specified using the fll_sel
// clock source specified. FLLD can also be configured using cfgSysFreq.
// Furthermore, the fll_freq paramater is provided in the event an external
// crystal is used as the FLL clock source.
// By default MCLK and SMCLK will be configured to be sourced from DCODIV, not
// DCO. If this is the case, MCLK and SMCLK frequencies will be F_DCO/FLLD.
clk_ret_t clock::cfgSysFreq(uint32_t  cfg_sys_freq,
                            clk_sel_t cfg_fll_selref,
                            uint32_t  cfg_fll_freq,
                            clk_div_t cfg_fll_refdiv,
                            clk_div_t cfg_fll_d_div)
{
  clk_ret_t status = CLK_NO_ERR;
  // Calculate the clock ratio by first dividing the system frequency by two,
  // then dividing by the FLL reference frequency
  uint16_t clk_ratio = (uint16_t) ((cfg_sys_freq / 2) / cfg_fll_freq);

  // The effective system frequency is taken by multiplying the clock ratio (+1
  // to account for N-1) by the FLL frequency divided by the FLL reference
  // divider. Finally, we need to account for the division by two performed to
  // calculate the clock ratio.
  uint32_t eff_sys_freq =
    (((clk_ratio + 1) * (cfg_fll_freq / (1 << (uint8_t)cfg_fll_refdiv))) * 2);

  if (sys_freq == eff_sys_freq)
    return CLK_NO_ERR;

  if (cfg_fll_refdiv > CLK_DIV_16)
    return CLK_INVALID_PARAMETER;

  // Increase Vcore if the new frequency is higher than the existing frequency
  if (eff_sys_freq > sys_freq)
  {
    if (eff_sys_freq < CLK_CORE_L0_MAX_DCO_F)
      status = cfgVcore(CLK_CORE_L0);
    else if (eff_sys_freq < CLK_CORE_L1_MAX_DCO_F)
      status = cfgVcore(CLK_CORE_L1);
    else if (eff_sys_freq < CLK_CORE_L2_MAX_DCO_F)
      status = cfgVcore(CLK_CORE_L2);
    else if (eff_sys_freq < CLK_CORE_L3_MAX_DCO_F)
      status = cfgVcore(CLK_CORE_L3);

    // Don't continue if there was an error increasing Vcore for the new system
    // frequency
    if (status != CLK_NO_ERR)
      return status;
  }

  // Set the FLL reference to default (XT1CLK) before we change it
  off(UCSCTL3, (CLK_DIVSEL_MAX << FLL_SELREF_OFFSET));
  // Configure the FLL reference select as specified
  // Valid selections:
  // XT1CLK | REFOCLK | XT2CLK
  // Invalid selection mapping
  // VLO -> XT1CLK
  // DCO -> DCODIV -> REFOCLK
  // If XT2CLK is selected and not available, REFOCLK will be used
  on (UCSCTL3, ((uint16_t) cfg_fll_selref << FLL_SELREF_OFFSET));

  // Set the FLL reference divider to default
  off(UCSCTL3, ((uint16_t) CLK_DIVSEL_MAX << FLL_REFDIV_OFFSET));
  // Configure the FLL reference divider as specified
  on (UCSCTL3, ((uint16_t) cfg_fll_refdiv << FLL_REFDIV_OFFSET));

  // Disable the FLL control loop
  __bis_SR_register(SCG0);

  // Set the lowest possible DCOx, MODx
  set(UCSCTL0, 0);

  // Determine the proper UCSCTL1 configuration
  if (eff_sys_freq <= CLK_DCORSEL_0_MAX_F)
    set(UCSCTL1, DCORSEL_0);
  else if (eff_sys_freq <= CLK_DCORSEL_1_MAX_F)
    set(UCSCTL1, DCORSEL_1);
  else if (eff_sys_freq <= CLK_DCORSEL_2_MAX_F)
    set(UCSCTL1, DCORSEL_2);
  else if (eff_sys_freq <= CLK_DCORSEL_3_MAX_F)
    set(UCSCTL1, DCORSEL_3);
  else if (eff_sys_freq <= CLK_DCORSEL_4_MAX_F)
    set(UCSCTL1, DCORSEL_4);
  else if (eff_sys_freq <= CLK_DCORSEL_5_MAX_F)
    set(UCSCTL1, DCORSEL_5);
  else if (eff_sys_freq <= CLK_DCORSEL_6_MAX_F)
    set(UCSCTL1, DCORSEL_6);
  else
    set(UCSCTL1, DCORSEL_7);

  // Clear the existing FLL multiplier value
  off(UCSCTL2, FLLN_MASK);
  // Configure the DCO multiplier for the specified frequency (don't subtract
  // one because we prefer to slightly overshoot rather than undershoot)
  on (UCSCTL2, (clk_ratio));

  // Clear the existing FLL divider value
  off(UCSCTL2, (CLK_DIVSEL_MAX << FLLD_OFFSET));
  // Configure the divider as specified
  on (UCSCTL2, (cfg_fll_d_div << FLLD_OFFSET));

  // Enable the FLL control loop
  __bic_SR_register(SCG0);

  // Loop until DCO, XT1, & XT2 stabilize
  do
  {
    // Clear DCO, XT1, & XT2 fault flags
    off(UCSCTL7, (DCOFFG | XT1LFOFFG | XT2OFFG));
    // Clear fault flags
    off(SFRIFG1, OFIFG);
  } while (UCSCTL7 & DCOFFG); // Test DCO fault flag

  // Decrease Vcore to lowest possible setting if frequency was decreased
  if (eff_sys_freq < sys_freq)
  {
    if (eff_sys_freq > CLK_CORE_L3_MAX_DCO_F)
      status = cfgVcore(CLK_CORE_L3);
    else if (eff_sys_freq > CLK_CORE_L2_MAX_DCO_F)
      status = cfgVcore(CLK_CORE_L2);
    else if (eff_sys_freq > CLK_CORE_L1_MAX_DCO_F)
      status = cfgVcore(CLK_CORE_L1);
    else if (eff_sys_freq > CLK_CORE_L0_MAX_DCO_F)
      status = cfgVcore(CLK_CORE_L0);
  }

  // Allow the FLL to stabilize
  // Make sure the timerA in use does not have its interrupt enabled (we only
  // want to use the IFG, don't need an ISR)
  off(tacctl(timer), CCIE);
  // Configure the clock's timerA for continuous mode and assign ACLK as its
  // source -- we'll assume REFO is sourcing ACLK for the moment
  set(tactl(timer), (TASSEL__ACLK | MC__UP | TACLR));
  set(taccr(timer), DCO_SETTLE_CYCLES_32K);

  // Wait until CCIFG is set
  while (!(read(tacctl(timer), CCIFG)));
  off(tacctl(timer), CCIFG);

  // Set the configured sys_freq value to the effective system frequency
  sys_freq = eff_sys_freq;
  // Set the configured fll_freq value
  // May decide to include divider as well later on -- for now, not necessary
  // fll_freq = (cfg_fll_freq / cfg_fll_refdiv);
  fll_freq = cfg_fll_freq;
  // Configure the delay timer values
  cfgDelay();
  // Re-configure the uptime counter based on our new configuration
  cfgUpTime();
  return CLK_NO_ERR;
}

// Configure the chip's WDT as an interval timer and use it to keep track of
// the chip's uptime in seconds
clk_ret_t clock::cfgUpTime(clk_uptime_clk_t uptime_clk)
{
  // We'll need to establish the WDT frequency
  uint32_t wdtfreq;
  // Determine what clock is sourcing our uptime clock -- we'll be using this
  // to track uptime with the interval timer
  clk_sel_t wdtclk = getCLKSel((clk_t) uptime_clk);
  // Determine the divider for this clock so we consider it in our calculations
  uint8_t   wdtdiv = (1 << ((uint8_t) getCLKDiv((clk_t) uptime_clk)));

  // Collect some info based on what source ACLK is using
  switch (wdtclk)
  {
    // Easiest case -> ACLK == REFO == F_32kHz
    case CLK_SEL_DCO:
      wdtfreq = (sys_freq / wdtdiv);
      break;
    case CLK_SEL_DCODIV:
      wdtfreq = ((sys_freq / getFLLD()) / wdtdiv);
      break;
    case CLK_SEL_REFO:
      wdtfreq = (F_32kHz / wdtdiv);
      break;
    case CLK_SEL_VLO:
      // VLO is useless for timing, disable uptime counter in this case
      uptime_is_enabled = false;
      return CLK_UPTIME_FREQ_UNSUPPORTED;
    case CLK_SEL_XT1:
      if (xt1_is_enabled)
      {
        wdtfreq = (xt1_freq / wdtdiv);
      }
      else
      {
        // If XT1 is selected and not configured, ACLK needs a source for the
        // interval timer to work
        cfgCLK(CLK_ACLK, CLK_SEL_REFO, CLK_DIV_1);
        wdtfreq = F_32kHz;
      }
      break;
    case CLK_SEL_XT2:
      if (xt2_is_enabled)
      {
        wdtfreq = (xt2_freq / wdtdiv);
      }
      else
      {
        // If XT2 is selected and not configured, ACLK needs a source for the
        // interval timer to work
        cfgCLK(CLK_ACLK, CLK_SEL_REFO, CLK_DIV_1);
        wdtfreq = F_32kHz;
      }
      break;
    default:
      uptime_is_enabled = false;
      return CLK_INVALID_PARAMETER;
  }

  // Check the WDT source frequency so we know how to configure the interval
  // timer
  if (wdtfreq == F_32kHz)
  {
    // Our interval is easy in this case (this tells us what our increment
    // needs to be in between interval timer interrupts -- in other words, for
    // 32.768K, there is exactly one interrupt per second)
    uptime_interval = 1;
  }
  else
  {
    // Need to calculate interval based on the frequency -- if our frequency is
    // not divisible by 32.768K we'll turn off the interval counter since our
    // accuracy won't be very good -- this is a limitation of the WDT clock
    // dividers that are available. Regardless, it's best to run the uptime
    // mechanism against a 32K clock to reduce unnecessary interrupt overhead.
    // Thus, if this mechanism is important to the user, make the WDT clock 
    // 32.768K.
    if (wdtfreq % F_32kHz)
    {
      uptime_is_enabled = false;
      return CLK_UPTIME_FREQ_UNSUPPORTED;
    }
    else
    {
      uptime_interval = (wdtfreq / F_32kHz);
    }
  }

  // Enable the WDT interrupt
  on(SFRIE1, WDTIE);
  // Enable global interrupts for this to work :)
  _EINT();

  if (uptime_clk == CLK_UPTIME_CLK_ACLK)
  {
    // WDT - password, ACLK, interval mode, reset counter, divide by 32.768K
    set(WDTCTL, (WDTPW | WDTSSEL__ACLK | WDTTMSEL | WDTCNTCL | WDTIS__32K));
  }
  else
  {
    // WDT - password, ACLK, interval mode, reset counter, divide by 32.768K
    set(WDTCTL, (WDTPW | WDTSSEL__SMCLK | WDTTMSEL | WDTCNTCL | WDTIS__32K));
  }

  // Uptime counter is now enabled
  uptime_is_enabled = true;
  return CLK_NO_ERR;
}

// Configure the level of the chip's Vcore
// For MSP430F5510, requirements for Vcore and Vcc versus maximum MCLK
// frequency are listed below
// PMMCOREVx = 0 | 1.8V <= Vcc <= 3.6V | 0 < Fmax <= 8MHz
// Vcore (Active) => 1.40V | Vcore (LPM) => 1.44V
// PMMCOREVx = 1 | 2.0V <= Vcc <= 3.6V | 0 < Fmax <= 12MHz
// Vcore (Active) => 1.60V | Vcore (LPM) => 1.64V
// PMMCOREVx = 2 | 2.2V <= Vcc <= 3.6V | 0 < Fmax <= 20MHz
// Vcore (Active) => 1.80V | Vcore (LPM) => 1.84V
// PMMCOREVx = 3 | 2.4V <= Vcc <= 3.6V | 0 < Fmax <= 25MHz
// Vcore (Active) => 1.90V | Vcore (LPM) => 1.94V
clk_ret_t clock::cfgVcore(clk_vcore_t level)
{
  clk_ret_t status = CLK_NO_ERR;
  // Read the previous level from hardware by masking the irrelevant bits in
  // the PMMCTL0 register
  uint8_t hw_level = (uint8_t)(PMMCTL0 & PMMCOREV_3);

  // If Vcore is already configured to this level, we're done
  if ((clk_vcore_t)hw_level == level)
    return CLK_NO_ERR;

  while ((hw_level != level) && (status != CLK_VCORE_ERR))
  {
    if (hw_level < level)
      status = cfgVcoreUp(++hw_level);
    else
      status = cfgVcoreDown(--hw_level);
  }

  return status;
}

// Decrease the level of the chip's Vcore
// NOTE: Calls to modify Vcore should go through cfgVcore rather than
// cfgVcoreDown or cfgVcoreUp directly
//
// Includes workaround to satisfy FLASH37 ERRATA -- see explanation in
// cfgVcoreUp description
clk_ret_t clock::cfgVcoreDown(uint8_t level)
{
  uint16_t PMMRIE_bak;
  clk_vcore_t hw_level = (clk_vcore_t) (PMMCTL0 & PMMCOREV_3);

  // Make sure we're only going down one level -- it's not safe to decrement by
  // more than one level. Furthermore, this function can't increment the Vcore
  // level. Thus, only (hw_level - 1) can be achieved with this function.
  if (level != (hw_level - 1))
    return CLK_VCORE_ERR;

  // Open PMM registers for write
  set(PMMCTL0_H, PMMPW_H);
  // Backup existing configuration
  set(PMMRIE_bak, PMMRIE);
  // Disable all interrupts
  set(PMMRIE, 0);
  // Clear any interrupt flags
  set(PMMIFG, 0);

  // Set SVM, SVS high & low side to new settings in normal mode
  set(SVSMHCTL, (SVMHE | (SVSMHRRL0 * level) |
                 SVSHE | (SVSHRVL0 * level)));
  set(SVSMLCTL, (SVMLE | (SVSMLRRL0 * level) |
                 SVSLE | (SVSLRVL0 * level)));
  // Wait for SVM high side and low side to settle
  while ((PMMIFG & SVSMHDLYIFG) == 0 || (PMMIFG & SVSMLDLYIFG) == 0);
  // Clear interrupt flags
  off(PMMIFG, (SVSMHDLYIFG | SVSMLDLYIFG));

  // SVM, SVS core and high side configured to protect for new core level

  // Set VCore to new level
  set(PMMCTL0_L, (PMMCOREV0 * level));

  // Make sure all PMM interrupt flags are cleared
  set(PMMIFG, 0);

  // Restore PMM interrupt enable register
  set(PMMRIE, PMMRIE_bak);

  // Lock PMM registers
  set(PMMCTL0_H, 0);
  // Return success
  return CLK_NO_ERR;
}

// Raise the level of the chip's Vcore as needed to increase the system freq
// NOTE: Calls to modify Vcore should go through cfgVcore rather than
// cfgVcoreDown or cfgVcoreUp directly
//
// Includes workaround to satisfy FLASH37 ERRATA -- rather than checking the
// IFG after configuring the new core voltage level, use SVS/SVM to make sure
// the level is within the bounds imposed by the Vcc minimum for that Vcore.
// Disable PMM interrupts prior to configuration of the new core level, and
// restore that configuration after the new core level is set.
clk_ret_t clock::cfgVcoreUp(uint8_t level)
{
  uint16_t PMMRIE_bak, SVSMHCTL_bak;
  clk_vcore_t hw_level = (clk_vcore_t) (PMMCTL0 & PMMCOREV_3);

  // Make sure we're only going up one level -- it's not safe to increment by
  // more than one level. Furthermore, this function can't decrement the Vcore
  // level. Thus, only (hw_level + 1) can be achieved with this function.
  if (level != (hw_level + 1))
    return CLK_VCORE_ERR;

  // Open PMM registers for write
  set(PMMCTL0_H, PMMPW_H);
  // Backup existing configuration
  set(PMMRIE_bak, PMMRIE);
  set(SVSMHCTL_bak, SVSMHCTL);
  // Disable all interrupts
  set(PMMRIE, 0);
  // Clear any interrupt flags
  set(PMMIFG, 0);
  // Set SVM high side to new level
  set(SVSMHCTL, (SVMHE | SVSHE | (SVSMHRRL0 * level)));
  // Wait until SVM high side settles
  while ((PMMIFG & SVSMHDLYIFG) == 0);
  // Clear flag
  off(PMMIFG, SVSMHDLYIFG);
  // Check if Vcc is high enough for Vcore increase
  if ((PMMIFG & SVMHIFG) == SVMHIFG)
  { // Vcc is too low for a Vcore increase
    // Clear PMM interrupt flags
    off(PMMIFG, 0);
    // Restore the backed up settings
    set(SVSMHCTL, SVSMHCTL_bak);
    // Wait until SVM high side settles
    while ((PMMIFG & SVSMHDLYIFG) == 0);
    // Clear PMM interrupt flags
    set(PMMIFG, 0);
    // Restore PMM interrupt enable register
    set(PMMRIE, PMMRIE_bak);
    // Lock PMM registers
    set(PMMCTL0_H, 0);
    return CLK_VCORE_ERR;
  }
  // Set SVS high side to new level
  on(SVSMHCTL, (SVSHRVL0 * level));
  // Wait until SVM high side settles
  while ((PMMIFG & SVSMHDLYIFG) == 0);
  // Clear flag
  off(PMMIFG, SVSMHDLYIFG);

  // Set VCore to new level
  set(PMMCTL0_L, (PMMCOREV0 * level));

  // Set SVM, SVS low side to new level
  set(SVSMLCTL, (SVMLE | (SVSMLRRL0 * level)) |
                (SVSLE | (SVSLRVL0 * level)));
  // Wait until SVM, SVS low side settles
  while ((PMMIFG & SVSMLDLYIFG) == 0);
  // Clear flag
  off(PMMIFG, SVSMLDLYIFG);

  // SVM, SVS core and high side configured to protect for new core level

  // Go ahead and make sure all PMM interrupt flags are cleared
  set(PMMIFG, 0);
  // Restore PMM interrupt enable register
  set(PMMRIE, PMMRIE_bak);

  // Lock PMM registers
  set(PMMCTL0_H, 0);
  // Return success
  return CLK_NO_ERR;
}

// Configure XT1 to be used as a clock source
clk_ret_t clock::cfgXT1(uint32_t cfg_xt1_freq,
                        clk_xt1_cap_t cfg_xt1_cap,
                        clk_xt_drive_t drive)
{
  // Enable the XT1 pin SEL register bits
  pinSelOn(CLK_XT1IN_PIN);
  pinSelOn(CLK_XT1OUT_PIN);

  // Configure proper mode of XT1 - LF or LF BYPASS
  if (drive != CLK_XT_DRIVE_BYPASS)
  {
     // The only supported frequency for LF XT1 with a crystal is 32.768kHz
     if (cfg_xt1_freq != F_32kHz)
     {
       disableXT1();
       return CLK_INVALID_PARAMETER;
     }
     // Set XT1 drive to max if it isn't already
     if ((UCSCTL6 & CLK_XT_DRIVE_3) != CLK_XT_DRIVE_3)
       on (UCSCTL6, (CLK_XT_DRIVE_3 << XT1_DRIVE_OFFSET));

     // Enable LF mode and clear XCAP and BYPASS
     off(UCSCTL6, (XTS | XCAP_3 | XT1BYPASS));
     // Enable XCAP as specified
     on (UCSCTL6, (cfg_xt1_cap << XT1_CAP_OFFSET));
  }
  else
  {
    // Make sure the XT1 LF bypass frequency being configured is within range
    if (cfg_xt1_freq < XT1_LF_BYPASS_MIN_F || 
        cfg_xt1_freq > XT1_LF_BYPASS_MAX_F)
    {
      disableXT1();
      return CLK_INVALID_PARAMETER;
    }

    // Turn off XT1 oscillator and enable bypass mode
    on (UCSCTL6, (XT1BYPASS | XT1OFF));
  }

  while (UCSCTL7 & XT1LFOFFG)
  {
    // Clear XT1 fault flag
    off(UCSCTL7, XT1LFOFFG);
    // CLear OFIFG fault flag
    off(SFRIFG1, OFIFG);
  }

  // Now that the the oscillator is stabilized, enable XT1 if we're not trying
  // to go into bypass
  if (drive != CLK_XT_DRIVE_BYPASS)
  {
    // Still need to set the requested drive mode for LF
    // Leave XT1 drive at max if auto was requested (main advantage for LF is
    // power savings, so user needs to know what the target is and manually
    // configure this for LF as needed)
    if (drive != CLK_XT_DRIVE_AUTO)
    {
      // Set the new configuration as specified
      set(UCSCTL6, (drive << XT1_DRIVE_OFFSET) |
                   (UCSCTL6 & ~(CLK_XT_DRIVE_3)));
    }

    // Enable XT1
    off(UCSCTL6, XT1OFF);
  }

  xt1_freq = cfg_xt1_freq;
  xt1_is_enabled = true;
  return CLK_NO_ERR;
}

clk_ret_t clock::cfgXT2(uint32_t       cfg_xt2_freq,
                        clk_xt_drive_t cfg_xt2_drive)
{
  // Configure proper mode - XT2 or BYPASS
  if (cfg_xt2_drive != CLK_XT_DRIVE_BYPASS)
  {
    // Make sure the XT2 frequency being configured is within range
    if (cfg_xt2_freq < XT_HF_MIN_F ||
        cfg_xt2_freq > XT_HF_MAX_F)
    {
      disableXT2();
      return CLK_INVALID_PARAMETER;
    }
    // Check if automatic drive selection was requested
    if (cfg_xt2_drive == CLK_XT_DRIVE_AUTO)
    {
      // Determine proper drive strength
      if (cfg_xt2_freq < F_4MHz)
      {
        disableXT2();
        return CLK_INVALID_PARAMETER;
      }
      else if (cfg_xt2_freq < F_8MHz)
        cfg_xt2_drive = CLK_XT_DRIVE_0;
      else if (cfg_xt2_freq < F_16MHz)
        cfg_xt2_drive = CLK_XT_DRIVE_1;
      else if (cfg_xt2_freq < F_24MHz)
        cfg_xt2_drive = CLK_XT_DRIVE_2;
      else if (cfg_xt2_freq <= F_32MHz)
        cfg_xt2_drive = CLK_XT_DRIVE_3;
      else // Should be unreachable
      {
        disableXT2();
        return CLK_INVALID_PARAMETER;
      }
    }
    // Configure the XT2 drive
    if ((UCSCTL6 & CLK_XT_DRIVE_3) != cfg_xt2_drive)
    {
      on (UCSCTL6, ((uint16_t)CLK_XT_DRIVE_3 << XT2_DRIVE_OFFSET));
    }
    // Disable BYPASS
    off(UCSCTL6, XT2BYPASS);
  }
  else
  {
    // Make sure the XT2 bypass frequency being configured is within range
    if (cfg_xt2_freq < XT_HF_BYPASS_MIN_F ||
        cfg_xt2_freq > XT_HF_BYPASS_MAX_F)
    {
      disableXT2();
      return CLK_INVALID_PARAMETER;
    }

    // Turn off XT2 oscillator and enable bypass mode
    on (UCSCTL6, (XT2BYPASS | XT2OFF));
  }

  while (UCSCTL7 & XT2OFFG)
  {
    // Clear XT2 fault flag
    off(UCSCTL7, XT2OFFG);
    // CLear OFIFG fault flag
    off(SFRIFG1, OFIFG);
  }

  if (cfg_xt2_drive != CLK_XT_DRIVE_BYPASS)
  {
    // Enable XT2
    off(UCSCTL6, XT2OFF);
  }

  xt2_freq = cfg_xt2_freq;
  xt2_is_enabled = true;
  return CLK_NO_ERR;
}

void clock::clk2PinDisable(clk_pin_t pin)
{
  // Turn the PXSEL bit off for this pin, but don't change other settings (we
  // don't know what the user might be expecting)
  pinSelOff(pin);
}

void clock::clk2PinEnable(clk_pin_t pin)
{
  // Enable the pin as an output and enable its corresponding PXSEL bit
  pinOutput(pin);
  pinSelOn(pin);
}

clk_div_t inline clock::clk2PinGetAclkDivider(void)
{
  return ((clk_div_t)((UCSCTL5 &
          (CLK_DIVSEL_MAX << DIVPA_OFFSET)) >> DIVPA_OFFSET));
}

void clock::clk2PinSetAclkDivider(clk_div_t div)
{
  // Clear existing divider
  off(UCSCTL5, (CLK_DIVSEL_MAX << DIVPA_OFFSET));
  // Configure new divider
  on (UCSCTL5, (div << DIVPA_OFFSET));
}

// Delay for the number of seconds specified as an argument
void clock::delayS(uint32_t s)
{
  s_count=s;
  while (s_count > 0)
  {
    delayMS(1000);
    s_count--;
  }
}


// Delay for the number of milliseconds specified as an argument
void clock::delayMS(uint32_t ms)
{
  // Set the TACCR to the number of ticks in a ms
  set(taccr(timer), ticks_in_a_ms);
  for (ms_count=(ms + 1); ms_count>0; ms_count--)
  {
    // Loop until CCIFG is set
    while (!read(tacctl(timer), CCIFG));
    // Reset CCIFG
    off(tacctl(timer), CCIFG);
  }
  // Reset and disable the timer since we're done with it
  set(taccr(timer), 0);
}

void inline clock::disableSMCLK(void)
{
  on (UCSCTL6, SMCLKOFF);
}

void inline clock::enableSMCLK(void)
{
  off(UCSCTL6, SMCLKOFF);
}

void clock::disableWDT(void)
{
  set(WDTCTL, (WDTPW | WDTHOLD));
}

void clock::disableXT1(void)
{
  // We have to re-configure the FLL source selection if XT1 is currently in
  // use for this purpose. Use the default FLL source (REFO) in this case, and
  // attempt to reconfigure the system frequency to the same frequency it's
  // configured for now
  if (getFLLSelRef() == CLK_SEL_XT1)
    cfgSysFreq(sys_freq);

  // Also need to replace source for any clocks which may be using XT1
  if (getCLKSel(CLK_ACLK) == CLK_SEL_XT1)
    cfgCLK(CLK_ACLK, CLK_SEL_REFO, CLK_DIV_1);
  if (getCLKSel(CLK_MCLK) == CLK_SEL_XT1)
    cfgCLK(CLK_MCLK, CLK_SEL_DCO, CLK_DIV_1);
  if (getCLKSel(CLK_SMCLK) == CLK_SEL_XT1)
    cfgCLK(CLK_SMCLK, CLK_SEL_DCO, CLK_DIV_1);

  // Reflect our change in the corresponding state variable
  xt1_is_enabled = false;
  // Since XT1 is disabled, reflect that in the XT1 frequency
  xt1_freq = 0;
  // Disable the XT1 pins' corresponding SEL bits
  pinSelOff(CLK_XT1IN_PIN);
  pinSelOff(CLK_XT1OUT_PIN);
}

void clock::disableXT2(void)
{
  // We have to re-configure the FLL source selection if XT2 is currently in
  // use for this purpose. Use the default FLL source (REFO) in this case.
  if (getFLLSelRef() == CLK_SEL_XT2)
    cfgSysFreq(sys_freq);

  // Also need to replace source for any clocks which may be using XT2
  if (getCLKSel(CLK_ACLK) == CLK_SEL_XT2)
    cfgCLK(CLK_ACLK, CLK_SEL_REFO, CLK_DIV_1);
  if (getCLKSel(CLK_MCLK) == CLK_SEL_XT2)
    cfgCLK(CLK_MCLK, CLK_SEL_DCO, CLK_DIV_1);
  if (getCLKSel(CLK_SMCLK) == CLK_SEL_XT2)
    cfgCLK(CLK_SMCLK, CLK_SEL_DCO, CLK_DIV_1);

  // Reflect our change in the corresponding state variable
  xt2_is_enabled = false;
  // Since XT2 is disabled, reflect that in the XT2 frequency
  xt2_freq = 0;
  // Disable the XT2 pins' corresponding SEL bits
  pinSelOff(CLK_XT2IN_PIN);
  pinSelOff(CLK_XT2OUT_PIN);
}

clk_div_t inline clock::getCLKDiv(clk_t clk)
{
  return ((clk_div_t)((UCSCTL5 &
          (CLK_DIVSEL_MAX << clk)) >> clk));
}

clk_sel_t inline clock::getCLKSel(clk_t clk)
{
  return ((clk_sel_t)((UCSCTL4 &
          (CLK_DIVSEL_MAX << clk)) >> clk));
}

uint32_t inline clock::getFLLFreq(void)
{
  return (fll_freq);
}

clk_div_t inline clock::getFLLRefDiv(void)
{
  return ((clk_div_t)((UCSCTL3 &
          (CLK_DIVSEL_MAX << FLL_REFDIV_OFFSET)) >> FLL_REFDIV_OFFSET));
}

clk_sel_t inline clock::getFLLSelRef(void)
{
  return ((clk_sel_t)((UCSCTL3 &
          (CLK_DIVSEL_MAX << FLL_SELREF_OFFSET)) >> FLL_SELREF_OFFSET));
}

clk_div_t inline clock::getFLLD(void)
{
  return ((clk_div_t)((UCSCTL2 & 
          (CLK_DIVSEL_MAX << FLLD_OFFSET)) >> FLLD_OFFSET));
}

uint16_t inline clock::getFLLN(void)
{
  return ((uint16_t)((UCSCTL2 & FLLN_MASK) >> FLLN_OFFSET));
}

uint32_t inline clock::getSysFreq(void)
{
  return (sys_freq);
}

uint32_t inline clock::getUpTime(void)
{
  if (uptime_is_enabled)
    return (uptime_count);
  else
    return 0;
}

clk_t clock::getUpTimeClk(void)
{
  // Read the value of the WDTSSEL bits
  uint16_t wdtclk = read(WDTCTL, (WDTSSEL_3));

  switch (wdtclk)
  {
    case WDTSSEL__ACLK:
      return CLK_ACLK;
    case WDTSSEL__SMCLK:
      return CLK_SMCLK;
    case WDTSSEL__VLO: // not good
    default:
      return ((clk_t) 0xDEAD);
  }
}

#pragma vector=WDT_VECTOR
__interrupt void clock::uptime_increment(void)
{
  uptime_interval_count++;
  if (uptime_interval_count >= uptime_interval)
  {
    uptime_count++;
    uptime_interval_count = 0;
  }
}
