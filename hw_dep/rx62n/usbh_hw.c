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
#include "usbh_timer.h"
#include "usbh_hw.h"
#include "usbh_config.h"

#define USBH_USE_MSD

//#define USBH_POLLING
//#define USBH_DEBUG
//#define USBH_DEBUG_ERR
//#define USBH_DEBUG_FIFO
//#define USBH_RD
//#define USBH_WT
//#define USBH_DEBUG_INT
//#define USBH_DEBUG_INT_BRDY
//#define USBH_DEBUG_INT_BEMP
//#define USBH_DEBUG_INT_NRDY
//#define USBH_DEBUG_FIFO
//#define USBH_DEBUG_PIPE_PW
//#define USBH_DEBUG_PIPE_PR
//#define USBH_DEBUG_CS
//#define USBH_DEBUG_CO
//#define USBH_DEBUG_CI
//#define USBH_DEBUG_AO
//#define USBH_DEBUG_AI
//#define USBH_DEBUG_BO
//#define USBH_DEBUG_BI
//#define USBH_DEBUG_WAIT
//#define USBH_DEBUG_SETUP
//#define USBH_DEBUG_API_FUNC
//#define USBH_DEBUG_TOKEN
//#define USBH_DEBUG_DUMP_REGS_BO
//#define USBH_DEBUG_DUMP_REGS_BI
//#define USBH_DEBUG_TOKEN_ERR

static void usbh_pipe_set_pidnak(int pipe);
static inline void usbh_pipe_set_pidbuf(int pipe);

#define PIPE_CLEAR_FIFO

#ifdef USBH_DEBUG
#define dprintk(fmt, ...)   debug_printf("%s(): "fmt, __func__, ## __VA_ARGS__)
#else
#define dprintk(fmt, ...)
#endif

#ifdef USBH_DEBUG_API_FUNC
#define API_FUNC_ENTER()    debug_printf("%s enter\n", __func__)
#define API_FUNC_EXIT()     debug_printf("%s exit\n", __func__)
#else
#define API_FUNC_ENTER()
#define API_FUNC_EXIT()
#endif

#define MAX_PACKET  64
#define USBH_INT_LEVEL  14
#define PIPE0   0
#define BULK_IN_PIPENO  1
#define BULK_OUT_PIPENO 2
#define INT_IN_PIPENO   3
#define INT_OUT_PIPENO  4

#define DIR_R   0
#define DIR_W   1

#define ISEL_R  0
#define ISEL_W  1

#define PIPE_BULK   1
#define PIPE_INT    2
#define PIPE_ISO    3

#define USB_MAX_EP      6   // max number of end point
#define USB_MAX_PIPE    10  // max number of pipes
#define USB_MAX_FIFO    3   // max number of fifo
#define FIFO_CFIFO      0   // CFIFO type
#define FIFO_D0FIFO     1   // DOFIFO type
#define FIFO_D1FIFO     2   // D1FIFO type
#define FIFO_LOOP       4
#define FIFO_TIMEOUT    10000

#define MBW_BYTE 0
#define MBW_WORD 1
#define MBW_VAL (MBW_BYTE)

#define PID_NAK     0
#define PID_BUF     1
#define PID_STALL_1 2
#define PID_STALL_2 3

#define PIPE_IDLE  0x00
#define PIPE_WAIT  0x01
#define PIPE_DONE  0x02
#define PIPE_NORES 0x03
#define PIPE_STALL 0x04
#define PIPE_ERROR 0x05

#define M_BCHGE (0x4000)
#define M_DTCHE (0x1000)
#define M_ATTCHE (0x0800)
#define M_BEMPE (0x0400)
#define M_NRDYE (0x0200)
#define M_BRDYE (0x0100)
#define M_SIGNE (0x0020)
#define M_SACKE (0x0010)

#define M_BCHG  (0x4000)
#define M_DTCH  (0x1000)
#define M_ATTCH (0x0800)
#define M_BEMP  (0x0400)
#define M_NRDY  (0x0200)
#define M_BRDY  (0x0100)
#define M_SIGN  (0x0020)
#define M_SACK  (0x0010)

#define M_SUREQ (0x4000)
#define M_SQCLR (0x0100)
#define M_SQSET (0x0080)
#define M_DIR   (0x0010)

/* FIFOSEL */
#define M_RCNT  (0x8000)
#define M_MBW   (0x0400)
#define M_ISEL  (0x0020)
#define M_BIGEND (0x0010)
#define M_CURPIPE   (0x000f)

/* FIFOCTR */
#define M_BVAL  (0x8000)
#define M_BCLR  (0x4000)
#define M_FRDY  (0x2000)
#define M_DTLN  (0x0FFF)

volatile static int usb_attach = 0;
volatile static int usb_speed = SPEED_FULL;
volatile static int intsts1_sack = 0;
volatile static int intsts0_brdy = 0;
volatile static int intsts0_bemp = 0;
volatile static int intsts0_nrdy = 0;

static int USBH_FifoType[USB_MAX_PIPE] = {
FIFO_CFIFO,     // pipe 0
        FIFO_D0FIFO,    // pipe 1
        FIFO_D1FIFO,    // pipe 2
        FIFO_D0FIFO,
        FIFO_D1FIFO,
        FIFO_D0FIFO,
        FIFO_D1FIFO,
        FIFO_D0FIFO,
        FIFO_D1FIFO,
        FIFO_D0FIFO };

static volatile PIPECTR *pPIPECTR[USB_MAX_PIPE] = {
        (volatile PIPECTR *)&USB0.DCPCTR.WORD,
        (volatile PIPECTR *)&USB0.PIPE1CTR.WORD,
        (volatile PIPECTR *)&USB0.PIPE2CTR.WORD,
        (volatile PIPECTR *)&USB0.PIPE3CTR.WORD,
        (volatile PIPECTR *)&USB0.PIPE4CTR.WORD,
        (volatile PIPECTR *)&USB0.PIPE5CTR.WORD,
        (volatile PIPECTR *)&USB0.PIPE6CTR.WORD,
        (volatile PIPECTR *)&USB0.PIPE7CTR.WORD,
        (volatile PIPECTR *)&USB0.PIPE8CTR.WORD,
        (volatile PIPECTR *)&USB0.PIPE9CTR.WORD };

static volatile unsigned short *pFIFOSEL[USB_MAX_FIFO] = {
        (volatile unsigned short *)&USB0.CFIFOSEL.WORD,
        (volatile unsigned short *)&USB0.D0FIFOSEL.WORD,
        (volatile unsigned short *)&USB0.D1FIFOSEL.WORD };

static volatile unsigned short *pFIFOCTR[USB_MAX_FIFO] = {
        (volatile unsigned short *)&USB0.CFIFOCTR.WORD,
        (volatile unsigned short *)&USB0.D0FIFOCTR.WORD,
        (volatile unsigned short *)&USB0.D1FIFOCTR.WORD };

static volatile unsigned char *pFIFO[USB_MAX_FIFO] = {
        (volatile unsigned char *)&USB0.CFIFO.BYTE.L,
        (volatile unsigned char *)&USB0.D0FIFO.BYTE.L,
        (volatile unsigned char *)&USB0.D1FIFO.BYTE.L };

