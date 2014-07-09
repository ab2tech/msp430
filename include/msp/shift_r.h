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

class shift_r : public spi
{
public:
  shift_r(spi_usci_t spi_usci, msp_pin_t le,
          msp_pin_t oe = MSP_PIN_SIZE) :
          le(le), spi(spi_usci), sr_data(0), oe(oe) {
    if (oe != MSP_PIN_SIZE)
      pinOutput(oe);
    outputDisable();
    pinOff(le);
    pinOutput(le);
  };

  void        write(uint8_t byte);

  void inline clear(void)         { spi::tx(0); latch(); };
  void inline latch(void)         { pinPulse(le); };
  void inline outputDisable(void) { if (oe != MSP_PIN_SIZE) pinOn (oe); };
  void inline outputEnable(void)  { if (oe != MSP_PIN_SIZE) pinOff(oe); };
  void inline restore(void)       { write(sr_data); };

protected:
  msp_pin_t   oe;

private:
  msp_pin_t   le;
  uint8_t     sr_data;
};

