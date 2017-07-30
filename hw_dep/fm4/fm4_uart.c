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

#include "fm4_config.h"
#include "fm4_uart.h"

//#define BITBAND
#define UART_BUF_SIZE   128

struct UART_FIFO {
    int ri, wi, ct, run;
    unsigned char buff[UART_BUF_SIZE];
};

#ifdef MB9BF568R

#define MFS0RX_IRQHandler	MFS0_RX_IRQHandler
#define MFS1RX_IRQHandler	MFS1_RX_IRQHandler
#define MFS2RX_IRQHandler	MFS2_RX_IRQHandler
#define MFS3RX_IRQHandler	MFS3_RX_IRQHandler
#define MFS4RX_IRQHandler	MFS4_RX_IRQHandler
#define MFS5RX_IRQHandler	MFS5_RX_IRQHandler
#define MFS6RX_IRQHandler	MFS6_RX_IRQHandler
#define MFS7RX_IRQHandler	MFS7_RX_IRQHandler
#define MFS0TX_IRQHandler	MFS0_TX_IRQHandler
#define MFS1TX_IRQHandler	MFS1_TX_IRQHandler
#define MFS2TX_IRQHandler	MFS2_TX_IRQHandler
#define MFS3TX_IRQHandler	MFS3_TX_IRQHandler
#define MFS4TX_IRQHandler	MFS4_TX_IRQHandler
#define MFS5TX_IRQHandler	MFS5_TX_IRQHandler
#define MFS6TX_IRQHandler	MFS6_TX_IRQHandler
#define MFS7TX_IRQHandler	MFS7_TX_IRQHandler

#define MFS0_UART   FM4_MFS0
#define MFS1_UART   FM4_MFS1
#define MFS2_UART   FM4_MFS2
#define MFS3_UART   FM4_MFS3
#define MFS4_UART   FM4_MFS4
#define MFS5_UART   FM4_MFS5
#define MFS6_UART   FM4_MFS6
#define MFS7_UART   FM4_MFS7

#ifndef UART47
#define MFS03_UART_TypeDef FM4_MFS_TypeDef
#define MFS47_UART_TypeDef FM4_MFS_TypeDef

MFS03_UART_TypeDef *UART[] = {
    MFS0_UART,
    MFS1_UART,
    MFS2_UART,
    MFS3_UART
};
#else
MFS47_UART_TypeDef *UART[] = {
    MFS4_UART,
    MFS5_UART,
    MFS6_UART,
    MFS7_UART
};
#endif
#elif defined(MB9BF618T) || defined(MB9BF506N)
#ifndef UART47
#define MFS0_UART   FM3_MFS0_UART
#define MFS1_UART   FM3_MFS1_UART
#define MFS2_UART   FM3_MFS2_UART
#define MFS3_UART   FM3_MFS3_UART
#define MFS4_UART   FM3_MFS4_UART
#define MFS5_UART   FM3_MFS5_UART
#define MFS6_UART   FM3_MFS6_UART
#define MFS7_UART   FM3_MFS7_UART

#define MFS03_UART_TypeDef FM3_MFS03_UART_TypeDef
#define MFS47_UART_TypeDef FM3_MFS47_UART_TypeDef

MFS03_UART_TypeDef *UART[] = {
    MFS0_UART,
    MFS1_UART,
    MFS2_UART,
    MFS3_UART
};
#else
MFS47_UART_TypeDef *UART[] = {
    MFS4_UART,
    MFS5_UART,
    MFS6_UART,
    MFS7_UART
};
#endif
#else
#error "Err: CPU type not defined!"
#endif

volatile struct UART_FIFO TxFifo, RxFifo;

void delay_ms(volatile unsigned int ms)
{
    ms *= 1000;
    while (ms-- > 0)
        ;
}

#ifndef UART47
void USART_RX(int ch, MFS03_UART_TypeDef *uart)
#else
void USART_RX(int ch, MFS47_UART_TypeDef *uart)
#endif
{
    int i;
    if ((uart->SSR & SSR_RDRF) != 0) {
        unsigned short c = (unsigned short)(uart->RDR);
        if (RxFifo.ct < UART_BUF_SIZE) {
            i = RxFifo.wi;
            RxFifo.buff[i++] = c;
            RxFifo.wi = i % UART_BUF_SIZE;
            RxFifo.ct++;
        }
    } else {
        uart->SSR = uart->SSR | SSR_REC;
    }
}

