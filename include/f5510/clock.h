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

#pragma once
#define AB2_CLOCK

#include <msp430f5510.h>
#include "msp/ab2.h"
#include "pin_fw.h"
#include "timerA_fw.h"

// Define the maximum value for a 3-bit div or sel configuration
#define CLK_DIVSEL_MAX                 7

// Define bit offsets needed by the clock library
#define DIVPA_OFFSET                   12
#define FLL_SELREF_OFFSET              4
#define FLL_REFDIV_OFFSET              0
#define FLLD_OFFSET                    12
#define FLLN_OFFSET                    0
#define XT1_CAP_OFFSET                 2
#define XT1_DRIVE_OFFSET               6
#define XT2_DRIVE_OFFSET               14

// Define bit masks needed by the clock library
#define FLLN_MASK                      (FLLN0 | FLLN1 | FLLN2 | FLLN3 | FLLN4 |\
                                        FLLN5 | FLLN6 | FLLN7 | FLLN8 | FLLN9)

// Define supported frequency ranges for XT
#define XT_HF_MIN_F                    F_4MHz
#define XT_HF_MAX_F                    F_32MHz
#define XT_HF_BYPASS_MIN_F             700000UL
#define XT_HF_BYPASS_MAX_F             XT_HF_MAX_F

#define XT1_LF_BYPASS_MIN_F            10000
#define XT1_LF_BYPASS_MAX_F            50000

// Define maximum frequencies for each Vcore level
#define CLK_CORE_L0_MAX_DCO_F          8000000UL
#define CLK_CORE_L1_MAX_DCO_F          12000000UL
#define CLK_CORE_L2_MAX_DCO_F          20000000UL
#define CLK_CORE_L3_MAX_DCO_F          25000000UL

// Define maximum frequencies for each DCOCORSEL region
#define CLK_DCORSEL_0_MAX_F            630000UL
#define CLK_DCORSEL_1_MAX_F            1250000UL
#define CLK_DCORSEL_2_MAX_F            2500000UL
#define CLK_DCORSEL_3_MAX_F            5000000UL
#define CLK_DCORSEL_4_MAX_F            10000000UL
#define CLK_DCORSEL_5_MAX_F            20000000UL
#define CLK_DCORSEL_6_MAX_F            40000000UL

// Define number of units in a second
#define CLK_MS_IN_S                    1000

// Define the XT pins
#define CLK_XT1IN_PIN                  p5_4
#define CLK_XT1OUT_PIN                 p5_5
#define CLK_XT2IN_PIN                  p5_2
#define CLK_XT2OUT_PIN                 p5_3

// Define the number of cycles to delay for DCO settling @32kHz -- worst case
#define DCO_SETTLE_CYCLES_32K          (32 * 32)

// Define the timer we're going to allocate to the clock...this is important
// because the other CCRs for this TA are going to be available for allocation
// by other libraries
#define CLK_TIMERA                     ta0_0
// Total number of CCRs for this timerA (excluding CCR0)
#define CLK_AUX_TIMERS                 4

// Default return type for clock module functions
typedef enum _clk_ret_t
{
  CLK_NO_ERR,
  CLK_INVALID_PARAMETER,
  CLK_SEL_NOT_INITED,
  CLK_UPTIME_FREQ_UNSUPPORTED,
  CLK_VCORE_ERR,
  CLK_ERR
} clk_ret_t;

// Define the system CLKs available -- use the bit offset in the respective UCS
// registers for each clock as the enum values in order to make other
// calculations easier
typedef enum _clk_t
{
  CLK_ACLK  = 8,
  CLK_MCLK  = 0,
  CLK_SMCLK = 4
} clk_t;

// Define the CLK selections supported by the clock module -- these are
// applicable for ACLK, SMCLK, MCLK, and FLL
typedef enum _clk_sel_t
{
  CLK_SEL_XT1    = 0,
  CLK_SEL_VLO    = 1,
  CLK_SEL_REFO   = 2,
  CLK_SEL_DCO    = 3,
  CLK_SEL_DCODIV = 4,
  CLK_SEL_XT2    = 5
} clk_sel_t;

// Define the CLK dividers supported by the clock module -- these are
// applicable for ACLK, SMCLK, MCLK, and FLL
typedef enum _clk_div_t
{
  CLK_DIV_1  = 0,
  CLK_DIV_2  = 1,
  CLK_DIV_4  = 2,
  CLK_DIV_8  = 3,
  CLK_DIV_16 = 4,
  CLK_DIV_32 = 5
} clk_div_t;

// Define uptime clock types
typedef enum _clk_uptime_clk_t
{
  CLK_UPTIME_CLK_ACLK = CLK_ACLK,
  CLK_UPTIME_CLK_SMCLK = CLK_SMCLK
} clk_uptime_clk_t;

// Define core voltage levels
typedef enum _clk_vcore_t
{
  CLK_CORE_L0 = PMMCOREV_0,
  CLK_CORE_L1 = PMMCOREV_1,
  CLK_CORE_L2 = PMMCOREV_2,
  CLK_CORE_L3 = PMMCOREV_3
} clk_vcore_t;

// Define XT1 CAP values
typedef enum _clk_xt1_cap_t
{
  CLK_XT1_CAP_NONE  = 0,
  CLK_XT1_CAP_2pF   = 0,
  CLK_XT1_CAP_6pF   = 1,
  CLK_XT1_CAP_9pF   = 2,
  CLK_XT1_CAP_12pF  = 3
} clk_xt1_cap_t;

