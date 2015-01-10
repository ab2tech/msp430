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

// MSP430 DAC8568 Library

#pragma once
#define AB2_DAC_8568

#include "ab2.h"
// Steal our MSP include from pin_fw to make this a generic library
#include "../pin_fw.h"
#include "../spi.h"

// DAC8568 Grades
// A => GAIN=1 PON=ZERO
// B => GAIN=1 PON=MID
// C => GAIN=2 PON=ZERO
// D => GAIN=2 PON=MID

// Miscellaneous definitions
#define DAC8568_MILLI_TO_MICRO         1000
// DAC8568 definitions
#define DAC8568_PRECISION              0xFFFF // (1<<16 - 1) == (2^16 - 1)
// DAC8568 SPI definitions
#define DAC8568_DISABLE_INT_REF        0x08000000UL
#define DAC8568_ENABLE_INT_REF         0x08000001UL

#define DAC8568_PREFIX_OFFSET          28 // Positions 31, 30, 29, 28
                                          //           P3  P2  P1  P0
#define DAC8568_CONTROL_OFFSET         24 // Positions 27, 26, 25, 24
                                          //           C3  C2  C1  C0
#define DAC8568_CHANNEL_OFFSET         20 // Positions 23, 22, 21, 20
                                          //           A3  A2  A1  A0
#define DAC8568_DATA_OFFSET            4  // Positions 19...........4
                                          //          D16..........D1
#define DAC8568_FEATURE_OFFSET         0  // Positions  3,  2,  1,  0
                                          //           F3  F2  F1  F0

// Channel write command
#define DAC8568_WRITE_CH              (0UL << DAC8568_CONTROL_OFFSET)
// Channel update command
#define DAC8568_UPDATE_CH             (1UL << DAC8568_CONTROL_OFFSET)
// Channel write, update all command
#define DAC8568_WRITE_UPDATE_ALL_CH   (2UL << DAC8568_CONTROL_OFFSET)
// Channel write, update channel command
#define DAC8568_WRITE_UPDATE_CH       (3UL << DAC8568_CONTROL_OFFSET)

// Base power command for power-down and power-up.
#define DAC8568_CH_POWER_CMD          (4UL << DAC8568_CONTROL_OFFSET)
// These values must be OR'd with bits 0-7 representing channels A-H
// (DAC8568_CH_POWER_x). If the corresponding bit is set to '1', the
// command action is taken (i.e. power-up, power-down).
#define DAC8568_CH_POWER_UP           (DAC8568_CH_POWER_CMD)
#define DAC8568_CH_POWER_DOWN_1K      (DAC8568_CH_POWER_CMD) | 0x100
#define DAC8568_CH_POWER_DOWN_100K    (DAC8568_CH_POWER_CMD) | 0x200
#define DAC8568_CH_POWER_DOWN_HI_Z    (DAC8568_CH_POWER_CMD) | 0x200

#define DAC8568_CH_POWER_A             BIT0
#define DAC8568_CH_POWER_B             BIT1
#define DAC8568_CH_POWER_C             BIT2
#define DAC8568_CH_POWER_D             BIT3
#define DAC8568_CH_POWER_E             BIT4
#define DAC8568_CH_POWER_F             BIT5
#define DAC8568_CH_POWER_G             BIT6
#define DAC8568_CH_POWER_H             BIT7
#define DAC8568_CH_POWER_ALL           0xFF

// Base command for writing the clear code register
#define DAC8568_CLR_CODE              (5UL << DAC8568_CONTROL_OFFSET)
// Various levels that can be written to the clear code register
#define DAC8568_CLR_ZERO               0
#define DAC8568_CLR_MID                1
#define DAC8568_CLR_FULL               2

#define DAC8568_LDAC                  (6UL << DAC8568_CONTROL_OFFSET)
#define DAC8568_CH_LDAC_A              BIT0
#define DAC8568_CH_LDAC_B              BIT1
#define DAC8568_CH_LDAC_C              BIT2
#define DAC8568_CH_LDAC_D              BIT3
#define DAC8568_CH_LDAC_E              BIT4
#define DAC8568_CH_LDAC_F              BIT5
#define DAC8568_CH_LDAC_G              BIT6
#define DAC8568_CH_LDAC_H              BIT7
#define DAC8568_CH_LDAC_ALL            0xFF

// Software reset command
#define DAC8568_SW_RESET              (7UL << DAC8568_CONTROL_OFFSET)

