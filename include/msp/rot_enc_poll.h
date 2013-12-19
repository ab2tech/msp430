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
#include "../isr_dispatcher.h"

#ifndef ROT_ENC_DIVISIONS
#define ROT_ENC_DIVISIONS 32
#endif

class rot_enc_poll
{
public:
  // Construct a rotary encoder object with the A and B pins plus a callback to
  // execute in the ISR and a timerA0. If a timer is provided in the
  // constructor, that timer is assumed to be already configured with SMCLK as
  // the source (need clock's MSTicks value to be accurate) and in continuous
  // mode.
  rot_enc_poll(msp_pin_t a, msp_pin_t b,
      void (*callbackFunc)(rot_enc_poll *enc),
      int8_t divs = ROT_ENC_DIVISIONS,
      msp_timerA_t timer = MSP_TIMERA_SIZE) :
      a(a), b(b), callbackFunc(callbackFunc), timer(timer),
      divs(divs), delta(0), last(0), val(0)
  {
    // Configure the rotary encoder pin pullups
    pinPullup(a);
    pinPullup(b);
    pinIesHighToLow(a);
    pinIesHighToLow(b);

    // Initialize
    init();
  }

  void   inline clearVal(void)             { val = 0; };
  int8_t inline getDelta(void)             { return delta; };
  int8_t inline getDivs(void)              { return divs;  };
  int8_t inline getLast(void)              { return last;  };
  int8_t inline getPrev(void)              { return prev;  };
  int8_t inline getVal(void)               { return val;   };
  void   inline setDivs(uint16_t new_divs) { divs = new_divs; };
  void          init(void);
  void          update(void);

  // Static wrapper for updateEncoder to be installed as an ISR
  static isr_t  ISR(void *encoder);

private:
  void        (*callbackFunc)(rot_enc_poll *enc);

  msp_pin_t     a;
  msp_pin_t     b;
  int8_t        delta;
  int8_t        divs;
  int8_t        last;
  int8_t        prev;
  msp_timerA_t  timer;
  int8_t        val;
};
