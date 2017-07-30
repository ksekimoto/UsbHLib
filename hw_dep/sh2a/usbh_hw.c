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
#define USBH_DEBUG          // Need to set for debugging (switching printf)
#define USBH_DEBUG_ERR      // print when an error happens
#define USBH_DEBUG_FIFO
#define USBH_RD
#define USBH_WT
#define USBH_DEBUG_INT
#define USBH_DEBUG_INT_BRDY
#define USBH_DEBUG_INT_BEMP
#define USBH_DEBUG_INT_NRDY
#define USBH_DEBUG_PIPE_PW
#define USBH_DEBUG_PIPE_PR
#define USBH_DEBUG_CS
#define USBH_DEBUG_CO
#define USBH_DEBUG_CI
#define USBH_DEBUG_AO
#define USBH_DEBUG_AI
#define USBH_DEBUG_BO
#define USBH_DEBUG_BI
//#define USBH_DEBUG_WAIT
//#define USBH_DEBUG_SETUP
//#define USBH_DEBUG_BUSRESET
//#define USBH_DEBUG_API_FUNC
#define USBH_DEBUG_DUMP_REGS_BO
#define USBH_DEBUG_DUMP_REGS_BI

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

/* FIFOSEL */
#define M_RCNT  (0x8000)
#define M_MBW   (0x0400)
#define M_ISEL  (0x0020)
#define M_BIGEND (0x0010)
#define M_CURPIPE (0x000f)

/* FIFOCTR */
#define M_BVAL  (0x8000)
#define M_BCLR  (0x4000)
#define M_FRDY  (0x2000)
#define M_DTLN  (0x0FFF)

volatile static int usb_attach = 0;
volatile static int usb_speed = 2;      // Default: High Speed
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
        (volatile PIPECTR *)&USB.DCPCTR.WORD,
        (volatile PIPECTR *)&USB.PIPE1CTR.WORD,
        (volatile PIPECTR *)&USB.PIPE2CTR.WORD,
        (volatile PIPECTR *)&USB.PIPE3CTR.WORD,
        (volatile PIPECTR *)&USB.PIPE4CTR.WORD,
        (volatile PIPECTR *)&USB.PIPE5CTR.WORD,
        (volatile PIPECTR *)&USB.PIPE6CTR.WORD,
        (volatile PIPECTR *)&USB.PIPE7CTR.WORD,
        (volatile PIPECTR *)&USB.PIPE8CTR.WORD,
        (volatile PIPECTR *)&USB.PIPE9CTR.WORD };

static volatile uint16_t *pFIFOSEL[USB_MAX_FIFO] = {
        (volatile uint16_t *)&USB.CFIFOSEL.WORD,
        (volatile uint16_t *)&USB.D0FIFOSEL.WORD,
        (volatile uint16_t *)&USB.D1FIFOSEL.WORD };

static volatile uint16_t *pFIFOCTR[USB_MAX_FIFO] = {
        (volatile uint16_t *)&USB.CFIFOCTR.WORD,
        (volatile uint16_t *)&USB.D0FIFOCTR.WORD,
        (volatile uint16_t *)&USB.D1FIFOCTR.WORD };

static volatile unsigned char *pFIFO[USB_MAX_FIFO] = {
        (volatile unsigned char *)&USB.CFIFO.BYTE,
        (volatile unsigned char *)&USB.D0FIFO.BYTE,
        (volatile unsigned char *)&USB.D1FIFO.BYTE };

static uint16_t *DEVADD[11] = { (uint16_t *)&USB.DEVADD0.WORD,
        (uint16_t *)&USB.DEVADD1.WORD, (uint16_t *)&USB.DEVADD2.WORD,
        (uint16_t *)&USB.DEVADD3.WORD, (uint16_t *)&USB.DEVADD4.WORD,
        (uint16_t *)&USB.DEVADD5.WORD, (uint16_t *)&USB.DEVADD6.WORD,
        (uint16_t *)&USB.DEVADD7.WORD, (uint16_t *)&USB.DEVADD8.WORD,
        (uint16_t *)&USB.DEVADD9.WORD, (uint16_t *)&USB.DEVADDA.WORD };

unsigned char *rd_ptr[USB_MAX_PIPE];
volatile int rd_cnt[USB_MAX_PIPE];
volatile int rd_end[USB_MAX_PIPE];
volatile int rd_err[USB_MAX_PIPE];

unsigned char *wt_ptr[USB_MAX_PIPE];
volatile int wt_cnt[USB_MAX_PIPE];
volatile int wt_end[USB_MAX_PIPE];

static uint16_t MaxPacket[USB_MAX_PIPE] = { 64, 512, 512, 512, 512, 512, 64, 64,
        64, 64 };

volatile int PipeFlag[USB_MAX_PIPE];

//#ifdef USBH_DEBUG_dump_int_regs
static void dump_int_regs(unsigned char *s_func, int f_delay)
{
    if (s_func)
        debug_printf("[%s]\r\n", s_func);
    debug_printf("sts0=%04X sts1=%04X enb0=%04X enb1=%04X\r\n",
    USB.INTSTS0.WORD, USB.INTSTS1.WORD, USB.INTENB0.WORD, USB.INTENB1.WORD);
    debug_printf("brdy=%04X nrdy=%04X bemp=%04X\r\n",
    USB.BRDYENB.WORD, USB.NRDYENB.WORD, USB.BEMPENB.WORD);
    if (f_delay)
        udelay(100000);
}

static void dump_pipe_regs(unsigned char *s_func, int f_delay)
{
    uint16_t tmp;
    if (s_func)
        debug_printf("[%s]\r\n", s_func);
    debug_printf("psel=%04X p0ctr=%04X p1ctr=%04X p2ctr=%04X\r\n",
    USB.PIPESEL.WORD, USB.DCPCTR.WORD, USB.PIPE1CTR.WORD, USB.PIPE2CTR.WORD);
    tmp = USB.PIPESEL.WORD;
    USB.PIPESEL.WORD = 1;
    debug_printf("P1 MAX:%04X CFG:%04X\r\n", USB.PIPEMAXP.WORD, USB.PIPECFG.WORD);
    USB.PIPESEL.WORD = 2;
    debug_printf("P2 MAX:%04X CFG:%04X\r\n", USB.PIPEMAXP.WORD, USB.PIPECFG.WORD);
    USB.PIPESEL.WORD = tmp;
    if (f_delay)
        udelay(100000);
}

static void dump_fifo_regs(unsigned char *s_func, int f_delay)
{
    if (s_func)
        debug_printf("[%s]\r\n", s_func);
    debug_printf("cfifosel=%04X d0fifosel=%04X d1fifosel=%04X\r\n",
    USB.CFIFOSEL.WORD, USB.D0FIFOSEL.WORD, USB.D1FIFOSEL.WORD);
    debug_printf("cfifoctr=%04X d0fifoctr=%04X d1fifoctr=%04X\r\n",
    USB.CFIFOCTR.WORD, USB.D0FIFOCTR.WORD, USB.D1FIFOCTR.WORD);
    if (f_delay)
        udelay(100000);
}
//#endif

