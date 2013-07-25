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
#include <stdint.h>

// HW Register Access
#define REG32(x) \
        (*((volatile uint32_t *)((uint16_t)(x))))
#define REG16(x) \
        (*((volatile uint16_t *)((uint16_t)(x))))
#define REG8(x)  \
        (*((volatile uint8_t *)((uint16_t)(x))))
		
// Register Macros
#define andeq(reg, bits)  ((reg) &= (bits))
#define eq(reg, bits)     ((reg)  = (bits))
#define lsheq(reg, bits)  ((reg) <<= (bits))
#define oreq(reg, bits)   ((reg) |= (bits))
#define rsheq(reg, bits)  ((reg) >>= (bits))
#define xoreq(reg, bits)  ((reg) ^= (bits))

#define on(reg, bits)     (oreq((reg), (bits)))
#define off(reg, bits)    (andeq((reg), (~(bits))))
#define set(reg, bits)    (eq((reg), (bits)))
#define toggle(reg, bits) (xoreq((reg), (bits)))

#define read(reg, bits)   ((reg) & (bits))

// Wrap the pulse macro in a do/while loop to enable single line capability
#define pulse(out, bits) do {\
                             on((out), (bits)); \
                             off((out), (bits)); \
                           } while (0);

#define pulseDuration(out, bits, duration) do {\
                             on((out), (bits)); \
                             __delay_cycles((duration)); \
                             off((out), (bits)); \
                           } while (0);

// Bit manipulation macros
// Need this for rotate with carry macros
#define BYTE_SIZE 8
// Rotate left with carry
#define rlc(a) (((a)>>1) | (a)<<((sizeof((a)) * BYTE_SIZE) - 1))
// Rotate right with carry
#define rrc(a) (((a)<<1) | (a)>>((sizeof((a)) * BYTE_SIZE) - 1))
