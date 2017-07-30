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

#ifndef ETH_H_
#define ETH_H_
#include "ip.h"

#define ETHARP_HWADDR_LEN	6
#define ETH_PACKET_SIZE	1536

struct eth_addr {
    unsigned char addr[ETHARP_HWADDR_LEN];
};

struct eth_hdr {
    struct eth_addr dst;
    struct eth_addr src;
    unsigned short type;
};

struct eth_packet {
    struct eth_hdr ehdr;
    unsigned char data[ETH_PACKET_SIZE];
};

struct etharp_hdr {
    unsigned short hwtype;			// 00
    unsigned short proto;			// 02
    unsigned short _hwlen_protolen;	// 04
    unsigned short opcode;			// 06
    struct eth_addr shwaddr;		// 08
    struct ip_addr1 sipaddr;		// 0E
    struct eth_addr dhwaddr;		// 12
    struct ip_addr1 dipaddr;		// 18
};

unsigned short htons(unsigned short n);
unsigned short ntohs(unsigned short n);
#endif /* ETH_H_ */
