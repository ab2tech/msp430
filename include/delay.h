// delay.h - delay library providing macros for MSP430 delays. These are for
// convenience and shouldn't be considered 100% accurate, especially if timing
// is important. For the most accurate delays, make sure F_CPU matches the
// operating frequency of your MSP430 device.

#pragma once

// F_CPU should be defined to the frequency to base delays upon before sourcing
// the delay library as follows:
// Example for 16MHz
// #define F_CPU 16000000UL
// Example for 25MHz
// #define F_CPU 25000000UL

#ifndef F_CPU
  #error "F_CPU is not set..."
#endif

// Delay Functions
#define _delay_ns(__ns) \
  if((unsigned long) (F_CPU/1000000000.0 * __ns) != F_CPU/1000000000.0 * __ns)\
        __delay_cycles((unsigned long) ( F_CPU/1000000000.0 * __ns)+1);\
  else __delay_cycles((unsigned long) ( F_CPU/1000000000.0 * __ns))
#define _delay_us(__us) \
  if((unsigned long) (F_CPU/1000000.0 * __us) != F_CPU/1000000.0 * __us)\
        __delay_cycles((unsigned long) ( F_CPU/1000000.0 * __us)+1);\
  else __delay_cycles((unsigned long) ( F_CPU/1000000.0 * __us))
#define _delay_ms(__ms) \
  if((unsigned long) (F_CPU/1000.0 * __ms) != F_CPU/1000.0 * __ms)\
        __delay_cycles((unsigned long) ( F_CPU/1000.0 * __ms)+1);\
  else __delay_cycles((unsigned long) ( F_CPU/1000.0 * __ms))
#define _delay_s(__s) \
  if((unsigned long) (F_CPU/1.0 * __s) != F_CPU/1.0 * __s)\
        __delay_cycles((unsigned long) ( F_CPU/1.0 * __s)+1);\
  else __delay_cycles((unsigned long) ( F_CPU/1.0 * __s))
