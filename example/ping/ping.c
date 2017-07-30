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
#include "arp.h"
#include "icmp.h"
#include "debug_printf.h"

int Packet_Receive(unsigned char *packet, int size);
void Packet_Send(unsigned char *packet, int size);
int Packet_Avail(void);

//#define DUMP_MEMORY
#define DUMP_ARP_SEND
//#define DUMP_ARP_RECEIVE
#define DUMP_UNKNOWN

#define PACKET_BUF_SIZE 1536
#define ICMP_PACKET_SIZE 74
#define CH_MAX  2

unsigned char SrcMACAddr[6] = { 0x00, 0x1d, 0x12, 0xb7, 0x1a, 0x1f };
unsigned char DstMACAddr[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
unsigned char SrcIPAddr[4] = { 192, 168, 0, 200 };
unsigned char DstIPAddr[4] = { 192, 168, 0, 2 };
//static unsigned char Subnet[4] = {255, 255, 255, 0};
//static unsigned char Gateway[4] = {192, 168, 0, 1};
static unsigned char DummyMACAddr[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
static unsigned char DstMACAddr_Initialized = FALSE;

unsigned char packet_buf[PACKET_BUF_SIZE];
struct eth_packet ep;

// memory dump
void Memory_Dump(unsigned long addr, int len)
{
    int i, j;
    char *p = (char *)addr;
    char *q = (char *)addr;
    for (i = 0; i < len; i++) {
        if ((i & 0xf) == 0) {
            debug_printf("%08x ", (unsigned long)p);
        }
        debug_printf("%02x ", (unsigned long)(*p));
        if (((i & 0xf) == 0xf) || (i == (len - 1))) {
            j = 16 - (i & 0xf);
            while (j-- > 0)
                debug_printf("   ");
            for (j = 0; j <= (i & 0xf); j++) {
                if (*q >= 0x20 && *q <= 0x7e) {
                    debug_printf("%c", *q);
                } else
                    debug_printf(" ");
                *q++;
            }
            debug_printf("\r\n");
        }
        p++;
    }
    debug_printf("\r\n");
}

// ARP Packet
// |DstMAC(6)|SrcMAC(6)|Type(2)|
// |HWType(2)|ProtoType(2)|HLenPLen(2)|OpCode(2)|SrcMAC(6)|SrcIP(4)|DstMAC(6)|DstIP(4)
// code
// 1: ARP Request
// 2: ARP Response
// 3: RARP Request
// 4: RARP Response
void ARP_Send(unsigned short code, unsigned char*sm, unsigned char*si, unsigned char *dm, unsigned char *di)
{
    ARP_Packet(&ep, code, sm, si, dm, di);
#ifdef DUMP_ARP_SEND
    Memory_Dump((unsigned long)&ep, ARP_PACKET_SIZE);
#endif
    Packet_Send((unsigned char *)&ep, ARP_PACKET_SIZE);
}

void ICMP_ECHO_REQUEST_Send(void)
{
    unsigned short id = 1;
    unsigned short seqno = 1;
    ICMP_Packet(&ep, SrcMACAddr, SrcIPAddr, DstMACAddr, DstIPAddr, ICMP_ECHO, 0, id, seqno);
    Packet_Send((unsigned char *)&ep, ICMP_PACKET_SIZE);
}

static void wait(volatile unsigned int count)
{
    while (count-- > 0)
        ;
}

void RXPacket_Process(void)
{
    unsigned int size;
    struct eth_packet *ethp;
    struct etharp_hdr *ethah;
    struct ip_hdr *iph;
    struct icmp_echo_hdr *icmphdr;
    unsigned char ip_proto;
    int target = 0;
    int broadcast = 0;

    while (Packet_Avail()) {
        size = Packet_Receive(&packet_buf, PACKET_BUF_SIZE);
        ethp = (struct eth_packet *)&packet_buf;
        target = 0;
        broadcast = 0;
        if (strncmp((void *)&ethp->ehdr.dst.addr, (void *)SrcMACAddr, 6) == 0) {
            target = 1;
        }
        if (strncmp((void *)&ethp->ehdr.dst.addr, (void *)DummyMACAddr, 6) == 0) {
            broadcast = 1;
        }
        if (target || broadcast) {
            switch(ntohs(ethp->ehdr.type)) {
            case 0x0800:
                iph = (struct ip_hdr *)&ethp->data;
                ip_proto = ntohs(iph->ttl_proto) & 0xff;
                //debug_printf("Receiving a IP packet\n");
                switch(ip_proto) {
                case IP_PROTO_ICMP:
                    icmphdr = (struct icmp_echo_hdr *)((unsigned int)iph + sizeof(struct ip_hdr));
                    if (icmphdr->type == ICMP_ECHO) {
                        debug_printf("Receiving Ping Packet\n");
#ifdef DUMP_MEMORY
                        Memory_Dump((unsigned long)ethp, size);
#endif
                        memcpy((void *)&ep, (void *)ethp, ICMP_PACKET_SIZE);
                        ICMP_ECHO_REPLY_Packet(&ep);
                        Packet_Send((unsigned char *)&ep, ICMP_PACKET_SIZE);
                        debug_printf("Sending Ping Packet\n");
#ifdef DUMP_MEMORY
                        Memory_Dump((unsigned long)&ep, size);
#endif
                    } else if (icmphdr->type == ICMP_ER) {
                        debug_printf("Receiving Ping Packet\n");
//#ifdef DUMP_MEMORY
//                        Memory_Dump((unsigned long)ethp, size);
//#endif
                    }
                    break;
                default:
                    debug_printf("protocol %02x\n", ip_proto);
                    break;
                }
                break;
            case 0x0806:
                ethah = (struct etharp_hdr *)&ethp->data;
                switch((int)ntohs(ethah->opcode)) {
                case ARP_REQ:
                    debug_printf("Receiving ARP packet\n");
                    if (strncmp(&ethah->dipaddr.addr[0], SrcIPAddr, 4) == 0) {
                        debug_printf("Who has %d.%d.%d.%d? Tell %d.%d.%d.%d?\n", ethah->dipaddr.addr[0], ethah->dipaddr.addr[1], ethah->dipaddr.addr[2], ethah->dipaddr.addr[3], DstIPAddr[0], DstIPAddr[1], DstIPAddr[2], DstIPAddr[3]);
                        ARP_Send(ARP_RES, SrcMACAddr, SrcIPAddr, DstMACAddr, DstIPAddr);
                    }
                    break;
                case ARP_RES:
                    debug_printf("Receiving ARP packet\n");
                    debug_printf("%d.%d.%d.%d is at %02x:%02x:%02x:%02x:%02x:%02x\n", DstIPAddr[0], DstIPAddr[1], DstIPAddr[2], DstIPAddr[3], ethah->shwaddr.addr[0], ethah->shwaddr.addr[1], ethah->shwaddr.addr[2], ethah->shwaddr.addr[3], ethah->shwaddr.addr[4], ethah->shwaddr.addr[5]);
                    memcpy((void *)DstMACAddr, (void *)&ethah->shwaddr.addr, 6);
                    DstMACAddr_Initialized = TRUE;
#ifdef DUMP_ARP_RECEIVE
                    Memory_Dump((unsigned long)ethp, size);
#endif
                    break;
                case RARP_REQ:
                    break;
                case RARP_RES:
                    break;
                default:
                    break;
                }
                break;
            default:
                debug_printf("Receiving a unknown packet\n");
//#ifdef DUMP_UNKNOWN
//                Memory_Dump((unsigned long)ethp, size);
//#endif
                break;
            }
        }
    }
}

unsigned char get_char(void)
{
    return SCI_Rx(SCI_CH);
}

void Menu_Disp(void)
{
    unsigned int i;

    i = 0;
    debug_printf("\n========= BP3595 Ping Test ===============\n");
    debug_printf("My  IP Addr: %d.%d.%d.%d (%02x:%02x:%02x:%02x:%02x:%02x)\n", SrcIPAddr[0], SrcIPAddr[1], SrcIPAddr[2], SrcIPAddr[3], SrcMACAddr[0], SrcMACAddr[1], SrcMACAddr[2], SrcMACAddr[3], SrcMACAddr[4], SrcMACAddr[5]);
    debug_printf("Dst IP Addr: %d.%d.%d.%d \n", DstIPAddr[0], DstIPAddr[1], DstIPAddr[2], DstIPAddr[3]);
    debug_printf("1. Send ARP Packet  -> 1 key\n");
    debug_printf("2. Send PING Packet -> 2 key\n");
    debug_printf("==================================================\n\n");
}

void set_mac_addr(unsigned char *mac_addr)
{
    memcpy((void *)&SrcMACAddr, (void *)mac_addr, 6);
}

void ping_test(void)
{
    unsigned char c;
    Menu_Disp();
    while (1) {
        c = get_char();
        if (c != 0) {
            debug_printf("%c key pushed\r\n", c);
            switch((int)c) {
            case '0':
                break;
            case '1':
                debug_printf("Sending ARP Packet\n");
                ARP_Send(ARP_REQ, SrcMACAddr, SrcIPAddr, DstMACAddr, DstIPAddr);
                break;
            case '2':
                while (DstMACAddr_Initialized == FALSE) {
                    ARP_Send(ARP_REQ, SrcMACAddr, SrcIPAddr, DstMACAddr, DstIPAddr);
                    RXPacket_Process();
                }
                debug_printf("Sending PING Packet\n");
                wait(10000);
                ICMP_ECHO_REQUEST_Send();
                //Memory_Dump((unsigned long)&ep, 74);
                break;
            default:
                break;
            }
        }
        RXPacket_Process();
    }
}
