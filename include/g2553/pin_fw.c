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

const uint16_t msp_pin_in[MSP_PIN_SIZE] = {
  (uint16_t) &P1IN, (uint16_t) &P1IN, (uint16_t) &P1IN, (uint16_t) &P1IN,
    (uint16_t) &P1IN, (uint16_t) &P1IN, (uint16_t) &P1IN, (uint16_t) &P1IN,
  (uint16_t) &P2IN, (uint16_t) &P2IN, (uint16_t) &P2IN, (uint16_t) &P2IN,
    (uint16_t) &P2IN, (uint16_t) &P2IN, (uint16_t) &P2IN, (uint16_t) &P2IN
#ifdef MSP430G2553_EXT
  , (uint16_t) &P3IN, (uint16_t) &P3IN, (uint16_t) &P3IN, (uint16_t) &P3IN,
    (uint16_t) &P3IN, (uint16_t) &P3IN, (uint16_t) &P3IN, (uint16_t) &P3IN
#endif
};

const uint16_t msp_pin_out[MSP_PIN_SIZE] = {
  (uint16_t) &P1OUT, (uint16_t) &P1OUT, (uint16_t) &P1OUT, (uint16_t) &P1OUT,
    (uint16_t) &P1OUT, (uint16_t) &P1OUT, (uint16_t) &P1OUT, (uint16_t) &P1OUT,
  (uint16_t) &P2OUT, (uint16_t) &P2OUT, (uint16_t) &P2OUT, (uint16_t) &P2OUT,
    (uint16_t) &P2OUT, (uint16_t) &P2OUT, (uint16_t) &P2OUT, (uint16_t) &P2OUT
#ifdef MSP430G2553_EXT
  , (uint16_t) &P3OUT, (uint16_t) &P3OUT, (uint16_t) &P3OUT, (uint16_t) &P3OUT,
    (uint16_t) &P3OUT, (uint16_t) &P3OUT, (uint16_t) &P3OUT, (uint16_t) &P3OUT
#endif
};

const uint16_t msp_pin_dir[MSP_PIN_SIZE] = {
  (uint16_t) &P1DIR, (uint16_t) &P1DIR, (uint16_t) &P1DIR, (uint16_t) &P1DIR,
    (uint16_t) &P1DIR, (uint16_t) &P1DIR, (uint16_t) &P1DIR, (uint16_t) &P1DIR,
  (uint16_t) &P2DIR, (uint16_t) &P2DIR, (uint16_t) &P2DIR, (uint16_t) &P2DIR,
    (uint16_t) &P2DIR, (uint16_t) &P2DIR, (uint16_t) &P2DIR, (uint16_t) &P2DIR
#ifdef MSP430G2553_EXT
  , (uint16_t) &P3DIR, (uint16_t) &P3DIR, (uint16_t) &P3DIR, (uint16_t) &P3DIR,
    (uint16_t) &P3DIR, (uint16_t) &P3DIR, (uint16_t) &P3DIR, (uint16_t) &P3DIR
#endif
};

#ifndef DISABLE_PFW_REN
const uint16_t msp_pin_ren[MSP_PIN_SIZE] = {
  (uint16_t) &P1REN, (uint16_t) &P1REN, (uint16_t) &P1REN, (uint16_t) &P1REN,
    (uint16_t) &P1REN, (uint16_t) &P1REN, (uint16_t) &P1REN, (uint16_t) &P1REN,
  (uint16_t) &P2REN, (uint16_t) &P2REN, (uint16_t) &P2REN, (uint16_t) &P2REN,
    (uint16_t) &P2REN, (uint16_t) &P2REN, (uint16_t) &P2REN, (uint16_t) &P2REN
#ifdef MSP430G2553_EXT
  , (uint16_t) &P3REN, (uint16_t) &P3REN, (uint16_t) &P3REN, (uint16_t) &P3REN,
    (uint16_t) &P3REN, (uint16_t) &P3REN, (uint16_t) &P3REN, (uint16_t) &P3REN
#endif
};
#endif