static unsigned short *DEVADD[6] = { (unsigned short *)&USB0.DEVADD0.WORD,
        (unsigned short *)&USB0.DEVADD1.WORD,
        (unsigned short *)&USB0.DEVADD2.WORD,
        (unsigned short *)&USB0.DEVADD3.WORD,
        (unsigned short *)&USB0.DEVADD4.WORD,
        (unsigned short *)&USB0.DEVADD5.WORD };

static unsigned char *rd_ptr[USB_MAX_PIPE];
static volatile int rd_cnt[USB_MAX_PIPE];
static volatile int rd_end[USB_MAX_PIPE];
static volatile int rd_err[USB_MAX_PIPE];

static unsigned char *wt_ptr[USB_MAX_PIPE];
static volatile int wt_cnt[USB_MAX_PIPE];
static volatile int wt_end[USB_MAX_PIPE];

static uint16_t pipe_max_packet[USB_MAX_PIPE] = { 64, 512, 512, 512, 512, 512,
        64, 64, 64, 64 };

//#ifdef USBH_DEBUG_dump_int_regs
static void dump_int_regs(unsigned char *s_func, int f_delay)
{
    if (s_func)
        debug_printf("[%s]\r\n", s_func);
    debug_printf("sts0=%04X sts1=%04X enb0=%04X enb1=%04X\r\n",
    USB0.INTSTS0.WORD, USB0.INTSTS1.WORD, USB0.INTENB0.WORD, USB0.INTENB1.WORD);
    debug_printf("brdy=%04X nrdy=%04X bemp=%04X\r\n",
    USB0.BRDYENB.WORD, USB0.NRDYENB.WORD, USB0.BEMPENB.WORD);
    if (f_delay)
        udelay(100000);
}

static void dump_pipe_regs(unsigned char *s_func, int f_delay)
{
    unsigned short tmp;
    if (s_func)
        debug_printf("[%s]\r\n", s_func);
    debug_printf("psel=%04X p0ctr=%04X p1ctr=%04X p2ctr=%04X\r\n",
    USB0.PIPESEL.WORD, USB0.DCPCTR.WORD, USB0.PIPE1CTR.WORD, USB0.PIPE2CTR.WORD);
    tmp = USB0.PIPESEL.WORD;
    USB0.PIPESEL.WORD = 1;
    debug_printf("P1 MAX:%04X CFG:%04X\r\n", USB0.PIPEMAXP.WORD, USB0.PIPECFG.WORD);
    USB0.PIPESEL.WORD = 2;
    debug_printf("P2 MAX:%04X CFG:%04X\r\n", USB0.PIPEMAXP.WORD, USB0.PIPECFG.WORD);
    USB0.PIPESEL.WORD = tmp;
    if (f_delay)
        udelay(100000);
}

static void dump_fifo_regs(unsigned char *s_func, int f_delay)
{
    if (s_func)
        debug_printf("[%s]\r\n", s_func);
    debug_printf("cfifosel=%04X d0fifosel=%04X d1fifosel=%04X\r\n",
    USB0.CFIFOSEL.WORD, USB0.D0FIFOSEL.WORD, USB0.D1FIFOSEL.WORD);
    debug_printf("cfifoctr=%04X d0fifoctr=%04X d1fifoctr=%04X\r\n",
    USB0.CFIFOCTR.WORD, USB0.D0FIFOCTR.WORD, USB0.D1FIFOCTR.WORD);
    if (f_delay)
        udelay(100000);
}
//#endif

//static void wait(volatile int count)
//{
//    while (count-- > 0) {
//        __asm__ __volatile__ ("nop");
//    }
//}

static void usb_set_max_packet_size(int speed)
{
    if (speed == SPEED_HI) {           // High Speed
        pipe_max_packet[0] = 64;
        pipe_max_packet[1] = 512;
        pipe_max_packet[2] = 512;
        pipe_max_packet[3] = 512;
        pipe_max_packet[4] = 512;
    } else if (speed == SPEED_FULL) {    // Full Speed
        pipe_max_packet[0] = 64;
        pipe_max_packet[1] = 64;
        pipe_max_packet[2] = 64;
        pipe_max_packet[3] = 64;
        pipe_max_packet[4] = 64;
    } else {                    // Low Speed
        pipe_max_packet[0] = 8;
        pipe_max_packet[1] = 8;
        pipe_max_packet[2] = 8;
        pipe_max_packet[3] = 8;
        pipe_max_packet[4] = 8;
    }
}

static void usb_int_brdy_enable(int pipe)
{
    unsigned short tmp = USB0.INTENB0.WORD;
    USB0.INTENB0.WORD = (unsigned short)(tmp & (unsigned short)~(M_BEMPE | M_NRDYE | M_BRDYE));
    USB0.BRDYENB.WORD |= (1 << pipe);
    USB0.INTENB0.WORD = tmp;
}

static void usb_int_brdy_disable(int pipe)
{
    unsigned short tmp = USB0.INTENB0.WORD;
    USB0.INTENB0.WORD = (unsigned short)(tmp & (unsigned short)~(M_BEMPE | M_NRDYE | M_BRDYE));
    USB0.BRDYENB.WORD &= ~(1 << pipe);
    USB0.INTENB0.WORD = tmp;
}

static void usb_int_nrdy_enable(int pipe)
{
    unsigned short tmp = USB0.INTENB0.WORD;
    USB0.INTENB0.WORD = (unsigned short)(tmp & (unsigned short)~(M_BEMPE | M_NRDYE | M_BRDYE));
    USB0.NRDYENB.WORD |= (1 << pipe);
    USB0.INTENB0.WORD = tmp;
}

static void usb_int_nrdy_disable(int pipe)
{
    unsigned short tmp = USB0.INTENB0.WORD;
    USB0.INTENB0.WORD = (unsigned short)(tmp & (unsigned short)~(M_BEMPE | M_NRDYE | M_BRDYE));
    USB0.NRDYENB.WORD &= ~(1 << pipe);
    USB0.INTENB0.WORD = tmp;
}

static void usb_int_bemp_enable(int pipe)
{
    unsigned short tmp = USB0.INTENB0.WORD;
    USB0.INTENB0.WORD = (unsigned short)(tmp & (unsigned short)~(M_BEMPE | M_NRDYE | M_BRDYE));
    USB0.BEMPENB.WORD |= (1 << pipe);
    USB0.INTENB0.WORD = tmp;
}

static void usb_int_bemp_disable(int pipe)
{
    unsigned short tmp = USB0.INTENB0.WORD;
    USB0.INTENB0.WORD = (unsigned short)(tmp & (unsigned short)~(M_BEMPE | M_NRDYE | M_BRDYE));
    USB0.BEMPENB.WORD &= ~(1 << pipe);
    USB0.INTENB0.WORD = tmp;
}

