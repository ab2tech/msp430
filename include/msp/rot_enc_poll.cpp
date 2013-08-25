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

#include "rot_enc_poll.h"

isr_t rot_enc_poll::ISR(void *encoder)
{
  ((rot_enc_poll *)(encoder))->update();
}

void rot_enc_poll::init(void)
{
  int8_t new_val = 0;

  // Allocate a free timer from the clock library unless one was specified at
  // encoder initialization
  if (timer == MSP_TIMERA_SIZE)
  {
    // Should probably check to make sure a timer is actually being allocated
    // here...if we over-allocate there is the potential for undefined behavior
    timer = clock::allocTimer();
  }
  // Install the encoder ISR for the specified timer
  isr_d::install(isr_d::taVector(timer), this, &ISR);

  // Gather initial values
  if (pinRead(a))
    new_val = 3;
  if (pinRead(b))
    new_val ^= 1;
  last = new_val;
  delta = 0;

  // Configure the timer
  // Set the CCR for 1ms from now
  set(taccr(timer), (tar(timer) + clock::getMSTicks()));
  // Enable the timer interrupt
  on(tacctl(timer), CCIE);
  //// Configure the timer in up mode
  //set(tactl(timer), (TASSEL__SMCLK | MC__UP | TACLR));
  //// Enable the timer interrupt
  //on (tacctl(timer), CCIE);
  //// Set the CCR for 1ms
  //set(taccr(timer), clock::getMSTicks());

  val = 1;
}

void rot_enc_poll::update(void)
{
  int8_t new_val, diff;

  new_val = 0;
  if (pinRead(a))
    new_val = 3;
  if (pinRead(b))
    new_val ^= 1;
  diff = last - new_val;
  if (diff & 1)
  {
    // new_val value is now the last value
    last = new_val;
    // Bit 1 provides the direction
    delta += (diff & 2) - 1;
  }

  // Update the encoder value, keeping it within the bounds of the divisions
  // set up
  val += delta;
  if (val > (divs - 1))
    val = 0;
  else if (val < 0)
    val = (divs - 1);

  if (val != prev)
    // Invoke the callback function to do something with our encoder data
    (*callbackFunc)(this);

  // Reset the encoder delta
  delta = 0;
  prev = val;

  // Set up the next interrupt
  addeq(taccr(timer), clock::getMSTicks());
}
