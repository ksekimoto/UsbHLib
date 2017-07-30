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

#ifndef FM3_CONFIG_H_
#define FM3_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

//#define USE_DBG_PRINT

///////////////////////////////////////////////////////////////////////////////
// FM3 clock configuration
//
// BSC_PSR_Val      0x00000000    // 0 = HCLK = Master Clock
// APBC0_PSR_Val    0x00000001    // 1 = PCLK0 = HCLK / 2
// APBC1_PSR_Val    0x00000081    // 1 = PCLK1 = HCLK / 2
// APBC2_PSR_Val    0x00000081    // 1 = PCLK2 = HCLK / 2
//
// TYPE0 - MB9BF506
// PLL_CTL1_Val     0x00000000    // 1 = Division(PLLM) = 1/2
// PLL_CTL2_Val     0x00000013    // 36 = Division(PLLN) = 1/20 -> 80MHz
//
// TYPE2 - MB9BF618
// PLL_CTL1_Val     0x00000001    // 1 = Division(PLLM) = 1/2
// PLL_CTL2_Val     0x00000023    // 36 = Division(PLLN) = 1/36
///////////////////////////////////////////////////////////////////////////////

// TYPE0 - MB9BF506
// CLK_4M:  (K,N,M)=(1,20,1)    -> 80MHz
// CLK_16M: (K,N,M)=(1,5,1)     -> 80MHz

// TYPE2 - MB9BF618
// CLL_4M:  (K,N,M)=(1,36,2)    -> 144MHz

#if defined(MB9BF568R)
#if !defined(CONFIG_FM4)
#define CONFIG_FM4
#endif
#define SYSTEM_CYCLE_CLOCK_HZ   160000000
#define PERIOD  (SYSTEM_CYCLE_CLOCK_HZ/1000)    // 1ms
#define SysFrePCLK2 (SYSTEM_CYCLE_CLOCK_HZ/2)
#define UART_CH 0
#define UART_BAUD 	115200
#elif defined(MB9BF618T)
#if !defined(CONFIG_FM3)
#define CONFIG_FM3
#endif
#define SYSTEM_CYCLE_CLOCK_HZ   144000000
#define PERIOD  (SYSTEM_CYCLE_CLOCK_HZ/1000)    // 1ms
#define SysFrePCLK2 (SYSTEM_CYCLE_CLOCK_HZ/2)
#define UART_CH 0
#define UART_BAUD 	115200
#elif defined(MB9BF506N)
#if !defined(CONFIG_FM3)
#define CONFIG_FM3
#endif
#define SYSTEM_CYCLE_CLOCK_HZ   80000000
#define PERIOD  (SYSTEM_CYCLE_CLOCK_HZ/1000)    // 1ms
#define SysFrePCLK2 (SYSTEM_CYCLE_CLOCK_HZ/2)
#define UART_CH 0
#define UART_BAUD 	115200
#else
#error "Err: CPU type not defined!"
#endif

#define DEBUG_CH UART_CH
#define DEBUG_TXSTR UART_TxStr

void fm3_init(void);
void bootstrap(void);
void Init_SysTick(void);
void Serial_Init(void);
unsigned char Serial_Rx(void);
void Serial_Tx(unsigned char c);
void Serial_TxStr(unsigned char *p);

#ifdef __cplusplus
}
#endif

#endif /* FM3_CONFIG_H_ */
