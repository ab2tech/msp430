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

// MSP430F5510 ISR Dispatcher
// Manage all device ISRs centrally. Allow installation of callbacks as ISRs in
// order to allow dynamic allocation of each interrupt vector. This ensures the
// most efficient use of interrupt vectors and also helps make libraries be
// configuration-independent.
#pragma once
#define AB2_ISR_D

#include <msp430f5510.h>
#include "msp/ab2.h"
#include "pin_fw.h"
#include "timerA_fw.h"

// Definitions that can be used to disable dispatching for various interrupts
// #define DISABLE_RTC_VECT
// #define DISABLE_PORT2_VECT
// #define DISABLE_TA2_1_VECT
// #define DISABLE_TA2_0_VECT
// #define DISABLE_USCI_B1_VECT
// #define DISABLE_USCI_A1_VECT
// #define DISABLE_PORT1_VECT
// #define DISABLE_TA1_1_VECT
// #define DISABLE_TA1_0_VECT
// #define DISABLE_DMA_VECT
// #define DISABLE_USB_UBM_VECT
// #define DISABLE_TA0_1_VECT
// #define DISABLE_TA0_0_VECT
// #define DISABLE_ADC10_VECT
// #define DISABLE_USCI_B0_VECT
// #define DISABLE_USCI_A0_VECT
// #define DISABLE_WDT_VECT
// #define DISABLE_TB0_1_VECT
// #define DISABLE_TB0_0_VECT
// #define DISABLE_COMP_B_VECT
// #define DISABLE_UNMI_VECT
// #define DISABLE_SYSNMI_VECT

// The reset vector is disabled by default from the ISR dispatcher. The
// following definition will enable it (though it's likely not useful).
// #define ENABLE_RESET_VECT

// Definition of the above will disable dispatching, but will still result in a
// default handler being created for that interrupt. To disable both
// dispatching and default handling of a given interrupt, use the following
// definitions instead:
// #define DISABLE_RTC_VECTOR
// #define DISABLE_PORT2_VECTOR
// #define DISABLE_TA2_1_VECTOR
// #define DISABLE_TA2_0_VECTOR
// #define DISABLE_USCI_B1_VECTOR
// #define DISABLE_USCI_A1_VECTOR
// #define DISABLE_PORT1_VECTOR
// #define DISABLE_TA1_1_VECTOR
// #define DISABLE_TA1_0_VECTOR
// #define DISABLE_DMA_VECTOR
// #define DISABLE_USB_UBM_VECTOR
// #define DISABLE_TA0_1_VECTOR
// #define DISABLE_TA0_0_VECTOR
// #define DISABLE_ADC10_VECTOR
// #define DISABLE_USCI_B0_VECTOR
// #define DISABLE_USCI_A0_VECTOR
// #define DISABLE_WDT_VECTOR
// #define DISABLE_TB0_1_VECTOR
// #define DISABLE_TB0_0_VECTOR
// #define DISABLE_COMP_B_VECTOR
// #define DISABLE_UNMI_VECTOR
// #define DISABLE_SYSNMI_VECTOR

// Since a default RESET handler is already installed, the ISR dispatcher won't
// (and shouldn't) install one

// Define a type that should be used to specify the return type of an ISR that
// will be dispatched by the ISR dispatcher. Primary objective of this is to
// ensure the callback has taken into account the requirements of the ISR
// dispatcher.
typedef void isr_t;

