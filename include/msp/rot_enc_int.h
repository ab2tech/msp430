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
#include "msp/ab2.h"
#include "../clock.h"
#include "../pin_fw.h"
#include "../isr_dispatcher.h"

#ifndef ROT_ENC_DIVISIONS
#define ROT_ENC_DIVISIONS 32
#endif

class rot_enc_int
{
public:
  // Construct a rotary encoder object with the A and B pins plus a callback to
  // execute in the ISR.
  rot_enc_int(msp_pin_t a, msp_pin_t b,
      void (*callbackFunc)(rot_enc_int *enc),
      int8_t divs = ROT_ENC_DIVISIONS) :
      a(a), b(b), callbackFunc(callbackFunc),
      divs(divs), val(0), prev(0)
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

    // Install the encoder ISR
    isr_d::install(isr_d::pinVector(a), this, &ISR);
  }

  void   inline clearVal(void)             { val = 0; };
  int8_t inline getDivs(void)              { return divs;  };
  int8_t inline getPrev(void)              { return prev;  };
  int8_t inline getVal(void)               { return val;   };
  void   inline setDivs(uint16_t new_divs) { divs = new_divs; };
  void          update(void);

  // Static wrapper for update to be installed as an ISR
  static isr_t  ISR(void *encoder);

private:
  void        (*callbackFunc)(rot_enc_int *enc);

  msp_pin_t     a;
  msp_pin_t     b;
  int8_t        divs;
  int8_t        prev;
  int8_t        val;
};
