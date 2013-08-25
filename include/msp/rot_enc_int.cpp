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

#include "rot_enc_int.h"

isr_t rot_enc_int::ISR(void *encoder)
{
  ((rot_enc_int *)(encoder))->update();
  pinIfgClear(((rot_enc_int *)(encoder))->a);
}

void rot_enc_int::update(void)
{
  if (!pinRead(b)) // B is low -> CW
    val += 1;
  else // B is high -> CCW
    val -= 1;

  if (val > (divs - 1))
    val = 0;
  else if (val < 0)
    val = (divs - 1);

  if (val != prev)
  // Invoke the callback function to do something with our encoder data
    (*callbackFunc)(this);

  // Save the previous value
  prev = val;
}