typedef enum
{
#if !defined(DISABLE_RTC_VECTOR) && !defined(DISABLE_RTC_VECT)
  RTC_VECT,          // RTC_VECTOR
#endif

#if !defined(DISABLE_PORT2_VECTOR) && !defined(DISABLE_PORT2_VECT)
#ifdef MSP430F5510_EXT
  PORT2_7_VECT,
  PORT2_6_VECT,
  PORT2_5_VECT,
  PORT2_4_VECT,
  PORT2_3_VECT,
  PORT2_2_VECT,
  PORT2_1_VECT,
#endif
  PORT2_0_VECT,      // PORT2_VECTOR
#endif

#if !defined(DISABLE_TA2_1_VECTOR) && !defined(DISABLE_TA2_1_VECT)
  TA2_IFG_VECT,
  TA2_2_VECT,
  TA2_1_VECT,        // TIMER2_A1_VECTOR
#endif

#if !defined(DISABLE_TA2_0_VECTOR) && !defined(DISABLE_TA2_0_VECT)
  TA2_0_VECT,        // TIMER2_A0_VECTOR
#endif

#if !defined(DISABLE_USCI_B1_VECTOR) && !defined(DISABLE_USCI_B1_VECT)
  USCI_B1_VECT,      // USCI_B1_VECTOR
#endif

#if !defined(DISABLE_USCI_A1_VECTOR) && !defined(DISABLE_USCI_A1_VECT)
  USCI_A1_VECT,      // USCI_A1_VECTOR
#endif

#if !defined(DISABLE_PORT1_VECTOR) && !defined(DISABLE_PORT1_VECT)
  PORT1_7_VECT,
  PORT1_6_VECT,
  PORT1_5_VECT,
  PORT1_4_VECT,
  PORT1_3_VECT,
  PORT1_2_VECT,
  PORT1_1_VECT,
  PORT1_0_VECT,      // PORT1_VECTOR
#endif

#if !defined(DISABLE_TA1_1_VECTOR) && !defined(DISABLE_TA1_1_VECT)
  TA1_IFG_VECT,
  TA1_2_VECT,
  TA1_1_VECT,        // TIMER1_A1_VECTOR
#endif

#if !defined(DISABLE_TA1_0_VECTOR) && !defined(DISABLE_TA1_0_VECT)
  TA1_0_VECT,        // TIMER1_A0_VECTOR
#endif

#if !defined(DISABLE_DMA_VECTOR) && !defined(DISABLE_DMA_VECT)
  DMA_VECT,          // DMA_VECTOR
#endif

#if !defined(DISABLE_USB_UBM_VECTOR) && !defined(DISABLE_USB_UBM_VECT)
  USB_UBM_VECT,      // USB_UBM_VECTOR
#endif

#if !defined(DISABLE_TA0_1_VECTOR) && !defined(DISABLE_TA0_1_VECT)
  TA0_IFG_VECT,
  TA0_4_VECT,
  TA0_3_VECT,
  TA0_2_VECT,
  TA0_1_VECT,        // TIMER0_A1_VECTOR
#endif

#if !defined(DISABLE_TA0_0_VECTOR) && !defined(DISABLE_TA0_0_VECT)
  TA0_0_VECT,        // TIMER0_A0_VECTOR
#endif

#if !defined(DISABLE_ADC10_VECTOR) && !defined(DISABLE_ADC10_VECT)
  ADC10_VECT,        // ADC10_VECTOR
#endif

#if !defined(DISABLE_USCI_B0_VECTOR) && !defined(DISABLE_USCI_B0_VECT)
  USCI_B0_VECT,      // USCI_B0_VECTOR
#endif

#if !defined(DISABLE_USCI_A0_VECTOR) && !defined(DISABLE_USCI_A0_VECT)
  USCI_A0_VECT,      // USCI_A0_VECTOR
#endif

#if !defined(DISABLE_WDT_VECTOR) && !defined(DISABLE_WDT_VECT)
  WDT_VECT,          // WDT_VECTOR
#endif

#if !defined(DISABLE_TB0_1_VECTOR) && !defined(DISABLE_TB0_1_VECT)
  TB0_IFG_VECT,
  TB0_6_VECT,
  TB0_5_VECT,
  TB0_4_VECT,
  TB0_3_VECT,
  TB0_2_VECT,
  TB0_1_VECT,        // TIMER0_B1_VECTOR
#endif

#if !defined(DISABLE_TB0_0_VECTOR) && !defined(DISABLE_TB0_0_VECT)
  TB0_0_VECT,        // TIMER0_B0_VECTOR
#endif

#if !defined(DISABLE_COMP_B_VECTOR) && !defined(DISABLE_COMP_B_VECT)
  COMP_B_VECT,       // COMP_B_VECTOR
#endif

#if !defined(DISABLE_UNMI_VECTOR) && !defined(DISABLE_UNMI_VECT)
  UNMI_VECT,         // UNMI_VECTOR
#endif

#if !defined(DISABLE_SYSNMI_VECTOR) && !defined(DISABLE_SYSNMI_VECT)
  SYSNMI_VECT,       // SYSNMI_VECTOR
#endif

#ifdef ENABLE_RESET_VECT
  RESET_VECT,        // RESET_VECTOR
#endif

  NUM_ISR_VECTORS
} isr_vector_t;

