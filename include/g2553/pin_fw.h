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
  typedef enum
  {
    p1_0, p1_1, p1_2, p1_3, p1_4, p1_5, p1_6, p1_7,
    p2_0, p2_1, p2_2, p2_3, p2_4, p2_5, p2_6, p2_7,
    p3_0, p3_1, p3_2, p3_3, p3_4, p3_5, p3_6, p3_7,
    MSP_PIN_SIZE
  } msp_pin_t;

#else
// MSP430G2553 20-pin (TSSOP or PDIP)
  typedef enum
  {
    p1_0, p1_1, p1_2, p1_3, p1_4, p1_5, p1_6, p1_7,
    p2_0, p2_1, p2_2, p2_3, p2_4, p2_5, p2_6, p2_7,
    MSP_PIN_SIZE
  } msp_pin_t;
#endif

// Define the maximum pin for each port
#define PORT1 p1_7
#define PORT2 p2_7
#ifdef MSP430G2553_EXT
#define PORT3 p3_7
#endif

#define INT_PORT_MAX (PORT2+1)

// MSP pin bit and base address lookup table definitions
extern const uint8_t  msp_pin_bit[MSP_PIN_SIZE];
extern const uint16_t msp_pin_base[MSP_PIN_SIZE];

// Pin register access macros

// NOTE: It would be good to restrict other files from using these macros.
// However, that isn't possible while maintaining the 'inline' status of the
// pin operation functions (they must be defined here in the header file, can't
// be moved to the cpp file). Since the performance implications of not having
// the pin operation functions inlined are tremendous, the need for pin
// operation functions to be inlined trumps the need for these macros to be
// inaccessible to other files.

// WARNING: Users should observe the restriction that these macros only be used
// in other files if absolutely necessary to avoid potential issues that can
// occur from improper usage of the macros
#define bit(pin) (msp_pin_bit[(pin)])
#define PIN_OFFSET  0x00
#define in(pin)  (REG8(msp_pin_base[(pin)] + PIN_OFFSET))
#define POUT_OFFSET 0x01
#define out(pin) (REG8(msp_pin_base[(pin)] + POUT_OFFSET))
#define PDIR_OFFSET 0x02
#define dir(pin) (REG8(msp_pin_base[(pin)] + PDIR_OFFSET))
#ifndef DISABLE_PFW_REN
#define PREN_OFFSET 0x07
#define ren(pin) (REG8(msp_pin_base[(pin)] + PREN_OFFSET))
#endif
#ifndef DISABLE_PFW_SEL
#define PSEL_OFFSET 0x06
#define sel(pin) (REG8(msp_pin_base[(pin)] + PSEL_OFFSET))
#endif
#ifndef DISABLE_PFW_SEL2
// These can't be used with the offset method because the offset is
// arbitrary relative to the base address of the module. Calculating the
// SEL2 address would be costly from a performance perspective, so just
// store all these addresses in flash instead.
#define sel2(pin) (REG8(msp_pin_sel2[(pin)])
#endif
#ifndef DISABLE_PFW_IES
#define PIES_OFFSET 0x04
#define ies(pin) (REG8(msp_pin_base[(pin)] + PIES_OFFSET))
#endif
#ifndef DISABLE_PFW_IE
#define PIE_OFFSET  0x05
#define ie(pin)  (REG8(msp_pin_base[(pin)] + PIE_OFFSET))
#endif
#ifndef DISABLE_PFW_IFG
#define PIFG_OFFSET 0x03
#define ifg(pin) (REG8(msp_pin_base[(pin)] + PIFG_OFFSET))
#endif
// END pin register access macros

// Pin operation function definitions
//
// pinRead - reads a given pin's in bit
bool inline pinRead(msp_pin_t pin) { return (bool) read(in(pin), bit(pin)); }

// pinOn - turns a given pin's corresponding out bit on
void inline pinOn(msp_pin_t pin) { on(out(pin), bit(pin)); }
// pinOff - turns a given pin's corresponding out bit off
void inline pinOff(msp_pin_t pin) { off(out(pin), bit(pin)); }
// pinToggle - toggles a given pin's corresponding out bit
void inline pinToggle(msp_pin_t pin) { toggle(out(pin), bit(pin)); }
// pinPulse - pulses a given pin's corresponding out bit (turns it on, then
//            turns it off in sequence)
void inline pinPulse(msp_pin_t pin) { pulse(out(pin), bit(pin)); }
// pinPulseDuration - pulses a given pin's corresponding out bit for the
//                    specified duration
#define pinPulseDuration(pin, duration) \
  pulseDuration(out(pin), bit(pin), duration)

