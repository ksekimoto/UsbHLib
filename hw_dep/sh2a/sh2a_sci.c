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

#define SCI_BUF_SIZE    128

#define SCIF_CR_TFRST       (0x0004)
#define SCIF_CR_RFRST       (0x0002)
#define SCIF_SCR_TIE        (0x0080)
#define SCIF_SCR_RIE        (0x0040)
#define SCIF_SCR_TE         (0x0020)
#define SCIF_SCR_RE         (0x0010)
#define SCIF_SCR_REIE       (0x0008)
#define SCIF_SCR_CKE1       (0x0002)
#define SCIF_SCR_CKE0       (0x0001)

#define SCFSR_TDFE  0x0020  // TX FIFO data empty
#define SCFSR_RDF   0x0002  // RX FIFO full
#define SCFSR_DR    0x0001  // RX Data Ready

struct SH7262_SCI_FIFO {
    volatile int ri, wi, ct;
    volatile unsigned char buff[SCI_BUF_SIZE];
};

typedef volatile struct st_scif SH7262_SCI_TypeDef;

SH7262_SCI_TypeDef *SCI[] = { (SH7262_SCI_TypeDef *)0xFFFE8000,
        (SH7262_SCI_TypeDef *)0xFFFE8800, (SH7262_SCI_TypeDef *)0xFFFE9000,
        (SH7262_SCI_TypeDef *)0xFFFE9800 };

volatile struct SH7262_SCI_FIFO TxFifo, RxFifo;

static void delay_ms(volatile unsigned int ms)
{
    ms *= 1000;
    while (ms-- > 0)
        ;
}

void SH7262_SCI_RX(int ch)
{
    int i;
    unsigned char c;
    SH7262_SCI_TypeDef *sci = SCI[ch];
    if ((sci->SCFSR.WORD & 0x0002) != 0) {
        c = (unsigned char)(sci->SCFRDR.BYTE);
        sci->SCFSR.WORD &= ~(SCFSR_RDF | SCFSR_DR);
        if (RxFifo.ct < SCI_BUF_SIZE) {
            i = RxFifo.wi;
            RxFifo.buff[i++] = c;
            RxFifo.wi = i % SCI_BUF_SIZE;
            RxFifo.ct++;
        }
    }
}

void SH7262_SCI_TX(int ch)
{
    int i;
    char c;
    SH7262_SCI_TypeDef *sci = SCI[ch];
    if (TxFifo.ct) {
        i = TxFifo.ri;
        c = TxFifo.buff[i++];
        sci->SCFTDR.BYTE = (unsigned short)c;
        sci->SCFSR.WORD &= ~SCFSR_TDFE;    // clear TDFE
        TxFifo.ri = i % SCI_BUF_SIZE;
        TxFifo.ct--;
    } else {
        sci->SCSCR.WORD &= ~SCIF_SCR_TIE;
    }
}

void SH7262_SCI_BR(int ch)
{
}

void SH7262_SCI_ER(int ch)
{
}

void INT_SCIF0_RXI0()
{
    SH7262_SCI_RX(0);
}

void INT_SCIF0_RXI1()
{
    SH7262_SCI_RX(1);
}

void INT_SCIF0_RXI2()
{
    SH7262_SCI_RX(2);
}

void INT_SCIF0_RXI3()
{
    SH7262_SCI_RX(3);
}

void INT_SCIF0_TXI0()
{
    SH7262_SCI_TX(0);
}

void INT_SCIF0_TXI1()
{
    SH7262_SCI_TX(1);
}

void INT_SCIF0_TXI2()
{
    SH7262_SCI_TX(2);
}

void INT_SCIF0_TXI3()
{
    SH7262_SCI_TX(3);
}

void INT_SCIF0_BRI0()
{

}

void INT_SCIF0_BRI1()
{

}

void INT_SCIF0_BRI2()
{

}

void INT_SCIF0_BRI3()
{

}

