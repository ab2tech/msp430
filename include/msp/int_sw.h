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

// MSP430 Interrupt Switch Library

#pragma once
#define AB2_INT_SW

#include "ab2.h"
// Steal our MSP include from pin_fw to make this a generic library
#include "../pin_fw.h"

typedef enum
{
  INT_EDGE_L2H = false,
  INT_EDGE_H2L = true
} int_edge_t;

class int_sw
{
public:
  int_sw(msp_pin_t pin, int_edge_t edge = INT_EDGE_H2L) : pin(pin), edge(edge)
  {
    // For low-to-high detection, we want the pin to have its pulldown enabled
    // For high-to-low detection, we want the pin to have its pullup enabled
    // Use the pinPull API for this to make the source cleaner :)
    pinPull(pin, edge);
    pinIes(pin, edge);

    // Enable the pin's interrupt and clear its interrupt flag
    pinIfgClear(pin);
    pinIeEnable(pin);
  };
private:
  msp_pin_t    pin;
  int_edge_t   edge;
};


/*
// Sample ISR for P1.0
#pragma vector=PORT1_VECTOR
void __interrupt intSW_ISR(void)
{
  // Clear the IFG for P1.0
  // DO STUFF
}

// Sample ISR for P1.0 where other P1 IRQs exist
#pragma vector=PORT1_VECTOR
void __interrupt intSW_ISR(void)
{
  // Check the IFG for P1.0
  // If set
  // |
  //  -> Clear the IFG for P1.0
  //  -> DO STUFF
}
*/
