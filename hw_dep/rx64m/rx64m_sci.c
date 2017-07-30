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
#include "interrupt_handlers.h"
#include "iodefine.h"

#define SCI_BUF_SIZE 4096

volatile struct st_sci0 *SCI[] = { (volatile struct st_sci0 *)0x8A000, /* sci 0 */
(volatile struct st_sci0 *)0x8A020, /* sci 1 */
(volatile struct st_sci0 *)0x8A040, /* sci 2 */
(volatile struct st_sci0 *)0x8A060, /* sci 3 */
(volatile struct st_sci0 *)0x8A080, /* sci 4 */
(volatile struct st_sci0 *)0x8A0A0, /* sci 5 */
(volatile struct st_sci0 *)0x8A0C0, /* sci 6 */
(volatile struct st_sci0 *)0x8A0E0, /* sci 7 */
(volatile struct st_sci0 *)0x8A100, /* sci 8 */
(volatile struct st_sci0 *)0x8A120, /* sci 9 */
(volatile struct st_sci0 *)0x8A140, /* sci 10 */
(volatile struct st_sci0 *)0x8A160, /* sci 11 */
(volatile struct st_sci0 *)0x8B300 /* sci 12 */
};

static volatile struct SCI_FIFO {
    int ri, wi, ct, run;
    unsigned char buff[SCI_BUF_SIZE];
} TxFifo, RxFifo;

static volatile struct SCI_FIFO TxFifo;
static volatile struct SCI_FIFO RxFifo;

static void delay_ms(volatile unsigned int ms)
{
    ms *= 1000;
    while (ms-- > 0)
        ;
}

static void sci_isr_rx(int ch)
{
    int i;
    unsigned char d;
    volatile struct st_sci0 *sci = SCI[ch];
    d = sci->RDR;
    if (RxFifo.ct < SCI_BUF_SIZE) {
        i = RxFifo.wi;
        RxFifo.buff[i++] = d;
        RxFifo.wi = i % SCI_BUF_SIZE;
        RxFifo.ct++;
    }
}

static void sci_isr_er(int ch)
{
    volatile struct st_sci0 *sci = SCI[ch];
    sci->RDR;
    sci->SSR.BYTE = 0x84;
}

static void sci_isr_tx(int ch)
{
    int i;
    volatile struct st_sci0 *sci = SCI[ch];
    if (TxFifo.ct) {
        i = TxFifo.ri;
        sci->TDR = TxFifo.buff[i++];
        TxFifo.ri = i % SCI_BUF_SIZE;
        TxFifo.ct--;
    } else {
        TxFifo.run = 0;
    }
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
    sci_isr_tx(7);
}

void SCI_RxEnable(int ch)
{
    switch(ch) {
    case 0:
        IEN(SCI0, RXI0)= 1;
        break;
        case 1:
        IEN(SCI1, RXI1) = 1;
        break;
        case 2:
        IEN(SCI2, RXI2) = 1;
        break;
        case 3:
        IEN(SCI3, RXI3) = 1;
        break;
        case 4:
        IEN(SCI4, RXI4) = 1;
        break;
        case 5:
        IEN(SCI5, RXI5) = 1;
        break;
        case 6:
        IEN(SCI6, RXI6) = 1;
        break;
        case 7:
        IEN(SCI7, RXI7) = 1;
        break;
        default:
        break;
    }
}

void SCI_RxDisable(int ch)
{
    switch(ch) {
    case 0:
        IEN(SCI0, RXI0)= 0;
        break;
        case 1:
        IEN(SCI1, RXI1) = 0;
        break;
        case 2:
        IEN(SCI2, RXI2) = 0;
        break;
        case 3:
        IEN(SCI3, RXI3) = 0;
        break;
        case 4:
        IEN(SCI4, RXI4) = 0;
        break;
        case 5:
        IEN(SCI5, RXI5) = 0;
        break;
        case 6:
        IEN(SCI6, RXI6) = 0;
        break;
        case 7:
        IEN(SCI7, RXI7) = 0;
        break;
        default:
        break;
    }
}

