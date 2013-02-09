// Copyright (C) 2012, 2013
// AB2 Technologies
// Austin Beam, Alan Bullick

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
