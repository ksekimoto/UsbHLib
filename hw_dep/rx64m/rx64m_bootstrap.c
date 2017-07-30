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

#include "iodefine.h"

#define BSP_CFG_XTAL_HZ (12000000)
#define BSP_CFG_PLL_DIV (1)
#define BSP_CFG_PLL_MUL (20.0)
#define BSP_CFG_CLOCK_SOURCE 4

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

static void clock_source_select(void)
{
    /* Main clock will be not oscillate in software standby or deep software standby modes. */
    SYSTEM.MOFCR.BIT.MOFXIN = 0;
    /* Resonator is used for clock input. */
    SYSTEM.MOFCR.BIT.MOSEL = 0;
    /* Use HOCO if HOCO is chosen or if PLL is chosen with HOCO as source. */
    /* Stop HOCO. */
    SYSTEM.HOCOCR.BYTE = 0x01;
    /* Turn off power to HOCO. */
    SYSTEM.HOCOPCR.BYTE = 0x01;
    /* Use Main clock if Main clock is chosen or if PLL is chosen with Main clock as source. */
    /* Main clock oscillator is chosen. Start it operating. */
    /* If the main oscillator is >10MHz then the main clock oscillator forced oscillation control register (MOFCR) must
     be changed. */
    if (BSP_CFG_XTAL_HZ > 20000000) {
        /* 20 - 24MHz. */
        SYSTEM.MOFCR.BIT.MODRV2 = 0;
    } else if (BSP_CFG_XTAL_HZ > 16000000) {
        /* 16 - 20MHz. */
        SYSTEM.MOFCR.BIT.MODRV2 = 1;
    } else if (BSP_CFG_XTAL_HZ > 8000000) {
        /* 8 - 16MHz. */
        SYSTEM.MOFCR.BIT.MODRV2 = 2;
    } else {
        /* 8MHz. */
        SYSTEM.MOFCR.BIT.MODRV2 = 3;
    }
    /* Wait 5.4 ms.
     Waiting time (sec) = {[(setting of the MSTS[7:0] bits * 32) - 16]}/fLOCO
     Waiting time (sec) = ((41 * 32) - 16) / 240kHz
     Waiting time (sec) = 5.4ms
     */
    SYSTEM.MOSCWTCR.BYTE = 0x29;
    /* Set the main clock to operating. */
    SYSTEM.MOSCCR.BYTE = 0x00;
    if (0x00 == SYSTEM.MOSCCR.BYTE) {
        /* Dummy read and compare. cf."5. I/O Registers", "(2) Notes on writing to I/O registers" in User's manual.
         This is done to ensure that the register has been written before the next register access. The RX has a
         pipeline architecture so the next instruction could be executed before the previous write had finished. */
        __asm__ __volatile__ ("nop");
    }
    while (0 == SYSTEM.OSCOVFSR.BIT.MOOVF) {
        /* The delay period needed is to make sure that the Main clock has stabilized. */
    }
    /* Set the sub-clock to stopped. */
    SYSTEM.SOSCCR.BYTE = 0x01;
    if (0x01 == SYSTEM.SOSCCR.BYTE) {
        /* Dummy read and compare. cf."5. I/O Registers", "(2) Notes on writing to I/O registers" in User's manual.
         This is done to ensure that the register has been written before the next register access. The RX has a
         pipeline architecture so the next instruction could be executed before the previous write had finished. */
        __asm__ __volatile__ ("nop");
    }
    /* Set PLL Input Divisor. */
    SYSTEM.PLLCR.BIT.PLIDIV = BSP_CFG_PLL_DIV - 1;
    /* Clear PLL clock source if PLL clock source is Main clock.  */
    SYSTEM.PLLCR.BIT.PLLSRCSEL = 0;
    /* Set PLL Multiplier. */
    SYSTEM.PLLCR.BIT.STC = ((uint8_t)((float)BSP_CFG_PLL_MUL * 2.0)) - 1;
    /* Set the PLL to operating. */
    SYSTEM.PLLCR2.BYTE = 0x00;
    while (0 == SYSTEM.OSCOVFSR.BIT.PLOVF) {
        /* The delay period needed is to make sure that the PLL has stabilized. */
    }
}

