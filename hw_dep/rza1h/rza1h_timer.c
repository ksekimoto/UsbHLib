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
#include "MBRZA1H.h"
#include "rza1h_timer.h"
#include "iodefine.h"

#define CPG_STBCR5_BIT_MSTP50   (0x01u) /* OSTM1 */
#define OS_TIMER_CH         1
#define OS_TIMER_IRQn       (OSTMI1TINT_IRQn)
#define OS_TIMER_INTERVAL   666

volatile unsigned int os_timer_count[] = {0, 0};

static void os_timer(unsigned int ch)
{
    os_timer_count[ch] += 1;
}

void OS_TIMER0(void) { os_timer(0); }
void OS_TIMER1(void) { os_timer(1); }

void timer_init(unsigned int ch)
{
    os_timer_count[ch] = 0;
    CPGSTBCR5 &= ~(CPG_STBCR5_BIT_MSTP50);
    OSTM1TT   = (unsigned char)0x01;            // counter stop
    OSTM1CTL  = (unsigned char)0x00;            // interval timer mode
    OSTM1CMP  = (unsigned int)(OS_TIMER_INTERVAL);
    InterruptHandlerRegister(OS_TIMER_IRQn, (IRQHandler)OS_TIMER1);
    GIC_SetPriority(OS_TIMER_IRQn, 4);
    GIC_EnableIRQ(OS_TIMER_IRQn);
    OSTM1TS   = (unsigned char)0x01;
}

void udelay_init(void)
{
    timer_init(OS_TIMER_CH);
}

void udelay(int m)
{
#if 0
    volatile unsigned int start;
    if (m >= 10) {
        m /= 10;
        start = os_timer_count[OS_TIMER_CH];
        while (os_timer_count[OS_TIMER_CH] - start < m) {
            __asm__ __volatile__ ("nop");
        }
    } else {
        while (m-- > 0) {
            __asm__ __volatile__ ("nop");
        }
    }
#else
    m *= 100;
    while (m-- > 0) {
        __asm__ __volatile__ ("nop");
    }
#endif
}
