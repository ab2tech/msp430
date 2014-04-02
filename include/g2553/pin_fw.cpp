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

#include "pin_fw.h"

const uint8_t msp_pin_bit[MSP_PIN_SIZE] = {
  // PORT1
  BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7,
  // PORT2
  BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7
  // PORT3
#ifdef MSP430G2553_EXT
  , BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7
#endif
};

const uint16_t msp_pin_base[MSP_PIN_SIZE] = {
  (uint16_t) &P1IN, (uint16_t) &P1IN, (uint16_t) &P1IN, (uint16_t) &P1IN,
    (uint16_t) &P1IN, (uint16_t) &P1IN, (uint16_t) &P1IN, (uint16_t) &P1IN,
  (uint16_t) &P2IN, (uint16_t) &P2IN, (uint16_t) &P2IN, (uint16_t) &P2IN,
    (uint16_t) &P2IN, (uint16_t) &P2IN, (uint16_t) &P2IN, (uint16_t) &P2IN
#ifdef MSP430G2553_EXT
  , (uint16_t) &P3IN, (uint16_t) &P3IN, (uint16_t) &P3IN, (uint16_t) &P3IN,
    (uint16_t) &P3IN, (uint16_t) &P3IN, (uint16_t) &P3IN, (uint16_t) &P3IN
#endif
};

#ifndef DISABLE_PFW_SEL2
const uint16_t msp_pin_sel2[MSP_PIN_SIZE] = {
  (uint16_t) &P1SEL2, (uint16_t) &P1SEL2, (uint16_t) &P1SEL2, (uint16_t) &P1SEL2,
    (uint16_t) &P1SEL2, (uint16_t) &P1SEL2, (uint16_t) &P1SEL2, (uint16_t) &P1SEL2,
  (uint16_t) &P2SEL2, (uint16_t) &P2SEL2, (uint16_t) &P2SEL2, (uint16_t) &P2SEL2,
    (uint16_t) &P2SEL2, (uint16_t) &P2SEL2, (uint16_t) &P2SEL2, (uint16_t) &P2SEL2
#ifdef MSP430G2553_EXT
  , (uint16_t) &P3SEL2, (uint16_t) &P3SEL2, (uint16_t) &P3SEL2, (uint16_t) &P3SEL2,
    (uint16_t) &P3SEL2, (uint16_t) &P3SEL2, (uint16_t) &P3SEL2, (uint16_t) &P3SEL2
#endif
};
#endif

