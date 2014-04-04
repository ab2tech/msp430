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

// MSP430G2553 ISR Dispatcher
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

void isr_d::clr_sr_on_exit(uint16_t mask)
{
  clr_sr = true;
  clr_sr_bits = mask;
}

void isr_d::set_sr_on_exit(uint16_t mask)
{
  set_sr = true;
  set_sr_bits = mask;
}
#endif

isr_t (*isr_d::isr[NUM_ISR_VECTORS])(void *) = { &(isr_d::defaultHandler) };
void  (*isr_d::obj[NUM_ISR_VECTORS])         = { (void *) 0 };

isr_t isr_d::defaultHandler(void *pObj)
{
  return;
}

void isr_d::install(isr_vector_t vect,
                    void *pObject,
                    isr_t (*pCallback)(void* pObj))
{
  isr[vect] = pCallback;
  obj[vect] = pObject;
}

isr_vector_t isr_d::pinVector(msp_pin_t pin)
{
  switch (pin)
  {
    #if !defined(DISABLE_PORT1_VECTOR) && !defined(DISABLE_PORT1_ID)
    case p1_0:
      return PORT1_0_ID;
    case p1_1:
      return PORT1_1_ID;
    case p1_2:
      return PORT1_2_ID;
    case p1_3:
      return PORT1_3_ID;
    case p1_4:
      return PORT1_4_ID;
    case p1_5:
      return PORT1_5_ID;
    case p1_6:
      return PORT1_6_ID;
    case p1_7:
      return PORT1_7_ID;
    #endif

    #if !defined(DISABLE_PORT2_VECTOR) && !defined(DISABLE_PORT2_ID)
    case p2_0:
      return PORT2_0_ID;
#ifdef MSP430F5510_EXT
    case p2_1:
      return PORT2_1_ID;
    case p2_2:
      return PORT2_2_ID;
    case p2_3:
      return PORT2_3_ID;
    case p2_4:
      return PORT2_4_ID;
    case p2_5:
      return PORT2_5_ID;
    case p2_6:
      return PORT2_6_ID;
    case p2_7:
      return PORT2_7_ID;
#endif
    #endif
    default:
      return NUM_ISR_VECTORS;
  }
}

isr_vector_t isr_d::taVector(msp_timerA_t timer)
{
  switch (timer)
  {
    #if defined(ENABLE_TA0_0_VECTOR) || defined(ENABLE_TA0_0_ID)
    case ta0_0:
      return TA0_0_ID;
    #endif
    #if !defined(DISABLE_TA0_1_VECTOR) && !defined(DISABLE_TA0_1_ID)
    case ta0_1:
      return TA0_1_ID;
    case ta0_2:
      return TA0_2_ID;
    #endif
    #if !defined(DISABLE_TA1_0_VECTOR) && !defined(DISABLE_TA1_0_ID)
    case ta1_0:
      return TA1_0_ID;
    #endif
    #if !defined(DISABLE_TA1_1_VECTOR) && !defined(DISABLE_TA1_1_ID)
    case ta1_1:
      return TA1_1_ID;
    case ta1_2:
      return TA1_2_ID;
    #endif
    default:
      return NUM_ISR_VECTORS;
  }
}

void isr_d::uninstall(isr_vector_t vect)
{
  isr[vect] = &defaultHandler;
  obj[vect] = (void *) 0;
}

