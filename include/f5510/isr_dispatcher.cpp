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

#include "isr_dispatcher.h"

// Initialize static variables
#ifdef ENABLE_SR_CHECKING
bool     isr_d::clr_sr = false;
uint16_t isr_d::clr_sr_bits = 0;
bool     isr_d::set_sr = false;
uint16_t isr_d::set_sr_bits = 0;
#endif

isr_t (*isr_d::isr[NUM_ISR_VECTORS])(void *) = { &(isr_d::defaultHandler) };
void  (*isr_d::obj[NUM_ISR_VECTORS])         = { (void *) 0 };

#ifdef ENABLE_SR_CHECKING
void isr_d::clr_sr_on_exit(uint16_t mask)
{
  clr_sr = true;
  clr_sr_bits = mask;
}
#endif

isr_t isr_d::defaultHandler(void *pObj)
{
  return;
}

void isr_d::installISR(isr_vector_t vect,
    void *pObject, isr_t (*pCallback)(void* pObj))
{
  isr[vect] = pCallback;
  obj[vect] = pObject;
}

isr_vector_t isr_d::pinVector(msp_pin_t pin)
{
  switch (pin)
  {
    #if !defined(DISABLE_PORT1_VECTOR) && !defined(DISABLE_PORT1_VECT)
    case p1_0:
      return PORT1_0_VECT;
    case p1_1:
      return PORT1_1_VECT;
    case p1_2:
      return PORT1_2_VECT;
    case p1_3:
      return PORT1_3_VECT;
    case p1_4:
      return PORT1_4_VECT;
    case p1_5:
      return PORT1_5_VECT;
    case p1_6:
      return PORT1_6_VECT;
    case p1_7:
      return PORT1_7_VECT;
    #endif

    #if !defined(DISABLE_PORT2_VECTOR) && !defined(DISABLE_PORT2_VECT)
    case p2_0:
      return PORT2_0_VECT;
#ifdef MSP430F5510_EXT
    case p2_1:
      return PORT2_1_VECT;
    case p2_2:
      return PORT2_2_VECT;
    case p2_3:
      return PORT2_3_VECT;
    case p2_4:
      return PORT2_4_VECT;
    case p2_5:
      return PORT2_5_VECT;
    case p2_6:
      return PORT2_6_VECT;
    case p2_7:
      return PORT2_7_VECT;
#endif
    #endif
    default:
      return NUM_ISR_VECTORS;
  }
}

#ifdef ENABLE_SR_CHECKING
void isr_d::set_sr_on_exit(uint16_t mask)
{
  set_sr = true;
  set_sr_bits = mask;
}
#endif

isr_vector_t isr_d::taVector(msp_timerA_t timer)
{
  switch (timer)
  {
    #if defined(ENABLE_TA0_0_VECTOR) || defined(ENABLE_TA0_0_VECT)
    case ta0_0:
      return TA0_0_VECT;
    #endif
    #if !defined(DISABLE_TA0_1_VECTOR) && !defined(DISABLE_TA0_1_VECT)
    case ta0_1:
      return TA0_1_VECT;
    case ta0_2:
      return TA0_2_VECT;
    case ta0_3:
      return TA0_3_VECT;
    case ta0_4:
      return TA0_4_VECT;
    #endif
    #if !defined(DISABLE_TA1_0_VECTOR) && !defined(DISABLE_TA1_0_VECT)
    case ta1_0:
      return TA1_0_VECT;
    #endif
    #if !defined(DISABLE_TA1_1_VECTOR) && !defined(DISABLE_TA1_1_VECT)
    case ta1_1:
      return TA1_1_VECT;
    case ta1_2:
      return TA1_2_VECT;
    #endif
    #if !defined(DISABLE_TA2_0_VECTOR) && !defined(DISABLE_TA2_0_VECT)
    case ta2_0:
      return TA2_0_VECT;
    #endif
    #if !defined(DISABLE_TA2_1_VECTOR) && !defined(DISABLE_TA2_1_VECT)
    case ta2_1:
      return TA2_1_VECT;
    case ta2_2:
      return TA2_2_VECT;
    #endif
    default:
      return NUM_ISR_VECTORS;
  }
}

void isr_d::uninstallISR(isr_vector_t vect)
{
  isr[vect] = &defaultHandler;
  obj[vect] = (void *) 0;
}