void usbh_ctrl_int_enable(void)
{
    unsigned short buf;

    buf = USB0.INTENB0.WORD;
    buf |= (M_BEMP | M_NRDY | M_BRDY);
    USB0.INTENB0.WORD = buf;

    buf = USB0.INTENB1.WORD;
    buf |= (M_SIGN | M_SACK);
    USB0.INTENB1.WORD = buf;

    usb_int_nrdy_enable(PIPE0);
    usb_int_bemp_enable(PIPE0);
}

static void usbh_handle_vbus(void)
{
    //debug_printf("ivb\r\n");
}

static int usbh_buf2fifo(int pipe)
{
    int ret = 0;
    int count;
    int len;
    int type = USBH_FifoType[pipe];
    volatile unsigned short *pfifoctr = (volatile unsigned short *)pFIFOCTR[type];
    volatile unsigned char *pfifo = (volatile unsigned char *)pFIFO[type];
    unsigned char *buf = wt_ptr[pipe];
    if (wt_cnt[pipe] <= (int)pipe_max_packet[pipe]) {
        ret = 1;
        count = wt_cnt[pipe];
    } else {
        count = (int)pipe_max_packet[pipe];
    }
    len = count;
    if (count == 0) {
        *pfifoctr = (M_BVAL | M_BCLR);
        wt_end[pipe] = 1;
    } else {
        while (count-- > 0) {
            if (buf != NULL) {
                *pfifo = *buf;
                buf++;
            }
        }
        if (len != (int)pipe_max_packet[pipe]) {
            *pfifoctr |= M_BVAL;
        }
    }
    wt_cnt[pipe] -= len;
    wt_ptr[pipe] = buf;
#ifdef USBH_DEBUG_FIFO
    debug_printf("FW%d\r\n", len);
#endif
    return ret;
}

static int usbh_fifo2buf_sub(int pipe)
{
    int ret = 0;
    int dtln;
    int count;
    int len;
    unsigned char dummy;
    int type = USBH_FifoType[pipe];
    volatile uint16_t *pfifosel = (volatile uint16_t *)pFIFOSEL[type];
    volatile uint16_t *pfifoctr = (volatile uint16_t *)pFIFOCTR[type];
    volatile unsigned char *pfifo = (volatile unsigned char *)pFIFO[type];
    unsigned char *buf = rd_ptr[pipe];
    *pfifosel |= M_RCNT;
    dtln = (*pfifoctr) & M_DTLN;
    if (rd_cnt[pipe] < dtln) {
        count = rd_cnt[pipe];
        rd_cnt[pipe] = dtln;
        ret = 1;
    } else if (rd_cnt[pipe] == dtln) {
        count = dtln;
        ret = 1;
    } else {
        count = dtln;
        if (count == 0)
            ret = 1; /* null packect */
        if ((count % (int)pipe_max_packet[pipe]) != 0)
            ret = 1; /* short packect */
    }
    if (ret == 1)
        usbh_pipe_set_pidnak(pipe);
    len = count;
    if (dtln == 0) {
        *pfifoctr = M_BCLR;
    } else {
        while (count-- > 0) {
            if (buf != NULL) {
                *buf = *pfifo;
                buf++;
            } else {
                dummy = *pfifo;
            }
        }
    }
    rd_cnt[pipe] -= len;
    rd_ptr[pipe] = buf;
#ifdef USBH_DEBUG_FIFO
    debug_printf("FR%d-%d-%d\r\n", len, dtln, rd_cnt[pipe]);
#endif
    return ret;
}

static void usbh_fifo2buf(int pipe)
{
    int ret;
    ret = usbh_fifo2buf_sub(pipe);
    if (ret == 1) {
        rd_end[pipe] = 1;
    }
}

static void usbh_handle_bemp(void)
{
    int pipe;
    unsigned short bempsts = USB0.BEMPSTS.WORD;
    unsigned short bempenb = USB0.BEMPENB.WORD;
    if ((USB0.BEMPSTS.BIT.PIPE0BEMP == 1) && (USB0.BEMPENB.BIT.PIPE0BEMPE == 1)) {
        USB0.BEMPSTS.WORD = ~0x0001;
        if (wt_cnt[PIPE0] != 0)
            usbh_buf2fifo(PIPE0);
        else {
            wt_end[PIPE0] = 1;
            usb_int_bemp_disable(PIPE0);
        }
        intsts0_bemp = 1;
    } else {
        USB0.BEMPSTS.WORD = ~bempsts;
        for (pipe = 1; pipe < USB_MAX_PIPE; pipe++) {
            int mask = (1 << pipe);
            if ((bempsts & mask) && (bempenb & mask)) {
                USB0.PIPESEL.BIT.PIPESEL = pipe;
                if (USB0.PIPECFG.BIT.DIR == DIR_R) {
                    usbh_fifo2buf(pipe);
                    if (rd_end[pipe] == 1) {
                        usbh_pipe_set_pidnak(pipe);
                        usb_int_brdy_disable(pipe);
                    }
                } else {
                    if (wt_cnt[pipe] != 0)
                        usbh_buf2fifo(pipe);
                    else {
                        wt_end[pipe] = 1;
                        usbh_pipe_set_pidnak(pipe);
                        usb_int_bemp_disable(pipe);
                    }
                    intsts0_bemp = 1;
                }
                USB0.PIPESEL.BIT.PIPESEL = pipe;
            }
        }
    }
}

static void usbh_handle_nrdy(void)
{
    int pipe;
    unsigned short nrdysts = USB0.NRDYSTS.WORD;
    unsigned short nrdyenb = USB0.NRDYENB.WORD;
    if ((USB0.NRDYSTS.BIT.PIPE0NRDY == 1) && (USB0.NRDYENB.BIT.PIPE0NRDYE == 1)) {
        USB0.NRDYSTS.WORD = ~0x0001;
        usb_int_nrdy_disable(PIPE0);
        intsts0_nrdy = 1;
    } else {
        USB0.NRDYSTS.WORD = ~nrdysts;
        for (pipe = 1; pipe < USB_MAX_PIPE; pipe++) {
            int mask = (1 << pipe);
            if ((nrdysts & mask) && (nrdyenb & mask)) {
                rd_err[pipe]++;
                if (rd_err[pipe] > 3) {
                    usb_int_nrdy_disable(pipe);
                    intsts0_nrdy = 1;
#ifdef USBH_DEBUG_ERR_NS
                    debug_printf("NS\n");
#endif
                } else {
                    usbh_pipe_set_pidbuf(pipe);
                }
                //}
            }
        }
    }
}

