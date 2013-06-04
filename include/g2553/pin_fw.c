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
  &P1IN, &P2IN, &P3IN
};
volatile uint8_t *msp_pin_out[NUM_PORTS] = {
  &P1OUT, &P2OUT, &P3OUT
};
volatile uint8_t *msp_pin_dir[NUM_PORTS] = {
  &P1DIR, &P2DIR, &P3DIR
};
#ifndef DISABLE_PFW_REN
  volatile uint8_t *msp_pin_ren[NUM_PORTS] = {
    &P1REN, &P2REN, &P3REN
  };
#endif
#ifndef DISABLE_PFW_SEL
volatile uint8_t *msp_pin_sel[NUM_PORTS] = {
  &P1SEL, &P2SEL, &P3SEL
};
#endif
#ifndef DISABLE_PFW_SEL2
volatile uint8_t *msp_pin_sel2[NUM_PORTS] = {
  &P1SEL2, &P2SEL2, &P3SEL2
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
