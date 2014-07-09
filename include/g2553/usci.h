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

// MSP430G2553 USCI Definitions
// The definitions below provide access to the USCI registers for USCI_A and
// USCI_B modules of the MSP430G2553. Definitions prefixed with UC_ are
// universal (applicable to both USCI_A and USCI_B, while definitions prefixed
// with prefix UCA_ or UCB_ are only applicable to the respective USCI.

#define USCI_A0_BASE                   ((uint16_t) &UCA0CTL0)
#define USCI_B0_BASE                   ((uint16_t) &UCB0CTL0)

#define UC_CTLW0_OFFSET                0x00
#define UC_CTLW0(base_addr)            REG16(base_addr + UC_CTLW0_OFFSET)
#define UC_CTL0_OFFSET                 0x00
#define UC_CTL0(base_addr)             REG8(base_addr + UC_CTL0_OFFSET)
#define UC_CTL1_OFFSET                 0x01
#define UC_CTL1(base_addr)             REG8(base_addr + UC_CTL1_OFFSET)

#define UC_BRW_OFFSET                  0x02
#define UC_BRW(base_addr)              REG16(base_addr + UC_BRW_OFFSET)
#define UC_BR0_OFFSET                  0x02
#define UC_BR0(base_addr)              REG8(base_addr + UC_BR0_OFFSET)
#define UC_BR1_OFFSET                  0x03
#define UC_BR1(base_addr)              REG8(base_addr + UC_BR1_OFFSET)

#define UCA_MCTL_OFFSET                0x04
#define UCA_MCTL(base_addr)            REG8(base_addr + UCA_MCTL_OFFSET)

#define UCB_I2CIE_OFFSET               0x04
#define UCB_I2CIE(base_addr)           REG8(base_addr + UCB_I2CIE_OFFSET)

#define UC_STAT_OFFSET                 0x05
#define UC_STAT(base_addr)             REG8(base_addr + UC_STAT_OFFSET)

#define UC_RXBUF_OFFSET                0x06
#define UC_RXBUF(base_addr)            REG8(base_addr + UC_RXBUF_OFFSET)

#define UC_TXBUF_OFFSET                0x07
#define UC_TXBUF(base_addr)            REG8(base_addr + UC_TXBUF_OFFSET)

// Negative offsets below

#define UCA_ABCTL_OFFSET               0x03 // negative offset
#define UCA_ABCTL(base_addr)           REG8(base_addr - UCA_ABCTL_OFFSET)

#define UCA_IRCTL_OFFSET               0x02 // negative offset
#define UCA_IRCTL(base_addr)           REG16(base_addr - UCA_ABCTL_OFFSET)
#define UCA_IRTCTL_OFFSET              0x02 // negative offset
#define UCA_IRTCTL(base_addr)          REG8(base_addr - UCA_ABCTL_OFFSET)
#define UCA_IRRCTL_OFFSET              0x01 // negative offset
#define UCA_IRRCTL(base_addr)          REG8(base_addr - UCA_ABCTL_OFFSET)

// HACK offsets below

// For completeness, I'm adding these offsets as if they were fully proper.
// PLEASE NOTE that they are not, in fact, fully correct. This is because the
// G2553 does not have fully modular UCSIs such as the MSP430F55xx devices do.
// Since the G2553 only has a single USCIA and USCIB interface, having an offset
// that is only valid for one USCI is okay. However, if this is ever ported to
// an MSP430x2xx device which supports multiple USCIAs or USCIBs, these will
// need to be modified accordingly.
#define UCB_I2COA_OFFSET               0xB0
#define UCB_I2COA(base_addr)           REG16(base_addr + UCB_I2COA_OFFSET)

#define UCB_I2CSA_OFFSET               0xB2
#define UCB_I2CSA(base_addr)           REG16(base_addr + UCB_I2CSA_OFFSET)

#define UC_IE(base_addr)               IE2
#define UCA_RXIE(base_addr)            UCA0RXIE
#define UCA_TXIE(base_addr)            UCA0TXIE

#define UC_IFG(base_addr)              IFG2
#define UCA_RXIFG(base_addr)           UCA0RXIFG
#define UCA_TXIFG(base_addr)           UCA0TXIFG