static void usbh_handle_brdy(void)
{
    int pipe;
    unsigned short brdysts = USB0.BRDYSTS.WORD;
    unsigned short brdyenb = USB0.BRDYENB.WORD;
    if ((USB0.BRDYSTS.BIT.PIPE0BRDY == 1) && (USB0.BRDYENB.BIT.PIPE0BRDYE == 1)) {
        USB0.BRDYSTS.WORD = ~0x0001;
        USB0.PIPESEL.BIT.PIPESEL = PIPE0;
        if (USB0.PIPECFG.BIT.DIR == DIR_R) {
            usbh_fifo2buf(PIPE0);
            if (rd_end[PIPE0] == 1) {
                usbh_pipe_set_pidnak(PIPE0);
                usb_int_brdy_disable(PIPE0);
            }
        } else {
            usbh_buf2fifo(PIPE0);
        }
        intsts0_brdy = 1;
        USB0.PIPESEL.BIT.PIPESEL = 0;
    } else {
        USB0.BRDYSTS.WORD = ~brdysts;
        for (pipe = 1; pipe < USB_MAX_PIPE; pipe++) {
            int mask = (1 << pipe);
            if ((brdysts & mask) && (brdyenb & mask)) {
                USB0.PIPESEL.BIT.PIPESEL = pipe;
                if (USB0.PIPECFG.BIT.DIR == DIR_R) {
                    usbh_fifo2buf(pipe);
                    if (rd_end[pipe] == 1) {
                        usbh_pipe_set_pidnak(pipe);
                        usb_int_brdy_disable(pipe);
                    }
                } else {
                    usbh_buf2fifo(pipe);
                }
                intsts0_brdy = 1;
                USB0.PIPESEL.BIT.PIPESEL = 0;
            }
        }
    }
}

static void usbh_handle_attach(void)
{
    usb_attach = 1;
}

static void usbh_handle_detach(void)
{
    usb_attach = 0;
}

static void usbh_handle_bchg(void)
{

}

void __attribute__ ((interrupt)) INT_Excep_USB0_USBI0(void)
{
    unsigned short intsts0, intsts1;
    unsigned short intenb0, intenb1;
    unsigned short brdysts, nrdysts, bempsts;
    unsigned short brdyenb, nrdyenb, bempenb;

    unsigned short savepipe1, savepipe2;
    unsigned short tmp;

    savepipe1 = USB0.CFIFOSEL.WORD;
    savepipe2 = USB0.PIPESEL.WORD;

    intsts0 = USB0.INTSTS0.WORD;
    intsts1 = USB0.INTSTS1.WORD;
    intenb0 = USB0.INTENB0.WORD;
    intenb1 = USB0.INTENB1.WORD;

    if ((intsts1 & M_BCHG) && (intenb1 & M_BCHGE)) {
        USB0.INTSTS1.WORD = (unsigned short)~M_BCHG;
        USB0.INTENB1.BIT.BCHGE = 0;
#ifdef USBH_DEBUG_INT
        debug_printf("BC\n");
#endif
    } else if ((intsts1 & M_SACK) && (intenb1 & M_SACK)) {
        USB0.INTSTS1.WORD = (unsigned short)~M_SACK;
#ifdef USBH_DEBUG_INT
        debug_printf("SA\n");
#endif
        intsts1_sack = 1;
    } else if ((intsts1 & M_SIGN) && (intenb1 & M_SIGN)) {
        USB0.INTSTS1.WORD = (unsigned short)~M_SIGN;
#ifdef USBH_DEBUG_INT
        debug_printf("SI\n");
#endif
    } else if (((intsts1 & M_DTCH) == M_DTCH) && ((intenb1 & M_DTCHE) == M_DTCHE)) {
        USB0.INTSTS1.WORD = (unsigned short)~M_DTCH;
        USB0.INTENB1.BIT.DTCHE = 0;
#ifdef USBH_DEBUG_INT
        debug_printf("DT\n");
#endif
    } else if (((intsts1 & M_ATTCH) == M_ATTCH) && ((intenb1 & M_ATTCHE) == M_ATTCHE)) {
        USB0.INTSTS1.WORD = (unsigned short)~M_ATTCH;
        USB0.INTENB1.BIT.ATTCHE = 0;
#ifdef USBH_DEBUG_INT
        debug_printf("AT\n");
#endif
    } else if ((intsts0 & intenb0 & (M_BEMP | M_NRDY | M_BRDY))) {
        brdysts = USB0.BRDYSTS.WORD;
        nrdysts = USB0.NRDYSTS.WORD;
        bempsts = USB0.BEMPSTS.WORD;
        brdyenb = USB0.BRDYENB.WORD;
        nrdyenb = USB0.NRDYENB.WORD;
        bempenb = USB0.BEMPENB.WORD;

        if ((intsts0 & M_BRDY) && (intenb0 & M_BRDY) && (brdysts & brdyenb)) {
            usbh_handle_brdy();
#if defined(USBH_DEBUG_INT) || defined(USBH_DEBUG_INT_BRDY)
            debug_printf("BR\n");
#endif
        } else if ((intsts0 & M_BEMP) && (intenb0 & M_BEMP) && (bempsts & bempenb)) {
            usbh_handle_bemp();
#if defined(USBH_DEBUG_INT) || defined(USBH_DEBUG_INT_BEMP)
            debug_printf("BE\n");
#endif
        } else if ((intsts0 & M_NRDY) && (intenb0 & M_NRDY) && (nrdysts & nrdyenb)) {
            usbh_handle_nrdy();
#if defined(USBH_DEBUG_INT) || defined(USBH_DEBUG_INT_NRDY)
            debug_printf("NR\n");
#endif
        }
    } else {
    }

    tmp = USB0.CFIFOSEL.WORD;
    tmp &= (unsigned short)~(M_CURPIPE);
    tmp |= (unsigned short)(savepipe1 & M_CURPIPE);
    USB0.CFIFOSEL.WORD = tmp;
    USB0.PIPESEL.WORD = savepipe2;
}

static void usbh_set_DEVADD(int n)
{
    uint16_t tmp = (*DEVADD[n]) & 0xff3f;
    switch(usb_speed) {
    case SPEED_HI:
        *DEVADD[n] = tmp | 0x00c0;
        break;
    case SPEED_FULL:
        *DEVADD[n] = tmp | 0x0080;
        break;
    case SPEED_LOW:
        *DEVADD[n] = tmp | 0x0040;
        break;
    default:
        *DEVADD[n] = tmp;
        break;
    }
}

static void usbh_pipe_init(void)
{
    int i;
    for (i = 0; i < 6; i++) {
        *DEVADD[i] = 0;
    }
}

void usbh_int_init(void)
{
    USB0.INTENB0.BIT.VBSE = 1;
    USB0.INTENB0.BIT.BEMPE = 1;
    USB0.INTENB0.BIT.NRDYE = 1;
    USB0.INTENB0.BIT.BRDYE = 1;

    USB0.BEMPENB.BIT.PIPE0BEMPE = 1;    // pipe 0 bemp interrupt enable
    USB0.BRDYENB.BIT.PIPE0BRDYE = 0;    // pipe 0 brdy inerrrupt disable
}

static inline void usbh_port_config(void)
{
    IOPORT.PFKUSB.BYTE = 0x1E;
    PORT1.ICR.BYTE |= 0x50;
    PORT2.DR.BIT.B5 = 0;
}

static inline void usbh_int_disable(void)
{
    USB0.INTENB0.WORD = 0;
    USB0.INTENB1.WORD = 0;
}

static inline void usbh_int_level(unsigned char level)
{
    ICU.IPR[0x000E].BYTE = level;
    ICU.IER[0x0004].BYTE |= 0x40;
}

