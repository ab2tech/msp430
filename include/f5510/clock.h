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

// MSP430F5510 Clock Library
// Simplify MSP430F5510 clock initialization and frequency modification

// TODO: Needs additional features added for complete clock module support.
//       The library currently supports basic functionalities, but should be
//       enhanced to provide more complete support of the clock module.

#pragma once

#include <msp430f5510.h>
#include "msp/ab2.h"

// Define the ACLK sources supported by the clock module
typedef enum
{
  ACLK_SRC_REFO
} aclk_src_t;

// Define the DCO frequencies supported by the clock module
typedef enum
{
  DCO_F_1MHz,
  DCO_F_8MHz,
  DCO_F_12MHz,
  DCO_F_25MHz
} dco_freq_t;

void clockInit(void);
void clockSetACLK(aclk_src_t src);
void clockSetDCO(dco_freq_t freq);
void clockUpVcore (uint16_t level);
