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

#define DRV8825_HOME       0
#define DRV8825_DIR_OFFSET 5
#define DEGREES_PER_REV    360

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

typedef enum
{
  DRV8825_DIR_CW = 0,
  DRV8825_DIR_CCW = 1
} drv8825_dir_t;

class drv8825
{
public:
  drv8825(spi_usci_t spi_usci, msp_pin_t latch,
          uint16_t steps_per_rev, drv8825_mode_t mode = DRV8825_MODE_1) :
    sr(spi_usci, latch), steps_per_rev(steps_per_rev), location(0) {
    init();
    mode_steps_per_rev = (steps_per_rev * (getModeDiv(mode)));
    setMode(mode);
  }

  uint16_t        cw(uint16_t angle);
  uint16_t        ccw(uint16_t angle);
  void            disable(void);
  void            enable(void);
  drv8825_dir_t   getDir(void);
  drv8825_mode_t  getMode(void);
  uint8_t         getModeDiv(drv8825_mode_t mode);
  uint16_t        go(uint16_t index);
  uint16_t        goCrazy(uint16_t index);
  uint16_t        goEasy(uint16_t index);
  void            init(void);
  drv8825_mode_t  modeDown(void);
  drv8825_mode_t  modeUp(void);
  void            reset(void);
  void            reverseDir(void);
  uint16_t        rotate(uint16_t angle);
  void            setDir(drv8825_dir_t dir);
  void            setMode(drv8825_mode_t mode);
  void            sleep(void);
  uint16_t        step(void);
  void            update(void);
  void            wake(void);

  uint16_t inline getLocation(void)    { return location; };
  uint16_t inline getModeSteps(void)   { return mode_steps_per_rev; };
  uint16_t inline getStepsPerRev(void) { return steps_per_rev; };
  uint8_t  inline getStepData(void)    { return step_data; };
  uint16_t inline goCrazyHome(void)    { return goCrazy(DRV8825_HOME); };
  uint16_t inline goEasyHome(void)     { return goEasy(DRV8825_HOME); };
  uint16_t inline goHome(void)         { return go(DRV8825_HOME); };
  void inline     setHome(void)        { location = DRV8825_HOME; };

private:
  uint16_t        location;
  uint16_t        mode_steps_per_rev;
  shift_r         sr;
  uint8_t         step_data;
  const uint16_t  steps_per_rev;
};