#ifndef UART47
void USART_TX(int ch, MFS03_UART_TypeDef *uart)
#else
void USART_TX(int ch, MFS47_UART_TypeDef *uart)
#endif
{
    int i;
    char c;
    if ((uart->SSR & SSR_TDRE) != 0) {
        if (TxFifo.ct) {
            i = TxFifo.ri;
#if 1
            c = TxFifo.buff[i++];
            uart->TDR = (unsigned short)c;
#else
            uart->TDR = TxFifo.buff[i++];
#endif
            TxFifo.ri = i % UART_BUF_SIZE;
            TxFifo.ct--;
        } else {
            TxFifo.run = 0;
            uart->SCR = uart->SCR & ~(SCR_TIE);
        }
    }
}

#ifndef UART47
void USART_Handle_IRQ(int ch, MFS03_UART_TypeDef *uart)
#else
void USART_Handle_IRQ(int ch, MFS47_UART_TypeDef *uart)
#endif
{
    if ((uart->SSR & (SSR_RDRF | SSR_ORE | SSR_FRE | SSR_PE)) != 0) {
        USART_RX(ch, uart);
    } else {
        USART_TX(ch, uart);
    }
}

#ifdef __GCC__
void MFS0RX_IRQHandler(void *param) __attribute__ ((interrupt));
void MFS1RX_IRQHandler(void *param) __attribute__ ((interrupt));
void MFS2RX_IRQHandler(void *param) __attribute__ ((interrupt));
void MFS3RX_IRQHandler(void *param) __attribute__ ((interrupt));
void MFS0TX_IRQHandler(void *param) __attribute__ ((interrupt));
void MFS1TX_IRQHandler(void *param) __attribute__ ((interrupt));
void MFS2TX_IRQHandler(void *param) __attribute__ ((interrupt));
void MFS3TX_IRQHandler(void *param) __attribute__ ((interrupt));
void MFS4RX_IRQHandler(void *param) __attribute__ ((interrupt));
void MFS5RX_IRQHandler(void *param) __attribute__ ((interrupt));
void MFS6RX_IRQHandler(void *param) __attribute__ ((interrupt));
void MFS7RX_IRQHandler(void *param) __attribute__ ((interrupt));
void MFS4TX_IRQHandler(void *param) __attribute__ ((interrupt));
void MFS5TX_IRQHandler(void *param) __attribute__ ((interrupt));
void MFS6TX_IRQHandler(void *param) __attribute__ ((interrupt));
void MFS7TX_IRQHandler(void *param) __attribute__ ((interrupt));
#endif

#ifndef UART47
void MFS0RX_IRQHandler(void *param)
{
    USART_Handle_IRQ(0, MFS0_UART);
}

void MFS1RX_IRQHandler(void *param)
{
    USART_Handle_IRQ(1, MFS1_UART);
}

void MFS2RX_IRQHandler(void *param)
{
    USART_Handle_IRQ(2, MFS2_UART);
}

void MFS3RX_IRQHandler(void *param)
{
    USART_Handle_IRQ(3, MFS3_UART);
}

void MFS0TX_IRQHandler(void *param)
{
    USART_Handle_IRQ(0, MFS0_UART);
}

void MFS1TX_IRQHandler(void *param)
{
    USART_Handle_IRQ(1, MFS1_UART);
}

void MFS2TX_IRQHandler(void *param)
{
    USART_Handle_IRQ(2, MFS2_UART);
}

void MFS3TX_IRQHandler(void *param)
{
    USART_Handle_IRQ(3, MFS3_UART);
}
#else
void MFS4RX_IRQHandler(void *param)
{
    USART_Handle_IRQ(4, MFS4_UART);
}

void MFS5RX_IRQHandler(void *param)
{
    USART_Handle_IRQ(5, MFS5_UART);
}

void MFS6RX_IRQHandler(void *param)
{
    USART_Handle_IRQ(6, MFS6_UART);
}

void MFS7RX_IRQHandler(void *param)
{
    USART_Handle_IRQ(7, MFS7_UART);
}

void MFS4TX_IRQHandler(void *param)
{
    USART_Handle_IRQ(4, MFS4_UART);
}

