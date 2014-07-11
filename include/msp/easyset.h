// Copyright (C) [2014] [AB2 Technologies] [Austin Beam, Alan Bullick]
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

#pragma once

// MSP430 EasySet Library
#include "ab2.h"
// Steal our MSP include from pin_fw to make this a generic library
#include "../pin_fw.h"
#include "../clock.h"
#include "../timerA_fw.h"

// Delay Calculations           EXPLICIT DELAY + IMPLICIT DELAY = TOTAL DELAY
#define ES_EOS_DELAY       1 // 1*2Tcycle      + 2Tcycle        = 4Tcycle
#define ES_GSLAT_DELAY     3 // 3*2Tcycle      + 2Tcycle        = 8Tcycle

// EasySet Commands
#define ES_WRITECMD        0x3A
#define ES_WRITECMD_EXT    0xA

// EasySet none (OFF) -- anything 2-254 is valid for this, but let's just use 2
#define ES_NONE 2

// Potential EasySet resolutions
typedef enum
{
  ES_8BIT,
  ES_12BIT
} es_resolution_t;

// Potential EasySet outputs
typedef enum
{
  ES_OUT_0,
  ES_OUT_1,
  ES_OUT_2
} es_out_t;

class easyset
{
public:
  easyset(msp_pin_t sdo, uint16_t num_nodes,
          es_resolution_t res = ES_8BIT,
          msp_timerA_t timer = MSP_TIMERA_SIZE) :
    sdo(sdo), num_nodes(num_nodes), res(res), timer(timer)
  {
    pinOutput(sdo);
    pinOff(sdo);
    out0 = new uint16_t [num_nodes];
    out1 = new uint16_t [num_nodes];
    out2 = new uint16_t [num_nodes];
    // Configure the delay for 1us so we can achieve Tcycle of ~4us
    delay_cycles = (uint16_t)(clock::getSysFreq() / F_1MHz);
    // For system frequencies below 1MHz, delay only one cycle
    if (delay_cycles == 0)
      delay_cycles = 1;
    init();
  }

  ~easyset() {
    delete[] out0;
    delete[] out1;
    delete[] out2;
  }

  void *data(es_out_t out, uint16_t node, uint16_t val);
  void  update(void);

private:
  uint16_t         delay_cycles;
  uint16_t         num_nodes;
  uint16_t        *out0;
  uint16_t        *out1;
  uint16_t        *out2;
  es_resolution_t  res;
  msp_pin_t        sdo;
  msp_timerA_t     timer;

  void delay(uint8_t times);
  void init(void);
  void setupTimer(void);
  void writeByte(uint8_t byte);
  void writeCmd(void);
  void writeEOS(void);
  void writeGS(void);
  void writeNibble(uint8_t nibble);
  void writeNone(uint8_t times);
  void write(uint8_t val);
};