#ifndef DISABLE_PORT1_VECTOR
#pragma vector=PORT1_VECTOR
void __interrupt isr_d::isrPORT1(void)
{
#ifdef DISABLE_PORT1_ID
  return;
#else
  if (P1IFG & BIT7)
    (*isr[PORT1_7_ID])(obj[PORT1_7_ID]);
  else if (P1IFG & BIT6)
    (*isr[PORT1_6_ID])(obj[PORT1_6_ID]);
  else if (P1IFG & BIT5)
    (*isr[PORT1_5_ID])(obj[PORT1_5_ID]);
  else if (P1IFG & BIT4)
    (*isr[PORT1_4_ID])(obj[PORT1_4_ID]);
  else if (P1IFG & BIT3)
    (*isr[PORT1_3_ID])(obj[PORT1_3_ID]);
  else if (P1IFG & BIT2)
    (*isr[PORT1_2_ID])(obj[PORT1_2_ID]);
  else if (P1IFG & BIT1)
    (*isr[PORT1_1_ID])(obj[PORT1_1_ID]);
  else if (P1IFG & BIT0)
    (*isr[PORT1_0_ID])(obj[PORT1_0_ID]);
  else
    _never_executed();
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
#ifdef DISABLE_PORT2_ID
  return;
#else
  if (P2IFG & BIT7)
    (*isr[PORT2_7_ID])(obj[PORT2_7_ID]);
  else if (P2IFG & BIT6)
    (*isr[PORT2_6_ID])(obj[PORT2_6_ID]);
  else if (P2IFG & BIT5)
    (*isr[PORT2_5_ID])(obj[PORT2_5_ID]);
  else if (P2IFG & BIT4)
    (*isr[PORT2_4_ID])(obj[PORT2_4_ID]);
  else if (P2IFG & BIT3)
    (*isr[PORT2_3_ID])(obj[PORT2_3_ID]);
  else if (P2IFG & BIT2)
    (*isr[PORT2_2_ID])(obj[PORT2_2_ID]);
  else if (P2IFG & BIT1)
    (*isr[PORT2_1_ID])(obj[PORT2_1_ID]);
  else if (P2IFG & BIT0)
    (*isr[PORT2_0_ID])(obj[PORT2_0_ID]);
  else
    _never_executed();
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
#ifdef DISABLE_ADC10_ID
  return;
#else
  (*isr[ADC10_ID])(obj[ADC10_ID]);
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

#ifndef DISABLE_USCIAB0TX_VECTOR
#pragma vector=USCIAB0TX_VECTOR
void __interrupt isr_d::isrUSCIAB0TX(void)
{
#if defined(DISABLE_USCIA0TX_ID) && defined(DISABLE_USCIB0TX_ID)
  return;
#else
  if (IFG2 & UCA0TXIFG)
  {
    (*isr[USCIA0TX_ID])(obj[USCIA0TX_ID]);
  }

  if (IFG2 & UCB0TXIFG)
  {
    (*isr[USCIB0TX_ID])(obj[USCIB0TX_ID]);
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

#ifndef DISABLE_USCIAB0RX_VECTOR
#pragma vector=USCIAB0RX_VECTOR
void __interrupt isr_d::isrUSCIAB0RX(void)
{
#if defined(DISABLE_USCIA0RX_ID) && defined(DISABLE_USCIB0RX_ID)
  return;
#else
  if (IFG2 & UCA0RXIFG)
  {
    (*isr[USCIA0RX_ID])(obj[USCIA0RX_ID]);
  }

  if (IFG2 & UCB0RXIFG)
  {
    (*isr[USCIB0RX_ID])(obj[USCIB0RX_ID]);
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

#ifndef DISABLE_TA0_1_VECTOR
#pragma vector=TIMER0_A1_VECTOR
void __interrupt isr_d::isrTA0_1(void)
{
#ifdef DISABLE_TA0_1_ID
  return;
#else
  switch (__even_in_range(TA0IV, TA0IV_TAIFG))
  {
    case TA0IV_TAIFG:
      (*isr[TA0_IFG_ID])(obj[TA0_IFG_ID]);
      break;
    case TA0IV_8:
    case TA0IV_6:
      break;
    case TA0IV_TACCR2:
      (*isr[TA0_2_ID])(obj[TA0_2_ID]);
      break;
    case TA0IV_TACCR1:
      (*isr[TA0_1_ID])(obj[TA0_1_ID]);
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
#ifndef ENABLE_TA0_0_ID
  return;
#else
  (*isr[TA0_0_ID])(obj[TA0_0_ID]);
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
#ifndef ENABLE_WDT_ID
  return;
#else
  (*isr[WDT_ID])(obj[WDT_ID]);
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
#ifdef DISABLE_TA1_1_ID
  return;
#else
  switch (__even_in_range(TA1IV, TA1IV_TAIFG))
  {
    case TA1IV_TAIFG:
      (*isr[TA1_IFG_ID])(obj[TA1_IFG_ID]);
      break;
    case TA0IV_8:
    case TA0IV_6:
      break;
    case TA1IV_TACCR2:
      (*isr[TA1_2_ID])(obj[TA1_2_ID]);
      break;
    case TA1IV_TACCR1:
      (*isr[TA1_1_ID])(obj[TA1_1_ID]);
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
#ifdef DISABLE_TA1_0_ID
  return;
#else
  (*isr[TA1_0_ID])(obj[TA1_0_ID]);
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

#ifndef DISABLE_NMI_VECTOR
#pragma vector=NMI_VECTOR
void __interrupt isr_d::isrNMI(void)
{
#ifdef DISABLE_NMI_ID
  return;
#else
  (*isr[NMI_ID])(obj[NMI_ID]);
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

#ifdef ENABLE_RESET_ID
#pragma vector=RESET_VECTOR
void __interrupt isr_d::isrRESET(void)
{
  (*isr[RESET_ID])(obj[RESET_ID]);
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