void MFS5TX_IRQHandler(void *param)
{
    USART_Handle_IRQ(5, MFS5_UART);
}

void MFS6TX_IRQHandler(void *param)
{
    USART_Handle_IRQ(6, MFS6_UART);
}

void MFS7TX_IRQHandler(void *param)
{
    USART_Handle_IRQ(7, MFS7_UART);
}
#endif

void UARTFIFO_Init(void)
{
    TxFifo.ct = 0;
    TxFifo.ri = 0;
    TxFifo.wi = 0;
    TxFifo.run = 0;
    RxFifo.ct = 0;
    RxFifo.ri = 0;
    RxFifo.wi = 0;
    RxFifo.run = 0;
}

void UART_TxEnable(unsigned int ch)
{
#ifndef UART47
    MFS03_UART_TypeDef *uart = UART[ch];
#else
    MFS47_UART_TypeDef *uart = UART[ch-4];
#endif
    uart->SCR |= (SCR_TIE);
}

void UART_TxDisable(unsigned int ch)
{
#ifndef UART47
    MFS03_UART_TypeDef *uart = UART[ch];
#else
    MFS47_UART_TypeDef *uart = UART[ch-4];
#endif
    uart->SCR &= ~(SCR_TIE);
}
void UART_RxEnable(unsigned int ch)
{
#ifndef UART47
    MFS03_UART_TypeDef *uart = UART[ch];
#else
    MFS47_UART_TypeDef *uart = UART[ch-4];
#endif
    uart->SCR |= (SCR_RIE);
}
void UART_RxDisable(unsigned int ch)
{
#ifndef UART47
    MFS03_UART_TypeDef *uart = UART[ch];
#else
    MFS47_UART_TypeDef *uart = UART[ch-4];
#endif
    uart->SCR &= ~(SCR_RIE);
}

