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

// MSP430 Shift Register Library
#include "shift_r.h"

void shift_r::write(uint8_t byte)
{
  if (oe != MSP_PIN_SIZE)
    outputDisable();
  spi::write(byte);
  latch();
  if (oe != MSP_PIN_SIZE)
    outputEnable();
  sr_data = byte;
}
