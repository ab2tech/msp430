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

// Store the respective bit for each MSP pin
const uint8_t msp_pin_bit[MSP_PIN_SIZE] = {
  // PORT1
  BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7,
  // PORT2
#ifdef MSP430F5510_EXT
  BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7,
#else
  BIT0,
#endif
  // PORT3
#ifdef MSP430F5510_EXT
  BIT0, BIT1, BIT2, BIT3, BIT4
#endif
  // PORT4
  BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7,
  // PORT5
  BIT0, BIT1, BIT2, BIT3, BIT4, BIT5,
  // PORT6
#ifdef MSP430F5510_EXT
  BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7,
#else
  BIT0, BIT1, BIT2, BIT3,
#endif
  // PORTJ
  BIT0, BIT1, BIT2, BIT3
};

// Store the respective base address for each MSP pin
const uint16_t msp_pin_base[MSP_PIN_SIZE] = {
  (uint16_t) &P1IN, (uint16_t) &P1IN, (uint16_t) &P1IN, (uint16_t) &P1IN,
    (uint16_t) &P1IN, (uint16_t) &P1IN, (uint16_t) &P1IN, (uint16_t) &P1IN,
#ifdef MSP430F5510_EXT
  (uint16_t) &P2IN, (uint16_t) &P2IN, (uint16_t) &P2IN, (uint16_t) &P2IN,
    (uint16_t) &P2IN, (uint16_t) &P2IN, (uint16_t) &P2IN, (uint16_t) &P2IN,
#else
  (uint16_t) &P2IN,
#endif
#ifdef MSP430F5510_EXT
  (uint16_t) &P3IN, (uint16_t) &P3IN, (uint16_t) &P3IN, (uint16_t) &P3IN,
    (uint16_t) &P3IN, (uint16_t) &P3IN, (uint16_t) &P3IN, (uint16_t) &P3IN,
#endif
  (uint16_t) &P4IN, (uint16_t) &P4IN, (uint16_t) &P4IN, (uint16_t) &P4IN,
    (uint16_t) &P4IN, (uint16_t) &P4IN, (uint16_t) &P4IN, (uint16_t) &P4IN,
  (uint16_t) &P5IN, (uint16_t) &P5IN, (uint16_t) &P5IN, (uint16_t) &P5IN,
    (uint16_t) &P5IN, (uint16_t) &P5IN,
#ifdef MSP430F5510_EXT
  (uint16_t) &P6IN, (uint16_t) &P6IN, (uint16_t) &P6IN, (uint16_t) &P6IN,
    (uint16_t) &P6IN, (uint16_t) &P6IN, (uint16_t) &P6IN, (uint16_t) &P6IN,
#else
  (uint16_t) &P6IN, (uint16_t) &P6IN, (uint16_t) &P6IN, (uint16_t) &P6IN,
#endif
  (uint16_t) &PJIN_L, (uint16_t) &PJIN_L, (uint16_t) &PJIN_L,
    (uint16_t) &PJIN_L
};
