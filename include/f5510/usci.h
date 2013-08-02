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

// MSP430F5510 USCI Definitions
// The definitions below provide access to the USCI registers for USCI_A and
// USCI_B modules of the MSP430F5510. Definitions prefixed with UC_ are
// universal (applicable to both USCI_A and USCI_B, while definitions prefixed
// with prefix UCA_ or UCB_ are only applicable to the respective USCI.

#define UC_CTLW0_OFFSET                0x00
#define UC_CTLW0(base_addr)            REG16(base_addr + UC_CTLW0_OFFSET)
#define UC_CTL1_OFFSET                 0x00
#define UC_CTL1(base_addr)             REG8(base_addr + UC_CTL1_OFFSET)
#define UC_CTL0_OFFSET                 0x01
#define UC_CTL0(base_addr)             REG8(base_addr + UC_CTL0_OFFSET)

#define UC_BRW_OFFSET                  0x06
#define UC_BRW(base_addr)              REG16(base_addr + UC_BRW_OFFSET)
#define UC_BR0_OFFSET                  0x06
#define UC_BR0(base_addr)              REG8(base_addr + UC_BR0_OFFSET)
#define UC_BR1_OFFSET                  0x07
#define UC_BR1(base_addr)              REG8(base_addr + UC_BR1_OFFSET)

#define UC_MCTL_OFFSET                 0x08
#define UC_MCTL(base_addr)             REG8(base_addr + UC_MCTL_OFFSET)

#define UC_STAT_OFFSET                 0x0A
#define UC_STAT(base_addr)             REG8(base_addr + UC_STAT_OFFSET)

#define UC_RXBUF_OFFSET                0x0C
#define UC_RXBUF(base_addr)            REG8(base_addr + UC_RXBUF_OFFSET)

#define UC_TXBUF_OFFSET                0x0E
#define UC_TXBUF(base_addr)            REG8(base_addr + UC_TXBUF_OFFSET)

#define UCA_ABCTL_OFFSET               0x10
#define UCA_ABCTL(base_addr)           REG8(base_addr + UCA_ABCTL_OFFSET)

#define UCB_I2COA_OFFSET               0x10
#define UCB_I2COA(base_addr)           REG16(base_addr + UCB_I2COA_OFFSET)

#define UCA_IRCTL_OFFSET               0x12
#define UCA_IRCTL(base_addr)           REG16(base_addr + UCA_ABCTL_OFFSET)
#define UCA_IRTCTL_OFFSET              0x12
#define UCA_IRTCTL(base_addr)          REG8(base_addr + UCA_ABCTL_OFFSET)
#define UCA_IRRCTL_OFFSET              0x13
#define UCA_IRRCTL(base_addr)          REG8(base_addr + UCA_ABCTL_OFFSET)

#define UCB_I2CSA_OFFSET               0x12
#define UCB_I2CSA(base_addr)           REG16(base_addr + UCB_I2CSA_OFFSET)

#define UC_ICTL_OFFSET                 0x1C
#define UC_ICTL(base_addr)             REG16(base_addr + UC_ICTL_OFFSET)
#define UC_IE_OFFSET                   0x1C
#define UC_IE(base_addr)               REG8(base_addr + UC_IE_OFFSET)
#define UC_IFG_OFFSET                  0x1D
#define UC_IFG(base_addr)              REG8(base_addr + UC_IFG_OFFSET)

#define UC_IV_OFFSET                   0x1E
#define UC_IV(base_addr)               REG16(base_addr + UC_IV_OFFSET)

