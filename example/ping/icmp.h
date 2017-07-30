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

#ifndef ICMP_H_
#define ICMP_H_

// ===== From lwip icmp.h =====

#define ICMP_ER 0      /* echo reply */
#define ICMP_DUR 3     /* destination unreachable */
#define ICMP_SQ 4      /* source quench */
#define ICMP_RD 5      /* redirect */
#define ICMP_ECHO 8    /* echo */
#define ICMP_TE 11     /* time exceeded */
#define ICMP_PP 12     /* parameter problem */
#define ICMP_TS 13     /* timestamp */
#define ICMP_TSR 14    /* timestamp reply */
#define ICMP_IRQ 15    /* information request */
#define ICMP_IR 16     /* information reply */

enum icmp_dur_type {
    ICMP_DUR_NET = 0, /* net unreachable */
    ICMP_DUR_HOST = 1, /* host unreachable */
    ICMP_DUR_PROTO = 2, /* protocol unreachable */
    ICMP_DUR_PORT = 3, /* port unreachable */
    ICMP_DUR_FRAG = 4, /* fragmentation needed and DF set */
    ICMP_DUR_SR = 5 /* source route failed */
};

enum icmp_te_type {
    ICMP_TE_TTL = 0, /* time to live exceeded in transit */
    ICMP_TE_FRAG = 1 /* fragment reassembly time exceeded */
};

// ===== end of lwip icmp.h =====

struct icmp_echo_hdr {
    unsigned char type;
    unsigned char code;
    unsigned short chksum;
    unsigned short id;
    unsigned short seqno;
};

#define IP_PROTO_ICMP	0x01

unsigned short icmp_checksum(struct icmp_echo_hdr *icmphdr);
void ICMP_ECHO_REPLY_Packet(struct eth_packet *ep);
void ICMP_Packet(struct eth_packet *ep, unsigned char *srcmac, unsigned char *srcip, unsigned char *dstmac, unsigned char *dstip, unsigned char type, unsigned char code, unsigned short id, unsigned short seqno);

#endif /* ICMP_H_ */