static void wait(volatile int count)
{
    while (count-- > 0) {
        __asm__ __volatile__ ("nop");
    }
}

static void usb_set_max_packet_size(int speed)
{
    if (speed == 2) {           // High Speed
        MaxPacket[0] = 64;
        MaxPacket[1] = 512;
        MaxPacket[2] = 512;
        MaxPacket[3] = 512;
        MaxPacket[4] = 512;
    } else if (speed == 1) {    // Full Speed
        MaxPacket[0] = 64;
        MaxPacket[1] = 64;
        MaxPacket[2] = 64;
        MaxPacket[3] = 64;
        MaxPacket[4] = 64;
    } else {                    // Low Speed
        MaxPacket[0] = 8;
        MaxPacket[1] = 8;
        MaxPacket[2] = 8;
        MaxPacket[3] = 8;
        MaxPacket[4] = 8;
    }
}

static void usb_int_brdy_enable(int pipe)
{
    uint16_t tmp = USB.INTENB0.WORD;
    USB.INTENB0.WORD = (uint16_t)(tmp & (uint16_t)~(M_BEMPE | M_NRDYE | M_BRDYE));
    USB.BRDYENB.WORD |= (1 << pipe);
    USB.INTENB0.WORD = tmp;
}

static void usb_int_brdy_disable(int pipe)
{
    uint16_t tmp = USB.INTENB0.WORD;
    USB.INTENB0.WORD = (uint16_t)(tmp & (uint16_t)~(M_BEMPE | M_NRDYE | M_BRDYE));
    USB.BRDYENB.WORD &= ~(1 << pipe);
    USB.INTENB0.WORD = tmp;
}

static void usb_int_nrdy_enable(int pipe)
{
    uint16_t tmp = USB.INTENB0.WORD;
    USB.INTENB0.WORD = (uint16_t)(tmp & (uint16_t)~(M_BEMPE | M_NRDYE | M_BRDYE));
    USB.NRDYENB.WORD |= (1 << pipe);
    USB.INTENB0.WORD = tmp;
}

static void usb_int_nrdy_disable(int pipe)
{
    uint16_t tmp = USB.INTENB0.WORD;
    USB.INTENB0.WORD = (uint16_t)(tmp & (uint16_t)~(M_BEMPE | M_NRDYE | M_BRDYE));
    USB.NRDYENB.WORD &= ~(1 << pipe);
    USB.INTENB0.WORD = tmp;
}

static void usb_int_bemp_enable(int pipe)
{
    uint16_t tmp = USB.INTENB0.WORD;
    USB.INTENB0.WORD = (uint16_t)(tmp & (uint16_t)~(M_BEMPE | M_NRDYE | M_BRDYE));
    USB.BEMPENB.WORD |= (1 << pipe);
    USB.INTENB0.WORD = tmp;
}

static void usb_int_bemp_disable(int pipe)
{
    uint16_t tmp = USB.INTENB0.WORD;
    USB.INTENB0.WORD = (uint16_t)(tmp & (uint16_t)~(M_BEMPE | M_NRDYE | M_BRDYE));
    USB.BEMPENB.WORD &= ~(1 << pipe);
    USB.INTENB0.WORD = tmp;
}

void usbh_ctrl_int_enable(void)
{
    uint16_t buf;

    buf = USB.INTENB0.WORD;
    buf |= (M_BEMP | M_NRDY | M_BRDY);
    USB.INTENB0.WORD = buf;

    buf = USB.INTENB1.WORD;
    buf |= (M_SIGN | M_SACK);
    USB.INTENB1.WORD = buf;

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
    volatile uint16_t *pfifoctr = (volatile uint16_t *)pFIFOCTR[type];
    volatile unsigned char *pfifo = (volatile unsigned char *)pFIFO[type];
    unsigned char *buf = wt_ptr[pipe];
    if (wt_cnt[pipe] <= (int)MaxPacket[pipe]) {
        ret = 1;
        count = wt_cnt[pipe];
    } else {
        count = (int)MaxPacket[pipe];
    }
    len = count;
    if (count == 0) {
        *pfifoctr = (M_BVAL | M_BCLR);
    } else {
        while (count-- > 0) {
            if (buf != NULL) {
                *pfifo = *buf;
                buf++;
            }
        }
        if (len != (int)MaxPacket[pipe]) {
            *pfifoctr |= M_BVAL;
        }
    }
    wt_cnt[pipe] -= len;
    wt_ptr[pipe] = buf;
#ifdef USBH_DEBUG_FIFO
    debug_printf("FW(%d)\r\n", len);
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
        if ((count % (int)MaxPacket[pipe]) != 0)
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
    debug_printf("FR%d\r\n", len);
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
    uint16_t bempsts = USB.BEMPSTS.WORD;
    uint16_t bempenb = USB.BEMPENB.WORD;
    if ((USB.BEMPSTS.BIT.PIPE0BEMP == 1) && (USB.BEMPENB.BIT.PIPE0BEMPE == 1)) {
        USB.BEMPSTS.WORD = ~0x0001;
#ifdef USBH_POLLING
        usb_int_bemp_disable(PIPE0);
        intsts0_bemp = 1;
#else
        if (wt_cnt[PIPE0] != 0)
            usbh_buf2fifo(PIPE0);
        else {
            wt_end[PIPE0] = 1;
            usb_int_bemp_disable(PIPE0);
        }
        intsts0_bemp = 1;
#endif
    } else {
        USB.BEMPSTS.WORD = ~bempsts;
        for (pipe = 1; pipe < USB_MAX_PIPE; pipe++) {
            int mask = (1 << pipe);
            if ((bempsts & mask) && (bempenb & mask)) {
                USB.PIPESEL.BIT.PIPESEL = pipe;
                if (USB.PIPECFG.BIT.DIR == DIR_R) {
                    usbh_fifo2buf(pipe);
                    if (rd_end[pipe] == 1) {
                        usbh_pipe_set_pidnak(pipe);
                        usb_int_brdy_disable(pipe);
                    }
                } else {
#ifdef USBH_POLLING
                    usb_int_bemp_disable(pipe);
                    intsts0_bemp = 1;
#else
                    if (wt_cnt[pipe] != 0)
                        usbh_buf2fifo(pipe);
                    else {
                        wt_end[pipe] = 1;
                        usbh_pipe_set_pidnak(pipe);
                        usb_int_bemp_disable(pipe);
                    }
                    intsts0_bemp = 1;
#endif
                }
                USB.PIPESEL.BIT.PIPESEL = pipe;
            }
        }
    }
}

