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
#include "eth.h"
#include "ip.h"
#include "icmp.h"

// �`�F�b�N�T���v�Z
unsigned short checksum(unsigned short *p, unsigned int size, unsigned int checksum_offset)
{
    int i;
    unsigned int checksum = 0;
    for (i = 0; i < size; i++) {
        if (i == checksum_offset) {
            p++;
            continue;
        }
        checksum += ntohs(*p++);
    }
    checksum = (checksum & 0xffff) + (checksum >> 16);
    return ~((unsigned short)checksum);
}

// IP�`�F�b�N�T���v�Z
unsigned short ip_checksum(struct ip_hdr *iphdr)
{
    return checksum((unsigned short *)iphdr, 10, 5);
}

// ICMP�`�F�b�N�T���v�Z
// FM3 Ether�̎����v�Z���g�p����̂ŁAICMP�`�F�b�N�T���̌v�Z�͎g�p���Ȃ�
unsigned short icmp_checksum(struct icmp_echo_hdr *icmphdr)
{
    return checksum((unsigned short *)icmphdr, 20, 1);
}

// ICMP�p�P�b�g�ݒ�
void ICMP_Packet(struct eth_packet *ep, unsigned char *srcmac, unsigned char *srcip, unsigned char *dstmac, unsigned char *dstip, unsigned char type, unsigned char code, unsigned short id, unsigned short seqno)
{
    int i;
    struct ip_hdr *iphdr = (struct ip_hdr *)ep->data;
    struct icmp_echo_hdr *icmphdr = (struct icmp_echo_hdr *)((unsigned int)iphdr + sizeof(struct ip_hdr));
    for (i = 0; i < 6; i++) {
        ep->ehdr.dst.addr[i] = dstmac[i];
    }
    for (i = 0; i < 6; i++) {
        ep->ehdr.src.addr[i] = srcmac[i];
    }
    ep->ehdr.type = htons(0x0800);
    iphdr->v_hl_tos = htons(0x4500);
    iphdr->len = htons(0x3c);
    iphdr->id = htons(0xcfc6);
    iphdr->offset = htons(0x0000);
    iphdr->ttl_proto = htons(0x8001);
    memcpy((void *)&iphdr->src.addr, (void *)srcip, 6);
    memcpy((void *)&iphdr->dst.addr, (void *)dstip, 6);
//#if defined(FM3_ETHER)
//    iphdr->chksum = 0;  // FM3 Ether engine automatically calculates and inserts checksum.
//#else
    iphdr->chksum = htons(ip_checksum(iphdr));
//#endif
    icmphdr->type = type;
    icmphdr->code = code;
    icmphdr->id = id;
    icmphdr->seqno = seqno;
//#if defined(FM3_ETHER)
//    icmphdr->chksum = 0;    // FM3 Ether engine automatically calculates and inserts checksum.
//#else
    icmphdr->chksum = htons(icmp_checksum(icmphdr));
//#endif
}

// ICMP ECHO REPLY Packet�ݒ�
void ICMP_ECHO_REPLY_Packet(struct eth_packet *ep)
{
    struct eth_addr tmp_mac;
    struct ip_addr tmp_ip;
    struct ip_hdr *iphdr = (struct ip_hdr *)ep->data;
    struct icmp_echo_hdr *icmphdr = (struct icmp_echo_hdr *)((unsigned int)iphdr + sizeof(struct ip_hdr));
    memcpy((void *)&tmp_mac.addr, (void *)&ep->ehdr.src.addr, 6);
    memcpy((void *)&ep->ehdr.src.addr, (void *)&ep->ehdr.dst.addr, 6);
    memcpy((void *)&ep->ehdr.dst.addr, (void *)&tmp_mac.addr, 6);
    memcpy((void *)&tmp_ip.addr, (void *)&iphdr->src.addr, 4);
    memcpy((void *)&iphdr->src.addr, (void *)&iphdr->dst.addr, 4);
    memcpy((void *)&iphdr->dst.addr, (void *)&tmp_ip.addr, 4);
//#if defined(FM3_ETHER)
//    iphdr->chksum = 0;      // FM3 Ether engine automatically calculates and inserts checksum.
//#else
//    iphdr->chksum = htons(ip_checksum(iphdr));
//#endif
    icmphdr->type = ICMP_ER;
//#if defined(FM3_ETHER)
//    icmphdr->chksum = 0;    // FM3 Ether engine automatically calculates and inserts checksum.
//#else
    icmphdr->chksum = htons(icmp_checksum(icmphdr));
//#endif
}
