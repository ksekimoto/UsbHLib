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

#include "common.h"
#include "iodefine.h"
#include "MBRZA1H.h"
#include "gic.h"
#include "rza1h_gpio.h"

#define SCI_BUF_SIZE 4096

volatile struct st_scif *SCI[] = { (volatile struct st_scif *)&SCIF0, /* sci 0 */
(volatile struct st_scif *)&SCIF1, /* sci 1 */
(volatile struct st_scif *)&SCIF2, /* sci 2 */
(volatile struct st_scif *)&SCIF3, /* sci 3 */
(volatile struct st_scif *)&SCIF4, /* sci 4 */
(volatile struct st_scif *)&SCIF5, /* sci 5 */
(volatile struct st_scif *)&SCIF6, /* sci 6 */
(volatile struct st_scif *)&SCIF7 /* sci 7 */
};

static volatile struct SCI_FIFO {
    int ri, wi, ct, run;
    unsigned char buff[SCI_BUF_SIZE];
} TxFifo, RxFifo;

static volatile struct SCI_FIFO TxFifo;
static volatile struct SCI_FIFO RxFifo;

static int SCI_RX_PIN[] = { 0x2f,       // CH0: P2_15
        0x26,       // CH1: P2_6
        0x62,       // CH2: P6_2
        0x54,       // CH3: P5_4
        0x8f,       // CH4: P8_15
        0x8b,       // CH5: P8_11
        0x51,       // CH6: P5_1
        -1          // CH7:
};

static int SCI_TX_PIN[] = { 0x2e,       // CH0: P2_14
        0x25,       // CH1: P2_5
        0x63,       // CH2: P6_3
        0x53,       // CH3: P5_3
        0x8e,       // CH4: P8_14
        0x8d,       // CH5: P8_13
        0x50,       // CH6: P5_0
        -1          // CH7:
};

static void delay_ms(volatile unsigned int ms)
{
    ms *= 1000;
    while (ms-- > 0)
        ;
}

static inline void SCI_RIEEnable(volatile struct st_scif *sci)
{
    sci->SCSCR |= 0x0040;
}

static inline void SCI_RIEDisable(volatile struct st_scif *sci)
{
    sci->SCSCR &= ~0x0040;
}

static inline void SCI_TIEEnable(volatile struct st_scif *sci)
{
    sci->SCSCR |= 0x0080;
}

static inline void SCI_TIEDisable(volatile struct st_scif *sci)
{
    sci->SCSCR &= ~0x0080;
}

void sci_int_enable(int ch)
{
    GIC_EnableIRQ(SCIFRXI0_IRQn + ch * 4);
    GIC_EnableIRQ(SCIFTXI0_IRQn + ch * 4);
}

void sci_int_disable(int ch)
{
    GIC_DisableIRQ(SCIFRXI0_IRQn + ch * 4);
    GIC_DisableIRQ(SCIFTXI0_IRQn + ch * 4);
}

static void sci_isr_br(int ch)
{
}

static void sci_isr_er(int ch)
{
    unsigned char dummy;
    volatile struct st_scif *sci = SCI[ch];
    sci->SCFRDR;
    dummy = sci->SCFSR;
    sci->SCFSR &= ~0x38;    // clear ORER, FER, PER
}

static void sci_isr_rx(int ch)
{
    int i;
    unsigned char d;
    volatile struct st_scif *sci = SCI[ch];
    if (sci->SCFSR & 0x02) {
        d = sci->SCFRDR;
        sci->SCFSR &= ~0x02;
        if (RxFifo.ct < SCI_BUF_SIZE) {
            i = RxFifo.wi;
            RxFifo.buff[i++] = d;
            RxFifo.wi = i % SCI_BUF_SIZE;
            RxFifo.ct++;
        }
    }
}

static void sci_isr_tx(int ch)
{
    int i;
    volatile struct st_scif *sci = SCI[ch];
    if (TxFifo.ct) {
        if (sci->SCFSR & 0x20) {
            i = TxFifo.ri;
            sci->SCFTDR = TxFifo.buff[i++];
            sci->SCFSR &= ~0x20;
            TxFifo.ri = i % SCI_BUF_SIZE;
            TxFifo.ct--;
        }
    } else {
        SCI_TIEDisable(sci);
        TxFifo.run = 0;
    }
}