static unsigned short usbh_wait_line_statble(void)
{
    unsigned short s0, s1;
    do {
        s0 = (unsigned short)USB0.SYSSTS0.BIT.LNST;
        udelay(1000);
        s1 = (unsigned short)USB0.SYSSTS0.BIT.LNST;
    } while (s0 != s1);
    return s0;
}

static void usbh_pipe_clr_sq(int pipe);

void usbh_module_init(void)
{
    unsigned short sts;
    USB0.SYSCFG.WORD |= 0x400;
    while (0x0400 != (USB0.SYSCFG.WORD & 0x0400)) {
    }
    usbh_pipe_init();
    USB0.SYSCFG.BIT.DCFM = 1;       // 1: host, 0: function
    USB0.SYSCFG.BIT.DRPD = 1;       // pull down enable
    sts = usbh_wait_line_statble();
    USB0.SYSCFG.BIT.USBE = 1;       // usb clock enable
    usbh_pipe_clr_sq(BULK_IN_PIPENO);
    usbh_pipe_clr_sq(BULK_OUT_PIPENO);
    usbh_pipe_clr_sq(INT_IN_PIPENO);
    usbh_pipe_clr_sq(INT_OUT_PIPENO);
}

void usbh_init(void)
{
    API_FUNC_ENTER();
    MSTP(USB0) = 0;                 // usb module enable
    usbh_port_config();
    usbh_int_disable();
    //USB0.SYSCFG.BIT.SCKE = 1;
    usbh_int_level(USBH_INT_LEVEL);
    usbh_module_init();
    //usbh_int_init();
#ifdef USBH_DEBUG_dump_int_regs
    dump_int_regs(__func__, 1);
#endif
    API_FUNC_EXIT();
}

#ifdef USBH_USE_TRANSACTION
static void usbh_pipe_set_counter(int pipe, int size)
{
    int count = 0;
    if (size == 0) {
        return;
    }
    if ((size % (int)pipe_max_packet[pipe]) == 0) {
        count = size / (int)pipe_max_packet[pipe];
    } else {
        count = size / (int)pipe_max_packet[pipe] + 1;
    }
    switch (pipe) {
        case 1:
        USB0.PIPE1TRE.BIT.TRCLR = 1;
        USB0.PIPE1TRN = count;
        USB0.PIPE1TRE.BIT.TRENB = 1;
        break;
        case 2:
        USB0.PIPE2TRE.BIT.TRCLR = 1;
        USB0.PIPE2TRN = count;
        USB0.PIPE2TRE.BIT.TRENB = 1;
        break;
        case 3:
        USB0.PIPE3TRE.BIT.TRCLR = 1;
        USB0.PIPE3TRN = count;
        USB0.PIPE3TRE.BIT.TRENB = 1;
        break;
        case 4:
        USB0.PIPE4TRE.BIT.TRCLR = 1;
        USB0.PIPE4TRN = count;
        USB0.PIPE4TRE.BIT.TRENB = 1;
        break;
        case 5:
        USB0.PIPE5TRE.BIT.TRCLR = 1;
        USB0.PIPE5TRN = count;
        USB0.PIPE5TRE.BIT.TRENB = 1;
        break;
        default:
        break;
    }
}

// should be called in NAK
static void usbh_pipe_clr_counter(int pipe)
{
    switch (pipe) {
        case 1:
        USB0.PIPE1TRE.BIT.TRENB = 0;
        USB0.PIPE1TRE.BIT.TRCLR = 1;
        break;
        case 2:
        USB0.PIPE2TRE.BIT.TRENB = 0;
        USB0.PIPE2TRE.BIT.TRCLR = 1;
        break;
        case 3:
        USB0.PIPE3TRE.BIT.TRENB = 0;
        USB0.PIPE3TRE.BIT.TRCLR = 1;
        break;
        case 4:
        USB0.PIPE4TRE.BIT.TRENB = 0;
        USB0.PIPE4TRE.BIT.TRCLR = 1;
        break;
        case 5:
        USB0.PIPE5TRE.BIT.TRENB = 0;
        USB0.PIPE5TRE.BIT.TRCLR = 1;
        break;
        default:
        break;
    }
}
#endif

#if 0
static volatile PIPECTR *usbh_get_pipectr(int pipe)
{
    volatile PIPECTR *pipectr;
    if (pipe == 0)
    pipectr = (volatile PIPECTR *)&USB.DCPCTR.WORD;
    else {
        pipectr = (volatile PIPECTR *)&USB0.PIPE1CTR.WORD;
        pipectr += (pipe - 1);
    }
    return pipectr;
}
#endif

static inline void usbh_pipe_set_pidbuf(int pipe)
{
    pPIPECTR[pipe]->BIT.PID = PID_BUF;
}

static void usbh_pipe_set_pidnak(int pipe)
{
    volatile PIPECTR *pipectr = pPIPECTR[pipe];
    unsigned short cpid = pipectr->BIT.PID;
    if ((cpid == PID_STALL_1) || (cpid == PID_STALL_2))
        debug_printf("LL\r\n");
    if (cpid == PID_STALL_2) {
        pipectr->BIT.PID = PID_STALL_1;
    }
    pipectr->BIT.PID = PID_NAK;
    if (cpid == PID_BUF) {
        while (pipectr->BIT.PBUSY == 1)
            ;
    }
}

static void usbh_pipe_clr_buf(int pipe)
{
    volatile PIPECTR *pipectr = pPIPECTR[pipe];
    usbh_pipe_set_pidnak(pipe);
    pipectr->BIT.ACLRM = 1;
    udelay(10000);
    pipectr->BIT.ACLRM = 0;
}

#ifdef PIPE_CLEAR_FIFO
static void usbh_pipe_clear_fifo(int pipe)
{
    if (USB0.CFIFOSEL.BIT.CURPIPE == pipe)
        USB0.CFIFOSEL.BIT.CURPIPE = 0;
    if (USB0.D0FIFOSEL.BIT.CURPIPE == pipe)
        USB0.D0FIFOSEL.BIT.CURPIPE = 0;
    if (USB0.D1FIFOSEL.BIT.CURPIPE == pipe)
        USB0.D1FIFOSEL.BIT.CURPIPE = 0;
}
#endif

static void usbh_pipe_set_ep(int pipe, int ep, int dir, int attr, int size, int addr)
{
    usbh_pipe_set_pidnak(pipe);
    USB0.PIPESEL.BIT.PIPESEL = pipe;
    USB0.PIPECFG.WORD = 0;
    USB0.PIPECFG.BIT.EPNUM = ep;        // Endpoint
    USB0.PIPECFG.BIT.DIR = dir;         // Direction
    USB0.PIPECFG.BIT.DBLB = 0;          // Double Buffer
#ifdef USBH_USE_TRANSACTION
            if (dir == DIR_R) {
                USB0.PIPECFG.BIT.BFRE = 1;          // Interrupt
                USB0.PIPECFG.BIT.SHTNAK = 1;
            } else
            USB0.PIPECFG.BIT.BFRE = 0;          // Interrupt
#else
    USB0.PIPECFG.BIT.BFRE = 0;          // Interrupt
    if (dir == DIR_R) {
        USB0.PIPECFG.BIT.SHTNAK = 0;
    }
#endif
    if (attr == PIPE_BULK) {
        USB0.PIPECFG.BIT.TYPE = PIPE_BULK;
        //pipectr->BIT.SQCLR = 1;
    } else if (attr == PIPE_INT) {
        USB0.PIPECFG.BIT.TYPE = PIPE_INT;
    } else if (attr == PIPE_ISO) {
        USB0.PIPECFG.BIT.TYPE = PIPE_ISO;
    }
    USB0.PIPEMAXP.BIT.MXPS = (uint16_t)pipe_max_packet[pipe];    // Packet Size
    USB0.PIPEMAXP.BIT.DEVSEL = addr;        // Device Address
    //USB0.PIPESEL.BIT.PIPESEL = 0;         // Unselect Pipe
}

