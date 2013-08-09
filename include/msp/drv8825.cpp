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

uint16_t drv8825::cw(uint16_t angle)
{
  // Store the existing direction
  drv8825_dir_t dir = getDir();

  // Configure CW direction
  setDir(DRV8825_DIR_CW);

  // Rotate using the angle specified
  rotate(angle);

  // Restore the original direction
  setDir(dir);

  // Return the final location
  return (location);
}

uint16_t drv8825::ccw(uint16_t angle)
{
  // Store the existing direction
  drv8825_dir_t dir = getDir();

  // Configure CCW direction
  setDir(DRV8825_DIR_CCW);

  // Rotate using the angle specified
  rotate(angle);

  // Restore the original direction
  setDir(dir);

  // Return the final location
  return (location);
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

// Go to a specific index in the current direction. Note that this function
// will cross the home boundary if necessary to reach the target index.
uint16_t drv8825::go(uint16_t index)
{
  drv8825_dir_t dir = getDir();

  // Can't go beyond the max number of positions
  if (index >= mode_steps_per_rev)
    index = (mode_steps_per_rev - 1);

  while (location != index)
    step();

  return location;
}

// Go to a specific index no matter which direction is currently configured.
// This function will not cross the home boundary because it will take the
// shortest path to the specified index.
uint16_t drv8825::goCrazy(uint16_t index)
{
  // Can't go beyond the max number of positions
  if (index >= mode_steps_per_rev)
    index = (mode_steps_per_rev - 1);

  if (index < location)
  {
    setDir(DRV8825_DIR_CCW);
  }
  else if (index > location)
  {
    setDir(DRV8825_DIR_CW);
  }

  while (index != location)
    step();

  return location;
}

// Go to a specific index in the current direction (don't move if the index
// given is in the opposite direction). This function won't cross the home
// boundary.
uint16_t drv8825::goEasy(uint16_t index)
{
  drv8825_dir_t dir = getDir();

  // Can't go beyond the max number of positions
  if (index >= mode_steps_per_rev)
    index = (mode_steps_per_rev - 1);

  if (dir == DRV8825_DIR_CW)
  {
    if (location <= index)
      return location;
    else
    {
      while (location != index)
        step();
    }
  }
  else if (dir == DRV8825_DIR_CW)
  {
    if (location >= index)
      return location;
    else
    {
      while (location != index)
        step();
    }
  }

  return location;
}

drv8825_dir_t drv8825::getDir(void)
{
  return ((drv8825_dir_t) (read(step_data, DRV8825_DIR) >> DRV8825_DIR_OFFSET));
}

drv8825_mode_t drv8825::getMode(void)
{
  return ((drv8825_mode_t) read(step_data, DRV8825_MODE_MASK));
}

uint8_t drv8825::getModeDiv(drv8825_mode_t mode)
{
  // We have to check this at runtime because we don't know what order the Mx
  // bits will be in
  switch (mode)
  {
    case DRV8825_MODE_1:
      return 1;
    case DRV8825_MODE_2:
      return 2;
    case DRV8825_MODE_4:
      return 4;
    case DRV8825_MODE_8:
      return 8;
    case DRV8825_MODE_16:
      return 16;
    case DRV8825_MODE_32:
      return 32;
    default:
      return 0xFF; // This shouldn't happen
  }
}

void drv8825::init(void)
{
  reset();
  wake();
  setDir(DRV8825_DIR_CW);
  enable();
}

drv8825_mode_t drv8825::modeDown(void)
{
  drv8825_mode_t mode = getMode();

  switch (mode)
  {
    case DRV8825_MODE_1:
      mode = DRV8825_MODE_32;
      break;
    case DRV8825_MODE_2:
      mode = DRV8825_MODE_1;
      break;
    case DRV8825_MODE_4:
      mode = DRV8825_MODE_2;
      break;
    case DRV8825_MODE_8:
      mode = DRV8825_MODE_4;
      break;
    case DRV8825_MODE_16:
      mode = DRV8825_MODE_8;
      break;
    case DRV8825_MODE_32:
      mode = DRV8825_MODE_16;
      break;
  }

  setMode(mode);
  return (mode);
}

drv8825_mode_t drv8825::modeUp(void)
{
  drv8825_mode_t mode = getMode();

  switch (mode)
  {
    case DRV8825_MODE_1:
      mode = DRV8825_MODE_2;
      break;
    case DRV8825_MODE_2:
      mode = DRV8825_MODE_4;
      break;
    case DRV8825_MODE_4:
      mode = DRV8825_MODE_8;
      break;
    case DRV8825_MODE_8:
      mode = DRV8825_MODE_16;
      break;
    case DRV8825_MODE_16:
      mode = DRV8825_MODE_32;
      break;
    case DRV8825_MODE_32:
      mode = DRV8825_MODE_1;
      break;
  }

  setMode(mode);
  return (mode);
}

void drv8825::reset(void)
{
  off(step_data, DRV8825_nRESET);
  update();
  clk->delayMS(1);
  on (step_data, DRV8825_nRESET);
  update();
  clk->delayMS(1);
}

void drv8825::reverseDir(void)
{
  setDir((drv8825_dir_t)(getDir() ^ DRV8825_DIR_CCW));
}

uint16_t drv8825::rotate(uint16_t angle)
{
  // Calculate the angle in steps by multiplying by the number of steps per
  // revolution in this mode and dividing by the number of degrees in a
  // revolution. Need to do a 32-bit multiplication to make sure there isn't an
  // overflow.
  uint16_t angle_in_steps = (uint16_t)(
      ((uint32_t)angle * mode_steps_per_rev) / DEGREES_PER_REV);

  // Now step until we reach the target location.
  for ( ; angle_in_steps > 0; --angle_in_steps)
    step();

  return (location);
}

void drv8825::setDir(drv8825_dir_t dir)
{
  if (dir == DRV8825_DIR_CW)
    off(step_data, DRV8825_DIR);
  else
    on (step_data, DRV8825_DIR);

  update();
}

void drv8825::setMode(drv8825_mode_t mode)
{
  // What was the divider for this mode?
  uint8_t old_mode_div = getModeDiv(getMode());
  // What was the number of effective steps per revolution?
  uint32_t old_mode_steps_per_rev = mode_steps_per_rev;

  // What is the divider for our new mode?
  uint8_t mode_div = getModeDiv(mode);

  // Calculate the new number of steps per division in this mode
  mode_steps_per_rev = ((uint16_t)mode_div * steps_per_rev);
  // Translate the stepper location from the old mode to the new mode
  location = ((location * (uint32_t)mode_steps_per_rev)
              / (old_mode_steps_per_rev));

  set(step_data, ((step_data & ~(DRV8825_MODE_MASK)) | mode));
  update();
}

void drv8825::sleep(void)
{
  off(step_data, DRV8825_nSLEEP);
  update();
  clk->delayMS(1);
}

uint16_t drv8825::step(void)
{
  if (getDir() == DRV8825_DIR_CW)
    (location >= (mode_steps_per_rev-1))
      ? location = DRV8825_HOME : location++;
  else
    (location == 0)
      ? (location = (mode_steps_per_rev-1)) : location--;

  on (step_data, DRV8825_STEP);
  update();
  clk->delayMS(1);
  off(step_data, DRV8825_STEP);
  update();
  clk->delayMS(1);

  return (location);
}

void drv8825::update(void)
{
  sr.write(step_data);
}

void drv8825::wake(void)
{
  on (step_data, DRV8825_nSLEEP);
  update();
  clk->delayMS(1);
}