#ifndef DISABLE_RTC_VECTOR
#pragma vector=RTC_VECTOR
void __interrupt isr_d::isrRTC(void)
{
#ifdef DISABLE_RTC_VECT
  return;
#else
  (*isr[RTC_VECT])(obj[RTC_VECT]);

  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_PORT2_VECTOR
#pragma vector=PORT2_VECTOR
void __interrupt isr_d::isrPORT2(void)
{
#ifdef DISABLE_PORT2_VECT
  return;
#else
  switch (__even_in_range(P2IV, P2IV_P2IFG7))
  {
    case P2IV_P2IFG7:
    #ifdef MSP430F5510_EXT
      (*isr[PORT2_7_VECT])(obj[PORT2_7_VECT]);
    #endif
      break;
    case P2IV_P2IFG6:
    #ifdef MSP430F5510_EXT
      (*isr[PORT2_6_VECT])(obj[PORT2_6_VECT]);
    #endif
      break;
    case P2IV_P2IFG5:
    #ifdef MSP430F5510_EXT
      (*isr[PORT2_5_VECT])(obj[PORT2_5_VECT]);
    #endif
      break;
    case P2IV_P2IFG4:
    #ifdef MSP430F5510_EXT
      (*isr[PORT2_4_VECT])(obj[PORT2_4_VECT]);
    #endif
      break;
    case P2IV_P2IFG3:
    #ifdef MSP430F5510_EXT
      (*isr[PORT2_3_VECT])(obj[PORT2_3_VECT]);
    #endif
      break;
    case P2IV_P2IFG2:
    #ifdef MSP430F5510_EXT
      (*isr[PORT2_2_VECT])(obj[PORT2_2_VECT]);
    #endif
      break;
    case P2IV_P2IFG1:
    #ifdef MSP430F5510_EXT
      (*isr[PORT2_1_VECT])(obj[PORT2_1_VECT]);
    #endif
      break;
    case P2IV_P2IFG0:
      (*isr[PORT2_0_VECT])(obj[PORT2_0_VECT]);
      break;
    case P2IV_NONE:
      break;
    default:
      _never_executed();
  }
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_TA2_1_VECTOR
#pragma vector=TIMER2_A1_VECTOR
void __interrupt isr_d::isrTA2_1(void)
{
#ifdef DISABLE_TA2_1_VECT
  return;
#else
  switch (__even_in_range(TA2IV, TA2IV_TA2IFG))
  {
    case TA2IV_TA2IFG:
      (*isr[TA2_IFG_VECT])(obj[TA2_IFG_VECT]);
      break;
    case TA2IV_6:
    case TA2IV_5:
    case TA2IV_4:
    case TA2IV_3:
      break;
    case TA2IV_TA1CCR2:
      (*isr[TA2_2_VECT])(obj[TA2_2_VECT]);
      break;
    case TA2IV_TA1CCR1:
      (*isr[TA2_1_VECT])(obj[TA2_1_VECT]);
      break;
    case TA2IV_NONE:
      break;
    default:
      _never_executed();
  }
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_TA2_0_VECTOR
#pragma vector=TIMER2_A0_VECTOR
void __interrupt isr_d::isrTA2_0(void)
{
#ifdef DISABLE_TA2_0_VECT
  return;
#else
  (*isr[TA2_0_VECT])(obj[TA2_0_VECT]);
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_USCI_B1_VECTOR
#pragma vector=USCI_B1_VECTOR
void __interrupt isr_d::isrUSCI_B1(void)
{
#ifdef DISABLE_USCI_B1_VECT
  return;
#else
  (*isr[USCI_B1_VECT])(obj[USCI_B1_VECT]);
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_USCI_A1_VECTOR
#pragma vector=USCI_A1_VECTOR
void __interrupt isr_d::isrUSCI_A1(void)
{
#ifdef DISABLE_USCI_A1_VECT
  return;
#else
  (*isr[USCI_A1_VECT])(obj[USCI_A1_VECT]);
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_PORT1_VECTOR
#pragma vector=PORT1_VECTOR
void __interrupt isr_d::isrPORT1(void)
{
#ifdef DISABLE_PORT1_VECT
  return;
#else
  switch (__even_in_range(P1IV, P1IV_P1IFG7))
  {
    case P1IV_P1IFG7:
      (*isr[PORT1_7_VECT])(obj[PORT1_7_VECT]);
      break;
    case P1IV_P1IFG6:
      (*isr[PORT1_6_VECT])(obj[PORT1_6_VECT]);
      break;
    case P1IV_P1IFG5:
      (*isr[PORT1_5_VECT])(obj[PORT1_5_VECT]);
      break;
    case P1IV_P1IFG4:
      (*isr[PORT1_4_VECT])(obj[PORT1_4_VECT]);
      break;
    case P1IV_P1IFG3:
      (*isr[PORT1_3_VECT])(obj[PORT1_3_VECT]);
      break;
    case P1IV_P1IFG2:
      (*isr[PORT1_2_VECT])(obj[PORT1_2_VECT]);
      break;
    case P1IV_P1IFG1:
      (*isr[PORT1_1_VECT])(obj[PORT1_1_VECT]);
      break;
    case P1IV_P1IFG0:
      (*isr[PORT1_0_VECT])(obj[PORT1_0_VECT]);
      break;
    case P1IV_NONE:
      break;
    default:
      _never_executed();
  }
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_TA1_1_VECTOR
#pragma vector=TIMER1_A1_VECTOR
void __interrupt isr_d::isrTA1_1(void)
{
#ifdef DISABLE_TA1_1_VECT
  return;
#else
  switch (__even_in_range(TA1IV, TA1IV_TA1IFG))
  {
    case TA1IV_TA1IFG:
      (*isr[TA1_IFG_VECT])(obj[TA1_IFG_VECT]);
      break;
    case TA1IV_6:
    case TA1IV_5:
    case TA1IV_4:
    case TA1IV_3:
      break;
    case TA1IV_TA1CCR2:
      (*isr[TA1_2_VECT])(obj[TA1_2_VECT]);
      break;
    case TA1IV_TA1CCR1:
      (*isr[TA1_1_VECT])(obj[TA1_1_VECT]);
      break;
    case TA1IV_NONE:
      break;
    default:
      _never_executed();
  }
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_TA1_0_VECTOR
#pragma vector=TIMER1_A0_VECTOR
void __interrupt isr_d::isrTA1_0(void)
{
#ifdef DISABLE_TA1_0_VECT
  return;
#else
  (*isr[TA1_0_VECT])(obj[TA1_0_VECT]);
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_DMA_VECTOR
#pragma vector=DMA_VECTOR
void __interrupt isr_d::isrDMA(void)
{
#ifdef DISABLE_DMA_VECT
  return;
#else
  (*isr[DMA_VECT])(obj[DMA_VECT]);
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_USB_UBM_VECTOR
#pragma vector=USB_UBM_VECTOR
void __interrupt isr_d::isrUSB_UBM(void)
{
#ifdef DISABLE_USB_UBM_VECT
  return;
#else
  (*isr[USB_UBM_VECT])(obj[USB_UBM_VECT]);
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_TA0_1_VECTOR
#pragma vector=TIMER0_A1_VECTOR
void __interrupt isr_d::isrTA0_1(void)
{
#ifdef DISABLE_TA0_1_VECT
  return;
#else
  switch (__even_in_range(TA0IV, TA0IV_TA0IFG))
  {
    case TA0IV_TA0IFG:
      (*isr[TA0_IFG_VECT])(obj[TA0_IFG_VECT]);
      break;
    case TA0IV_6:
    case TA0IV_5:
      break;
    case TA0IV_TA0CCR4:
      (*isr[TA0_4_VECT])(obj[TA0_4_VECT]);
      break;
    case TA0IV_TA0CCR3:
      (*isr[TA0_3_VECT])(obj[TA0_3_VECT]);
      break;
    case TA0IV_TA0CCR2:
      (*isr[TA0_2_VECT])(obj[TA0_2_VECT]);
      break;
    case TA0IV_TA0CCR1:
      (*isr[TA0_1_VECT])(obj[TA0_1_VECT]);
      break;
    case TA0IV_NONE:
      break;
    default:
      _never_executed();
  }
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifdef ENABLE_TA0_0_VECTOR
#pragma vector=TIMER0_A0_VECTOR
void __interrupt isr_d::isrTA0_0(void)
{
#ifndef ENABLE_TA0_0_VECT
  return;
#else
  (*isr[TA0_0_VECT])(obj[TA0_0_VECT]);
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_ADC10_VECTOR
#pragma vector=ADC10_VECTOR
void __interrupt isr_d::isrADC10(void)
{
#ifdef DISABLE_ADC10_VECT
  return;
#else
  (*isr[ADC10_VECT])(obj[ADC10_VECT]);
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_USCI_B0_VECTOR
#pragma vector=USCI_B0_VECTOR
void __interrupt isr_d::isrUSCI_B0(void)
{
#ifdef DISABLE_USCI_B0_VECT
  return;
#else
  (*isr[USCI_B0_VECT])(obj[USCI_B0_VECT]);
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_USCI_A0_VECTOR
#pragma vector=USCI_A0_VECTOR
void __interrupt isr_d::isrUSCI_A0(void)
{
#ifdef DISABLE_USCI_A0_VECT
  return;
#else
  (*isr[USCI_A0_VECT])(obj[USCI_A0_VECT]);
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifdef ENABLE_WDT_VECTOR
#pragma vector=WDT_VECTOR
void __interrupt isr_d::isrWDT(void)
{
#ifndef ENABLE_WDT_VECT
  return;
#else
  (*isr[WDT_VECT])(obj[WDT_VECT]);
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_TB0_1_VECTOR
#pragma vector=TIMER0_B1_VECTOR
void __interrupt isr_d::isrTB0_1(void)
{
#ifdef DISABLE_TB0_1_VECT
  return;
#else
  switch (__even_in_range(TB0IV, TB0IV_TB0IFG))
  {
    case TB0IV_TB0IFG:
      (*isr[TB0_IFG_VECT])(obj[TB0_IFG_VECT]);
      break;
    case TB0IV_6:
      (*isr[TB0_6_VECT])(obj[TB0_6_VECT]);
      break;
    case TB0IV_5:
      (*isr[TB0_5_VECT])(obj[TB0_5_VECT]);
      break;
    case TB0IV_4:
      (*isr[TB0_4_VECT])(obj[TB0_4_VECT]);
      break;
    case TB0IV_3:
      (*isr[TB0_3_VECT])(obj[TB0_3_VECT]);
      break;
    case TB0IV_TB1CCR2:
      (*isr[TB0_2_VECT])(obj[TB0_2_VECT]);
      break;
    case TB0IV_TB1CCR1:
      (*isr[TB0_1_VECT])(obj[TB0_1_VECT]);
      break;
    case TB0IV_NONE:
      break;
    default:
      _never_executed();
  }
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_TB0_0_VECTOR
#pragma vector=TIMER0_B0_VECTOR
void __interrupt isr_d::isrTB0_0(void)
{
#ifdef DISABLE_TB0_0_VECT
  return;
#else
  (*isr[TB0_0_VECT])(obj[TB0_0_VECT]);
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_COMP_B_VECTOR
#pragma vector=COMP_B_VECTOR
void __interrupt isr_d::isrCOMP_B(void)
{
#ifdef DISABLE_COMP_B_VECT
  return;
#else
  (*isr[COMP_B_VECT])(obj[COMP_B_VECT]);
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_UNMI_VECTOR
#pragma vector=UNMI_VECTOR
void __interrupt isr_d::isrUNMI(void)
{
#ifdef DISABLE_UNMI_VECT
  return;
#else
  (*isr[UNMI_VECT])(obj[UNMI_VECT]);
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifndef DISABLE_SYSNMI_VECTOR
#pragma vector=SYSNMI_VECTOR
void __interrupt isr_d::isrSYSNMI(void)
{
#ifdef DISABLE_SYSNMI_VECT
  return;
#else
  (*isr[SYSNMI_VECT])(obj[SYSNMI_VECT]);
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
#endif
}
#endif

#ifdef ENABLE_RESET_VECT
#pragma vector=RESET_VECTOR
void __interrupt isr_d::isrRESET(void)
{
  (*isr[RESET_VECT])(obj[RESET_VECT]);
  #ifdef ENABLE_SR_CHECKING
  if (clr_sr)
  {
    __bic_SR_register_on_exit(clr_sr_bits);
    clr_sr = false;
  }
  if (set_sr)
  {
    __bis_SR_register_on_exit(set_sr_bits);
    set_sr = false;
  }
  #endif
}
#endif