static void usbh_pipe_set_sq(int pipe)
{
    volatile PIPECTR *pipectr = pPIPECTR[pipe];
    pipectr->BIT.SQSET = 1;
}

static void usbh_pipe_clr_sq(int pipe)
{
    volatile PIPECTR *pipectr = pPIPECTR[pipe];
    pipectr->BIT.SQCLR = 1;
}

static void usbh_pipe_set_fifo(int pipe, int dir, int mbw)
{
    volatile unsigned short tmp;
    int type = USBH_FifoType[pipe];
    volatile unsigned short *pfifosel = pFIFOSEL[type];
    volatile unsigned short *pfifoctr = pFIFOCTR[type];
    if (pipe == PIPE0) {
        if (dir == DIR_W)
            USB0.DCPCFG.BIT.DIR = DIR_W;
        else
            USB0.DCPCFG.BIT.DIR = DIR_R;
    }
#if 0
    dump_fifo_regs(__func__, 1);
#endif
    tmp = *pfifosel;
    if (type == FIFO_CFIFO) {
        tmp &= ~(M_MBW | M_ISEL | M_CURPIPE);
        tmp |= (unsigned short)((mbw << 10) | (dir << 5) | pipe);
    } else {
        tmp &= ~(M_MBW | M_CURPIPE);
        tmp |= (unsigned short)((mbw << 10) | pipe);
    }
    *pfifosel = tmp;
    udelay(50);
    volatile int j = FIFO_TIMEOUT;
    while (j-- > 0) {
        if (*pfifosel == tmp)
            break;
        if (j == 0)
            goto usbh_pipe_set_fifo_exit;
        udelay(20);
    }
    if (dir) {
        j = FIFO_TIMEOUT;
        while (j-- > 0) {
            tmp = *pfifoctr;
            if ((tmp & M_FRDY) == M_FRDY)
                return;
            if (j == 0)
                goto usbh_pipe_set_fifo_exit;
            udelay(100);
        }
    } else
        return;
    usbh_pipe_set_fifo_exit:
#ifdef USBH_DEBUG_ERR
    debug_printf("* [%s] err pipe %d\r\n", __func__, pipe);
#endif
    return;
}

static void usbh_pipe_read(int pipe, unsigned char *buf, int size, int *len)
{
    unsigned char dummy;
    int count;
    int type = USBH_FifoType[pipe];
    volatile unsigned short *pfifosel = (volatile unsigned short *)pFIFOSEL[type];
    volatile unsigned short *pfifoctr = (volatile unsigned short *)pFIFOCTR[type];
    volatile unsigned char *pfifo = (volatile unsigned char *)pFIFO[type];
    *pfifosel |= M_RCNT;
    count = *pfifoctr & M_DTLN;
#ifdef USBH_DEBUG_PIPE_PR
    debug_printf("PR%d:%d\r\n", size, count);
#endif
    *len = 0;
    if (count == 0) {
        *pfifoctr = M_BCLR;
    } else {
        while (count-- > 0) {
            if (*len >= size)
                break;
            if (buf != NULL) {
                *buf = *pfifo;
                buf++;
            } else {
                dummy = *pfifo;
            }
            (*len)++;
        }
    }
    if (0 != (*pfifoctr & 0x01FF)) {
#ifdef USBH_DEBUG_ERR
        debug_printf("* [%s] err\r\n", __func__);
#endif
        while (0 != (*pfifoctr & 0x01FF)) {
            dummy = *pfifo;
        }
    }
#ifdef USBH_DEBUG_PIPE_PR
    debug_printf("PR(%d)\r\n", *len);
#endif
}

static void usbh_pipe_write(int pipe, unsigned char *buf, int size, int len)
{
#ifdef USBH_DEBUG_PIPE_PW
    debug_printf("PW%d\r\n", len);
#endif
    usbh_pipe_set_fifo(pipe, ISEL_W, MBW_VAL);
    int count = 0;
    int type = USBH_FifoType[pipe];
    volatile unsigned short *pfifoctr = (volatile unsigned short *)pFIFOCTR[type];
    volatile unsigned char *pfifo = (volatile unsigned char *)pFIFO[type];
    if (len == 0) {
        *pfifoctr = (M_BVAL | M_BCLR);
    } else if (buf != NULL)
        while ((len != 0) && (size > count)) {
            *pfifo = (unsigned char)*buf;
            buf++;
            count++;
            len--;
        }
    if (count != size) {
        *pfifoctr |= M_BVAL;
    }
    usbh_pipe_set_pidbuf(pipe);
    usb_int_brdy_disable(pipe); // disable interrupt
    usb_int_bemp_enable(pipe);  // enable interrupt
    usb_int_nrdy_enable(pipe);  // enable interrupt
}

static void usbh_attach(void)
{
    API_FUNC_ENTER();
    usbh_ctrl_int_enable();
    USB0.INTSTS1.BIT.BCHG = 0;
    USB0.INTENB1.BIT.BCHGE = 0;
    USB0.INTSTS1.BIT.ATTCH = 0;
    USB0.INTENB1.BIT.ATTCHE = 0;
    USB0.INTSTS1.BIT.DTCH = 0;
    USB0.INTENB1.BIT.DTCHE = 1;
    API_FUNC_EXIT();
}

void ul_host_config(int conn_num)
{
    API_FUNC_ENTER();
    usbh_init();
    usbh_attach();
    API_FUNC_EXIT();
}

void ul_host_bus_reset(int conn_num)
{
    unsigned short tmp, i;
    API_FUNC_ENTER();
    USB0.DVSTCTR0.BIT.USBRST = 1;
    USB0.DVSTCTR0.BIT.UACT = 0;
    udelay(50000);
    USB0.DVSTCTR0.BIT.USBRST = 0;
    USB0.DVSTCTR0.BIT.UACT = 1;
    udelay(50000);
    USB0.INTSTS1.WORD = ~0x0800;
    for (i = 0; i < 50; ++i) {
        tmp = USB0.DVSTCTR0.BIT.RHST;
        if ((tmp & 4) == 4) {
            udelay(20000);
        } else {
            break;
        }
    }
    if ((tmp & 7) == 0) {
        debug_printf("USB SPEED not detected.\n");
    } else if ((tmp & 2) == 2) {
        usb_speed = SPEED_FULL;
        debug_printf("FULL SPEED detected.\n");
    } else {
        usb_speed = SPEED_LOW;
        debug_printf("LOW SPEED detected.\n");
    }
    usb_set_max_packet_size(usb_speed);
#ifdef USBH_DEBUG_dump_int_regs
    dump_int_regs(__func__, 1);
#endif
    API_FUNC_EXIT();
    return;
}

