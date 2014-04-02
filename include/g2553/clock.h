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
#define AB2_CLOCK

#include <msp430g2553.h>
#include "msp/ab2.h"
#include "pin_fw.h"
#include "timerA_fw.h"
#include "basic_clock.h"

// TODO if needed: add uptime functionality a la F5510 clock library
//
// Define number of units in a second
#define CLK_MS_IN_S                    1000
// Define the timer we're going to allocate to the clock...this is important
// because the other CCRs for this TA are going to be available for allocation
// by other libraries
#define CLK_TIMERA                     ta0_0
// NOTE: This *must* be changed if CLK_TIMERA is changed
#define CLK_TIMER_VECTOR               TIMER0_A0_VECTOR
// Total number of CCRs for this timerA (excluding CCR0)
#define CLK_AUX_TIMERS                 2

// Define the pins that can have clocks output to them
typedef enum _clk_pin_t
{
  CLK_PIN_ACLK  = p1_0,
  CLK_PIN_SMCLK = p1_4
} clk_pin_t;

class clock : public basic_clock
{
public:
  clock(dco_freq_t dco_freq = DCO_F_16MHz) :
    basic_clock(dco_freq)
  {
    // WDT disabled as a part of basic_clock initialization
    sys_freq = calcSysFreq();
    cfgDelay();
  }

  static msp_timerA_t       allocTimer(void);

  static void               clk2PinDisable(clk_pin_t pin);
  static void               clk2PinEnable(clk_pin_t pin);

  static void               delayS(uint32_t s);
  static void               delayMS(uint32_t ms);

  static uint16_t inline    getMSTicks(void)       { return ticks_in_a_ms; };

  static bool               releaseTimer(msp_timerA_t timer);

private:
  static dco_freq_t         dco_freq;
  static uint32_t           sys_freq;

  // TimerA used with the clock library
  static msp_timerA_t       timer;
  static const msp_timerA_t aux_timer[CLK_AUX_TIMERS];
  static bool               aux_timer_in_use[CLK_AUX_TIMERS];

  // Number of ticks in a ms
  static uint16_t           ticks_in_a_ms;

  // Delay counter variables
  static uint32_t           s_count;
  static uint32_t           ms_count;

  static void               calcSysFreq(void);
  static void               cfgDelay(void);

  static void __interrupt   delayISR(void);
}
