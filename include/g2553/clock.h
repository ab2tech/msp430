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

// MSP430G2553 Clock Library
// Simplify MSP430G2553 clock initialization and frequency modification

#pragma once

#include <msp430g2553.h>
#include "msp/ab2.h"

// Define the default divisions supported by the various clock signals
// If a clock signal doesn't support all of these dividers or supports
// additional dividers, they should be defined independently
typedef enum
{
  CLK_DIV_1,
  CLK_DIV_2,
  CLK_DIV_4,
  CLK_DIV_8
} clk_div_t;

// Define the ACLK sources supported by the clock module
typedef enum
{
  ACLK_SRC_32kHz, // 32.768kHz external crystal
  ACLK_SRC_1MHz,  // 400kHz-1MHz external crystal
  ACLK_SRC_3MHz,  // 1MHz-3MHz external crystal
  ACLK_SRC_16MHz, // 3MHz-16MHz external crystal
  ACLK_SRC_DIG,   // Digital external clock source
  ACLK_SRC_VLO    // Internal very low frequency oscillator (~12kHz)
} aclk_src_t;

// Define the DCO frequencies supported by the clock module
typedef enum
{
  DCO_F_1MHz,
  DCO_F_8MHz,
  DCO_F_12MHz,
  DCO_F_16MHz
} dco_freq_t;

// Define the MCLK sources supported by the clock module
typedef enum
{
  MCLK_SRC_DCO,
  MCLK_SRC_ACLK, // Not technically source by ACLK, but by LFXT1CLK or VLO
  MCLK_SRC_XT2   // Must be enabled and present for this to work
} mclk_src_t;

// Define the SMCLK sources supported by the clock module
typedef enum
{
  SMCLK_SRC_DCO,
  SMCLK_SRC_XTAL // XT2 if present or same source as ACLK if XT2 not present
} smclk_src_t;

// Define the settings for XT1 capacitance as seen by the LFXT1 crystal when
// XTS=0. If XTS=1 or LFXT1Sx=11 XCAPx should be 00.
typedef enum
{
  XCAP_OFF,   // 00
  XCAP_1pF,   // 00
  XCAP_6pF,   // 01
  XCAP_10pF,  // 10
  XCAP_12_5pF // 11
} xcap_t;

// Define the XT2 frequency ranges supported by the clock module
typedef enum
{
  XT2_F_1MHz,  // 400kHz-1MHz external crystal
  XT2_F_3MHz,  // 1MHz-3MHz external crystal
  XT2_F_16MHz, // 3MHz-16MHz external crystal
  XT2_F_DIG   // Digital external clock source
} xt2_range_t;

void clockInitDefaults(void);
void clockInit(aclk_src_t asrc, clk_div_t adiv,
               dco_freq_t dcof,
               mclk_src_t msrc, clk_div_t mdiv,
               smclk_src_t ssrc, clk_div_t sdiv);
void clockSetACLK(aclk_src_t src, clk_div_t div);
void clockSetDCO(dco_freq_t freq);
void clockSetMCLK(mclk_src_t src, clk_div_t div);
void clockSetSMCLK(smclk_src_t src, clk_div_t div);
void clockSetXT1Caps(xcap_t cap);
void clockSetXT2(xt2_range_t range);

inline uint8_t clockGetLFXT1Fault(void);
inline uint8_t clockGetXT2Fault(void);