void SCI_TxEnable(int ch)
{
    switch(ch) {
    case 0:
        IEN(SCI0, TXI0)= 1;
        break;
        case 1:
        IEN(SCI1, TXI1) = 1;
        break;
        case 2:
        IEN(SCI2, TXI2) = 1;
        break;
        case 3:
        IEN(SCI3, TXI3) = 1;
        break;
        case 4:
        IEN(SCI4, TXI4) = 1;
        break;
        case 5:
        IEN(SCI5, TXI5) = 1;
        break;
        case 6:
        IEN(SCI6, TXI6) = 1;
        break;
        case 7:
        IEN(SCI7, TXI7) = 1;
        break;
        default:
        break;
    }
}

void SCI_TxDisable(int ch)
{
    switch(ch) {
    case 0:
        IEN(SCI0, TXI0)= 0;
        break;
        case 1:
        IEN(SCI1, TXI1) = 0;
        break;
        case 2:
        IEN(SCI2, TXI2) = 0;
        break;
        case 3:
        IEN(SCI3, TXI3) = 0;
        break;
        case 4:
        IEN(SCI4, TXI4) = 0;
        break;
        case 5:
        IEN(SCI5, TXI5) = 0;
        break;
        case 6:
        IEN(SCI6, TXI6) = 0;
        break;
        case 7:
        IEN(SCI7, TXI7) = 0;
        break;
        default:
        break;
    }
}

unsigned char SCI_Rx(int ch)
{
    unsigned char c;
    int i;
    //while (!RxFifo.ct) ;
    if (RxFifo.ct) {
        SCI_RxDisable(ch);
        i = RxFifo.ri;
        c = RxFifo.buff[i++];
        RxFifo.ri = i % SCI_BUF_SIZE;
        RxFifo.ct--;
        SCI_RxEnable(ch);
    } else
        c = 0;
    return c;
}