//int wait_int_event(int conn_num,en_int_status s) {
//  return USBH_ERROR;
//}

int ul_speed_configuration(int conn_num)
{
    API_FUNC_ENTER();API_FUNC_EXIT();
    return usb_speed;
}

unsigned short get_max_packet_size(int pipe)
{
    API_FUNC_ENTER();API_FUNC_EXIT();
    return (int)pipe_max_packet[pipe];
}

int rd_transfer(int pipe, int conn_num, st_usb_device *dev, int size, unsigned char* data)
{
    int ret = 0;
    int timeout = 20000;
    volatile unsigned short *pfifoctr = pFIFOCTR[USBH_FifoType[pipe]];
#ifdef USBH_RD
    debug_printf("RD%d\r\n", size);
#endif
    usbh_pipe_set_pidnak(pipe);
    rd_ptr[pipe] = data;
    rd_cnt[pipe] = size;
    rd_end[pipe] = 0;
    //usbh_pipe_clr_counter(pipe);
    usbh_pipe_set_fifo(pipe, ISEL_R, MBW_VAL);
#ifdef USBH_USE_TRANSACTION
    usbh_pipe_set_counter(pipe, size);
#endif
    *pfifoctr = M_BCLR;
#ifdef USBH_DEBUG_DUMP_REGS_BI
    dump_int_regs(__func__, 1);
    dump_pipe_regs(__func__, 1);
    dump_fifo_regs(__func__, 1);
#endif
    usbh_pipe_set_pidbuf(pipe);
    usb_int_brdy_enable(pipe);
    usb_int_bemp_enable(pipe);
    usb_int_nrdy_enable(pipe);
    while (rd_end[pipe] == 0) {
        if (timeout-- == 0) {
#ifdef USBH_DEBUG_ERR
            debug_printf("* [%s] timeout\r\n", __func__);
#endif
            ret = -1;
            break;
        }
        udelay(200);
    }
    usbh_pipe_set_pidnak(pipe);
    *pfifoctr = M_BCLR;
    usbh_pipe_set_fifo(PIPE0, ISEL_R, MBW_VAL);
    usb_int_brdy_disable(pipe);
    usb_int_bemp_disable(pipe);
    usb_int_nrdy_disable(pipe);
    USB0.PIPESEL.WORD = PIPE0;
    usbh_pipe_clr_buf(pipe);
    return ret;
}

int wt_transfer(int pipe, int conn_num, st_usb_device *dev, int size, unsigned char* data)
{
    int ret = 0;
    int timeout = 20000;
    volatile unsigned short *pfifoctr = pFIFOCTR[USBH_FifoType[pipe]];
#ifdef USBH_WT
    debug_printf("WT%d\r\n", size);
#endif
    wt_ptr[pipe] = data;
    wt_cnt[pipe] = size;
    wt_end[pipe] = 0;
    intsts0_bemp = 0;
    usbh_pipe_set_fifo(pipe, ISEL_W, MBW_VAL);
#ifdef USBH_USE_TRANSACTION
    usbh_pipe_clr_counter(pipe);
#endif
#ifdef USBH_DEBUG_DUMP_REGS_BO
    dump_int_regs(__func__, 1);
    dump_pipe_regs(__func__, 1);
    dump_fifo_regs(__func__, 1);
#endif
    usbh_buf2fifo(pipe);
    usbh_pipe_set_pidbuf(pipe);
    usb_int_bemp_enable(pipe);
    usb_int_nrdy_enable(pipe);
    while (wt_end[pipe] == 0) {
        if (timeout-- == 0) {
#ifdef USBH_DEBUG_ERR
            debug_printf("* [%s] timeout\r\n", __func__);
#endif
            ret = -1;
            break;
        }
        udelay(100);
    }
    usbh_pipe_set_pidnak(pipe);
    *pfifoctr = M_BCLR;
    usbh_pipe_set_fifo(PIPE0, ISEL_R, MBW_VAL);
    usb_int_brdy_disable(pipe);
    usb_int_bemp_disable(pipe);
    usb_int_nrdy_disable(pipe);
    USB0.PIPESEL.WORD = PIPE0;
    usbh_pipe_clr_buf(pipe);
    if (wt_cnt[pipe] != 0)
        ret = -1;
    return ret;
}

en_usb_status ul_control_setup_transaction(int conn_num, st_usb_device *dev, st_usb_setup_data *sd)
{
    int timeout;
    unsigned short max_packet_size = get_max_packet_size(PIPE0);
    int addr = dev->info.address;
#ifdef USBH_DEBUG_WAIT
    udelay(500000);
#endif
#ifdef USBH_DEBUG_CS
    debug_printf("CS\r\n");
#endif
    API_FUNC_ENTER();
    if (usb_speed == 1) {
        USB0.SOFCFG.BIT.TRNENSEL = 0;
    } else {
        USB0.SOFCFG.BIT.TRNENSEL = 1;
    }
    usbh_set_DEVADD(addr);
    intsts1_sack = 0;
    USB0.DCPMAXP.WORD = (unsigned short)((unsigned short)(addr << 12) + max_packet_size);
    USB0.INTSTS1.WORD = (unsigned short)~(M_SACK | M_SIGN); /* Status Clear */
#ifdef USBH_DEBUG_dump_int_regs
    dump_int_regs(__func__, 1);
#endif
    USB0.USBREQ.WORD = ((unsigned short)sd->BYTE.bRequest << 8) + (unsigned short)sd->BYTE.bmRequestType;
    USB0.USBVAL = ((unsigned short)sd->BYTE.wValue[1] << 8) + (unsigned short)sd->BYTE.wValue[0];
    USB0.USBINDX = ((unsigned short)sd->BYTE.wIndex[1] << 8) + (unsigned short)sd->BYTE.wIndex[0];
    USB0.USBLENG = ((unsigned short)sd->BYTE.wLength[1] << 8) + (unsigned short)sd->BYTE.wLength[0];
    USB0.DCPCTR.WORD = M_SUREQ;
#ifdef USBH_DEBUG_SETUP
    debug_printf("ST %2x %2x %2x %2x\r\n", USB0.USBREQ.WORD, USB0.USBVAL, USB0.USBINDX, USB0.USBLENG);
#endif
    timeout = 1000;
    while (1) {
        if (intsts1_sack == 1)
            break;
        udelay(100);
        if (timeout-- == 0) {
#ifdef USBH_DEBUG_ERR
            debug_printf("* [%s] timeout\r\n", __func__);
#endif
            return USBH_ERROR;
        }
    }API_FUNC_EXIT();
    return USBH_OK;
}