#ifndef DISABLE_PFW_SEL
const uint16_t msp_pin_sel[MSP_PIN_SIZE] = {
  (uint16_t) &P1SEL, (uint16_t) &P1SEL, (uint16_t) &P1SEL, (uint16_t) &P1SEL,
    (uint16_t) &P1SEL, (uint16_t) &P1SEL, (uint16_t) &P1SEL, (uint16_t) &P1SEL,
  (uint16_t) &P2SEL, (uint16_t) &P2SEL, (uint16_t) &P2SEL, (uint16_t) &P2SEL,
    (uint16_t) &P2SEL, (uint16_t) &P2SEL, (uint16_t) &P2SEL, (uint16_t) &P2SEL
#ifdef MSP430G2553_EXT
  , (uint16_t) &P3SEL, (uint16_t) &P3SEL, (uint16_t) &P3SEL, (uint16_t) &P3SEL,
    (uint16_t) &P3SEL, (uint16_t) &P3SEL, (uint16_t) &P3SEL, (uint16_t) &P3SEL
#endif
};
#endif

#ifndef DISABLE_PFW_SEL2
const uint16_t msp_pin_sel2[MSP_PIN_SIZE] = {
  (uint16_t) &P1SEL, (uint16_t) &P1SEL, (uint16_t) &P1SEL, (uint16_t) &P1SEL,
    (uint16_t) &P1SEL, (uint16_t) &P1SEL, (uint16_t) &P1SEL, (uint16_t) &P1SEL,
  (uint16_t) &P2SEL, (uint16_t) &P2SEL, (uint16_t) &P2SEL, (uint16_t) &P2SEL,
    (uint16_t) &P2SEL, (uint16_t) &P2SEL, (uint16_t) &P2SEL, (uint16_t) &P2SEL
#ifdef MSP430G2553_EXT
  , (uint16_t) &P3SEL, (uint16_t) &P3SEL, (uint16_t) &P3SEL, (uint16_t) &P3SEL,
    (uint16_t) &P3SEL, (uint16_t) &P3SEL, (uint16_t) &P3SEL, (uint16_t) &P3SEL
#endif
};
#endif

#ifndef DISABLE_PFW_IES
const uint16_t msp_pin_ies[INT_PORT_MAX] = {
  (uint16_t) &P1IES, (uint16_t) &P1IES, (uint16_t) &P1IES, (uint16_t) &P1IES,
    (uint16_t) &P1IES, (uint16_t) &P1IES, (uint16_t) &P1IES, (uint16_t) &P1IES,
  (uint16_t) &P2IES, (uint16_t) &P2IES, (uint16_t) &P2IES, (uint16_t) &P2IES,
    (uint16_t) &P2IES, (uint16_t) &P2IES, (uint16_t) &P2IES, (uint16_t) &P2IES
};
#endif

#ifndef DISABLE_PFW_IE
const uint16_t msp_pin_ie[INT_PORT_MAX] = {
  (uint16_t) &P1IE, (uint16_t) &P1IE, (uint16_t) &P1IE, (uint16_t) &P1IE,
    (uint16_t) &P1IE, (uint16_t) &P1IE, (uint16_t) &P1IE, (uint16_t) &P1IE,
  (uint16_t) &P2IE, (uint16_t) &P2IE, (uint16_t) &P2IE, (uint16_t) &P2IE,
    (uint16_t) &P2IE, (uint16_t) &P2IE, (uint16_t) &P2IE, (uint16_t) &P2IE
};
#endif

#ifndef DISABLE_PFW_IFG
const uint16_t msp_pin_ifg[INT_PORT_MAX] = {
  (uint16_t) &P1IFG, (uint16_t) &P1IFG, (uint16_t) &P1IFG, (uint16_t) &P1IFG,
    (uint16_t) &P1IFG, (uint16_t) &P1IFG, (uint16_t) &P1IFG, (uint16_t) &P1IFG,
  (uint16_t) &P2IFG, (uint16_t) &P2IFG, (uint16_t) &P2IFG, (uint16_t) &P2IFG,
    (uint16_t) &P2IFG, (uint16_t) &P2IFG, (uint16_t) &P2IFG, (uint16_t) &P2IFG
};
#endif