// END DAC8568 SPI Definitions

typedef enum
{
  DAC8568_CH_A    = ((uint32_t) 0x0 << DAC8568_CHANNEL_OFFSET),
  DAC8568_CH_B    = ((uint32_t) 0x1 << DAC8568_CHANNEL_OFFSET),
  DAC8568_CH_C    = ((uint32_t) 0x2 << DAC8568_CHANNEL_OFFSET),
  DAC8568_CH_D    = ((uint32_t) 0x3 << DAC8568_CHANNEL_OFFSET),
  DAC8568_CH_E    = ((uint32_t) 0x4 << DAC8568_CHANNEL_OFFSET),
  DAC8568_CH_F    = ((uint32_t) 0x5 << DAC8568_CHANNEL_OFFSET),
  DAC8568_CH_G    = ((uint32_t) 0x6 << DAC8568_CHANNEL_OFFSET),
  DAC8568_CH_H    = ((uint32_t) 0x7 << DAC8568_CHANNEL_OFFSET),
  DAC8568_CH_ALL  = ((uint32_t) 0xF << DAC8568_CHANNEL_OFFSET)
} dac8568_chan_t;

typedef enum
{
  DAC8568_GRADEA,
  DAC8568_GRADEB,
  DAC8568_GRADEC,
  DAC8568_GRADED
} dac8568_grade_t;

class dac8568 : public spi
{
public:
  dac8568(uint16_t ref_mV, spi_usci_t spi_usci, msp_pin_t sync,
          bool intref = false,
          dac8568_grade_t grade = DAC8568_GRADEB,
          msp_pin_t ldac = MSP_PIN_SIZE) :
          ref_mV(ref_mV), sync(sync), spi(spi_usci), grade(grade), ldac(ldac)
  {
    (grade <= DAC8568_GRADEB) ? gain = 1 : gain = 2;
    ref_uV = (uint32_t) ref_mV * DAC8568_MILLI_TO_MICRO;
    if (ldac != MSP_PIN_SIZE)
    {
      pinOff(ldac);
      pinOutput(ldac);
    }
    pinOff(sync);
    pinOutput(sync);
    if (intref)
    {
      intRefOn();
    }
  };


  uint16_t inline getDacValUV(uint32_t uV) { return ((uint16_t)
    ((uV * DAC8568_PRECISION) / (ref_uV * gain))); };

  uint16_t inline getDacValMV(uint32_t mV) { return ((uint16_t)
    ((mV * DAC8568_PRECISION) / (ref_mV * gain))); };

  void inline intRefOff() { tx(DAC8568_DISABLE_INT_REF); };

  void inline intRefOn() { tx(DAC8568_ENABLE_INT_REF); };

  void inline latch(void) { pinPulse(sync); };

  void inline ldacOn(uint16_t ldac_word) { tx(ldac_word | DAC8568_LDAC); };

  void inline powerDown(uint32_t type, uint16_t dac_word) { tx((uint32_t)
    (type | dac_word)); };

  void inline powerUp(uint16_t dac_word) { tx((uint32_t)
    (DAC8568_CH_POWER_UP | dac_word)); };

  void inline swRst(void) { tx(DAC8568_SW_RESET); };

  void        tx(uint32_t word);

  void inline update(dac8568_chan_t chan) { tx((uint32_t)
    (DAC8568_UPDATE_CH | chan)); };

  void inline write(dac8568_chan_t chan, uint16_t dac_val) { tx((uint32_t)
    (DAC8568_WRITE_CH | chan | ((uint32_t)dac_val << DAC8568_DATA_OFFSET))); };

  void inline writeUpdate(dac8568_chan_t chan, uint16_t dac_val) { tx((uint32_t)
    (DAC8568_WRITE_UPDATE_CH | chan |
      ((uint32_t)dac_val << DAC8568_DATA_OFFSET))); };

  void inline writeUpdateAll(dac8568_chan_t chan, uint16_t dac_val) {
    tx((uint32_t) (DAC8568_WRITE_UPDATE_ALL_CH | chan |
         ((uint32_t)dac_val << DAC8568_DATA_OFFSET))); };


private:
  uint8_t         gain;
  dac8568_grade_t grade;
  bool            intref;
  msp_pin_t       ldac;
  uint16_t        ref_mV;
  uint32_t        ref_uV;
  msp_pin_t       sync;
};