void INT_SCIF0_ERI0()
{

}

void INT_SCIF0_ERI1()
{

}

void INT_SCIF0_ERI2()
{

}

void INT_SCIF0_ERI3()
{

}

void SCIFIFO_Init(void)
{
    TxFifo.ct = 0;
    TxFifo.ri = 0;
    TxFifo.wi = 0;
    RxFifo.ct = 0;
    RxFifo.ri = 0;
    RxFifo.wi = 0;
}

void SCI_TxEnable(unsigned int ch)
{
    SH7262_SCI_TypeDef *sci = SCI[ch];
    sci->SCSCR.WORD |= SCIF_SCR_TIE;
}

void SCI_TxDisable(unsigned int ch)
{
    SH7262_SCI_TypeDef *sci = SCI[ch];
    sci->SCSCR.WORD &= ~SCIF_SCR_TIE;
}
void SCI_RxEnable(unsigned int ch)
{
    SH7262_SCI_TypeDef *sci = SCI[ch];
    sci->SCSCR.WORD |= (SCIF_SCR_RIE);
}
void SCI_RxDisable(unsigned int ch)
{
    SH7262_SCI_TypeDef *sci = SCI[ch];
    sci->SCSCR.WORD &= ~(SCIF_SCR_RIE);
}

void SCI_Init(int ch, int baud)
{
    volatile unsigned int i;
    SH7262_SCI_TypeDef *psci = SCI[ch];
    SCIFIFO_Init();
    switch(ch) {
    case 0:
        CPG.STBCR4.BYTE &= 0x7F;
        PORT.PJCR0.WORD = 0x44;
        INTC.IPR17.WORD |= 0xD000;
        break;
    case 1:
        CPG.STBCR4.BYTE &= 0x7F;
        PORT.PGCR3.WORD = 0x0044;
        INTC.IPR17.WORD |= 0x0D00;
        break;
    case 2:
        CPG.STBCR4.BYTE &= 0xDF;
        PORT.PFCR0.WORD = 0x0440;
        INTC.IPR17.WORD |= 0x00D0;
        break;
    case 3:
    default:
        CPG.STBCR4.BYTE &= 0xEF;
        PORT.PFCR0.WORD = (PORT.PFCR0.WORD & 0x0FFF) | 0x4000;
        PORT.PFCR1.WORD = (PORT.PFCR1.WORD & 0xFFF0) | 0x0004;
        INTC.IPR17.WORD |= 0x000D;
        break;
    }
    psci->SCSCR.WORD = 0;           // SCSCR Clear
    psci->SCFCR.WORD = SCIF_CR_TFRST | SCIF_CR_RFRST;  // Reset the TX/RX FIFOs'
    psci->SCFSR.WORD &= ~0x0091;    // clear ER, DR, BRK
    psci->SCLSR.WORD = 0x0000;      // clear LSR
    psci->SCSMR.WORD = 0;
    psci->SCEMR.WORD = 0x81;
    psci->SCBRR.BYTE = 25;      // 115200bps
    i = 31250; // 8 ns/instruction * 4 instructions * loop_count = 1ms = 1000000 ns
    while (i--)
        ;    // loop count = 31250
    psci->SCFCR.WORD = 0x0030;  // FIFO Transmit Trigger
    psci->SCSCR.WORD |= (SCIF_SCR_RIE | SCIF_SCR_RE | SCIF_SCR_TE);
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
    volatile SH7262_SCI_TypeDef *psci = SCI[ch];
    while (TxFifo.ct >= SCI_BUF_SIZE)
        ;
    if (TxFifo.ct < SCI_BUF_SIZE) {
        SCI_TxDisable(ch);
        i = TxFifo.wi;
        TxFifo.buff[i++] = c;
        TxFifo.wi = i % SCI_BUF_SIZE;
        TxFifo.ct++;
        SCI_TxEnable(ch);
    }
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