// pinOutput - makes the given pin an output
void inline pinOutput(msp_pin_t pin) { on(dir(pin), bit(pin)); }
// pinInput - makes the given pin an input
void inline pinInput(msp_pin_t pin) { off(dir(pin), bit(pin)); }

#ifndef DISABLE_PFW_REN
// pinRenOn - enables the REN bit for the given pin
void inline pinRenOn(msp_pin_t pin) { on(ren(pin), bit(pin)); }
// pinRenOff - disables the REN bit for the given pin
void inline pinRenOff(msp_pin_t pin) { off(ren(pin), bit(pin)); }
// pinPullup - enables the pin's pullup resistor
void inline pinPullup(msp_pin_t pin) {
  pinInput(pin); pinRenOn(pin); pinOn(pin); }
// pinPulldown - enables the pin's pulldown resistor
void inline pinPulldown(msp_pin_t pin) {
  pinInput(pin); pinRenOn(pin); pinOff(pin); }
void inline pinPull(msp_pin_t pin, bool up) {
  if (up) pinPullup(pin);
  else pinPulldown(pin); }
#endif
#ifndef DISABLE_PFW_SEL
// pinSelOn - enable the given pin's corresponding sel bit
void inline pinSelOn(msp_pin_t pin) { on(sel(pin), bit(pin)); }
// pinSelOff - disable the given pin's corresponding sel bit
void inline pinSelOff(msp_pin_t pin) { off(sel(pin), bit(pin)); }
#endif
#ifndef DISABLE_PFW_SEL2
// pinSelOn - enable the given pin's corresponding sel bit
void inline pinSel2On(msp_pin_t pin) { on(sel2(pin), bit(pin)); }
// pinSelOff - disable the given pin's corresponding sel bit
void inline pinSel2Off(msp_pin_t pin) { off(sel2(pin), bit(pin)); }
#endif
#ifndef DISABLE_PFW_IES
// pinIesHighToLow - make the interrupt edge high-to-low
// WARNING: Does nothing for pins that don't have IES
void inline pinIesHighToLow(msp_pin_t pin) {
  if (pin < INT_PORT_MAX) on(ies(pin), bit(pin)); }
// pinIesLowToHigh - make the interrupt edge low-to-high
// WARNING: Does nothing for pins that don't have IES
void inline pinIesLowToHigh(msp_pin_t pin) {
  if (pin < INT_PORT_MAX) off(ies(pin), bit(pin)); }
// pinIes - change the interrupt edge as specified
// WARNING: Does nothing for pins that don't have IES
void inline pinIes(msp_pin_t pin, bool edge) {
  if (edge) pinIesHighToLow(pin);
  else pinIesLowToHigh(pin); }
#endif
#ifndef DISABLE_PFW_IE
// pinIeEnable - enable the given pin's interrupt
// WARNING: Does nothing for pins that don't have IE
void inline pinIeEnable(msp_pin_t pin) {
  if (pin < INT_PORT_MAX) on(ie(pin), bit(pin)); }
// pinIeDisable - disable the given pin's interrupt
// WARNING: Does nothing for pins that don't have IE
void inline pinIeDisable(msp_pin_t pin) {
  if (pin < INT_PORT_MAX) off(ie(pin), bit(pin)); }
#endif
#ifndef DISABLE_PFW_IFG
// pinIfgRead - read the pin's interrupt flag
// WARNING: Does nothing for pins that don't have IFG
bool inline pinIfgRead(msp_pin_t pin) {
  if (pin < INT_PORT_MAX) return (bool) read(ifg(pin), bit(pin));
  else return false; }
// pinIfgClear - clear the given pin's interrupt flag
// WARNING: Does nothing for pins that don't have IFG
void inline pinIfgClear(msp_pin_t pin) {
  if (pin < INT_PORT_MAX) off(ifg(pin), bit(pin)); }
#endif
// END pin operation functions