void INT_Excep_SCI0_BRI0(void)
{
    sci_isr_br(0);
}
void INT_Excep_SCI1_BRI1(void)
{
    sci_isr_br(1);
}
void INT_Excep_SCI2_BRI2(void)
{
    sci_isr_br(2);
}
void INT_Excep_SCI3_BRI3(void)
{
    sci_isr_br(3);
}
void INT_Excep_SCI4_BRI4(void)
{
    sci_isr_br(4);
}
void INT_Excep_SCI5_BRI5(void)
{
    sci_isr_br(5);
}
void INT_Excep_SCI6_BRI6(void)
{
    sci_isr_br(6);
}
void INT_Excep_SCI7_BRI7(void)
{
    sci_isr_br(7);
}

void INT_Excep_SCI0_ERI0(void)
{
    sci_isr_er(0);
}
void INT_Excep_SCI1_ERI1(void)
{
    sci_isr_er(1);
}
void INT_Excep_SCI2_ERI2(void)
{
    sci_isr_er(2);
}
void INT_Excep_SCI3_ERI3(void)
{
    sci_isr_er(3);
}
void INT_Excep_SCI4_ERI4(void)
{
    sci_isr_er(4);
}
void INT_Excep_SCI5_ERI5(void)
{
    sci_isr_er(5);
}
void INT_Excep_SCI6_ERI6(void)
{
    sci_isr_er(6);
}
void INT_Excep_SCI7_ERI7(void)
{
    sci_isr_er(7);
}

void INT_Excep_SCI0_RXI0(void)
{
    sci_isr_rx(0);
}
void INT_Excep_SCI1_RXI1(void)
{
    sci_isr_rx(1);
}
void INT_Excep_SCI2_RXI2(void)
{
    sci_isr_rx(2);
}
void INT_Excep_SCI3_RXI3(void)
{
    sci_isr_rx(3);
}
void INT_Excep_SCI4_RXI4(void)
{
    sci_isr_rx(4);
}
void INT_Excep_SCI5_RXI5(void)
{
    sci_isr_rx(5);
}
void INT_Excep_SCI6_RXI6(void)
{
    sci_isr_rx(6);
}
void INT_Excep_SCI7_RXI7(void)
{
    sci_isr_rx(7);
}

void INT_Excep_SCI0_TXI0(void)
{
    sci_isr_tx(0);
}
void INT_Excep_SCI1_TXI1(void)
{
    sci_isr_tx(1);
}
void INT_Excep_SCI2_TXI2(void)
{
    sci_isr_tx(2);
}
void INT_Excep_SCI3_TXI3(void)
{
    sci_isr_tx(3);
}
void INT_Excep_SCI4_TXI4(void)
{
    sci_isr_tx(4);
}
void INT_Excep_SCI5_TXI5(void)
{
    sci_isr_tx(5);
}
void INT_Excep_SCI6_TXI6(void)
{
    sci_isr_tx(6);
}
void INT_Excep_SCI7_TXI7(void)
{
    sci_isr_tx(6);
}

IRQHandler SCI_BR_ISR[] = { (IRQHandler)INT_Excep_SCI0_BRI0,
        (IRQHandler)INT_Excep_SCI1_BRI1, (IRQHandler)INT_Excep_SCI2_BRI2,
        (IRQHandler)INT_Excep_SCI3_BRI3, (IRQHandler)INT_Excep_SCI4_BRI4,
        (IRQHandler)INT_Excep_SCI5_BRI5, (IRQHandler)INT_Excep_SCI6_BRI6,
        (IRQHandler)INT_Excep_SCI7_BRI7 };

