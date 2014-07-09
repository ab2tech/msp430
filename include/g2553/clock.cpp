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

// MSP430G2553 Clock Library
// Simplify MSP430G2553 clock initialization and frequency modification
#include "clock.h"

// Static class variable initialization
uint32_t   clock::ms_count = 0;
uint32_t   clock::s_count = 0;
uint16_t   clock::ticks_in_a_ms = 0;
uint32_t   clock::sys_freq = 0;

// Commit a timerA CCR0 to the clock library
msp_timerA_t clock::timer = CLK_TIMERA;

const msp_timerA_t clock::aux_timer[CLK_AUX_TIMERS] = {
  ta0_1, ta0_2 };
bool clock::aux_timer_in_use[CLK_AUX_TIMERS] = {
  false, false };

// Allocate one of the auxiliary CCRs for use by another library
msp_timerA_t clock::allocTimer(void)
{
  uint8_t i;
  // Loop through the timer CCRs
  for (i=(CLK_AUX_TIMERS-1); i>0; --i)
  {
    // If one isn't in use
    if (!aux_timer_in_use[i])
    {
      // Mark it used
      aux_timer_in_use[i] = true;
      // Return it
      return aux_timer[i];
    }
  }

  // Unable to return a valid timer -- none are available
  return MSP_TIMERA_SIZE;
}

uint32_t clock::calcSysFreq(dco_freq_t dco_freq)
{
  switch (dco_freq)
  {
    case DCO_F_16MHz:
      return F_16MHz;
    case DCO_F_12MHz:
      return F_12MHz;
    case DCO_F_8MHz:
      return F_8MHz;
    case DCO_F_1MHz:
    default:
      return F_1MHz;
  }
}

// Configure the values used to establish delay functions that are integrated
// into the clock library
void clock::cfgDelay(void)
{
  // Calculate the number of ticks in a ms
  ticks_in_a_ms = (sys_freq / CLK_MS_IN_S);

  // Configure the clock's timerA for continuous mode and assign SMCLK as its
  // source
  set(tactl(timer), (TASSEL_2 | MC_2 | TACLR));
  // Enable the timer interrupt (ISR ignores anything not a delay)
  on (tacctl(timer), CCIE);
  // Set the CCR at 0 to start
  set(taccr(timer), 0);
}

// Delay for the number of milliseconds specified as an argument
void clock::delayMS(uint32_t ms)
{
  // Set the TACCR to the number of ticks in a ms plus the current timer value
  set(taccr(timer), (ticks_in_a_ms + tar(timer)));
  // Configure the number of milliseconds to delay
  ms_count = ms;
  // Enter LPM0 -- clocks stay enabled, disables CPU
  // GIE bit is already enabled by clock initialization
  LPM0;
}

// Delay for the number of seconds specified as an argument
void clock::delayS(uint32_t s)
{
  s_count = s;
  while (s_count > 0)
  {
    delayMS(1000);
    s_count--;
  }
}

// Release a timer that was previously allocated from the clock's CCR pool
bool clock::releaseTimer(msp_timerA_t timer)
{
  uint8_t i;
  // Loop through all the CCRs
  for (i=(CLK_AUX_TIMERS-1); i>0; --i)
  {
    // If we find the timer to release
    if (aux_timer[i] == timer)
    {
      // Release it
      aux_timer_in_use[i] = false;
      // Return true to indicate the timer was properly released
      return true;
    }
  }
  // Failed to release the timer
  return false;
}

void clock::clk2PinDisable(clk_pin_t pin)
{
  // Turn the PXSEL bit off for this pin, but don't change other settings (we
  // don't know what the user might be expecting)
  pinSelOff((msp_pin_t) pin);
}

void clock::clk2PinEnable(clk_pin_t pin)
{
  // Enable the pin as an output and enable its corresponding PXSEL bit
  pinOutput((msp_pin_t) pin);
  pinSelOn((msp_pin_t) pin);
}

//TODO use definition instead if I can
#pragma vector=TIMER0_A0_VECTOR
__interrupt void clock::delayISR(void)
{
  // Ignore interrupts when a MS delay is not in progress
  if (ms_count == 0)
    return;
  // Subtract one from our delay count
  ms_count--;
  // If we're done delaying, exit LPM0
  if (ms_count == 0)
    LPM0_EXIT;
  else // Add ticks_in_a_ms to the timer CCR
    addeq(taccr(timer), ticks_in_a_ms);
}