static void usbh_handle_nrdy(void)
{
    int pipe;
    uint16_t nrdysts = USB.NRDYSTS.WORD;
    uint16_t nrdyenb = USB.NRDYENB.WORD;
    if ((USB.NRDYSTS.BIT.PIPE0NRDY == 1) && (USB.NRDYENB.BIT.PIPE0NRDYE == 1)) {
        USB.NRDYSTS.WORD = ~0x0001;
        usb_int_nrdy_disable(PIPE0);
        intsts0_nrdy = 1;
    } else {
        USB.NRDYSTS.WORD = ~nrdysts;
        for (pipe = 1; pipe < USB_MAX_PIPE; pipe++) {
            int mask = (1 << pipe);
            if ((nrdysts & mask) && (nrdyenb & mask)) {
                //uint16_t pid = pPIPECTR[pipe]->BIT.PID;
                //if ((pid == PID_STALL_1) || (pid ==PID_STALL_2)) {
                //    PipeFlag[pipe] = PIPE_STALL;
                //    debug_printf("TA\n");
                //} else {
                rd_err[pipe]++;
                if (rd_err[pipe] > 10) {
                    usb_int_nrdy_disable(pipe);
                    intsts0_nrdy = 1;
                    PipeFlag[pipe] = PIPE_NORES;
                    debug_printf("NS\n");
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
    uint16_t brdysts = USB.BRDYSTS.WORD;
    uint16_t brdyenb = USB.BRDYENB.WORD;
    if ((USB.BRDYSTS.BIT.PIPE0BRDY == 1) && (USB.BRDYENB.BIT.PIPE0BRDYE == 1)) {
        USB.BRDYSTS.WORD = ~0x0001;
#ifdef USBH_POLLING
        usb_int_brdy_disable(PIPE0);
        intsts0_brdy = 1;
#else
        if (usb_speed > 0) {
            usbh_fifo2buf(PIPE0);
            usb_int_brdy_disable(PIPE0);
            intsts0_brdy = 1;
        } else {
            USB.PIPESEL.BIT.PIPESEL = PIPE0;
            if (USB.PIPECFG.BIT.DIR == DIR_R) {
                usbh_fifo2buf(PIPE0);
                if (rd_end[PIPE0] == 1) {
                    usbh_pipe_set_pidnak(PIPE0);
                    usb_int_brdy_disable(PIPE0);
                }
            } else {
                usbh_buf2fifo(PIPE0);
            }
            intsts0_brdy = 1;
            USB.PIPESEL.BIT.PIPESEL = 0;
        }
#endif
    } else {
        USB.BRDYSTS.WORD = ~brdysts;
        for (pipe = 1; pipe < USB_MAX_PIPE; pipe++) {
            int mask = (1 << pipe);
            if ((brdysts & mask) && (brdyenb & mask)) {
#ifdef USBH_POLLING
                intsts0_brdy = 1;
#else
                USB.PIPESEL.BIT.PIPESEL = pipe;
                if (USB.PIPECFG.BIT.DIR == DIR_R) {
                    usbh_fifo2buf(pipe);
                    if (rd_end[pipe] == 1) {
                        usbh_pipe_set_pidnak(pipe);
                        usb_int_brdy_disable(pipe);
                    }
                } else {
                    usbh_buf2fifo(pipe);
                }
                intsts0_brdy = 1;
                USB.PIPESEL.BIT.PIPESEL = 0;
#endif
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

void INT_USB_USBI(void)
{
    uint16_t intsts0, intsts1;
    uint16_t intenb0, intenb1;
    uint16_t brdysts, nrdysts, bempsts;
    uint16_t brdyenb, nrdyenb, bempenb;

    uint16_t savepipe1, savepipe2;
    uint16_t tmp;

    savepipe1 = USB.CFIFOSEL.WORD;
    savepipe2 = USB.PIPESEL.WORD;

    intsts0 = USB.INTSTS0.WORD;
    intsts1 = USB.INTSTS1.WORD;
    intenb0 = USB.INTENB0.WORD;
    intenb1 = USB.INTENB1.WORD;

    if ((intsts1 & M_BCHG) && (intenb1 & M_BCHGE)) {
        USB.INTSTS1.WORD = (uint16_t)~M_BCHG;
        //USB.INTENB1.BIT.BCHGE = 0;
#ifdef USBH_DEBUG_INT
        debug_printf("BC\n");
#endif
    } else if ((intsts1 & M_SACK) && (intenb1 & M_SACK)) {
        USB.INTSTS1.WORD = (uint16_t)~M_SACK;
#ifdef USBH_DEBUG_INT
        debug_printf("SA\n");
#endif
        intsts1_sack = 1;
    } else if ((intsts1 & M_SIGN) && (intenb1 & M_SIGN)) {
        USB.INTSTS1.WORD = (uint16_t)~M_SIGN;
#ifdef USBH_DEBUG_INT
        debug_printf("SI\n");
#endif
    } else if (((intsts1 & M_DTCH) == M_DTCH) && ((intenb1 & M_DTCHE) == M_DTCHE)) {
        USB.INTSTS1.WORD = (uint16_t)~M_DTCH;
        USB.INTENB1.BIT.DTCHE = 0;
#ifdef USBH_DEBUG_INT
        debug_printf("DT\n");
#endif
        USB.INTENB1.BIT.ATTCHE = 1;
    } else if (((intsts1 & M_ATTCH) == M_ATTCH) && ((intenb1 & M_ATTCHE) == M_ATTCHE)) {
        USB.INTSTS1.WORD = (uint16_t)~M_ATTCH;
        USB.INTENB1.BIT.ATTCHE = 0;
#ifdef USBH_DEBUG_INT
        debug_printf("AT\n");
#endif
        USB.INTENB1.BIT.DTCHE = 1;
    } else if ((intsts0 & intenb0 & (M_BEMP | M_NRDY | M_BRDY))) {
        brdysts = USB.BRDYSTS.WORD;
        nrdysts = USB.NRDYSTS.WORD;
        bempsts = USB.BEMPSTS.WORD;
        brdyenb = USB.BRDYENB.WORD;
        nrdyenb = USB.NRDYENB.WORD;
        bempenb = USB.BEMPENB.WORD;

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

    tmp = USB.CFIFOSEL.WORD;
    tmp &= (uint16_t)~(M_CURPIPE);
    tmp |= (uint16_t)(savepipe1 & M_CURPIPE);
    USB.CFIFOSEL.WORD = tmp;
    USB.PIPESEL.WORD = savepipe2;
}

static void usbh_DEVADD_set_speed(int n)
{
    uint16_t tmp = (*DEVADD[n]) & 0xff3f;
    switch(usb_speed) {
    case 2:
        *DEVADD[n] = tmp | 0x00c0;
        break;
    case 1:
        *DEVADD[n] = tmp | 0x0080;
        break;
    case 0:
        *DEVADD[n] = tmp | 0x0040;
        break;
    default:
        *DEVADD[n] = tmp;
        break;
    }
}

static void usbh_DEVADD_init(void)
{
    int i;
    for (i = 0; i < 10; i++) {
        usbh_DEVADD_set_speed(i);
    }
}

void usbh_int_init(void)
{
    USB.BRDYENB.WORD = 0x0001;      // BRDY interrupt enable for PIPE0
    USB.BEMPENB.WORD = 0x0001;      // BEMP interrupt enable for PIPE0
    USB.NRDYENB.WORD = 0x0001;      // BEMP interrupt enable for PIPE0

    USB.INTENB1.WORD |= (0x1 << 14); // USB bus change interrupt enable
    USB.INTENB1.WORD &= ~(0x1 << 12); // Disconnection detection interrupt disable
    USB.INTENB0.BIT.BEMPE = 1;
    USB.INTENB0.BIT.NRDYE = 1;
    USB.INTENB0.BIT.BRDYE = 1;

    //USB.BEMPENB.BIT.PIPE0BEMPE = 1;    // pipe 0 bemp interrupt enable
    //USB.BRDYENB.BIT.PIPE0BRDYE = 0;    // pipe 0 brdy inerrrupt disable
}

static inline void usbh_port_config(void)
{
    //PORT.PADR0.BYTE.L = 0x01;       // PA0 1
    //PORT.PADR0.BYTE.H = 0x00;       // PA1 0
    //PORT.PADR1.BYTE.L = 0x00;       // PA2 0
}

static inline void usbh_int_disable(void)
{
    USB.INTENB0.WORD = 0;
    USB.INTENB1.WORD = 0;
}

static inline void usbh_int_level(unsigned char level)
{
    INTC.IPR10.BIT._USB = level;
}

static void usbh_wait_line_statble(void)
{
    unsigned char s0, s1;
    do {
        s0 = USB.SYSSTS.BIT.LNST;
        udelay(50000);
        s1 = USB.SYSSTS.BIT.LNST;
    } while (s0 != s1);
}

static void usbh_pipe_clr_sq(int pipe);

void usbh_pipe_init(void)
{
    uint16_t pipe;
    usbh_DEVADD_init();
    usbh_wait_line_statble();
    usbh_pipe_clr_sq(BULK_IN_PIPENO);
    usbh_pipe_clr_sq(BULK_OUT_PIPENO);
    usbh_pipe_clr_sq(INT_IN_PIPENO);
    usbh_pipe_clr_sq(INT_OUT_PIPENO);
    for (pipe = 1; pipe <= 2; pipe++) {
        USB.PIPESEL.BIT.PIPESEL = pipe;
        USB.PIPEMAXP.BIT.MXPS = MaxPacket[pipe];
        USB.PIPEBUF.BIT.BUFNMB = pipe * 8;
        USB.PIPEBUF.BIT.BUFSIZE = 7;
    }
    USB.PIPESEL.BIT.PIPESEL = 0;
}

void usbh_reset_module(void)
{
    USB.USBACSWR1.BIT.UACS25 = 1;
    USB.SYSCFG.WORD &= ~0x0400;     // SCKE = 0
    udelay(1000);
    USB.SYSCFG.WORD |= 0x0400;      // SCKE = 1
    USB.BUSWAIT.WORD = 3;
}

void usbh_init_host(void)
{
    uint16_t buf1, buf2, buf3;
    USB.SYSCFG.WORD |= (0x7 << 5);  // USB host (DCFM, DRPD, HSE)
    do {
        buf1 = USB.SYSSTS.BIT.LNST;
        udelay(50000);
        buf2 = USB.SYSSTS.BIT.LNST;
        udelay(50000);
        buf3 = USB.SYSSTS.BIT.LNST;

    } while ((buf1 != buf2) || (buf1 != buf3));
    USB.SYSCFG.WORD |= 0x1;         // USB clock operation enabled
    USB.CFIFOSEL.WORD = 0x8100;    // 8bit
    USB.D0FIFOSEL.WORD = 0x0100;    // 8bit
    USB.D1FIFOSEL.WORD = 0x0100;    // 8bit
}

void usbh_init(void)
{
    uint16_t buf;
    API_FUNC_ENTER();

    CPG.STBCR6.BIT.MSTP60 = 0;
    usbh_port_config();
    usbh_int_level(11);
    usbh_reset_module();
    usbh_init_host();
    usbh_int_init();
    udelay(400);
    //usbh_int_disable();
    usbh_pipe_init();
    udelay(500);
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
    if ((size % (int)MaxPacket[pipe]) == 0) {
        count = size / (int)MaxPacket[pipe];
    } else {
        count = size / (int)MaxPacket[pipe] + 1;
    }
    switch (pipe) {
        case 1:
        USB.PIPE1TRE.BIT.TRCLR = 1;
        USB.PIPE1TRN = count;
        USB.PIPE1TRE.BIT.TRENB = 1;
        break;
        case 2:
        USB.PIPE2TRE.BIT.TRCLR = 1;
        USB.PIPE2TRN = count;
        USB.PIPE2TRE.BIT.TRENB = 1;
        break;
        case 3:
        USB.PIPE3TRE.BIT.TRCLR = 1;
        USB.PIPE3TRN = count;
        USB.PIPE3TRE.BIT.TRENB = 1;
        break;
        case 4:
        USB.PIPE4TRE.BIT.TRCLR = 1;
        USB.PIPE4TRN = count;
        USB.PIPE4TRE.BIT.TRENB = 1;
        break;
        case 5:
        USB.PIPE5TRE.BIT.TRCLR = 1;
        USB.PIPE5TRN = count;
        USB.PIPE5TRE.BIT.TRENB = 1;
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
        USB.PIPE1TRE.BIT.TRENB = 0;
        USB.PIPE1TRE.BIT.TRCLR = 1;
        break;
        case 2:
        USB.PIPE2TRE.BIT.TRENB = 0;
        USB.PIPE2TRE.BIT.TRCLR = 1;
        break;
        case 3:
        USB.PIPE3TRE.BIT.TRENB = 0;
        USB.PIPE3TRE.BIT.TRCLR = 1;
        break;
        case 4:
        USB.PIPE4TRE.BIT.TRENB = 0;
        USB.PIPE4TRE.BIT.TRCLR = 1;
        break;
        case 5:
        USB.PIPE5TRE.BIT.TRENB = 0;
        USB.PIPE5TRE.BIT.TRCLR = 1;
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
        pipectr = (volatile PIPECTR *)&USB.PIPE1CTR.WORD;
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
    uint16_t cpid = pipectr->BIT.PID;
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
    if (USB.CFIFOSEL.BIT.CURPIPE == pipe)
        USB.CFIFOSEL.BIT.CURPIPE = 0;
    if (USB.D0FIFOSEL.BIT.CURPIPE == pipe)
        USB.D0FIFOSEL.BIT.CURPIPE = 0;
    if (USB.D1FIFOSEL.BIT.CURPIPE == pipe)
        USB.D1FIFOSEL.BIT.CURPIPE = 0;
}
#endif

static void usbh_pipe_set_ep(int pipe, int ep, int dir, int attr, int size, int addr)
{
    usbh_pipe_set_pidnak(pipe);
    USB.PIPESEL.BIT.PIPESEL = pipe;
    USB.PIPECFG.WORD = 0;
    USB.PIPECFG.BIT.EPNUM = ep;        // Endpoint
    USB.PIPECFG.BIT.DIR = dir;         // Direction
    USB.PIPECFG.BIT.DBLB = 0;          // Double Buffer
#ifdef USBH_USE_TRANSACTION
            if (dir == DIR_R) {
                USB.PIPECFG.BIT.BFRE = 1;          // Interrupt
                USB.PIPECFG.BIT.SHTNAK = 1;
            } else
            USB.PIPECFG.BIT.BFRE = 0;          // Interrupt
#else
    USB.PIPECFG.BIT.BFRE = 0;          // Interrupt
    if (dir == DIR_R) {
        USB.PIPECFG.BIT.SHTNAK = 0;
    }
#endif
    if (attr == PIPE_BULK) {
        USB.PIPECFG.BIT.TYPE = PIPE_BULK;
        //pipectr->BIT.SQCLR = 1;
    } else if (attr == PIPE_INT) {
        USB.PIPECFG.BIT.TYPE = PIPE_INT;
    } else if (attr == PIPE_ISO) {
        USB.PIPECFG.BIT.TYPE = PIPE_ISO;
    }
    USB.PIPEMAXP.BIT.MXPS = MaxPacket[pipe];    // Packet Size
    USB.PIPEMAXP.BIT.DEVSEL = addr;             // Device Address
    USB.PIPEBUF.BIT.BUFNMB = pipe * 8;
    USB.PIPEBUF.BIT.BUFSIZE = 7;
    USB.PIPESEL.BIT.PIPESEL = 0;                // Unselect Pipe
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
    volatile uint16_t tmp;
    int type = USBH_FifoType[pipe];
    volatile uint16_t *pfifosel = pFIFOSEL[type];
    volatile uint16_t *pfifoctr = pFIFOCTR[type];
    if (pipe == PIPE0) {
        if (dir == DIR_W)
            USB.DCPCFG.BIT.DIR = DIR_W;
        else
            USB.DCPCFG.BIT.DIR = DIR_R;
    }
#if 0
    dump_fifo_regs(__func__, 1);
#endif
    tmp = *pfifosel;
    if (type == FIFO_CFIFO) {
        tmp &= ~(M_MBW | M_ISEL | M_CURPIPE);
        tmp |= (uint16_t)((mbw << 10) | (dir << 5) | pipe);
    } else {
        tmp &= ~(M_MBW | M_CURPIPE);
        tmp |= (uint16_t)((mbw << 10) | pipe);
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
    volatile uint16_t *pfifosel = (volatile uint16_t *)pFIFOSEL[type];
    volatile uint16_t *pfifoctr = (volatile uint16_t *)pFIFOCTR[type];
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
    volatile uint16_t *pfifoctr = (volatile uint16_t *)pFIFOCTR[type];
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
#ifdef USBH_DEBUG
    debug_printf("attach\n");
#endif
    usbh_ctrl_int_enable();
    USB.INTSTS1.BIT.BCHG = 0;
    USB.INTENB1.BIT.BCHGE = 0;
    USB.INTSTS1.BIT.ATTCH = 0;
    USB.INTENB1.BIT.ATTCHE = 1;
    USB.INTSTS1.BIT.DTCH = 0;
    USB.INTENB1.BIT.DTCHE = 1;
    API_FUNC_EXIT();
}

void ul_host_config(int conn_num)
{
    API_FUNC_ENTER();
    usbh_init();
    usbh_attach();
    API_FUNC_EXIT();
}

int usbh_check_attach(void)
{
    uint16_t buf1, buf2, buf3;
    uint16_t rhst;
    int attach = 0;
    API_FUNC_ENTER();
#ifdef USBH_DEBUG
    debug_printf("check attach\n");
#endif
    do {
        buf1 = USB.SYSSTS.BIT.LNST;
        udelay(50000);
        buf2 = USB.SYSSTS.BIT.LNST;
        udelay(50000);
        buf3 = USB.SYSSTS.BIT.LNST;
    } while ((buf1 != buf2) || (buf1 != buf3));
    rhst = USB.DVSTCTR.BIT.RHST;
    if (rhst == 0) {
        if (buf1 == 1) {
            USB.SYSCFG.BIT.HSE = 1; // High
            usb_speed = 2;
            attach = 1;
        } else if (buf1 == 2) {
            USB.SYSCFG.BIT.HSE = 0; // Low
            usb_speed = 0;
            attach = 1;
        }
    } else if (rhst > 0) {
        if (rhst == 3)
            USB.SYSCFG.BIT.HSE = 1; // High
        else
            USB.SYSCFG.BIT.HSE = 0; // Full or Low
        usb_speed = rhst - 1;
        attach = 1;
    }
    usb_set_max_packet_size(usb_speed);
    if (attach) {
        debug_printf("USB attached. speed %d\n", usb_speed);
    } else
        debug_printf("USB detached.\n");
    API_FUNC_EXIT();
    return attach;
}

void ul_host_bus_reset(int conn_num)
{
    uint16_t i, buffer;
#ifdef USBH_DEBUG_BUSRESET
    debug_printf("bus reset start\r\n");
#endif
    USB.DVSTCTR.BIT.USBRST = 1;
    USB.DVSTCTR.BIT.UACT = 0;
    udelay(50000);
    USB.DVSTCTR.BIT.UACT = 1;       // SOF out enable
    USB.DVSTCTR.BIT.USBRST = 0;
    udelay(20000);

    for (i = 0, buffer = 4; i < 3; ++i) {
        buffer = USB.DVSTCTR.BIT.RHST;
        if (buffer == 4) {
            udelay(10000);
        } else {
            break;
        }
    }
#ifdef USBH_DEBUG_BUSRESET
    debug_printf("bus reset end\r\n");
#endif
    return;
}

//int wait_int_event(int conn_num,en_int_status s) {
//  return USBH_ERROR;
//}

int ul_speed_configuration(int conn_num)
{
    int attach;
    int timeout = 1;
    API_FUNC_ENTER();
    do {
        attach = usbh_check_attach();
        if (attach)
            break;
        udelay(500000);
    } while (timeout-- > 0); API_FUNC_EXIT();
    return usb_speed;
}

//uint16_t get_max_packet_size(void)
//{
//  API_FUNC_ENTER();
//  API_FUNC_EXIT();
//  return MaxPacket[0];
//}

int rd_transfer(int pipe, int conn_num, st_usb_device *dev, int size, unsigned char* data)
{
    int ret = 0;
    int timeout = 20000;
    PipeFlag[pipe] = PIPE_WAIT;
    volatile uint16_t *pfifoctr = pFIFOCTR[USBH_FifoType[pipe]];
#ifdef USBH_RD
    debug_printf("RD%d\r\n", size);
#endif
    usbh_pipe_set_pidnak(pipe);
    rd_ptr[pipe] = data;
    rd_cnt[pipe] = size;
    rd_end[pipe] = 0;
    rd_err[pipe] = 0;
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
            debug_printf("TO\r\n");
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
    USB.PIPESEL.WORD = PIPE0;
    usbh_pipe_clr_buf(pipe);
    if (rd_cnt[pipe] != 0) {
        ret = -1;
    }
    return ret;
}

int wt_transfer(int pipe, int conn_num, st_usb_device *dev, int size, unsigned char* data)
{
    int ret = 0;
    int timeout = 20000;
    PipeFlag[pipe] = PIPE_WAIT;
    volatile uint16_t *pfifoctr = pFIFOCTR[USBH_FifoType[pipe]];
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
    USB.PIPESEL.WORD = PIPE0;
    usbh_pipe_clr_buf(pipe);
    if (wt_cnt[pipe] != 0)
        ret = -1;
    return ret;
}

en_usb_status ul_control_setup_transaction(int conn_num, st_usb_device *dev, st_usb_setup_data *sd)
{
    int timeout;
    int addr = dev->info.address;
#ifdef USBH_DEBUG_WAIT
    udelay(500000);
#endif
#ifdef USBH_DEBUG_CS
    debug_printf("CS\r\n");
#endif
    API_FUNC_ENTER();
    if (usb_speed > 0) {
        USB.SOFCFG.BIT.TRNENSEL = 0;
    } else {
        USB.SOFCFG.BIT.TRNENSEL = 1;
    }
    usbh_DEVADD_set_speed(addr);
    intsts1_sack = 0;
    USB.DCPMAXP.WORD = (uint16_t)((uint16_t)(addr << 12) + MaxPacket[PIPE0]);
    USB.INTSTS1.WORD = (uint16_t)~(M_SACK | M_SIGN); /* Status Clear */
#ifdef USBH_DEBUG_dump_int_regs
    dump_int_regs(__func__, 1);
#endif
    USB.USBREQ.WORD = ((uint16_t)sd->BYTE.bRequest << 8) + (uint16_t)sd->BYTE.bmRequestType;
    USB.USBVAL = ((uint16_t)sd->BYTE.wValue[1] << 8) + (uint16_t)sd->BYTE.wValue[0];
    USB.USBINDX = ((uint16_t)sd->BYTE.wIndex[1] << 8) + (uint16_t)sd->BYTE.wIndex[0];
    USB.USBLENG = ((uint16_t)sd->BYTE.wLength[1] << 8) + (uint16_t)sd->BYTE.wLength[0];
    USB.DCPCTR.WORD = M_SUREQ;
#ifdef USBH_DEBUG_SETUP
    debug_printf("ST %2x %2x %2x %2x\r\n", USB.USBREQ.WORD, USB.USBVAL, USB.USBINDX, USB.USBLENG);
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
    } API_FUNC_EXIT();
    return USBH_OK;
}

en_usb_status ul_control_in_transaction(int conn_num, st_usb_device *dev, int size, unsigned char *data)
{
#ifdef USBH_POLLING
    int timeout;
    int len = 0;
    API_FUNC_ENTER();
#ifdef USBH_DEBUG_CI
    debug_printf("CI%d\r\n",size);
#endif
    intsts0_brdy = 0;
    usbh_pipe_set_pidnak(PIPE0);
    USB.DCPCTR.WORD = M_SQSET;
    USB.DCPCFG.BIT.DIR = DIR_R;
    USB.CFIFOSEL.WORD = (USB.CFIFOSEL.WORD & ~(M_ISEL | M_CURPIPE | M_MBW)) | ((ISEL_R << 5) | PIPE0 | MBW_VAL);
    udelay(20);
    USB.CFIFOCTR.WORD = M_BCLR;
    usb_int_brdy_enable(PIPE0);
    usb_int_nrdy_enable(PIPE0);
    usbh_pipe_set_pidbuf(PIPE0);
    timeout = 1000;
    while (1) {
        if (intsts0_brdy == 1)
        break;
        udelay(100);
        if (timeout-- == 0) {
#ifdef USBH_DEBUG_ERR
            debug_printf("* [%s] timeout\r\n", __func__);
#endif
            return USBH_ERROR;
            //break;
        }
    }
    usbh_pipe_set_pidnak(PIPE0);
    usbh_pipe_read(PIPE0, data, (int)MaxPacket[PIPE0]], &len);
#ifdef USBH_DEBUG_CI
    debug_printf("CI(%d)\r\n", len);
#endif
    API_FUNC_EXIT();
    return USBH_OK;
#else
    int ret;
    int pipe = PIPE0;
    API_FUNC_ENTER();
    usbh_pipe_set_pidnak(pipe);
    USB.DCPCTR.WORD = M_SQSET;
    ret = rd_transfer(pipe, conn_num, dev, size, data);
    API_FUNC_EXIT();
    if (ret < 0)
        return USBH_ERROR;
    return USBH_OK;
#endif
}

en_usb_status ul_control_out_transaction(int conn_num, st_usb_device *dev, int size, unsigned char *data)
{
#ifdef USBH_POLLING
    int timeout;
#ifdef USBH_DEBUG_CS
    debug_printf("CO%d\r\n",size);
#endif
    API_FUNC_ENTER();
    intsts0_bemp = 0;
    usbh_pipe_set_pidnak(PIPE0);
    USB.DCPCTR.WORD = M_SQSET;
    USB.DCPCFG.BIT.DIR = DIR_W;
    usbh_pipe_write(PIPE0, data, (int)MaxPacket[PIPE0], size);
    timeout = 1000;
    while (1) {
        if (intsts0_bemp == 1)
        break;
        udelay(100);
        if (timeout-- == 0) {
#ifdef USBH_DEBUG_ERR
            debug_printf("* [%s] timeout\r\n", __func__);
#endif
            return USBH_ERROR;
        }
    }
    usbh_pipe_set_pidnak(PIPE0);
    API_FUNC_EXIT();
    return USBH_OK;
#else
    int ret;
    int pipe = PIPE0;
    API_FUNC_ENTER();
    usbh_pipe_set_pidnak(pipe);
    USB.DCPCTR.WORD = M_SQSET;
    ret = wt_transfer(pipe, conn_num, dev, size, data);
    API_FUNC_EXIT();
    if (ret < 0)
        return USBH_ERROR;
    return USBH_OK;
#endif
}

en_usb_status ul_control_ack_out_transaction(int conn_num, st_usb_device *dev)
{
#ifdef USBH_POLLING
    int timeout;
#ifdef USBH_DEBUG_AO
    debug_printf("AO\r\n");
#endif
    API_FUNC_ENTER();
    intsts0_bemp = 0;
    usbh_pipe_set_pidnak(PIPE0);
    USB.DCPCTR.WORD = M_SQSET;
    USB.DCPCFG.BIT.DIR = DIR_W;
    USB.CFIFOSEL.WORD = (USB.CFIFOSEL.WORD & ~(M_ISEL | M_CURPIPE | M_MBW)) | ((ISEL_W << 5) | PIPE0 | MBW_VAL);
    USB.CFIFOCTR.WORD = M_BCLR;
    usbh_pipe_write(PIPE0, 0, 0, 0);
    timeout = 1000;
    while (1) {
        if (intsts0_bemp == 1)
        break;
        udelay(100);
        if (timeout-- == 0) {
#ifdef USBH_DEBUG_ERR
            debug_printf("* [%s] timeout\r\n", __func__);
#endif
            return USBH_ERROR;
        }
    }
    usbh_pipe_set_pidnak(PIPE0);
    API_FUNC_EXIT();
    return USBH_OK;
#else
    int ret;
    int pipe = PIPE0;
    API_FUNC_ENTER();
    usbh_pipe_set_pidnak(pipe);
    USB.DCPCTR.WORD = M_SQSET;
    ret = wt_transfer(pipe, conn_num, dev, 0, 0);
    API_FUNC_EXIT();
    if (ret < 0)
        return USBH_ERROR;
    return USBH_OK;
#endif
}

en_usb_status ul_control_ack_in_transaction(int conn_num, st_usb_device *dev)
{
#ifdef USBH_POLLING
    int timeout;
    int len = 0;
#ifdef USBH_DEBUG_AI
    debug_printf("AI\r\n");
#endif
    API_FUNC_ENTER();
    intsts0_brdy = 0;
    usbh_pipe_set_pidnak(PIPE0);
    USB.DCPCTR.WORD = M_SQSET;
    USB.DCPCFG.BIT.DIR = DIR_R;
    USB.CFIFOSEL.WORD = (USB.CFIFOSEL.WORD & ~(M_ISEL | M_CURPIPE | M_MBW)) | ((ISEL_R << 5) | PIPE0 | MBW_VAL );
    udelay(20);
    USB.CFIFOCTR.WORD = M_BCLR;
    usb_int_brdy_enable(PIPE0);
    usb_int_nrdy_enable(PIPE0);
    usbh_pipe_set_pidbuf(PIPE0);
    timeout = 1000;
    while (1) {
        if (intsts0_brdy == 1)
        break;
        udelay(100);
        if (timeout-- == 0) {
#ifdef USBH_DEBUG_ERR
            debug_printf("* [%s] timeout\r\n", __func__);
#endif
            return USBH_ERROR;
        }
    }
    usbh_pipe_set_pidnak(PIPE0);
    usbh_pipe_read(PIPE0, 0, 0, &len);
    API_FUNC_EXIT();
    return USBH_OK;
#else
    int ret;
    int pipe = PIPE0;
    API_FUNC_ENTER();
    usbh_pipe_set_pidnak(pipe);
    USB.DCPCTR.WORD = M_SQSET;
    ret = rd_transfer(pipe, conn_num, dev, 0, 0);
    API_FUNC_EXIT();
    if (ret < 0)
        return USBH_ERROR;
    return USBH_OK;
#endif
}

// interrupt transfer(one or more transactions)
en_usb_status ul_interrupt_in_transfer(int conn_num, st_usb_device *dev, int *size, unsigned char *data)
{
    int ret;
    int addr = dev->info.address;
    int pipe = INT_IN_PIPENO;
    API_FUNC_ENTER();
    *size = MaxPacket[pipe];
#ifdef USBH_DEBUG_II
    debug_printf("II%d P:%d A:%d E:%d\r\n", *size, pipe, addr, dev->info.interface[dev->info.cur_interface].ea_in);
#endif
    usbh_pipe_set_pidnak(pipe);
    usbh_pipe_set_ep(pipe, (int)dev->info.interface[dev->info.cur_interface].ea_in, DIR_R, PIPE_INT, (int)MaxPacket[pipe], addr);
    //usbh_pipe_clr_sq(pipe);
    ret = rd_transfer(pipe, conn_num, dev, *size, data);
    udelay(20000);
#ifdef USBH_DEBUG_II
    debug_printf("II(%d) ret=%d\r\n", *size - rd_cnt[pipe], ret);
#endif
    *size = rd_cnt[pipe];
    API_FUNC_EXIT();
    //if (ret < 0)
    //  return USBH_ERROR;
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
    usbh_pipe_set_ep(pipe, (int)dev->info.interface[dev->info.cur_interface].ea_out, DIR_W, PIPE_INT, (int)MaxPacket[pipe], addr);
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
#ifdef USBH_POLLING
    int timeout;
    int addr = dev->info.address;
    int pipe = BULK_OUT_PIPENO;
    volatile uint16_t *pfifoctr = pFIFOCTR[(int)USBH_FifoType[pipe]];
#ifdef USBH_DEBUG_BO
    debug_printf("BO%d P:%d A:%d E:%d\r\n",size, pipe, addr, dev->info.interface[dev->info.cur_interface].ea_out);
#endif
    API_FUNC_ENTER();
    intsts0_bemp = 0;
    usbh_pipe_set_pidnak(pipe);
    usbh_pipe_set_ep(pipe, (int)dev->info.interface[dev->info.cur_interface].ea_out, DIR_W, PIPE_BULK, (int)MaxPacket[pipe], addr);
    usbh_pipe_clr_sq(pipe);
    //usbh_pipe_clear_counter(pipe);
#ifdef USBH_DEBUG_DUMP_REGS_BO
    dump_int_regs(__func__, 1);
    dump_pipe_regs(__func__, 1);
    dump_fifo_regs(__func__, 1);
#endif
    usbh_pipe_write(pipe, data, (int)MaxPacket[pipe], size);
    timeout = 10000;
    while (1) {
        if (intsts0_bemp == 1)
        break;
        udelay(100);
        if (timeout-- == 0) {
#ifdef USBH_DEBUG_ERR
            debug_printf("* [%s] timeout\r\n", __func__);
#endif
            return USBH_ERROR;
        }
    }
    usbh_pipe_set_pidnak(pipe);
    usb_int_brdy_disable(pipe);   // disable interrupt
    usb_int_bemp_disable(pipe);// disable interrupt
    usb_int_nrdy_disable(pipe);// disable interrupt
    //usbh_pipe_clear_counter(pipe);
    usbh_pipe_clr_buf(pipe);
    API_FUNC_EXIT();
    return USBH_OK;
#else
    int ret;
    int addr = dev->info.address;
    int pipe = BULK_OUT_PIPENO;
    API_FUNC_ENTER();
#ifdef USBH_DEBUG_BO
    debug_printf("BO%d P:%d A:%d E:%d\r\n", size, pipe, addr, dev->info.interface[dev->info.cur_interface].ea_out);
#endif
    usbh_DEVADD_set_speed(addr);
    usbh_pipe_set_pidnak(pipe);
    usbh_pipe_set_ep(pipe, (int)dev->info.interface[dev->info.cur_interface].ea_out, DIR_W, PIPE_BULK, (int)MaxPacket[pipe], addr);
    //usbh_pipe_clr_buf(pipe);
    ret = wt_transfer(pipe, conn_num, dev, size, data);
#ifdef USBH_DEBUG_BO
    debug_printf("B0(%d) ret=%d\r\n", size - wt_cnt[pipe], ret);
    udelay(20000);
#endif
    API_FUNC_EXIT();
    if (ret < 0) {
        //if (PipeFlag[pipe] == PIPE_STALL)
        //  return USBH_STALL;
        return USBH_ERROR;
    }
    return USBH_OK;
#endif
}

en_usb_status ul_bulk_in_transfer(int conn_num, st_usb_device *dev, int size, unsigned char* data)
{
#ifdef USBH_POLLING
    int timeout;
    int tmp, len = 0;
    int addr = dev->info.address;
    int pipe = BULK_IN_PIPENO;
    volatile uint16_t *pfifoctr = pFIFOCTR[(int)USBH_FifoType[pipe]];
    API_FUNC_ENTER();
#ifdef USBH_DEBUG_BI
    debug_printf("BI%d A:%d E:%d\r\n",size, addr, dev->info.interface[dev->info.cur_interface].ea_in);
#endif
    usbh_pipe_set_pidnak(pipe);
    usbh_pipe_set_ep(pipe, dev->info.interface[dev->info.cur_interface].ea_in, DIR_R, PIPE_BULK, (int)MaxPacket[pipe], addr);
    usbh_pipe_clr_sq(pipe);
    usbh_pipe_set_fifo(pipe, ISEL_R, MBW_BYTE);
#ifdef USBH_DEBUG_DUMP_REGS_BI
    dump_int_regs(__func__, 1);
    dump_pipe_regs(__func__, 1);
    dump_fifo_regs(__func__, 1);
#endif
    udelay(1000);
    while (len < size) {
        if (size > (int)MaxPacket[pipe]) {
            tmp = (int)MaxPacket[pipe];
        } else {
            tmp = size;
        }
        intsts0_brdy = 0;
        *pfifoctr = M_BCLR;
        usb_int_brdy_enable(pipe);
        usb_int_nrdy_enable(pipe);
        usbh_pipe_set_pidbuf(pipe);
        timeout = 4000;
        while (1) {
            if (intsts0_brdy == 1)
            break;
            udelay(500);
            if (timeout-- == 0) {
                usbh_pipe_set_pidnak(pipe);
                usb_int_brdy_disable(pipe);   // disable interrupt
                usb_int_bemp_disable(pipe);// disable interrupt
                usb_int_nrdy_disable(pipe);// disable interrupt
#ifdef USBH_DEBUG_ERR
                debug_printf("* [%s] timeout\r\n", __func__);
#endif
                debug_printf("BI(%d)\r\n", len);
                return USBH_ERROR;
            }
        }
        usbh_pipe_set_pidnak(pipe);
        usbh_pipe_read(pipe, data, (int)MaxPacket[pipe], &tmp);
        data += tmp;
        len += tmp;
        size -= tmp;
    }
    usbh_pipe_clr_buf(pipe);
    usb_int_brdy_disable(pipe);   // disable interrupt
    usb_int_bemp_disable(pipe);// disable interrupt
    usb_int_nrdy_disable(pipe);// disable interrupt
#ifdef USBH_DEBUG_BI
    debug_printf("BI(%d)\r\n", len);
#endif
    API_FUNC_EXIT();
    return USBH_OK;
#else
    int ret;
    int addr = dev->info.address;
    int pipe = BULK_IN_PIPENO;
    API_FUNC_ENTER();
#ifdef USBH_DEBUG_BI
    debug_printf("BI%d P:%d A:%d E:%d\r\n", size, pipe, addr, dev->info.interface[dev->info.cur_interface].ea_in);
#endif
    usbh_DEVADD_set_speed(addr);
    usbh_pipe_set_pidnak(pipe);
    usbh_pipe_set_ep(pipe, (int)dev->info.interface[dev->info.cur_interface].ea_in, DIR_R, PIPE_BULK, (int)MaxPacket[pipe], addr);
    //usbh_pipe_clr_sq(pipe);
    ret = rd_transfer(pipe, conn_num, dev, size, data);
#ifdef USBH_DEBUG_BI
    debug_printf("BI(%d) ret=%d err=%d\r\n", size - rd_cnt[pipe], ret, rd_err[pipe]);
    udelay(20000);
#endif
    API_FUNC_EXIT();
    if (ret < 0) {
        if (PipeFlag[pipe] == PIPE_STALL)
            return USBH_STALL;
        return USBH_ERROR;
    }
    return USBH_OK;
#endif
}
#endif

void ul_hw_init(void)
{
    sh2a_init();
    ul_host_config(0); /* opencores USB host configuration */
    ul_host_bus_reset(0); /* bus reset */
}

void ul_hw_device_setup(int conn_num, st_usb_device *dev, int adrs)
{
    /* this function is called from um_device_setup */
    MPRINT("ul_hw_device_setup called\n");
    //usbh_DEVADD_set_speed(adrs);
    //usbh_pipe_set_pidnak(BULK_OUT_PIPENO);
    //usbh_pipe_set_ep(BULK_OUT_PIPENO, (int)dev->info.interface[dev->info.cur_interface].ea_out, DIR_W, PIPE_BULK, (int)MaxPacket[pipe], adrs);
    //usbh_pipe_set_pidnak(BULK_IN_PIPENO);
    //usbh_pipe_set_ep(BULK_IN_PIPENO, (int)dev->info.interface[dev->info.cur_interface].ea_in, DIR_R, PIPE_BULK, (int)MaxPacket[pipe], adrs);
}
