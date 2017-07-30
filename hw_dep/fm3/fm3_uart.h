//
// Copyright (c) 2017, Kentaro Sekimoto
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//  -Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//  -Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#ifndef FM3_UART_H_
#define FM3_UART_H_

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (UART_CH == 4) || (UART_CH == 5) || (UART_CH == 6) || (UART_CH == 7)
#define UART47
#endif

#define FM3_UART_TX_PRIORITY 6
#define	FM3_UART_RX_PRIORITY 5

//	SIN0_0	P21
//	SIN0_1	P14/AN04
//	SIN0_2	PB4/AN20
//	SOT0_0	P22/AN31
//	SOT0_1	P15/AN05
//	SOT0_2	PB5/AN21
//	SCK0_0	P23/AN30
//	SCK0_1	P16/AN06
//	SCK0_2	PB6/AN22

//	SIN1_0	P56
//	SIN1_1	P11/AN01
//	SIN1_2	PF0
//	SOT1_0	P57
//	SOT1_1	P12/AN02
//	SOT1_2	PF1
//	SCK1_0	P58
//	SCK1_1	P13/AN03
//	SCK1_2	PF2

//	SIN2_0	P72
//	SIN2_1	P24/AN29
//	SIN2_2	P17/AN07
//	SOT2_0	P73
//	SOT2_1	P25/AN28
//	SOT2_2	P18/AN08
//	SCK2_0	P74
//	SCK2_1	P26/AN27
//	SCK2_2	P19/AN09

//	SIN3_0	P75
//	SIN3_1	P50
//	SIN3_2	P48
//	SOT3_0	P76
//	SOT3_1	P51
//	SOT3_2	P49
//	SCK3_0	P77
//	SCK3_1	P52
//	SCK3_2	P4A

//	SIN4_0	PD2
//	SIN4_1	P1A/AN10
//	SIN4_2	P05
//	SOT4_0	PD1
//	SOT4_1	P1B/AN11
//	SOT4_2	P06
//	SCK4_0	PD0
//	SCK4_1	P1C/AN12
//	SCK4_2	P07

//	SIN5_0	P60
//	SIN5_1	P92
//	SIN5_2	P36
//	SOT5_0	P61
//	SOT5_1	P93
//	SOT5_2	P37
//	SCK5_0	P62
//	SCK5_1	P94
//	SCK5_2	P38

//	SIN6_0	P53
//	SIN6_1	P33
//	SIN6_2	PF3
//	SOT6_0	P54
//	SOT6_1	P32
//	SOT6_2	PF4
//	SCK6_0	P55
//	SCK6_1	P31
//	SCK6_2	PF5

//	SIN7_0	P59
//	SIN7_1	P4E
//	SIN7_2	PB0/AN16
//	SOT7_0	P5A
//	SOT7_1	P4D
//	SOT7_2	PB1/AN17
//	SCK7_0	P5B
//	SCK7_1	P4C
//	SCK7_2	PB2/AN18

/*
 *  Register Bit Definition
 */
#define SMR_SOE          0x01U
#define SMR_BDS          0x04U
#define SMR_SBL          0x08U
#define SMR_WUCR         0x10U
#define SMR_MD_UART      0x00U
#define SMR_MD_UART_MP   0x20U
#define SMR_MD_SIO       0x40U
#define SMR_MD_LIN       0x60U
#define SMR_MD_I2C       0x80U

#define SCR_TXE          0x01U
#define SCR_RXE          0x02U
#define SCR_TBIE         0x04U
#define SCR_TIE          0x08U
#define SCR_RIE          0x10U
#define SCR_UPGL         0x80U

#define SSR_TBI          0x01U
#define SSR_TDRE         0x02U
#define SSR_RDRF         0x04U
#define SSR_ORE          0x08U
#define SSR_FRE          0x10U
#define SSR_PE           0x20U
#define SSR_REC          0x80U

#define ESCR_P           0x08U
#define ESCR_PEN         0x10U
#define ESCR_INV         0x20U
#define ESCR_ESBL        0x40U
#define ESCR_FLWEN       0x80U
#define ESCR_DATABITS_8  0x00U
#define ESCR_DATABITS_5  0x01U
#define ESCR_DATABITS_6  0x02U
#define ESCR_DATABITS_7  0x03U
#define ESCR_DATABITS_9  0x04U

#define BGR_EXT          0x8000U

#define FCR1_FSEL        0x01U
#define FCR1_FTIE        0x02U
#define FCR1_FDRQ        0x04U
#define FCR1_FRIIE       0x08U
#define FCR1_FLSTE       0x10U

#define FCR0_FE1         0x01U
#define FCR0_FE2         0x02U
#define FCR0_FCL1        0x04U
#define FCR0_FCL2        0x08U
#define FCR0_FSET        0x10U
#define FCR0_FLD         0x20U
#define FCR0_FLST        0x40U

void UART_Init(unsigned int ch, unsigned int BaudRate);
unsigned char UART_Rx(unsigned int ch);
void UART_TxStr(unsigned int ch, unsigned char *p);

#ifdef __cplusplus
}
#endif

#endif /* FM3_UART_H_ */
