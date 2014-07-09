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

// This is an MSP430F5510 timer framework. The idea is to simplify the passing
// of timers to allow more dynamic use of them.

#pragma once

#include "msp/ab2.h"
#include <msp430f5510.h>

typedef enum
{
  ta0_0, ta0_1, ta0_2, ta0_3, ta0_4,
  ta1_0, ta1_1, ta1_2,
  ta2_0, ta2_1, ta2_2,
  MSP_TIMERA_SIZE
} msp_timerA_t;

// Base address of each timerA's registers
extern const uint16_t msp_timerA_base[MSP_TIMERA_SIZE];
// CCTL register for each timerA
extern const uint16_t msp_timerA_cctl[MSP_TIMERA_SIZE];
// CCR register for each timerA
extern const uint16_t msp_timerA_ccr[MSP_TIMERA_SIZE];

#define TACTL_OFFSET 0x00
#define tactl(timer)  (REG16(msp_timerA_base[(timer)] + TACTL_OFFSET))
#define TAR_OFFSET   0x10
#define tar(timer)    (REG16(msp_timerA_base[(timer)] + TAR_OFFSET))
#define TAIV_OFFSET  0x2E
#define taiv(timer)   (REG16(msp_timerA_base[(timer)] + TAIV_OFFSET))
#define TAEX_OFFSET  0x20
#define taex(timer)   (REG16(msp_timerA_base[(timer)] + TAEX_OFFSET))

#define tacctl(timer) (REG16(msp_timerA_cctl[(timer)]))
#define taccr(timer)  (REG16(msp_timerA_ccr[(timer)]))