void UART_Init(unsigned int ch, unsigned int BaudRate)
{
    unsigned short bgr;
#ifndef UART47
    volatile MFS03_UART_TypeDef *puart = UART[ch];
#else
    volatile MFS47_UART_TypeDef *puart = UART[ch-4];
#endif
    switch(ch) {
    case 0:
#if defined(MB9BF568R)
        FM4_GPIO->ADE &= ~0x00030000;   // Disable AN16, AN17
        FM4_GPIO->PFR2 |= 0x0006;// SIN0_0, SOT0_0, SCK0_0
        FM4_GPIO->EPFR07 |= 0x00000050;// SIN0_0, SOT0_0, SCK0_0
#elif defined(MB9BF618T)
        FM3_GPIO->ADE &= ~0x80000000;   // Disable AN31
#elif defined(MB9BF506N)
        FM3_GPIO->ADE &= ~0x8000;       // Disable AN15
#endif
#if defined(MB9BF618T) || defined(MB9BF506N)
#ifndef BITBAND
        FM3_GPIO->PFR2 |= 0x0006;       // SIN0_0, SOT0_0, SCK0_0
        FM3_GPIO->EPFR07 |= 0x00000050;// SIN0_0, SOT0_0, SCK0_0
#else
        bFM3_GPIO_PFR2_P1 = 1;
        bFM3_GPIO_EPFR07_SIN0S0 = 1;
        bFM3_GPIO_EPFR07_SIN0S1 = 0;
        bFM3_GPIO_PFR2_P2 = 1;
        bFM3_GPIO_EPFR07_SOT0B0 = 1;
        bFM3_GPIO_EPFR07_SOT0B1 = 0;
#endif
#endif
        break;
    case 3:
#if defined(MB9BF618T) || defined(MB9BF506N)
#ifndef BITBAND
        FM3_GPIO->PFR4 |= 0x0300;       // SIN3_2, SOT3_2
        FM3_GPIO->EPFR07 |= 0x03C00000;// SIN3_2, SOT3_2
#else
        bFM3_GPIO_PFR4_P8 = 1;
        bFM3_GPIO_EPFR07_SIN3S0 = 1;
        bFM3_GPIO_EPFR07_SIN3S1 = 1;
        bFM3_GPIO_PFR4_P9 = 1;
        bFM3_GPIO_EPFR07_SOT3B0 = 1;
        bFM3_GPIO_EPFR07_SOT3B1 = 1;
#endif
#endif
        break;
    case 4:
#if defined(MB9BF618T) || defined(MB9BF506N)
#ifndef BITBAND
        FM3_GPIO->PFR0 |= 0x0060;       // SIN4_2, SOT4_2, SCK4_2
        FM3_GPIO->EPFR08 |= 0x000000F0;// SIN4_2, SOT4_2, SCK4_2
#else
// ToDo:
        bFM3_GPIO_PFR0_P5 = 1;// P05 SIN4_2
        bFM3_GPIO_EPFR08_SIN4B0 = 1;
        bFM3_GPIO_EPFR08_SIN4B1 = 1;
        bFM3_GPIO_PFR0_P6 = 1;// P06 SOT4_2
        bFM3_GPIO_EPFR08_SOT4B0 = 1;
        bFM3_GPIO_EPFR08_SOT4B1 = 1;
#endif
#endif
        break;
    default:
        break;
    }
    puart->SMR = (SMR_MD_UART | SMR_SOE);
    puart->SCR = 0x00;
    bgr = (unsigned short)(((SysFrePCLK2 + ((unsigned int)BaudRate / 2)) / (unsigned int)BaudRate) - 1);
    puart->BGR = bgr;
    puart->SCR = puart->SCR & ~SCR_UPGL;
    puart->SSR = 0;
    //puart->BGR = (unsigned short)(((SysFrePCLK2 + ((unsigned int)BaudRate / 2)) / (unsigned int)BaudRate) - 1);
    puart->ESCR = (ESCR_DATABITS_8);
    UARTFIFO_Init();
#if defined(MB9BF568R)
    NVIC_EnableIRQ((IRQn_Type)(MFS0_TX_IRQn + (ch << 1)));
    //NVIC_SetPriority((IRQn_Type)(MFS0_TX_IRQn + (ch << 1)), FM3_UART_TX_PRIORITY);
    NVIC_EnableIRQ((IRQn_Type)(MFS0_RX_IRQn + (ch << 1)));
    //NVIC_SetPriority((IRQn_Type)(MFS0_RX_IRQn + (ch << 1)), FM3_UART_RX_PRIORITY);
#endif
#if defined(MB9BF618T) || defined(MB9BF506N)
    NVIC_EnableIRQ((IRQn_Type)(MFS0TX_IRQn + (ch << 1)));
    NVIC_SetPriority((IRQn_Type)(MFS0TX_IRQn + (ch << 1)), FM3_UART_TX_PRIORITY);
    NVIC_EnableIRQ((IRQn_Type)(MFS0RX_IRQn + (ch << 1)));
    NVIC_SetPriority((IRQn_Type)(MFS0RX_IRQn + (ch << 1)), FM3_UART_RX_PRIORITY);
#endif
    puart->SCR = puart->SCR | (SCR_RXE | SCR_TXE | SCR_RIE | SCR_TIE);
}

unsigned char UART_Rx(unsigned int ch)
{
    unsigned char c;
    int i;
    //while (!RxFifo.ct) ;
    if (RxFifo.ct) {
        UART_RxDisable(ch);
        i = RxFifo.ri;
        c = RxFifo.buff[i++];
        RxFifo.ri = i % UART_BUF_SIZE;
        RxFifo.ct--;
        UART_RxEnable(ch);
    } else
        c = 0;
    return c;
}

void UART_Tx(unsigned int ch, unsigned char c)
{
    int i;
#ifndef UART47
    volatile MFS03_UART_TypeDef *puart = UART[ch];
#else
    volatile MFS47_UART_TypeDef *puart = UART[ch-4];
#endif
    while (TxFifo.ct >= UART_BUF_SIZE)
        ;
    UART_TxDisable(ch);
    if (TxFifo.run) {
        i = TxFifo.wi;
        TxFifo.buff[i++] = c;
        TxFifo.wi = i % UART_BUF_SIZE;
        TxFifo.ct++;
    } else {
        puart->TDR = c;
        TxFifo.run = 1;
    }
    UART_TxEnable(ch);
}

void UART_TxStr(unsigned int ch, unsigned char *p)
{
    unsigned char c;
    while ((c = *p++) != 0) {
        if (c == '\n')
            UART_Tx(ch, '\r');
        UART_Tx(ch, c);
    }
}