void SCI_Tx(int ch, unsigned char c)
{
    int i;
    volatile struct st_sci0 *sci = SCI[ch];
    while (TxFifo.ct >= SCI_BUF_SIZE)
        ;
    SCI_TxDisable(ch);
    if (TxFifo.run) {
        i = TxFifo.wi;
        TxFifo.buff[i++] = c;
        TxFifo.wi = i % SCI_BUF_SIZE;
        TxFifo.ct++;
    } else {
        sci->TDR = c;
        TxFifo.run = 1;
    }
    SCI_TxEnable(ch);
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

void sci_int_enable(int ch)
{
    switch(ch) {
    case 0:
        IPR(SCI0, RXI0)= 3;
        IEN(SCI0, RXI0) = 1;
        IPR(SCI0, TXI0) = 2;
        IEN(SCI0, TXI0) = 1;
        break;
        case 1:
        IPR(SCI1, RXI1) = 3;
        IEN(SCI1, RXI1) = 1;
        IPR(SCI1, TXI1) = 2;
        IEN(SCI1, TXI1) = 1;
        break;
        case 2:
        IPR(SCI2, RXI2) = 3;
        IEN(SCI2, RXI2) = 1;
        IPR(SCI2, TXI2) = 2;
        IEN(SCI2, TXI2) = 1;
        break;
        case 3:
        IPR(SCI3, RXI3) = 3;
        IEN(SCI3, RXI3) = 1;
        IPR(SCI3, TXI3) = 2;
        IEN(SCI3, TXI3) = 1;
        break;
        case 4:
        IPR(SCI4, RXI4) = 3;
        IEN(SCI4, RXI4) = 1;
        IPR(SCI4, TXI4) = 2;
        IEN(SCI4, TXI4) = 1;
        break;
        case 5:
        IPR(SCI5, RXI5) = 3;
        IEN(SCI5, RXI5) = 1;
        IPR(SCI5, TXI5) = 2;
        IEN(SCI5, TXI5) = 1;
        break;
        case 6:
        IPR(SCI6, RXI6) = 3;
        IEN(SCI6, RXI6) = 1;
        IPR(SCI6, TXI6) = 2;
        IEN(SCI6, TXI6) = 1;
        break;
        case 7:
        IPR(SCI7, RXI7) = 3;
        IEN(SCI7, RXI7) = 1;
        IPR(SCI7, TXI7) = 2;
        IEN(SCI7, TXI7) = 1;
        break;
        default:
        break;
    }
}

void SCI_Init(int ch, int baud)
{
    volatile struct st_sci0 *sci = SCI[ch];

    sci_fifo_init();
    SYSTEM.PRCR.WORD = 0xA502;
    MPC.PWPR.BIT.B0WI = 0u;   // enable write PFSWE bit
    MPC.PWPR.BIT.PFSWE = 1u;   // enable write PFS register
    switch(ch) {
    case 0:
        MSTP_SCI0 = 0;
        MPC.P20PFS.BYTE = 0x0a;     // P20 TX
        PORT2.PMR.BIT.B0 = 1;       // P20 Peripheral
        MPC.P21PFS.BYTE = 0x0a;     // P21 RX
        PORT2.PMR.BIT.B1 = 1;       // P21 Peripheral
        break;
    case 1:
        MSTP_SCI1 = 0;
        MPC.P15PFS.BYTE = 0x0A;     // RXD1
        MPC.P16PFS.BYTE = 0x0A;     // TXD1
        PORT1.PMR.BIT.B5 = 1;
        PORT1.PMR.BIT.B6 = 1;
        break;
    case 2:
        MSTP_SCI2 = 0;
        MPC.P12PFS.BYTE = 0x0A;     // RXD2
        MPC.P13PFS.BYTE = 0x0A;     // TDX2
        PORT1.PMR.BIT.B2 = 1;
        PORT1.PMR.BIT.B3 = 1;
        break;
    case 3:
        MSTP_SCI3 = 0;
        /* Set RXD3 pin */
        MPC.P25PFS.BYTE = 0x0AU;
        PORT2.PMR.BYTE |= 0x20U;
        /* Set TXD3 pin */
        PORT2.PODR.BYTE |= 0x08U;
        MPC.P23PFS.BYTE = 0x0AU;
        PORT2.PMR.BYTE |= 0x08U;
        PORT2.PDR.BYTE |= 0x08U;
        break;
    case 4:
        MSTP_SCI4 = 0;
        /* Set RXD4 pin */
        MPC.PB0PFS.BYTE = 0x0AU;
        PORTB.PMR.BYTE |= 0x01U;
        /* Set TXD4 pin */
        PORTB.PODR.BYTE |= 0x02U;
        MPC.PB1PFS.BYTE = 0x0AU;
        PORTB.PMR.BYTE |= 0x02U;
        PORTB.PDR.BYTE |= 0x02U;
        break;
    case 5:
        MSTP_SCI5 = 0;
        /* Set RXD5 pin */
        MPC.PA2PFS.BYTE = 0x0AU;
        PORTA.PMR.BYTE |= 0x04U;
        /* Set TXD5 pin */
        PORTA.PODR.BYTE |= 0x10U;
        MPC.PA4PFS.BYTE = 0x0AU;
        PORTA.PMR.BYTE |= 0x10U;
        PORTA.PDR.BYTE |= 0x10U;
        break;
    case 6:
        MSTP_SCI6 = 0;
        /* Set RXD6 pin */
        MPC.P01PFS.BYTE = 0x0AU;
        PORT0.PMR.BYTE |= 0x02U;
        /* Set TXD6 pin */
        PORT0.PODR.BYTE |= 0x01U;
        MPC.P00PFS.BYTE = 0x0AU;
        PORT0.PMR.BYTE |= 0x01U;
        PORT0.PDR.BYTE |= 0x01U;
        break;
    case 7:
        MSTP_SCI7 = 0;
        /* Set RXD7 pin */
        MPC.P92PFS.BYTE = 0x0AU;
        PORT9.PMR.BYTE |= 0x04U;
        /* Set TXD7 pin */
        PORT9.PODR.BYTE |= 0x01U;
        MPC.P90PFS.BYTE = 0x0AU;
        PORT9.PMR.BYTE |= 0x01U;
        PORT9.PDR.BYTE |= 0x01U;
        break;
    default:
        break;
    }
    SYSTEM.PRCR.WORD = 0xA500;
    sci->SCR.BYTE = 0;
    sci->SMR.BYTE = 0x00;
    if (baud != 0)
        sci->BRR = (unsigned char)((int)PCLK / baud / 32 - 1);
    else
        sci->BRR = (unsigned char)((int)PCLK / 115200 / 32 - 1);
    delay_ms(1);
    sci->SCR.BYTE = 0xF0;
    sci_int_enable(ch);
}