IRQHandler SCI_ER_ISR[] = { (IRQHandler)INT_Excep_SCI0_ERI0,
        (IRQHandler)INT_Excep_SCI1_ERI1, (IRQHandler)INT_Excep_SCI2_ERI2,
        (IRQHandler)INT_Excep_SCI3_ERI3, (IRQHandler)INT_Excep_SCI4_ERI4,
        (IRQHandler)INT_Excep_SCI5_ERI5, (IRQHandler)INT_Excep_SCI6_ERI6,
        (IRQHandler)INT_Excep_SCI7_ERI7 };

IRQHandler SCI_RX_ISR[] = { (IRQHandler)INT_Excep_SCI0_RXI0,
        (IRQHandler)INT_Excep_SCI1_RXI1, (IRQHandler)INT_Excep_SCI2_RXI2,
        (IRQHandler)INT_Excep_SCI3_RXI3, (IRQHandler)INT_Excep_SCI4_RXI4,
        (IRQHandler)INT_Excep_SCI5_RXI5, (IRQHandler)INT_Excep_SCI6_RXI6,
        (IRQHandler)INT_Excep_SCI7_RXI7 };

IRQHandler SCI_TX_ISR[] = { (IRQHandler)INT_Excep_SCI0_TXI0,
        (IRQHandler)INT_Excep_SCI1_TXI1, (IRQHandler)INT_Excep_SCI2_TXI2,
        (IRQHandler)INT_Excep_SCI3_TXI3, (IRQHandler)INT_Excep_SCI4_TXI4,
        (IRQHandler)INT_Excep_SCI5_TXI5, (IRQHandler)INT_Excep_SCI6_TXI6,
        (IRQHandler)INT_Excep_SCI7_TXI7 };

unsigned char SCI_Rx(int ch)
{
    unsigned char c;
    int i;
    volatile struct st_scif *sci;
    //while (!RxFifo.ct) ;
    if (RxFifo.ct) {
        sci = SCI[ch];
        SCI_RIEDisable(sci);
        i = RxFifo.ri;
        c = RxFifo.buff[i++];
        RxFifo.ri = i % SCI_BUF_SIZE;
        RxFifo.ct--;
        SCI_RIEEnable(sci);
    } else
        c = 0;
    return c;
}

void SCI_Tx(int ch, unsigned char c)
{
    int i;
    volatile struct st_scif *sci = SCI[ch];
    while (TxFifo.ct >= SCI_BUF_SIZE)
        ;
    SCI_TIEDisable(sci);
    if (TxFifo.run) {
        i = TxFifo.wi;
        TxFifo.buff[i++] = c;
        TxFifo.wi = i % SCI_BUF_SIZE;
        TxFifo.ct++;
    } else {
        if (sci->SCFSR & 0x20) {
            sci->SCFTDR = c;
            sci->SCFSR &= ~0x20;
            TxFifo.run = 1;
        }
    }
    SCI_TIEEnable(sci);
}

void SCI_TxStr(int ch, unsigned char *p)
{
    unsigned char c;
    while ((c = *p++) != 0) {
        if (c == '\n')
            SCI_Tx(ch, '\r');
        SCI_Tx(ch, c);
    }
}

static void sci_fifo_init()
{
    TxFifo.ri = 0;
    TxFifo.wi = 0;
    TxFifo.ct = 0;
    TxFifo.run = 0;
    RxFifo.ri = 0;
    RxFifo.wi = 0;
    RxFifo.ct = 0;
}

void sci_power(int ch, int onoff)
{
    if (onoff) {
        CPG.STBCR4 &= ~(1 << (7 - ch));
    } else {
        CPG.STBCR4 |= (1 << (7 - ch));
    }
}

