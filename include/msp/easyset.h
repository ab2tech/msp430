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
#define ES_EOS_DELAY       4 // 4Tcycle        + 1Tcycle        = 5Tcycle (3.5-5.5)
#define ES_GSLAT_DELAY     7 // 7Tcycle        + 1Tcycle        = 8Tcycle (8+)

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

// Potential RGB configurations
typedef enum
{
  ES_RGB,
  ES_GBR,
  ES_BRG,
  ES_RBG,
  ES_GRB,
  ES_BGR
} es_rgb_t;

class easyset
{
public:
  easyset(msp_pin_t sdo, uint16_t num_nodes,
          es_resolution_t res = ES_8BIT,
          es_rgb_t rgb = ES_BRG,
          msp_timerA_t timer = MSP_TIMERA_SIZE) :
    sdo(sdo), num_nodes(num_nodes), res(res), rgb(rgb), timer(timer)
  {
    pinOutput(sdo);
    pinOff(sdo);
    pOut0 = new uint16_t [num_nodes];
    pOut1 = new uint16_t [num_nodes];
    pOut2 = new uint16_t [num_nodes];
    switch(rgb)
    {
      case ES_RGB:
        pRed   = pOut0;
        pGreen = pOut1;
        pBlue  = pOut2;
        break;
      case ES_RBG:
        pRed   = pOut0;
        pGreen = pOut2;
        pBlue  = pOut1;
        break;
      case ES_GRB:
        pRed   = pOut1;
        pGreen = pOut0;
        pBlue  = pOut2;
        break;
      case ES_GBR:
        pRed   = pOut2;
        pGreen = pOut0;
        pBlue  = pOut1;
        break;
      case ES_BRG:
        pRed   = pOut1;
        pGreen = pOut2;
        pBlue  = pOut0;
        break;
      case ES_BGR:
        pRed   = pOut2;
        pGreen = pOut1;
        pBlue  = pOut0;
        break;
      default:
        _never_executed();
    }
    // Configure the delay for 1us so we can achieve Tcycle of ~4us
    delay_cycles = (uint16_t)(clock::getSysFreq() / F_1MHz);
    // For system frequencies below 1MHz, delay only one cycle
    if (delay_cycles == 0)
      delay_cycles = 1;
    init();
  }

  ~easyset() {
    delete[] pOut0;
    delete[] pOut1;
    delete[] pOut2;
  }

  void *data(uint16_t node, es_out_t out, uint16_t val);
  void  rgbData(uint16_t node, uint16_t r, uint16_t g, uint16_t b);
  void  update(void);

private:
  uint16_t         delay_cycles;
  uint16_t         num_nodes;
  uint16_t        *pOut0;
  uint16_t        *pOut1;
  uint16_t        *pOut2;
  uint16_t        *pRed;
  uint16_t        *pGreen;
  uint16_t        *pBlue;
  es_rgb_t         rgb;
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