// Define XT drive values (valid for XT1 & XT2)
typedef enum _clk_xt_drive_t
{
  CLK_XT_DRIVE_0 = 0,
  CLK_XT_DRIVE_1 = 1,
  CLK_XT_DRIVE_2 = 2,
  CLK_XT_DRIVE_3 = 3,
  CLK_XT_DRIVE_BYPASS,
  CLK_XT_DRIVE_AUTO
} clk_xt_drive_t;

// Define the pins that can have clocks output to them
typedef enum _clk_pin_t
{
  CLK_PIN_ACLK = p1_0
#ifdef MSP430F5510_EXT
  , CLK_PIN_SMCLK = p2_2
#endif
} clk_pin_t;

// Define clock
class clock
{
public:
  // Configure only system frequency, or leave it as default (F_25MHz)
  explicit clock(uint32_t sys_freq = F_25MHz) :
    sys_freq(F_2MHz),
    uptime_is_enabled(false),
    xt1_is_enabled(false),
    xt2_is_enabled(false),
    xt1_freq(0),
    xt2_freq(0) {
    // WDT *must* be disabled before proceeding or we'll get reset waiting for
    // the clock to settle
    disableWDT();
    // Initialize the clock to the default settings
    // System -> 25MHz
    // ACLK   -> REFO
    // SMCLK  -> DCO
    // MCLK   -> DCO
    cfgSysFreq(sys_freq);
    cfgCLK(CLK_ACLK, CLK_SEL_REFO, CLK_DIV_1);
    cfgCLK(CLK_MCLK, CLK_SEL_DCO, CLK_DIV_1);
    cfgCLK(CLK_SMCLK, CLK_SEL_DCO, CLK_DIV_1);
  };

  msp_timerA_t     allocTimer(void);

  clk_ret_t cfgCLK(clk_t clk, clk_sel_t sel, clk_div_t div);
  clk_ret_t cfgSysFreq(uint32_t  cfg_sys_freq,
                       clk_sel_t cfg_fll_selref = CLK_SEL_REFO,
                       uint32_t  cfg_fll_freq   = F_32kHz,
                       clk_div_t cfg_fll_refdiv = CLK_DIV_1,
                       clk_div_t cfg_fll_d_div  = CLK_DIV_2);
  clk_ret_t        cfgUpTime(clk_uptime_clk_t uptime_clk = CLK_UPTIME_CLK_ACLK);
  clk_ret_t        cfgXT1(uint32_t cfg_xt1_freq,
                          clk_xt1_cap_t cfg_xt1_cap,
                          clk_xt_drive_t drive = CLK_XT_DRIVE_AUTO);
  clk_ret_t        cfgXT2(uint32_t cfg_xt2_freq,
                   clk_xt_drive_t drive = CLK_XT_DRIVE_AUTO);

  void             clk2PinDisable(clk_pin_t pin);
  void             clk2PinEnable(clk_pin_t pin);
  clk_div_t        clk2PinGetAclkDivider(void);
  void             clk2PinSetAclkDivider(clk_div_t div);

  static void      delayS(uint32_t s);
  static void      delayMS(uint32_t ms);

  void             disableUptime(void);

  void             disableXT1(void);
  void             disableXT2(void);

  clk_div_t        getCLKDiv(clk_t clk);
  clk_sel_t        getCLKSel(clk_t clk);

  clk_div_t        getFLLRefDiv(void);
  clk_sel_t        getFLLSelRef(void);
  clk_div_t        getFLLD(void);
  uint16_t         getFLLN(void);

  uint32_t         getUpTime(void);

  void inline      disableSMCLK(void)     { on (UCSCTL6, SMCLKOFF); };
  void inline      enableSMCLK(void)      { off(UCSCTL6, SMCLKOFF); };

  void inline      disableWDT(void)       { set(WDTCTL, (WDTPW | WDTHOLD)); };

  uint32_t         getFLLFreq(void)       { return fll_freq; };
  uint32_t         getSysFreq(void)       { return sys_freq; };

  static uint16_t inline getMSTicks(void) { return ticks_in_a_ms; };

  bool             releaseTimer(msp_timerA_t timer);

private:
  uint32_t         sys_freq;
  uint32_t         fll_freq;

  // TimerA used with the clock library
  static msp_timerA_t timer;
  static const msp_timerA_t aux_timer[CLK_AUX_TIMERS];
  static bool               aux_timer_in_use[CLK_AUX_TIMERS];

  // Number of ticks in a ms
  static uint16_t  ticks_in_a_ms;

  // Delay counter variables
  static uint32_t  s_count;
  static uint32_t  ms_count;

  // State variable for uptime counter
  bool             uptime_is_enabled;

  // Uptime counter -- this is the uptime value in seconds -- has to be static
  // since we need to modify it in our interrupt -- chose 32-bit to maximize
  // the uptime count capability without going too extreme. 32-bit provides 136
  // years of uptime tracking :)
  static uint32_t  uptime_count;
  // Uptime interval -- has to be static since we need it in our interrupt --
  // chose 16-bit because the maximum value for us is 762 @25MHz
  static uint16_t  uptime_interval;
  // Uptime interval counter -- has to be static since we need it in our
  // interrupt
  static uint16_t  uptime_interval_count;

  // State variables for XT1 & XT2 initialization tracking
  bool             xt1_is_enabled;
  bool             xt2_is_enabled;

  // Keep track of the XT1 & XT2 frequency
  uint32_t         xt1_freq;
  uint32_t         xt2_freq;

  void             cfgDelay(void);
  clk_ret_t        cfgVcore(clk_vcore_t level);
  clk_ret_t        cfgVcoreDown(uint8_t level);
  clk_ret_t        cfgVcoreUp(uint8_t level);

  clk_t            getUpTimeClk(void);

  static void __interrupt delayISR(void);
  static void __interrupt uptime_increment(void);
};
