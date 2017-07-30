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

#define _GPIO_PORT(pin)  (pin >> 4)
#define _GPIO_MASK(pin)  (1 << (pin & 0xf))

#define _PORT(port) (*(volatile unsigned short *)(0xFCFE3000uL + (port << 2)))
#define _PSR(port)  (*(volatile unsigned int *)(0xFCFE3100uL + (port << 2)))
#define _PPR(port)  (*(volatile unsigned short *)(0xFCFE3200uL + (port << 2)))
#define _PM(port)   (*(volatile unsigned short *)(0xFCFE3300uL + (port << 2)))
#define _PMC(port)  (*(volatile unsigned short *)(0xFCFE3400uL + (port << 2)))
#define _PFC(port)  (*(volatile unsigned short *)(0xFCFE3500uL + (port << 2)))
#define _PFCE(port) (*(volatile unsigned short *)(0xFCFE3600uL + (port << 2)))
#define _PNOT(port) (*(volatile unsigned short *)(0xFCFE3700uL + (port << 2)))
#define _PMSR(port) (*(volatile unsigned int *)(0xFCFE3800uL + (port << 2)))
#define _PMCSR(pin) (*(volatile unsigned int *)(0xFCFE3900uL + (port << 2)))
#define _PFCAE(pin) (*(volatile unsigned short *)(0xFCFE3A00uL + (port << 2)))
#define _PIBC(pin)  (*(volatile unsigned short *)(0xFCFE7000uL + (port << 2)))
#define _PBDC(pin)  (*(volatile unsigned short *)(0xFCFE7100uL + (port << 2)))
#define _PIPC(pin)  (*(volatile unsigned short *)(0xFCFE7200uL + (port << 2)))

void GPIO_mode(int pin, int al_mode, int input, int al_input_ctrl);