// All members are static since this is a single-object instance class
class isr_d
{
public:
  // Install ISR -- callback method must be static if a class member, or else a
  // non-member function.
  //
  // The callback should return void and take a single parameter. For static
  // member wrapper functions, this will be a pointer to the object for which
  // the callback should reference. Otherwise, it should be ignored in the
  // context of the callback.
  //
  // If the desired function to be called is a non-static class member, the
  // callback must be a static wrapper around a non-static call which passes
  // the object ("this" pointer of arbitrary type) as a parameter. If it is a
  // static wrapper of a non-static function, the static wrapper must cast the
  // object pointer to the proper type. The reason for this limitation is due
  // to the structure of static versus non-static function calls and the
  // dependency of various callbacks on their respective object.
  static void installISR(isr_vector_t vect,
      void *pObject, isr_t (*pCallback)(void* pObj));

  // Uninstall ISR -- remove installed ISR for a given ISR vector
  static void uninstallISR(isr_vector_t vect);

private:
  static isr_t  defaultHandler(void *pObj);
  static isr_t (*isr[NUM_ISR_VECTORS])(void *);
  static void  (*obj[NUM_ISR_VECTORS]);

#ifndef DISABLE_RTC_VECTOR
  static void __interrupt isrRTC(void);
#endif
#ifndef DISABLE_PORT2_VECTOR
  static void __interrupt isrPORT2(void);
#endif
#ifndef DISABLE_TA2_1_VECTOR
  static void __interrupt isrTA2_1(void);
#endif
#ifndef DISABLE_TA2_0_VECTOR
  static void __interrupt isrTA2_0(void);
#endif
#ifndef DISABLE_USCI_B1_VECTOR
  static void __interrupt isrUSCI_B1(void);
#endif
#ifndef DISABLE_USCI_A1_VECTOR
  static void __interrupt isrUSCI_A1(void);
#endif
#ifndef DISABLE_PORT1_VECTOR
  static void __interrupt isrPORT1(void);
#endif
#ifndef DISABLE_TA1_1_VECTOR
  static void __interrupt isrTA1_1(void);
#endif
#ifndef DISABLE_TA1_0_VECTOR
  static void __interrupt isrTA1_0(void);
#endif
#ifndef DISABLE_DMA_VECTOR
  static void __interrupt isrDMA(void);
#endif
#ifndef DISABLE_USB_UBM_VECTOR
  static void __interrupt isrUSB_UBM(void);
#endif
#ifndef DISABLE_TA0_1_VECTOR
  static void __interrupt isrTA0_1(void);
#endif
#ifndef DISABLE_TA0_0_VECTOR
  static void __interrupt isrTA0_0(void);
#endif
#ifndef DISABLE_ADC10_VECTOR
  static void __interrupt isrADC10(void);
#endif
#ifndef DISABLE_USCI_B0_VECTOR
  static void __interrupt isrUSCI_B0(void);
#endif
#ifndef DISABLE_USCI_A0_VECTOR
  static void __interrupt isrUSCI_A0(void);
#endif
#ifndef DISABLE_WDT_VECTOR
  static void __interrupt isrWDT(void);
#endif
#ifndef DISABLE_TB0_1_VECTOR
  static void __interrupt isrTB0_1(void);
#endif
#ifndef DISABLE_TB0_0_VECTOR
  static void __interrupt isrTB0_0(void);
#endif
#ifndef DISABLE_COMP_B_VECTOR
  static void __interrupt isrCOMP_B(void);
#endif
#ifndef DISABLE_UNMI_VECTOR
  static void __interrupt isrUNMI(void);
#endif
#ifndef DISABLE_SYSNMI_VECTOR
  static void __interrupt isrSYSNMI(void);
#endif
#ifdef ENABLE_RESET_VECT
  static void __interrupt isrRESET(void);
#endif
};
