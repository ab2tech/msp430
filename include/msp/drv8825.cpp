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
#include "drv8825.h"

void drv8825::cw(void)
{
  off(step_data, DRV8825_DIR);
  update();
}

void drv8825::ccw(void)
{
  on (step_data, DRV8825_DIR);
  update();
}

void drv8825::disable(void)
{
  on (step_data, DRV8825_nENBL);
  update();
}

void drv8825::enable(void)
{
  off(step_data, DRV8825_nENBL);
  update();
}

drv8825_mode_t drv8825::getMode(void)
{
  return ((drv8825_mode_t) read(step_data, DRV8825_MODE_MASK));
}

void drv8825::init(void)
{
  reset();
  wake();
  cw();
  enable();
}

void drv8825::reset(void)
{
  off(step_data, DRV8825_nRESET);
  update();
  clk->delayMS(10);
  on (step_data, DRV8825_nRESET);
  update();
  clk->delayMS(10);
}

void drv8825::setMode(drv8825_mode_t mode)
{
  set(step_data,
      ((step_data & ~(DRV8825_MODE_MASK)) | mode));
  update();
}

void drv8825::sleep(void)
{
  off(step_data, DRV8825_nSLEEP);
  update();
}

void drv8825::step(void)
{
  on (step_data, DRV8825_STEP);
  update();
  clk->delayMS(1);
  off(step_data, DRV8825_STEP);
  update();
  clk->delayMS(1);
}

void drv8825::update(void)
{
  sr.write(step_data);
}

void drv8825::wake(void)
{
  on (step_data, DRV8825_nSLEEP);
  update();
}
