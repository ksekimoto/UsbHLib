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

#include "rza1h_gpio.h"

// PIBC     port input buffer control
//          0:disable
// PBDC     port buffer D control
//          0:disable
// PM       port mode
//          0:output, 1:input
// PMC      port mode control
//          0:port mode, 1:alternate mode
// PIPC     port ip control
//          0:SW control, 1:alternate mode control
// PFCE     port function control extension
//          1:alternate mode
// PFCAE    port function control additional extension
//          1:alternate mode
// alternate
//  0: port mode
//  n: alternate n mode
// input
//  0: output
//  1: input
void GPIO_mode(int pin, int al_mode, int input, int al_input_ctrl)
{
    int port = _GPIO_PORT(pin);
    unsigned short mask = _GPIO_MASK(pin);
    // port initialize
    _PIBC(port) &= ~mask;
    _PBDC(port) &= ~mask;
    _PM(port) |= mask;
    _PMC(port) &= ~mask;
    _PIPC(port) &= ~mask;
    switch(al_mode) {
    case 0:
        //port mode
        _PMC(port) &= ~mask;
        break;
    case 1:
        _PMC(port) |= mask;
        _PFCAE(port) &= ~mask;
        _PFCE(port) &= ~mask;
        _PFC(port) &= ~mask;
        break;
    case 2:
        _PMC(port) |= mask;
        _PFCAE(port) &= ~mask;
        _PFCE(port) &= ~mask;
        _PFC(port) |= mask;
        break;
    case 3:
        _PMC(port) |= mask;
        _PFCAE(port) &= ~mask;
        _PFCE(port) |= mask;
        _PFC(port) &= ~mask;
        break;
    case 4:
        _PMC(port) |= mask;
        _PFCAE(port) &= ~mask;
        _PFCE(port) |= mask;
        _PFC(port) |= mask;
        break;
    case 5:
        _PMC(port) |= mask;
        _PFCAE(port) |= mask;
        _PFCE(port) &= ~mask;
        _PFC(port) &= ~mask;
        break;
    case 6:
        _PMC(port) |= mask;
        _PFCAE(port) |= mask;
        _PFCE(port) &= ~mask;
        _PFC(port) |= mask;
        break;
    case 7:
        _PMC(port) |= mask;
        _PFCAE(port) |= mask;
        _PFCE(port) |= mask;
        _PFC(port) &= ~mask;
        break;
    case 8:
        _PMC(port) |= mask;
        _PFCAE(port) |= mask;
        _PFCE(port) |= mask;
        _PFC(port) |= mask;
        break;
    default:
        break;
    }
    if (input) {
        _PM(port) |= mask;
        if (al_mode == 0) {
            _PIBC(port) |= mask;
        }
    } else
        _PM(port) &= ~mask;
    if (al_input_ctrl) {
        _PIPC(port) |= mask;
    }
}