en_usb_status ul_control_in_transaction(int conn_num, st_usb_device *dev, int size, unsigned char *data)
{
    int ret;
    int pipe = PIPE0;
    API_FUNC_ENTER();
    usbh_pipe_set_pidnak(pipe);
    USB0.DCPCTR.WORD |= M_SQSET;
    ret = rd_transfer(pipe, conn_num, dev, size, data);
    API_FUNC_EXIT();
    if (ret < 0)
        return USBH_ERROR;
    return USBH_OK;
}

en_usb_status ul_control_out_transaction(int conn_num, st_usb_device *dev, int size, unsigned char *data)
{
    int ret;
    int pipe = PIPE0;
    API_FUNC_ENTER();
    usbh_pipe_set_pidnak(pipe);
    USB0.DCPCTR.WORD |= M_SQSET;
    ret = wt_transfer(pipe, conn_num, dev, size, data);
    API_FUNC_EXIT();
    if (ret < 0)
        return USBH_ERROR;
    return USBH_OK;
}

en_usb_status ul_control_ack_out_transaction(int conn_num, st_usb_device *dev)
{
    int ret;
    int pipe = PIPE0;
    API_FUNC_ENTER();
    usbh_pipe_set_pidnak(pipe);
    USB0.DCPCFG.WORD |= 0x0010;   //DIR_W
    USB0.DCPCTR.WORD = M_SQSET;
    ret = wt_transfer(pipe, conn_num, dev, 0, 0);
    API_FUNC_EXIT();
    if (ret < 0)
        return USBH_ERROR;
    return USBH_OK;
}

en_usb_status ul_control_ack_in_transaction(int conn_num, st_usb_device *dev)
{
    int ret;
    int pipe = PIPE0;
    API_FUNC_ENTER();
    usbh_pipe_set_pidnak(pipe);
    USB0.DCPCFG.WORD &= ~0x0010;   //DIR_R;
    USB0.DCPCTR.WORD = M_SQSET;
    ret = rd_transfer(pipe, conn_num, dev, 0, 0);
    API_FUNC_EXIT();
    if (ret < 0)
        return USBH_ERROR;
    return USBH_OK;
}

// interrupt transfer(one or more transactions)
en_usb_status ul_interrupt_in_transfer(int conn_num, st_usb_device *dev, int *size, unsigned char *data)
{
    int ret;
    int addr = dev->info.address;
    int pipe = INT_IN_PIPENO;
    API_FUNC_ENTER();
#ifdef USBH_DEBUG_II
    debug_printf("II%d P:%d A:%d E:%d\r\n", size, pipe, addr, dev->info.interface[dev->info.cur_interface].ea_in);
#endif
    usbh_pipe_set_pidnak(pipe);
    usbh_pipe_set_ep(pipe, (int)dev->info.interface[dev->info.cur_interface].ea_in, DIR_R, PIPE_INT, (int)pipe_max_packet[pipe], addr);
    //usbh_pipe_clr_sq(pipe);
    ret = rd_transfer(pipe, conn_num, dev, size, data);
    udelay(20000);
#ifdef USBH_DEBUG_II
    debug_printf("II(%d) ret=%d\r\n", size - rd_cnt[pipe], ret);
#endif
    API_FUNC_EXIT();
    if (ret < 0)
        return USBH_ERROR;
    return USBH_OK;
}

en_usb_status ul_interrupt_out_transfer(int conn_num, st_usb_device *dev, int size, unsigned char* data)
{
    int ret;
    int addr = dev->info.address;
    int pipe = INT_OUT_PIPENO;
    API_FUNC_ENTER();
#ifdef USBH_DEBUG_IO
    debug_printf("IO%d P:%d A:%d E:%d\r\n",size, pipe, addr, dev->info.interface[dev->info.cur_interface].ea_out);
#endif
    usbh_pipe_set_pidnak(pipe);
    usbh_pipe_set_ep(pipe, (int)dev->info.interface[dev->info.cur_interface].ea_out, DIR_W, PIPE_INT, MAX_PACKET, addr);
    //usbh_pipe_clr_sq(pipe);
    usbh_pipe_clr_buf(pipe);
    ret = wt_transfer(pipe, conn_num, dev, size, data);
    udelay(20000);
#ifdef USBH_DEBUG_IO
    debug_printf("I0(%d) ret=%d\r\n", size - wt_cnt[pipe], ret);
#endif
    API_FUNC_EXIT();
    if (ret < 0)
        return USBH_ERROR;
    return USBH_OK;
}

#ifdef USBH_USE_MSD
// bulk transfer(one or more transactions)
en_usb_status ul_bulk_out_transfer(int conn_num, st_usb_device *dev, int size, unsigned char* data)
{
#ifdef USBH_DEBUG_WAIT
    udelay(500000);
#endif
    int ret;
    int addr = dev->info.address;
    int pipe = BULK_OUT_PIPENO;
    API_FUNC_ENTER();
#ifdef USBH_DEBUG_BO
    debug_printf("BO%d P:%d A:%d E:%d\r\n",size, pipe, addr, dev->info.interface[dev->info.cur_interface].ea_out);
#endif
    usbh_pipe_set_pidnak(pipe);
    usbh_pipe_set_ep(pipe, (int)dev->info.interface[dev->info.cur_interface].ea_out, DIR_W, PIPE_BULK, (int)pipe_max_packet[pipe], addr);
    //usbh_pipe_clr_sq(pipe);
    usbh_pipe_clr_buf(pipe);
    ret = wt_transfer(pipe, conn_num, dev, size, data);
    udelay(20000);
#ifdef USBH_DEBUG_BO
    debug_printf("B0(%d) ret=%d\r\n", size - wt_cnt[pipe], ret);
#endif
    API_FUNC_EXIT();
    if (ret < 0)
        return USBH_ERROR;
    return USBH_OK;
}

en_usb_status ul_bulk_in_transfer(int conn_num, st_usb_device *dev, int size, unsigned char* data)
{
    int ret;
    int addr = dev->info.address;
    int pipe = BULK_IN_PIPENO;
    API_FUNC_ENTER();
#ifdef USBH_DEBUG_BI
    debug_printf("BI%d P:%d A:%d E:%d\r\n", size, pipe, addr, dev->info.interface[dev->info.cur_interface].ea_in);
#endif
    usbh_pipe_set_pidnak(pipe);
    usbh_pipe_set_ep(pipe, (int)dev->info.interface[dev->info.cur_interface].ea_in, DIR_R, PIPE_BULK, (int)pipe_max_packet[pipe], addr);
    //usbh_pipe_clr_sq(pipe);
    ret = rd_transfer(pipe, conn_num, dev, size, data);
    udelay(20000);
#ifdef USBH_DEBUG_BI
    debug_printf("BI(%d) ret=%d\r\n", size - rd_cnt[pipe], ret);
#endif
    API_FUNC_EXIT();
    if (ret < 0)
        return USBH_ERROR;
    return USBH_OK;
}
#endif

void ul_hw_init(void)
{
    rx62n_init();
    ul_host_config(0); /* opencores USB host configuration */
    ul_host_bus_reset(0); /* bus reset */
}

void ul_hw_device_setup(int conn_num, st_usb_device *dev, int adrs)
{
    /* this function is called from um_device_setup */
    MPRINT("ul_hw_device_setup called\n");
}
