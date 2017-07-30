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

#include "sh2a_timer.h"

#include "iodefine.h"

#define TIMER_USE_INT
#define CLKDEV  8
// PCLK = 48000000Hz default
#if CLKDEV == 8
#define CMT_VAL 0x0040;     // CMIE is Enable,CKS is PCLK/8
#elif CLKDEV == 32
#define CMT_VAL 0x0041;     // CMIE is Enable,CKS is PCLK/32
#elif CLKDEV == 128
#define CMT_VAL 0x0042;     // CMIE is Enable,CKS is PCLK/128
#else
#define CMT_VAL 0x0043;     // CMIE is Enable,CKS is PCLK/512
#endif

#define DELAY_CH    0

struct st_cmt0 {
    union {
        unsigned short WORD;
        struct {
            unsigned short :9;
            unsigned short CMIE :1;
            unsigned short :4;
            unsigned short CKS :2;
        } BIT;
    } CMSCR;
    unsigned short CMCNT;
    unsigned short CMCOR;
};

volatile struct st_cmt0 *CMTN[2] = { (volatile struct st_cmt0 *)0xFFFEC002,
        (volatile struct st_cmt0 *)0xFFFEC008, };

volatile static unsigned long cmcnt[4] = { 0L, 0L, 0L, 0L };

static void isr_timer(unsigned int ch)
{
    volatile struct st_cmt0 *cmtn = CMTN[ch];
    cmcnt[ch] += 1L;
    cmtn->CMSCR.WORD &= ~0x80;
}

void INT_CMT0_CMI0(void)
{
    isr_timer(0);
}
void INT_CMT1_CMI1(void)
{
    isr_timer(1);
}

void timer_init(unsigned int ch)
{
    int i;
    volatile struct st_cmt0 *cmtn = CMTN[ch];
    for (i = 0; i < 2; i++)
        cmcnt[i] = 0L;
    CMT.CMSTR.WORD &= (ch == 0 ? 2 : 1);   // disable clock
    cmtn->CMSCR.WORD = 0;
    INTC.IPR10.WORD |= (ch == 0 ? 0x00e0 : 0x000e);
    cmtn->CMSCR.WORD = CMT_VAL
    ;             // 48Mhz/8 = 6MHz count
    cmtn->CMCNT = 0;
    cmtn->CMCOR = 0xffff;
    CMT.CMSTR.WORD |= (ch == 0 ? 1 : 2);   // enable clock
}

void timer_set_count(unsigned int ch, unsigned int count)
{
    volatile struct st_cmt0 *cmtn = CMTN[ch];
    cmtn->CMCNT = (unsigned short)0;
    do {
        cmtn->CMCOR = (unsigned short)count;
    } while (cmtn->CMCOR != (unsigned short)count);
}

void udelay_init(void)
{
    timer_init(DELAY_CH);
    timer_set_count(DELAY_CH, 60);      // 10us interrupt
}

void udelay(int m)
{
#if defined(TIMER_USE_INT)
    volatile unsigned long start;
    if (m >= 10) {
        m /= 10;
        start = cmcnt[DELAY_CH];
        while (cmcnt[DELAY_CH] - start < (unsigned long)m) {
            __asm__ __volatile__ ("nop");
        }
    } else {
        while (m-- > 0) {
            __asm__ __volatile__ ("nop");
        }
    }
#else
    m *= 10;
    while (m-- > 0) {
        __asm__ __volatile__ ("nop");
    }
#endif
}
