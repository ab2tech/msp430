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

// MSP430 DRV8825 Library

#pragma once
#define AB2_DRV8825

#include "ab2.h"
// Steal our MSP include from pin_fw to make this a generic library
#include "../pin_fw.h"
#include "../spi.h"
#include "../clock.h"
#include "shift_r.h"

typedef enum
{
  DRV8825_M2     = BIT0,
  DRV8825_M1     = BIT1,
  DRV8825_M0     = BIT2,
  DRV8825_STEP   = BIT3,
  DRV8825_nENBL  = BIT4,
  DRV8825_DIR    = BIT5,
  DRV8825_nSLEEP = BIT6,
  DRV8825_nRESET = BIT7,
  DRV8825_MAX_PIN
} drv8825_pin_t;

#define DRV8825_MODE_MASK (DRV8825_M0 | DRV8825_M1 | DRV8825_M2)

typedef enum
{
  DRV8825_MODE_1  =  0,
  DRV8825_MODE_2  =  DRV8825_M0,
  DRV8825_MODE_4  =  DRV8825_M1,
  DRV8825_MODE_8  = (DRV8825_M0 | DRV8825_M1),
  DRV8825_MODE_16 =  DRV8825_M2,
  DRV8825_MODE_32 = (DRV8825_M0 | DRV8825_M2)
} drv8825_mode_t;

class drv8825
{
public:
  drv8825(clock *clk, spi spi_usci, msp_pin_t latch, drv8825_mode_t mode = DRV8825_MODE_1) :
    clk(clk), sr(spi_usci, latch), step_data(mode) {
    init();
  }

  void cw(void);
  void ccw(void);
  void disable(void);
  void enable(void);
  drv8825_mode_t getMode(void);
  void init(void);
  void reset(void);
  void setMode(drv8825_mode_t mode);
  void sleep(void);
  void step(void);
  void update(void);
  void wake(void);
private:
  clock  *clk;
  shift_r sr;
  uint8_t step_data;
};
