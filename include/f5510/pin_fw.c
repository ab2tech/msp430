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

const uint8_t msp_pin_bit[PORT_SIZE] = {
  BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7
};
volatile uint8_t *msp_pin_in[NUM_PORTS] = {
  &P1IN, &P2IN, &P3IN, &P4IN, &P5IN, &P6IN, &PJIN_L
};
volatile uint8_t *msp_pin_out[NUM_PORTS] = {
  &P1OUT, &P2OUT, &P3OUT, &P4OUT, &P5OUT, &P6OUT, &PJOUT_L
};
volatile uint8_t *msp_pin_dir[NUM_PORTS] = {
  &P1DIR, &P2DIR, &P3DIR, &P4DIR, &P5DIR, &P6DIR, &PJDIR_L
};
#ifndef DISABLE_PFW_REN
  volatile uint8_t *msp_pin_ren[NUM_PORTS] = {
    &P1REN, &P2REN, &P3REN, &P4REN, &P5REN, &P6REN, &PJREN_L
  };
#endif
#ifndef DISABLE_PFW_DS
volatile uint8_t *msp_pin_ds[NUM_PORTS] = {
  &P1DS, &P2DS, &P3DS, &P4DS, &P5DS, &P6DS, &PJDS_L
};
#endif
#ifndef DISABLE_PFW_SEL
volatile uint8_t *msp_pin_sel[NUM_SEL_PORTS] = {
  &P1SEL, &P2SEL, &P3SEL, &P4SEL, &P5SEL, &P6SEL

};
#endif
#ifndef DISABLE_PFW_IES
volatile uint8_t *msp_pin_ies[NUM_INT_PORTS] = {
  &P1IES, &P2IES
};
#endif
#ifndef DISABLE_PFW_IE
volatile uint8_t *msp_pin_ie[NUM_INT_PORTS] = {
  &P1IE, &P2IE
};
#endif
#ifndef DISABLE_PFW_IFG
volatile uint8_t *msp_pin_ifg[NUM_INT_PORTS] = {
  &P1IFG, &P2IFG
};
#endif
