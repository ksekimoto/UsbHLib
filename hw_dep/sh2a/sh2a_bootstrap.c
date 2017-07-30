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

void bootstrap(void)
{
    CPG.FRQCR.WORD = 0x1104;    // SH7262
    // Clock Mode 1 -> USB 48MHz x 1/4 -> 12MHz
    // STC = 1 -> x12 PLL
    // IFC = 0 -> x1  Internal Clock
    // PFC = 4 -> x1/6
    // Clock-in = 12MHz
    // I-clock = 144MHz
    // B-clock = 48MHz (CKIO = USB Clock)
    // P-clock = 24MHz
    INTC.IBNR.WORD = 0x0000;        // disable RESBANK for GDBSTUB
    CPG.STBCR3.BYTE = 0x02;
    CPG.STBCR4.BYTE = 0x00;
    CPG.STBCR5.BYTE = 0x10;
    CPG.STBCR6.BYTE = 0x02;
    CPG.STBCR7.BYTE = 0x2a;
    CPG.STBCR8.BYTE = 0x7e;
}

