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

// MSP430 tlc5925 Library
// Simple tlc5925 library for providing maximum re-use and ease of integration

#pragma once
#define AB2_tlc5925

#include "ab2.h"
// Steal our MSP include from pin_fw to make this a generic library
#include "../pin_fw.h"
#include "../spi.h"

// Define a return type for TLC5925 functions with no meaningful return value
// (return status)
typedef enum
{
  TLC5925_NO_ERR,     // No error
  TLC5925_REINIT,     // TLC5925 has already been initialized
  TLC5925_PARAM_ERR,  // Paramater Error
  TLC5925_ERR         // Error
} tlc5925_ret_t;

// Channel definitions
#define CH00                            BIT0
#define CH01                            BIT1
#define CH02                            BIT2
#define CH03                            BIT3
#define CH04                            BIT4
#define CH05                            BIT5
#define CH06                            BIT6
#define CH07                            BIT7
#define CH08                            BIT8
#define CH09                            BIT9
#define CH10                            BITA
#define CH11                            BITB
#define CH12                            BITC
#define CH13                            BITD
#define CH14                            BITE
#define CH15                            BITF

// Define a type to contain pertinent TLC data. For each TLC in use, an
// instance of this struct type must exist.
typedef struct tlc5925
{
  spi_usci_t   usci;
  msp_pin_t    le;
  msp_pin_t    oe;
  uint16_t     start_ch; // tlc arrays are not always 0-indexed

  // Internal state variable for tracking tlc5925 init state. This should never
  // be modified outside of the tlc5925 library
  bool_t       is_init;
} tlc5925_t;

// Function prototypes
tlc5925_ret_t tlc5925ChannelScanDown(tlc5925_t *tlc, uint16_t starting_channel,
                                     uint16_t scan_quantity);
tlc5925_ret_t tlc5925ChannelScanUp(tlc5925_t *tlc, uint16_t starting_channel,
                                   uint16_t scan_quantity);
tlc5925_ret_t tlc5925Init(tlc5925_t *tlc);
tlc5925_ret_t tlc5925Latch(tlc5925_t *tlc);
tlc5925_ret_t tlc5925OutputDisable(tlc5925_t *tlc);
tlc5925_ret_t tlc5925OutputEnable(tlc5925_t *tlc);
tlc5925_ret_t tlc5925Pulse(tlc5925_t *tlc, uint16_t pulse_quantity,
                           uint16_t channel_data);
tlc5925_ret_t tlc5925ShiftDown(tlc5925_t *tlc, uint16_t shift_quantity,
                               uint16_t channel_data);
tlc5925_ret_t tlc5925ShiftUp(tlc5925_t *tlc, uint16_t shift_quantity,
                             uint16_t channel_data);
tlc5925_ret_t tlc5925Write(tlc5925_t *tlc, uint16_t channel_data);

// Animation delay
#define ANIMATION_DELAY 50

// Channel presets (channel 0 referenced)
#define BOLD_CURSOR                     (CH03|CH04)
#define BOTTOM_LEFT_QUADRANT            (CH03|CH04|CH05|CH06|CH07)
#define BOTTOM_RIGHT_QUADRANT           (CH00|CH01|CH02|CH03|CH15)
#define CH0_15                          (CH00|CH01|CH02|CH03|CH04|CH05|CH06| \
                                         CH07|CH08|CH09|CH10|CH11|CH12|CH13| \
                                         CH14|CH15)
#define CH5555                          (CH00|CH02|CH04|CH06|CH08|CH10|CH12| \
                                         CH14)
#define CHAAAA                          (CH01|CH03|CH05|CH07|CH09|CH11|CH13| \
                                         CH15)
#define CRASH_DUMMY_LEFT                (CH00|CH01|CH02|CH03|CH07|CH08|CH09| \
                                         CH10|CH11|CH15)
#define CRASH_DUMMY_RIGHT               (CH03|CH04|CH05|CH06|CH07|CH11|CH12| \
                                         CH13|CH14|CH15)
#define RIGHT_HALF                      (CH03|CH04|CH05|CH06|CH07|CH08|CH09| \
                                         CH10|CH11)
#define LEFT_HALF                       (CH11|CH12|CH13|CH14|CH15|CH00|CH01| \
                                         CH02|CH03)
