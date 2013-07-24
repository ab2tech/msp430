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

const uint16_t msp_pin_in[MSP_PIN_SIZE] = {
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

const uint16_t msp_pin_out[MSP_PIN_SIZE] = {
  (uint16_t) &P1OUT, (uint16_t) &P1OUT, (uint16_t) &P1OUT, (uint16_t) &P1OUT,
    (uint16_t) &P1OUT, (uint16_t) &P1OUT, (uint16_t) &P1OUT, (uint16_t) &P1OUT,
#ifdef MSP430F5510_EXT
  (uint16_t) &P2OUT, (uint16_t) &P2OUT, (uint16_t) &P2OUT, (uint16_t) &P2OUT,
    (uint16_t) &P2OUT, (uint16_t) &P2OUT, (uint16_t) &P2OUT, (uint16_t) &P2OUT,
#else
  (uint16_t) &P2OUT,
#endif
#ifdef MSP430F5510_EXT
  (uint16_t) &P3OUT, (uint16_t) &P3OUT, (uint16_t) &P3OUT, (uint16_t) &P3OUT,
    (uint16_t) &P3OUT, (uint16_t) &P3OUT, (uint16_t) &P3OUT, (uint16_t) &P3OUT,
#endif
  (uint16_t) &P4OUT, (uint16_t) &P4OUT, (uint16_t) &P4OUT, (uint16_t) &P4OUT,
    (uint16_t) &P4OUT, (uint16_t) &P4OUT, (uint16_t) &P4OUT, (uint16_t) &P4OUT,
  (uint16_t) &P5OUT, (uint16_t) &P5OUT, (uint16_t) &P5OUT, (uint16_t) &P5OUT,
    (uint16_t) &P5OUT, (uint16_t) &P5OUT,
#ifdef MSP430F5510_EXT
  (uint16_t) &P6OUT, (uint16_t) &P6OUT, (uint16_t) &P6OUT, (uint16_t) &P6OUT,
    (uint16_t) &P6OUT, (uint16_t) &P6OUT, (uint16_t) &P6OUT, (uint16_t) &P6OUT,
#else
  (uint16_t) &P6OUT, (uint16_t) &P6OUT, (uint16_t) &P6OUT, (uint16_t) &P6OUT,
#endif
  (uint16_t) &PJOUT_L, (uint16_t) &PJOUT_L, (uint16_t) &PJOUT_L,
    (uint16_t) &PJOUT_L
};

const uint16_t msp_pin_dir[MSP_PIN_SIZE] = {
  (uint16_t) &P1DIR, (uint16_t) &P1DIR, (uint16_t) &P1DIR, (uint16_t) &P1DIR,
    (uint16_t) &P1DIR, (uint16_t) &P1DIR, (uint16_t) &P1DIR, (uint16_t) &P1DIR,
#ifdef MSP430F5510_EXT
  (uint16_t) &P2DIR, (uint16_t) &P2DIR, (uint16_t) &P2DIR, (uint16_t) &P2DIR,
    (uint16_t) &P2DIR, (uint16_t) &P2DIR, (uint16_t) &P2DIR, (uint16_t) &P2DIR,
#else
  (uint16_t) &P2DIR,
#endif
#ifdef MSP430F5510_EXT
  (uint16_t) &P3DIR, (uint16_t) &P3DIR, (uint16_t) &P3DIR, (uint16_t) &P3DIR,
    (uint16_t) &P3DIR, (uint16_t) &P3DIR, (uint16_t) &P3DIR, (uint16_t) &P3DIR,
#endif
  (uint16_t) &P4DIR, (uint16_t) &P4DIR, (uint16_t) &P4DIR, (uint16_t) &P4DIR,
    (uint16_t) &P4DIR, (uint16_t) &P4DIR, (uint16_t) &P4DIR, (uint16_t) &P4DIR,
  (uint16_t) &P5DIR, (uint16_t) &P5DIR, (uint16_t) &P5DIR, (uint16_t) &P5DIR,
    (uint16_t) &P5DIR, (uint16_t) &P5DIR,
#ifdef MSP430F5510_EXT
  (uint16_t) &P6DIR, (uint16_t) &P6DIR, (uint16_t) &P6DIR, (uint16_t) &P6DIR,
    (uint16_t) &P6DIR, (uint16_t) &P6DIR, (uint16_t) &P6DIR, (uint16_t) &P6DIR,
#else
  (uint16_t) &P6DIR, (uint16_t) &P6DIR, (uint16_t) &P6DIR, (uint16_t) &P6DIR,
#endif
  (uint16_t) &PJDIR_L, (uint16_t) &PJDIR_L, (uint16_t) &PJDIR_L,
    (uint16_t) &PJDIR_L
};

#ifndef DISABLE_PFW_REN
const uint16_t msp_pin_ren[MSP_PIN_SIZE] = {
  (uint16_t) &P1REN, (uint16_t) &P1REN, (uint16_t) &P1REN, (uint16_t) &P1REN,
    (uint16_t) &P1REN, (uint16_t) &P1REN, (uint16_t) &P1REN, (uint16_t) &P1REN,
#ifdef MSP430F5510_EXT
  (uint16_t) &P2REN, (uint16_t) &P2REN, (uint16_t) &P2REN, (uint16_t) &P2REN,
    (uint16_t) &P2REN, (uint16_t) &P2REN, (uint16_t) &P2REN, (uint16_t) &P2REN,
#else
  (uint16_t) &P2REN,
#endif
#ifdef MSP430F5510_EXT
  (uint16_t) &P3REN, (uint16_t) &P3REN, (uint16_t) &P3REN, (uint16_t) &P3REN,
    (uint16_t) &P3REN, (uint16_t) &P3REN, (uint16_t) &P3REN, (uint16_t) &P3REN,
#endif
  (uint16_t) &P4REN, (uint16_t) &P4REN, (uint16_t) &P4REN, (uint16_t) &P4REN,
    (uint16_t) &P4REN, (uint16_t) &P4REN, (uint16_t) &P4REN, (uint16_t) &P4REN,
  (uint16_t) &P5REN, (uint16_t) &P5REN, (uint16_t) &P5REN, (uint16_t) &P5REN,
    (uint16_t) &P5REN, (uint16_t) &P5REN,
#ifdef MSP430F5510_EXT
  (uint16_t) &P6REN, (uint16_t) &P6REN, (uint16_t) &P6REN, (uint16_t) &P6REN,
    (uint16_t) &P6REN, (uint16_t) &P6REN, (uint16_t) &P6REN, (uint16_t) &P6REN,
#else
  (uint16_t) &P6REN, (uint16_t) &P6REN, (uint16_t) &P6REN, (uint16_t) &P6REN,
#endif
  (uint16_t) &PJREN_L, (uint16_t) &PJREN_L, (uint16_t) &PJREN_L,
    (uint16_t) &PJREN_L
};
#endif

#ifndef DISABLE_PFW_DS
const uint16_t msp_pin_ds[MSP_PIN_SIZE] = {
  (uint16_t) &P1DS, (uint16_t) &P1DS, (uint16_t) &P1DS, (uint16_t) &P1DS,
    (uint16_t) &P1DS, (uint16_t) &P1DS, (uint16_t) &P1DS, (uint16_t) &P1DS,
#ifdef MSP430F5510_EXT
  (uint16_t) &P2DS, (uint16_t) &P2DS, (uint16_t) &P2DS, (uint16_t) &P2DS,
    (uint16_t) &P2DS, (uint16_t) &P2DS, (uint16_t) &P2DS, (uint16_t) &P2DS,
#else
  (uint16_t) &P2DS,
#endif
#ifdef MSP430F5510_EXT
  (uint16_t) &P3DS, (uint16_t) &P3DS, (uint16_t) &P3DS, (uint16_t) &P3DS,
    (uint16_t) &P3DS, (uint16_t) &P3DS, (uint16_t) &P3DS, (uint16_t) &P3DS,
#endif
  (uint16_t) &P4DS, (uint16_t) &P4DS, (uint16_t) &P4DS, (uint16_t) &P4DS,
    (uint16_t) &P4DS, (uint16_t) &P4DS, (uint16_t) &P4DS, (uint16_t) &P4DS,
  (uint16_t) &P5DS, (uint16_t) &P5DS, (uint16_t) &P5DS, (uint16_t) &P5DS,
    (uint16_t) &P5DS, (uint16_t) &P5DS,
#ifdef MSP430F5510_EXT
  (uint16_t) &P6DS, (uint16_t) &P6DS, (uint16_t) &P6DS, (uint16_t) &P6DS,
    (uint16_t) &P6DS, (uint16_t) &P6DS, (uint16_t) &P6DS, (uint16_t) &P6DS,
#else
  (uint16_t) &P6DS, (uint16_t) &P6DS, (uint16_t) &P6DS, (uint16_t) &P6DS,
#endif
  (uint16_t) &PJDS_L, (uint16_t) &PJDS_L, (uint16_t) &PJDS_L,
    (uint16_t) &PJDS_L
};
#endif

#ifndef DISABLE_PFW_SEL
const uint16_t msp_pin_sel[SEL_PORT_MAX] = {
  (uint16_t) &P1SEL, (uint16_t) &P1SEL, (uint16_t) &P1SEL, (uint16_t) &P1SEL,
    (uint16_t) &P1SEL, (uint16_t) &P1SEL, (uint16_t) &P1SEL, (uint16_t) &P1SEL,
#ifdef MSP430F5510_EXT
  (uint16_t) &P2SEL, (uint16_t) &P2SEL, (uint16_t) &P2SEL, (uint16_t) &P2SEL,
    (uint16_t) &P2SEL, (uint16_t) &P2SEL, (uint16_t) &P2SEL, (uint16_t) &P2SEL,
#else
  (uint16_t) &P2SEL,
#endif
#ifdef MSP430F5510_EXT
  (uint16_t) &P3SEL, (uint16_t) &P3SEL, (uint16_t) &P3SEL, (uint16_t) &P3SEL,
    (uint16_t) &P3SEL, (uint16_t) &P3SEL, (uint16_t) &P3SEL, (uint16_t) &P3SEL,
#endif
  (uint16_t) &P4SEL, (uint16_t) &P4SEL, (uint16_t) &P4SEL, (uint16_t) &P4SEL,
    (uint16_t) &P4SEL, (uint16_t) &P4SEL, (uint16_t) &P4SEL, (uint16_t) &P4SEL,
  (uint16_t) &P5SEL, (uint16_t) &P5SEL, (uint16_t) &P5SEL, (uint16_t) &P5SEL,
    (uint16_t) &P5SEL, (uint16_t) &P5SEL,
#ifdef MSP430F5510_EXT
  (uint16_t) &P6SEL, (uint16_t) &P6SEL, (uint16_t) &P6SEL, (uint16_t) &P6SEL,
    (uint16_t) &P6SEL, (uint16_t) &P6SEL, (uint16_t) &P6SEL, (uint16_t) &P6SEL
#else
  (uint16_t) &P6SEL, (uint16_t) &P6SEL, (uint16_t) &P6SEL, (uint16_t) &P6SEL
#endif
};
#endif

#ifndef DISABLE_PFW_IES
const uint16_t msp_pin_ies[INT_PORT_MAX] = {
  (uint16_t) &P1IES, (uint16_t) &P1IES, (uint16_t) &P1IES, (uint16_t) &P1IES,
    (uint16_t) &P1IES, (uint16_t) &P1IES, (uint16_t) &P1IES, (uint16_t) &P1IES,
#ifdef MSP430F5510_EXT
  (uint16_t) &P2IES, (uint16_t) &P2IES, (uint16_t) &P2IES, (uint16_t) &P2IES,
    (uint16_t) &P2IES, (uint16_t) &P2IES, (uint16_t) &P2IES, (uint16_t) &P2IES
#else
  (uint16_t) &P2IES
#endif
};
#endif

#ifndef DISABLE_PFW_IE
const uint16_t msp_pin_ie[INT_PORT_MAX] = {
  (uint16_t) &P1IE, (uint16_t) &P1IE, (uint16_t) &P1IE, (uint16_t) &P1IE,
    (uint16_t) &P1IE, (uint16_t) &P1IE, (uint16_t) &P1IE, (uint16_t) &P1IE,
#ifdef MSP430F5510_EXT
  (uint16_t) &P2IE, (uint16_t) &P2IE, (uint16_t) &P2IE, (uint16_t) &P2IE,
    (uint16_t) &P2IE, (uint16_t) &P2IE, (uint16_t) &P2IE, (uint16_t) &P2IE
#else
  (uint16_t) &P2IE
#endif
};
#endif

#ifndef DISABLE_PFW_IFG
const uint16_t msp_pin_ifg[INT_PORT_MAX] = {
  (uint16_t) &P1IFG, (uint16_t) &P1IFG, (uint16_t) &P1IFG, (uint16_t) &P1IFG,
    (uint16_t) &P1IFG, (uint16_t) &P1IFG, (uint16_t) &P1IFG, (uint16_t) &P1IFG,
#ifdef MSP430F5510_EXT
  (uint16_t) &P2IFG, (uint16_t) &P2IFG, (uint16_t) &P2IFG, (uint16_t) &P2IFG,
    (uint16_t) &P2IFG, (uint16_t) &P2IFG, (uint16_t) &P2IFG, (uint16_t) &P2IFG
#else
  (uint16_t) &P2IFG
#endif
};
#endif
