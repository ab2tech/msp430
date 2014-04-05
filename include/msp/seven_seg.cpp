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

// MSP430 Seven Segment library
// Seven segment library for custom-designed AB2 seven segment configuration

#include "seven_seg.h"

/*
----> From: http://en.wikipedia.org/wiki/Seven-segment_display
Hexadecimal encodings for displaying the digits 0 to F
Digit   gfedcba abcdefg a       b       c       d       e       f       g
0       0x3F    0x7E    on      on      on      on      on      on      off
1       0x06    0x30    off     on      on      off     off     off     off
2       0x5B    0x6D    on      on      off     on      on      off     on
3       0x4F    0x79    on      on      on      on      off     off     on
4       0x66    0x33    off     on      on      off     off     on      on
5       0x6D    0x5B    on      off     on      on      off     on      on
6       0x7D    0x5F    on      off     on      on      on      on      on
7       0x07    0x70    on      on      on      off     off     off     off
8       0x7F    0x7F    on      on      on      on      on      on      on
9       0x6F    0x7B    on      on      on      on      off     on      on
A       0x77    0x77    on      on      on      off     on      on      on
b       0x7C    0x1F    off     off     on      on      on      on      on
C       0x39    0x4E    on      off     off     on      on      on      off
d       0x5E    0x3D    off     on      on      on      on      off     on
E       0x79    0x4F    on      off     off     on      on      on      on
F       0x71    0x47    on      off     off     off     on      on      on
<----
*/

const uint8_t seven_seg::seven_seg_character[NUM_SEVEN_SEG_CHARS] = {
// 0     1     2     3     4     5     6     7     8     9
   0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,
// A     b     C     d     E     F
   0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
};

void seven_seg::blank(bool zero, bool one)
{
  blank0 = zero;
  blank1 = one;
}

void seven_seg::dp(bool top, bool bot)
{
  dp_top = top;
  dp_bot = bot;
}

void seven_seg::num(uint8_t num, uint8_t base, bool lead0)
{
  uint8_t  digit1 = num % base;
  uint8_t  digit0 = (num / base) % base;
  uint16_t ss_data = 0;

  blank1 ? digit1 = 0 : digit1 = seven_seg_character[digit1];

  ((digit0 == 0 && !lead0) || blank0) ? (digit0 = 0) \
                                      : (digit0 = seven_seg_character[digit0]);

  ss_data = (digit1 << BYTE_SIZE);
  ss_data |= digit0;

  dp_top ? (ss_data |= SEVEN_SEG_DP_TOP) \
         : (ss_data &= ~(SEVEN_SEG_DP_TOP));
  dp_bot ? (ss_data |= SEVEN_SEG_DP_BOT) \
         : (ss_data &= ~(SEVEN_SEG_DP_BOT));

  tlc5925::write(ss_data);
}
