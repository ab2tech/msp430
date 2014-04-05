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

// MSP430 Interrupt-based Rotary Encoder Library

#pragma once
#define AB2_ROT_ENC_INT
#include "ab2.h"
#include "../clock.h"
#include "../pin_fw.h"

#ifndef ROT_ENC_DIVISIONS
#define ROT_ENC_DIVISIONS 32
#endif

class rot_enc_int
{
public:
  // Construct a rotary encoder object with the A and B pins
  rot_enc_int(msp_pin_t a, msp_pin_t b, int8_t divs = ROT_ENC_DIVISIONS) :
      a(a), b(b), divs(divs), val(0), prev(0)
  {
    // Configure the rotary encoder pin pullups
    pinPullup(a);
    pinPullup(b);
    // Configure the IES bit for A as high-to-low
    pinIesHighToLow(a);
    // Enable the pin A interrupt
    pinIeEnable(a);
    // Clear the pin A interrupt flag
    pinIfgClear(a);
  }

  void      inline clearVal(void)             { val = 0; };

  // We could add getters/setters for each of these, but it doesn't really make
  // sense in the case where there is no protection needed. Thus, just make
  // these public to make life easier in the ISR.
  msp_pin_t     a;
  msp_pin_t     b;
  int8_t        divs;
  int8_t        prev;
  int8_t        val;
};


/*
// Sample initialization and ISR for P1.0

int main(void)
{
  // Stuff
  // Keep in mind 'B' doesn't have to have an interrupt
  rot_enc_int encoder = rot_enc_int(p1_0, pX_X);
  // More stuff
}

// Below is simple for case where only one PORT1 interrupt is enabled. Need to
// add logic to check the IFG if more than one PORT1 interrupt is in use.
#pragma vector=PORT1_VECTOR
void __interrupt intSW_ISR(void)
{
  if (!pinRead(encoder.b)) // B is low -> CW
    encoder.val += 1;
  else // B is high -> CCW
    encoder.val -= 1;

  if (encoder.val > (encoder.divs - 1))
    encoder.val = 0;
  else if (encoder.val < 0)
    encoder.val = (encoder.divs - 1);

  if (encoder.val != prev)
  {
    // DO STUFF
  }

  // Save the previous value
  encoder.prev = encoder.val;
}
*/
