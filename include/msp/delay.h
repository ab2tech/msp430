// delay.h - delay library providing MSP430 delay macros and functions. These
// are for convenience and shouldn't be considered 100% accurate, especially if
// timing is important. For the most accurate delays, make sure F_CPU matches
// the operating frequency of your MSP430 device.

#pragma once
#include "msp/ab2.h"
#include "../../main.h"

// F_CPU should be defined to the frequency to base delays upon before sourcing
// the delay library as follows:
// Example for 16MHz
// #define F_CPU 16000000UL
// Example for 25MHz
// #define F_CPU 25000000UL

#define F_CPU_NS (F_CPU/1000000000.0)
#define F_CPU_US (F_CPU/1000000.0)
#define F_CPU_MS (F_CPU/1000.0)
#define F_CPU_S  (F_CPU/1.0)

// Delay Macros (more accurate -- static delay required)
#define _delay_ns(__ns) \
  if((uint32_t) (F_CPU_NS * __ns) != F_CPU_NS * __ns)\
        __delay_cycles((uint32_t) ( F_CPU_NS * __ns)+1);\
  else __delay_cycles((uint32_t) ( F_CPU_NS * __ns))
#define _delay_us(__us) \
  if((uint32_t) (F_CPU_US * __us) != F_CPU_US * __us)\
        __delay_cycles((uint32_t) ( F_CPU_US * __us)+1);\
  else __delay_cycles((uint32_t) ( F_CPU_US * __us))
#define _delay_ms(__ms) \
  if((uint32_t) (F_CPU_MS * __ms) != F_CPU_MS * __ms)\
        __delay_cycles((uint32_t) ( F_CPU_MS * __ms)+1);\
  else __delay_cycles((uint32_t) ( F_CPU_MS * __ms))
#define _delay_s(__s) \
  if((uint32_t) (F_CPU_S * __s) != F_CPU_S * __s)\
        __delay_cycles((uint32_t) ( F_CPU_S * __s)+1);\
  else __delay_cycles((uint32_t) ( F_CPU_S * __s))