static void usb_clock_init(void)
{
    /* Used for constructing value to write to SCKCR register. */
    uint32_t temp_clock = 0;
    /*
     Default settings:
     Clock Description              Frequency
     ----------------------------------------
     Input Clock Frequency............  24 MHz
     PLL frequency (x10).............. 240 MHz
     Internal Clock Frequency......... 120 MHz
     Peripheral Clock Frequency A..... 120 MHz
     Peripheral Clock Frequency B.....  60 MHz
     Peripheral Clock Frequency C.....  60 MHz
     Peripheral Clock Frequency D.....  60 MHz
     External Bus Clock Frequency..... 120 MHz
     Flash IF Clock Frequency.........  60 MHz
     USB Clock Frequency..............  48 MHz */

    /* Protect off. */
    SYSTEM.PRCR.WORD = 0xA50B;
    /* Select the clock based upon user's choice. */
    clock_source_select();
    /* Figure out setting for FCK bits. 4 */
    temp_clock |= 0x20000000;
    /* Figure out setting for ICK bits. 2 */
    temp_clock |= 0x01000000;
    /* Figure out setting for BCK bits. 2 */
    //temp_clock |= 0x00010000;
    /* Figure out setting for BCK bits. 4 */
    temp_clock |= 0x00020000;
    /* Configure PSTOP1 bit for BCLK output. */
    /* Set PSTOP1 bit 0 */
    temp_clock |= 0x00800000;
    /* Configure PSTOP0 bit for SDCLK output. */
    /* Set PSTOP0 bit 0 */
    //temp_clock |= 0x00400000;
    temp_clock |= 0x00000000;
    /* Figure out setting for PCKA bits. 2 */
    temp_clock |= 0x00001000;
    /* Figure out setting for PCKB bits. 4 */
    temp_clock |= 0x00000200;
    /* Figure out setting for PCKC bits. 4 */
    temp_clock |= 0x00000020;
    /* Figure out setting for PCKD bits. 4 */
    temp_clock |= 0x00000002;
    /* Set SCKCR register. */
    SYSTEM.SCKCR.LONG = temp_clock;
    /* Re-init temp_clock to use to set SCKCR2. */
    temp_clock = 0;
    /* Figure out setting for UCK bits. 5 */
    temp_clock |= 0x00000041;
    /* Set SCKCR2 register. */
    SYSTEM.SCKCR2.WORD = (uint16_t)temp_clock;
    /* Choose clock source. Default for r_bsp_config.h is PLL. */
    SYSTEM.SCKCR3.WORD = ((uint16_t)BSP_CFG_CLOCK_SOURCE) << 8;
    /* We can now turn LOCO off since it is not going to be used. */
    SYSTEM.LOCOCR.BYTE = 0x01;
    SYSTEM.MSTPCRB.BIT.MSTPB19 = 0u;    //  Enable USB0 module
    //SYSTEM.MSTPCRB.BIT.MSTPB12 = 0u;    //  Enable USBA module
    /* Protect on. */
    //SYSTEM.PRCR.WORD = 0xA500;
}

void bootstrap(void)
{
    //SYSTEM.SCKCR.LONG = 0x21032200;     /* clock init: ICK=PLL/2, BCLK=PLL/8, PCLK=PLL/4 */
    usb_clock_init();
    MPC.PWPR.BIT.B0WI = 0;
    MPC.PWPR.BIT.PFSWE = 1;

    MPC.PFCSE.BIT.CS3E = 1;
    MPC.PFCSS0.BIT.CS3S = 2;
    MPC.PFAOE1.BIT.A17E = 1;

    BSC.CS3CR.WORD = 0x0001 | (2 << 4);
    BSC.CS3MOD.WORD = 0x8001;
    BSC.CS3WCR1.LONG = 0x01010101;
    BSC.CS3WCR2.LONG = 0x11110111;
    BSC.CS3REC.WORD = 0x0000;
}

