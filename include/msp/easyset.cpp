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

// MSP430 EasySet Library
#include "easyset.h"

void *easyset::data(es_out_t out, uint16_t node, uint16_t val)
{
  uint16_t *p;
  switch (out)
  {
    case ES_OUT_0:
      p = out0;
      break;
    case ES_OUT_1:
      p = out1;
      break;
    case ES_OUT_2:
      p = out2;
      break;
    default:
      _never_executed();
  }

  if (node >= num_nodes)
    return (void *)(0);
  p += node;
  *p = val;
  return p;
}

void easyset::delay(uint8_t times)
{
  // TODO: Figure out which method is more ideal. I'm assuming the for loop
  // style will produce a bit more repeatable timing. If I find out that
  // neither of these produce reliable delays, I may resort to calling the
  // function multiple times for any multiple-occurrence delay.
  off(tacctl(timer), CCIFG);
  //set(taccr(timer), (tar(timer) + (delay_cycles*times)));
  //while (!read(tacctl(timer), CCIFG));
  for ( ; times>0; times--)
  {
    set(taccr(timer), (tar(timer) + delay_cycles));
    while (!read(tacctl(timer), CCIFG));
  }
  set(taccr(timer), 0);
}

void easyset::init(void)
{
  // If a timer hasn't been defined, allocate one from the pool of extra clock
  // timers. If a timer is defined, it must be configured for continuous mode
  // to work properly with this library.
  if (timer != MSP_TIMERA_SIZE)
    timer = clock::allocTimer();
}

void easyset::update(void)
{
  int16_t i = 0;
  writeCmd();
  for (i=(num_nodes-1); i>=0; i--)
  {
    if (res == ES_12BIT)
      writeNibble(out0[i]>>BYTE_SIZE & 0xF);
    writeByte(out0[i] & 0xFF);
    if (res == ES_12BIT)
      writeNibble(out1[i]>>BYTE_SIZE & 0xF);
    writeByte(out1[i] & 0xFF);
    if (res == ES_12BIT)
      writeNibble(out2[i]>>BYTE_SIZE & 0xF);
    writeByte(out2[i] & 0xFF);
    writeEOS();
  }
  writeGS();
}

void easyset::writeByte(uint8_t byte)
{
  int8_t i;
  for (i=(BYTE_SIZE-1); i>=0; --i)
  {
    write((byte>>(i)) & 0x1);
  }
}

void easyset::writeCmd(void)
{
  writeByte(0x3A);
  if (res == ES_12BIT) { writeNibble(0xA); }
}

void easyset::writeEOS(void)
{
  // Total of at least 4Tcycle delay between first rising edge of last output
  // data and next rising edge
  writeNone(ES_EOS_DELAY);
}

void easyset::writeGS(void)
{
  // Total of at least 8Tcycle delay between first rising edge of last output
  // data and next rising edge
  writeNone(ES_GSLAT_DELAY);
}

void easyset::writeNibble(uint8_t nibble)
{
  int8_t i;
  for (i=(NIBL_SIZE-1); i>=0; --i)
  {
    write((nibble>>(i)) & 0x1);
  }
}

void easyset::writeNone(uint8_t times)
{
  for ( ; times>0; --times)
    write(ES_NONE);
}

void easyset::write(uint8_t val)
{
  /*
   * Tcycle
   * |<-->|
   * |    |
   * |‾|__|‾|__
   *  0 12 3 45
   */

  if (val <= 1) { pinOn(sdo); }
  delay(1);                     // 0
  pinOff(sdo);
  delay(2);                     // 1,2
  if (val == 1) { pinOn(sdo); }
  delay(1);                     // 3
  pinOff(sdo);
  delay(2);                     // 4,5
}
