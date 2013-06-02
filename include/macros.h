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

// Simplify MSP430 operations with macros

#pragma once

// Pin Macros
#define on(reg, bit) ((reg) |= (bit))
#define off(reg, bit) ((reg) &= ~(bit))
#define toggle(reg, bit) ((reg) ^= (bit))
// Wrap the pulse macro in a do/while loop to enable single line capability
#define pulse(out, bit) do {\
                             on((out), (bit)); \
                             off((out), (bit)); \
                           } while (0);

#define pulseDuration(out, bit, duration) do {\
                             on((out), (bit)); \
                             __delay_cycles((duration)); \
                             off((out), (bit)); \
                           } while (0);

// Bit manipulation macros
// Need this for rotate with carry macros
#define BYTE_SIZE 8
// Rotate left with carry
#define rlc(a) (((a)>>1) | (a)<<((sizeof((a)) * BYTE_SIZE) - 1))
// Rotate right with carry
#define rrc(a) (((a)<<1) | (a)>>((sizeof((a)) * BYTE_SIZE) - 1))
