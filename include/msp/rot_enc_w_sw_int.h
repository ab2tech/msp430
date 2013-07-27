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

// MSP430 Rotory Encoder with Switch (Interrupt-based) library
// Simple Rotory Encoder with Switch (Interrupt-based) library
// for providing maximum re-use and ease of integration

#pragma once
#define AB2_ROT_ENC_W_SW_INT

#include "ab2.h"
// Steal our MSP include from pin_fw to make this a generic library
#include "../pin_fw.h"

// Channel definitions
typedef enum _enc_dir_t
{
  CW = 1,
  CCW = 0,
} enc_dir_t;

// Rotary Encoder w/ Switch class declaration
class rotEncWSwInt
{
public:
  rotEncWSwInt(msp_pin_t a, msp_pin_t b, msp_pin_t sw = MSP_PIN_SIZE,
               uint16_t enc_div = 16)
    : a(a), b(b), sw(sw), enc_div(enc_div){
    //Wiper A and B setup
      // Set to inputs
      pinInput(a);
      pinInput(b);
      // Pull-up resistors selected
      pinRenOn(a);
      pinRenOn(b);
      // Pull-up resistors enabled
      pinOn(a);
      pinOn(b);
      // High to low interrupt detection selection
      pinIesHighToLow(a);
      pinIesHighToLow(b);
      // Interrupts enabled
      pinIeEnable(a);
      pinIeEnable(b);
      // Interrupt flags cleared
      pinIfgClear(a);
      pinIfgClear(b);
      // Enable Global Interrupts

    //Switch setup
      // Set to input
      pinInput(sw);
      // Pull-up resistor selected
      pinRenOn(sw);
      // Pull-up resistor enabled
      pinOn(sw);
      // High to low interrupt detection selection
      pinIesHighToLow(sw);
      // Interrupt enabled
      pinIeEnable(sw);
      // Interrupt flag cleared
      pinIfgClear(sw);

      // Enable Global Interrupt
      _BIS_SR(GIE)
  };
private:
  msp_pin_t           a;
  msp_pin_t           b;
  msp_pin_t           sw;

  uint16_t            enc_div;

  enc_dir_t           enc_dir;
  uint16_t            enc_val;
};
