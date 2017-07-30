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
#include "fm3_timer.h"

//#define USE_SYSTICK
#define TIMEDELAY_CALIBRATION 40
volatile unsigned long SysTick_Count = 0L;

void SysTick_Init()
{
    SysTick_Config(PERIOD);
}

unsigned long SysTick_Get_ticks(void)
{
    return SysTick_Count;
}

#ifdef __GNUC__
void SysTick_Handler(void) __attribute__((interrupt));
void SysTick_Handler(void)
#else
void SysTick_Handler(void)
#endif
{
#if 0
    static unsigned int LED_count = 0;
#endif
    SysTick_Count++;
#if 0
    if (LED_count++ >= 500) {
        LED_toggle();
        LED_count = 0;
    }
#endif
}

void mdelay(unsigned int ms)
{
    volatile unsigned long current_ms;
#ifdef USE_SYSTICK
    current_ms = SysTick_Get_ticks();
    while (SysTick_Get_ticks() - current_ms < (unsigned long)ms) {}
#else
    udelay(ms * 1000);
#endif
}

void udelay(unsigned int us)
{
    unsigned int CyclestoLoops;
    CyclestoLoops = SYSTEM_CYCLE_CLOCK_HZ / 1000000;
    CyclestoLoops *= us;
    if (CyclestoLoops <= TIMEDELAY_CALIBRATION)
        return;
    CyclestoLoops -= TIMEDELAY_CALIBRATION;
#if defined(MB9BF568R)
    CyclestoLoops /= 3; // cycle count per iteration
#elif defined(MB9BF618T)
    CyclestoLoops /= 3; // cycle count per iteration
#elif defined(MB9BF506N) || defined(MB9BF506R)
    CyclestoLoops /= 5; // cycle count per iteration
#else
#error "Err: CPU type not defined."
#endif
    if (!CyclestoLoops)
        return;

    __asm__ __volatile__ (
            " mov r3, %[loops]\n"
            " loop: subs r3, #1\n"
            " bne loop\n\n"
            : // No output registers
            : [loops] "r" (CyclestoLoops)// Input registers
            : "r3"// clobbered registers
    );
}

