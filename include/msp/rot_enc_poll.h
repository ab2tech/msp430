// Copyright (C) [2013] [AB2 Technologies] [Austin Beam, Alan Bullick]
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

// MSP430 Polling Rotary Encoder Library

#pragma once
#define AB2_ROT_ENC_POLL
#include "ab2.h"
#include "../clock.h"
#include "../pin_fw.h"
#include "../timerA_fw.h"

#ifndef ROT_ENC_DIVISIONS
#define ROT_ENC_DIVISIONS 32
#endif

class rot_enc_poll
{
public:
  // Construct a rotary encoder object with the A and B pins.
  rot_enc_poll(msp_pin_t a, msp_pin_t b, int8_t divs = ROT_ENC_DIVISIONS)
      a(a), b(b), divs(divs), last(0), val(1)
  {
    // Configure the rotary encoder pin pullups
    pinPullup(a);
    pinPullup(b);
    pinIesHighToLow(a);
    pinIesHighToLow(b);

    // Gather initial values
    if (pinRead(a))
      new_val = 3;
    if (pinRead(b))
      new_val ^= 1;
    last = new_val;
  }

  msp_pin_t     a;
  msp_pin_t     b;
  int8_t        divs;
  int8_t        last;
  int8_t        prev;
  int8_t        val;
};


/*
// Sample initialization and ISR

msp_timerA_t timer; 

int main(void)
{
  // Stuff
  // Should probably check to make sure a timer is actually being allocated
  // here...if we over-allocate there is the potential for undefined behavior
  timer = clock::allocTimer();
  // Keep in mind 'B' doesn't have to have an interrupt
  rot_enc_poll encoder = rot_enc_poll(pX_X, pX_X);
  // Set the CCR for 1ms from now
  set(taccr(timer), (tar(timer) + clock::getMSTicks()));
  // Enable the timer interrupt
  on(tacctl(timer), CCIE);
  // More stuff
}

// Below is simple for case where only one TIMER0_A1 interrupt is enabled. Need
// to add logic to check for the specific interrupt if more than one is enabled.
// NOTE: This is the reason for the global 'timer' variable.
#pragma vector=TIMER0_A1_VECTOR
void __interrupt intSW_ISR(void)
{
  int8_t delta, new_val, diff;

  new_val = 0;
  if (pinRead(encoder.a))
    new_val = 3;
  if (pinRead(encoder.b))
    new_val ^= 1;
  diff = encoder.last - new_val;
  if (diff & 1)
  {
    // new_val value is now the last value
    encoder.last = new_val;
    // Bit 1 provides the direction
    delta += (diff & 2) - 1;
  }

  // Update the encoder value, keeping it within the bounds of the divisions
  // set up
  encoder.val += delta;
  if (encoder.val > (encoder.divs - 1))
    encoder.val = 0;
  else if (encoder.val < 0)
    encoder.val = (encoder.divs - 1);

  if (encoder.val != encoder.prev)
  {
    // DO STUFF
  }

  encoder.prev = encoder.val;

  // Set up the next interrupt -- may differ based on timer configuration
  // This is applicable for continuous mode 1ms intervals
  addeq(taccr(encoder.timer), clock::getMSTicks());
}
*/
