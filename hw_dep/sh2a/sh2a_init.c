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

void LED_Init(void)
{
    PORT.PCIOR0.WORD |= 0x0100u;
    PORT.PCCR2.WORD &= 0xfff0;
    PORT.PCDR0.BIT.PC8DR = 1;       // LED OFF
}

void LED_On(void)
{
    PORT.PCDR0.BIT.PC8DR = 0;       // LED ON
}

void LED_Off(void)
{
    PORT.PCDR0.BIT.PC8DR = 1;       // LED OFF
}

void LED_Toggle(void)
{
    PORT.PCDR0.BIT.PC8DR ^= 1;      // LED OFF
}

void Serial_Init(void)
{
    SCI_Init(SCI_CH, SCI_BAUD);
}

unsigned char Serial_Rx(void)
{
    return SCI_Rx(SCI_CH);
}

void Serial_Tx(unsigned char c)
{
    SCI_Tx(SCI_CH, c);
}

void Serial_TxStr(unsigned char *p)
{
    SCI_TxStr(SCI_CH, p);
}

void sh2a_init(void)
{
    bootstrap();
    udelay_init();
    Serial_Init();
    LED_Init();
    Serial_TxStr("sh2a_init\r\n");
}

