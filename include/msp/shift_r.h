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

#pragma once
#define AB2_SHIFT_R

#include "ab2.h"
// Steal our MSP include from pin_fw to make this a generic library
#include "../pin_fw.h"
#include "../spi.h"
#include "../clock.h"

class shift_r : public spi
{
public:
  shift_r(spi_usci_t spi_usci, msp_pin_t latch_pin) :
    latch_pin(latch_pin), spi(spi_usci) {
    pinOutput(latch_pin);
  };
  void latch(void);
  using spi::write;
  void write(uint8_t byte);
  using spi::writeFrame;
  void writeFrame(uint8_t *buf, uint16_t size);
private:
  msp_pin_t latch_pin;
};

