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
#define AB2_PFW

#include "msp/ab2.h"
#include <msp430g2553.h>

// Set the individual port size (bits per port)
#define PORT_SIZE     8
// Define the number of ports on the device
#define NUM_PORTS     3
// Define the number of interrupt ports on the device
#define NUM_INT_PORTS 2

// Define the port offsets
#define PORT1 (PORT_SIZE)
#define PORT2 (PORT_SIZE*2)
#define PORT3 (PORT_SIZE*NUM_PORTS)

// Define the pins as msp_pin_t type
#ifdef MSP430G2553_EXT
// MSP430G2553 28-pin (TSSOP) or 32-pin (QFN)
  typedef enum
  {
    p1_0=(PORT1), p1_1, p1_2, p1_3, p1_4, p1_5, p1_6, p1_7,
    p2_0=(PORT2), p2_1, p2_2, p2_3, p2_4, p2_5, p2_6, p2_7,
    p3_0=(PORT3), p3_1, p3_2, p3_3, p3_4, p3_5, p3_6, p3_7
  } msp_pin_t;
#else
// MSP430G2553 20-pin (TSSOP or PDIP)
  typedef enum
  {
    p1_0=(PORT1), p1_1, p1_2, p1_3, p1_4, p1_5, p1_6, p1_7,
    p2_0=(PORT2), p2_1, p2_2, p2_3, p2_4, p2_5, p2_6, p2_7
  } msp_pin_t;
#endif

extern const    uint8_t  msp_pin_bit[PORT_SIZE];
extern volatile uint8_t *msp_pin_in[NUM_PORTS];
extern volatile uint8_t *msp_pin_out[NUM_PORTS];
extern volatile uint8_t *msp_pin_dir[NUM_PORTS];
extern volatile uint8_t *msp_pin_ren[NUM_PORTS];
extern volatile uint8_t *msp_pin_sel[NUM_PORTS];
extern volatile uint8_t *msp_pin_sel2[NUM_PORTS];
extern volatile uint8_t *msp_pin_ies[NUM_INT_PORTS];
extern volatile uint8_t *msp_pin_ie[NUM_INT_PORTS];
extern volatile uint8_t *msp_pin_ifg[NUM_INT_PORTS];

#define bit(pin) (msp_pin_bit[(pin % PORT_SIZE)])
#define in(pin)  (*msp_pin_in[(pin / PORT_SIZE) - 1])
#define out(pin) (*msp_pin_out[(pin / PORT_SIZE) - 1])
#define dir(pin) (*msp_pin_dir[(pin / PORT_SIZE) - 1])
#ifndef DISABLE_PFW_REN
  #define ren(pin) (*msp_pin_ren[(pin / PORT_SIZE) - 1])
#endif
#ifndef DISABLE_PFW_SEL
  #define sel(pin) (*msp_pin_sel[(pin / PORT_SIZE) - 1])
#endif
#ifndef DISABLE_PFW_SEL2
  #define sel2(pin) (*msp_pin_sel2[(pin / PORT_SIZE) - 1])
#endif
#ifndef DISABLE_PFW_IES
  #define ies(pin) (*msp_pin_ies[(pin / PORT_SIZE) - 1])
#endif
#ifndef DISABLE_PFW_IE
  #define ie(pin)  (*msp_pin_ie[(pin / PORT_SIZE) - 1])
#endif
#ifndef DISABLE_PFW_IFG
  #define ifg(pin) (*msp_pin_ifg[(pin / PORT_SIZE) - 1])
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
#ifndef DISABLE_PFW_SEL
  // pinSelOn - enable the given pin's corresponding sel bit
  #define pinSelOn(pin) on(sel((pin)), bit((pin)))
  // pinSelOff - disable the given pin's corresponding sel bit
  #define pinSelOff(pin) off(sel((pin)), bit((pin)))
#endif
#ifndef DISABLE_PFW_SEL2
  // pinSel2On - enable the given pin's corresponding sel2 bit
  #define pinSel2On(pin) on(sel2((pin)), bit((pin)))
  // pinSel2Off - disable the given pin's corresponding sel2 bit
  #define pinSel2Off(pin) off(sel2((pin)), bit((pin)))
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
