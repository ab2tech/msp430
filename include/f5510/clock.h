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

// MSP430F5510 Clock Library
// Simplify MSP430F5510 clock initialization and frequency modification

#pragma once

#include "main.h"

// Define the DCO frequencies supported by the clock module
typedef enum
{ 
  DCO_F_1MHz, 
  DCO_F_8MHz, 
  DCO_F_12MHz, 
  DCO_F_25MHz 
} dco_freq_t;

// Define the ACLK frequencies supported by the clock module
typedef enum 
{ 
  ACLK_F_REFO
} aclk_freq_t;
  
dco_freq_t clockGetDCOFreq(void);
void       clockInit(void);
void       clockSetACLK(aclk_freq_t freq);
void       clockSetDCO(dco_freq_t freq);
void       clockUpVcore (uint16_t level);
