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

// This is an MSP430G2553 pin framework. The idea is to simplify the passing of
// ports to allow more dynamic use of them.

#pragma once

#include "msp/ab2.h"
#include <msp430g2553.h>

// Define the pins as msp_pin_t type
#ifdef MSP430G2553_EXT
// MSP430G2553 28-pin (TSSOP) or 32-pin (QFN)
  typedef const enum
  {
    p1_0, p1_1, p1_2, p1_3, p1_4, p1_5, p1_6, p1_7,
    p2_0, p2_1, p2_2, p2_3, p2_4, p2_5, p2_6, p2_7,
    p3_0, p3_1, p3_2, p3_3, p3_4, p3_5, p3_6, p3_7,
    MSP_PIN_SIZE
  } msp_pin_t;

#else
// MSP430G2553 20-pin (TSSOP or PDIP)
  typedef const enum
  {
    p1_0, p1_1, p1_2, p1_3, p1_4, p1_5, p1_6, p1_7,
    p2_0, p2_1, p2_2, p2_3, p2_4, p2_5, p2_6, p2_7,
    MSP_PIN_SIZE
  } msp_pin_t;
#endif

#define PORT1 p1_7
#define PORT2 p2_7
#ifdef MSP430G2553_EXT
#define PORT3 p3_7
#endif

#define INT_PORT_MAX (PORT2+1)

extern const uint8_t  msp_pin_bit[MSP_PIN_SIZE];
extern const uint16_t msp_pin_in[MSP_PIN_SIZE];
extern const uint16_t msp_pin_out[MSP_PIN_SIZE];
extern const uint16_t msp_pin_dir[MSP_PIN_SIZE];
extern const uint16_t msp_pin_ren[MSP_PIN_SIZE];
extern const uint16_t msp_pin_ds[MSP_PIN_SIZE];
extern const uint16_t msp_pin_sel[MSP_PIN_SIZE];
extern const uint16_t msp_pin_sel2[MSP_PIN_SIZE];
extern const uint16_t msp_pin_ies[INT_PORT_MAX];
extern const uint16_t msp_pin_ie[INT_PORT_MAX];
extern const uint16_t msp_pin_ifg[INT_PORT_MAX];

#define bit(pin) (msp_pin_bit[(pin)])
#define in(pin)  (*(volatile uint8_t *) msp_pin_in[(pin)])
#define out(pin) (*(volatile uint8_t *) msp_pin_out[(pin)])
#define dir(pin) (*(volatile uint8_t *) msp_pin_dir[(pin)])
#ifndef DISABLE_PFW_REN
#define ren(pin) (*(volatile uint8_t *) msp_pin_ren[(pin)])
#endif
#ifndef DISABLE_PFW_DS
#define ds(pin)  (*(volatile uint8_t *) msp_pin_ds[(pin)])
#endif
#ifndef DISABLE_PFW_SEL
#define sel(pin) (*(volatile uint8_t *) msp_pin_sel[(pin)])
#endif
#ifndef DISABLE_PFW_SEL2
#define sel2(pin) (*(volatile uint8_t *) msp_pin_sel2[(pin)])
#endif
#ifndef DISABLE_PFW_IES
#define ies(pin) (*(volatile uint8_t *) msp_pin_ies[(pin)])
#endif
#ifndef DISABLE_PFW_IE
#define ie(pin)  (*(volatile uint8_t *) msp_pin_ie[(pin)])
#endif
#ifndef DISABLE_PFW_IFG
#define ifg(pin) (*(volatile uint8_t *) msp_pin_ifg[(pin)])
#endif

// Pin Macros
//
// pinOn - turns a given pin's corresponding dir bit on
#define pinOn(pin) on(out((pin)), bit((pin)))
// pinOff - turns a given pin's corresponding dir bit off
#define pinOff(pin) off(out((pin)), bit((pin)))
// pinToggle - toggles a given pin's corresponding out bit
#define pinToggle(pin) toggle(out((pin)), bit((pin)))
// pinPulse - pulses a given pin's corresponding out bit (turns it on, then
//            turns it off in sequence)
#define pinPulse(pin) pulse(out((pin)), bit((pin)))
// pinPulseDuration - pulses a given pin's corresponding out bit for the
//                    specified duration
#define pinPulseDuration(pin, duration) \
  pulseDuration(out((pin)), bit((pin)), (duration))

// pinOutput - makes the given pin an output
#define pinOutput(pin) on(dir((pin)), bit((pin)))
// pinInput - makes the given pin an input
#define pinInput(pin) off(dir((pin)), bit((pin)))

#ifndef DISABLE_PFW_REN
// pinRenOn - enables the pullup resistor for the given pin
#define pinRenOn(pin) on(ren((pin)), bit((pin)))
// pinRenOff - disables the pullup resistor for the given pin
#define pinRenOff(pin) off(ren((pin)), bit((pin)))
#endif
#ifndef DISABLE_PFW_DS
// pinDsFull - drive the given pin at full strength
#define pinDsFull(pin) on(ds((pin)), bit((pin))
// pinDsReduced - drive the given pin at reduced strength
#define pinDsReduced(pin) off(ds((pin)), bit((pin))
#endif
#ifndef DISABLE_PFW_SEL
// pinSelOn - enable the given pin's corresponding sel bit
#define pinSelOn(pin) on(sel((pin)), bit((pin)))
// pinSelOff - disable the given pin's corresponding sel bit
#define pinSelOff(pin) off(sel((pin)), bit((pin)))
#endif
#ifndef DISABLE_PFW_SEL2
// pinSelOn - enable the given pin's corresponding sel bit
#define pinSel2On(pin) on(sel((pin)), bit((pin)))
// pinSelOff - disable the given pin's corresponding sel bit
#define pinSel2Off(pin) off(sel((pin)), bit((pin)))
#endif
#ifndef DISABLE_PFW_IES
// pinIesHighToLow - make the interrupt edge high-to-low
#define pinIesHighToLow(pin) on(ies((pin)), bit((pin)))
// pinIesLowToHigh - make the interrupt edge low-to-high
#define pinIesLowToHigh(pin) off(ies((pin)), bit((pin)))
#endif
#ifndef DISABLE_PFW_IE
// pinIeEnable - enable the given pin's interrupt
#define pinIeEnable(pin) on(ie((pin)), bit((pin)))
// pinIeDisable - disable the given pin's interrupt
#define pinIeDisable(pin) off(ie((pin)), bit((pin)))
#endif
#ifndef DISABLE_PFS_IFG
// pinIfgClear - clear the given pin's interrupt flag
#define pinIfgClear(pin) off(ifg((pin)), bit((pin)))
#endif
// END Pin Macros