void sci_pin(int ch)
{
    int pin_rx = SCI_RX_PIN[ch];
    int pin_tx = SCI_TX_PIN[ch];
    int mask;
    switch(ch) {
    case 0:
        mask = _GPIO_MASK(pin_rx);
        GPIOPIBC2 &= ~mask;
        GPIOPBDC2 &= ~mask;
        GPIOPM2 |= mask;
        GPIOPMC2 &= ~mask;
        GPIOPIPC2 &= ~mask;

        // mode6
        GPIOPMC2 |= mask;
        GPIOPFCAE2 |= mask;
        GPIOPFCE2 &= ~mask;
        GPIOPFC2 |= mask;
        GPIOPM2 |= mask;
        GPIOPIPC2 |= mask;
        GPIOPMC2 |= mask;

        mask = _GPIO_MASK(pin_tx);
        GPIOPIBC2 &= ~mask;
        GPIOPBDC2 &= ~mask;
        GPIOPM2 |= mask;
        GPIOPMC2 &= ~mask;
        GPIOPIPC2 &= ~mask;

        // mode6
        GPIOPMC2 |= mask;
        GPIOPFCAE2 |= mask;
        GPIOPFCE2 &= ~mask;
        GPIOPFC2 |= mask;
        GPIOPM2 &= ~mask;
        GPIOPIPC2 |= mask;
        GPIOPMC2 |= mask;
        break;
    case 1:
    case 2:
        mask = _GPIO_MASK(pin_rx);
        GPIOPIBC6 &= ~mask;
        GPIOPBDC6 &= ~mask;
        GPIOPM6 |= mask;
        GPIOPMC6 &= ~mask;
        GPIOPIPC6 &= ~mask;

        // mode7
        GPIOPMC6 |= mask;
        GPIOPFCAE6 |= mask;
        GPIOPFCE6 |= mask;
        GPIOPFC6 &= ~mask;
        GPIOPM6 |= mask;
        GPIOPIPC6 |= mask;
        GPIOPMC6 |= mask;

        mask = _GPIO_MASK(pin_tx);
        GPIOPIBC6 &= ~mask;
        GPIOPBDC6 &= ~mask;
        GPIOPM6 |= mask;
        GPIOPMC6 &= ~mask;
        GPIOPIPC6 &= ~mask;

        // mode7
        GPIOPMC6 |= mask;
        GPIOPFCAE6 |= mask;
        GPIOPFCE6 |= mask;
        GPIOPFC6 &= ~mask;
        GPIOPM6 &= ~mask;
        GPIOPIPC6 |= mask;
        GPIOPMC6 |= mask;
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    default:
        break;
    }
}

#define SCI_PRIORITY    5

void SCI_Init(int ch, int baud)
{
    unsigned char dummy;
    volatile struct st_scif *sci = SCI[ch];
    sci_fifo_init();
    __disable_irq();
    sci_power(ch, 1);
    sci->SCSCR = 0x0000;    // stop operation
    sci->SCFCR = 0x0006;    // fifo reset
    dummy = sci->SCFSR;
    sci->SCFSR = (dummy & 0xff6c);  // clear error status and others
    dummy = sci->SCLSR;
    sci->SCLSR = (dummy & 0x0001);  // clear OR bit
    sci->SCEMR = 0x0000;
    sci->SCBRR = (unsigned short)((66666666 / (32 * baud)) - 1);
    sci->SCSMR = 0;         // 8bit, stop 1, non parity
    sci->SCFCR = 0x0030u;
    //sci->SCSPTR |= 0x0000u;
    sci->SCSCR = 0x0070;    // TIE,RIE,TE,RE on
    sci_pin(ch);
    GIC_SetPriority(SCIFBRI0_IRQn + ch * 4, SCI_PRIORITY);
    GIC_SetPriority(SCIFERI0_IRQn + ch * 4, SCI_PRIORITY);
    GIC_SetPriority(SCIFRXI0_IRQn + ch * 4, SCI_PRIORITY);
    GIC_SetPriority(SCIFTXI0_IRQn + ch * 4, SCI_PRIORITY);
    InterruptHandlerRegister(SCIFBRI0_IRQn + ch * 4, (IRQHandler)SCI_BR_ISR[ch]);
    InterruptHandlerRegister(SCIFERI0_IRQn + ch * 4, (IRQHandler)SCI_ER_ISR[ch]);
    InterruptHandlerRegister(SCIFRXI0_IRQn + ch * 4, (IRQHandler)SCI_RX_ISR[ch]);
    InterruptHandlerRegister(SCIFTXI0_IRQn + ch * 4, (IRQHandler)SCI_TX_ISR[ch]);
    sci_int_enable(ch);
    __enable_irq();
}
