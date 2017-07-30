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
#include "arp.h"

unsigned short checksum(unsigned short *p, unsigned int size, unsigned int checksum_offset);

unsigned short arp_checksum(struct eth_packet *ep)
{
    return checksum((unsigned short *)ep, ARP_PACKET_SIZE + ARP_PADDING_SIZE, 0);
}

// APR�p�P�b�g�ݒ�
void ARP_Packet(struct eth_packet *ep, unsigned short opcode, unsigned char *srcmac, unsigned char *srcip, unsigned char *dstmac, unsigned char *dstip)
{
    int i;
    unsigned char *padding;
    unsigned short *fcs;
    struct etharp_hdr *eahdr = (struct etharp_hdr *)&ep->data;
    for (i = 0; i < 6; i++) {
        if (opcode == 1)
            ep->ehdr.dst.addr[i] = 0xff;
        else
            ep->ehdr.dst.addr[i] = dstmac[i];
    }
    for (i = 0; i < 6; i++) {
        ep->ehdr.src.addr[i] = srcmac[i];
    }
    ep->ehdr.type = htons(0x0806);
    eahdr->hwtype = htons(0x0001);
    eahdr->proto = htons(0x0800);
    eahdr->_hwlen_protolen = htons(0x0604);
    eahdr->opcode = htons(opcode);
    for (i = 0; i < 6; i++)
        eahdr->shwaddr.addr[i] = srcmac[i];
    for (i = 0; i < 4; i++)
        eahdr->sipaddr.addr[i] = srcip[i];
    for (i = 0; i < 6; i++) {
        if (opcode == 1)
            eahdr->dhwaddr.addr[i] = 0x00;
        else
            eahdr->dhwaddr.addr[i] = dstmac[i];
    }
    for (i = 0; i < 4; i++)
        eahdr->dipaddr.addr[i] = dstip[i];
#ifdef ARP_PADDING
    padding = (unsigned char *)&ep->data + sizeof(struct etharp_hdr);
    for (i = 0; i < ARP_PADDING_SIZE; i++)
        *padding++ = 0;
    //fcs = (unsigned short *)padding;
    //*fcs = htons(arp_checksum(ep));
#endif
}
