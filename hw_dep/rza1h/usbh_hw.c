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
#include "rza_io_regrw.h"
#include "iodefine.h"
#include "MBRZA1H.h"

#define RegW16  RZA_IO_RegWrite_16
#define RegR16  RZA_IO_RegRead_16

//#define USBH_STATIC_PIPE_CFG
//#define USBH_CLEAR_INT_STATUS

//#define USBH_USE_DBLB
//#define USBH_USE_CNTMD
//#define USBH_USE_TRANSACTION
//#define RENESAS_SAMPLE
//#define USE_RENESAS_FUNCTION_INT
//#define USE_RENESAS_FUNCTION_SETEP
//#define USE_RENESAS_FUNCTION_SETFIFO
#define USBH_DEBUG
#define USBH_DEBUG_ERR
//#define USBH_DEBUG_ERR_NS
//#define USBH_DEBUG_ERR_TO
#define USBH_DEBUG_SPEED
//#define USBH_DEBUG_FIFOR
//#define USBH_DEBUG_FIFOW
//#define USBH_RD
//#define USBH_WT
//#define USBH_DEBUG_INT
//#define USBH_DEBUG_INT_BRDY
//#define USBH_DEBUG_INT_BEMP
//#define USBH_DEBUG_INT_NRDY
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

#define M_PIPE0BEMP     1
#define M_PIPE0BEMPE    1
#define M_PIPE0NRDY     1
#define M_PIPE0NRDYE    1
#define M_PIPE0BRDY     1
#define M_PIPE0BRDYE    1

#define M_PIPECFG_EPNUM     0x000f
#define M_PIPECFG_DIR       0x0010
#define M_PIPECFG_SHTNAK    0x0080
#define M_PIPECFG_CNTMD     0x0100
#define M_PIPECFG_DBLB      0x0200
#define M_PIPECFG_BFRE      0x0400
#define M_PIPECFG_TYPE      0xc000
#define S_PIPECFG_EPNUM     0
#define S_PIPECFG_DIR       4
#define S_PIPECFG_SHTNAK    7
#define S_PIPECFG_CNTMD     8
#define S_PIPECFG_DBLB      9
#define S_PIPECFG_BFRE      10
#define S_PIPECFG_TYPE      14

#define M_PIPEMAXP_DEVSEL   0xf000
#define M_PIPEMAXP_MXPS     0x03ff
#define S_PIPEMAXP_DEVSEL   12
#define S_PIPEMAXP_MXPS     0

#define M_PIPEBUF_BUFSIZE   0x7c00
#define M_PIPEBUF_BUFNMB    0x00ff
#define S_PIPEBUF_BUFSIZE   10
#define S_PIPEBUF_BUFNMB    0

#define M_PIPETRE_TRENB 0x0200
#define S_PIPETRE_TRENB 9
#define M_PIPETRE_TRCLR 0x0100
#define S_PIPETRE_TRCLR 8

#define M_PIPECTR_BSTS      0x8000
#define M_PIPECTR_INBUFM    0x4000
#define M_PIPECTR_CSCLR     0x2000
#define M_PIPECTR_CSSTS     0x1000
#define M_PIPECTR_ATREPM    0x0400
#define M_PIPECTR_ACLRM     0x0200
#define M_PIPECTR_SQCLR     0x0100
#define M_PIPECTR_SQSET     0x0080
#define M_PIPECTR_SQMON     0x0040
#define M_PIPECTR_PBUSY     0x0020
#define M_PIPECTR_PID       0x0003

#define S_PIPECTR_BSTS      15
#define S_PIPECTR_INBUFM    14
#define S_PIPECTR_CSCLR     13
#define S_PIPECTR_CSSTS     12
#define S_PIPECTR_ATREPM    10
#define S_PIPECTR_ACLRM     9
#define S_PIPECTR_SQCLR     8
#define S_PIPECTR_SQSET     7
#define S_PIPECTR_SQMON     6
#define S_PIPECTR_PBUSY     5
#define S_PIPECTR_PID       0

#define DIR_M   0x0010
#define DIR_R_M 0x0000
#define DIR_W_M 0x0010

#define M_HSE       (0x80)
#define M_DCFM      (0x40)
#define M_DRPD      (0x20)
#define M_DPRPU     (0x10)
#define M_UCKSEL    (0x04)
#define M_UPLLE     (0x02)
#define M_USBE      (0x01)

#define USB_HOST_BUSWAIT_02             (0x0000u)
#define USB_HOST_BUSWAIT_03             (0x0001u)
#define USB_HOST_BUSWAIT_04             (0x0002u)
#define USB_HOST_BUSWAIT_05             (0x0003u)
#define USB_HOST_BUSWAIT_06             (0x0004u)
#define USB_HOST_BUSWAIT_07             (0x0005u)
#define USB_HOST_BUSWAIT_08             (0x0006u)
#define USB_HOST_BUSWAIT_09             (0x0007u)
#define USB_HOST_BUSWAIT_10             (0x0008u)
#define USB_HOST_BUSWAIT_11             (0x0009u)
#define USB_HOST_BUSWAIT_12             (0x000au)
#define USB_HOST_BUSWAIT_13             (0x000bu)
#define USB_HOST_BUSWAIT_14             (0x000cu)
#define USB_HOST_BUSWAIT_15             (0x000du)
#define USB_HOST_BUSWAIT_16             (0x000eu)
#define USB_HOST_BUSWAIT_17             (0x000fu)

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

#define USB_MAX_DEVADD  11  // max number of device address register
#define USB_MAX_EP      6   // max number of end point
#define USB_MAX_PIPE    16  // max number of pipes
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

#define M_VBSE  (0x8000)
#define M_RSME  (0x4000)
#define M_SOFE  (0x2000)
#define M_DVSE  (0x1000)
#define M_CTRE  (0x0800)

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
volatile static int usb_speed = SPEED_HI;
volatile static int intsts1_sack = 0;
volatile static int intsts0_brdy = 0;
volatile static int intsts0_bemp = 0;
volatile static int intsts0_nrdy = 0;

static int USBH_FifoType[USB_MAX_PIPE] = {
    FIFO_CFIFO,     // pipe 0
    FIFO_D1FIFO,    // pipe 1
    FIFO_D0FIFO,    // pipe 2
    FIFO_D0FIFO,    // pipe 3
    FIFO_D1FIFO,
    FIFO_D0FIFO,
    FIFO_D1FIFO,
    FIFO_D0FIFO,
    FIFO_D1FIFO,
    FIFO_D0FIFO,
    FIFO_D1FIFO,
    FIFO_D0FIFO,
    FIFO_D1FIFO,
    FIFO_D0FIFO,
    FIFO_D1FIFO,
    FIFO_D0FIFO
};

static volatile PIPECTR *pPIPECTR[USB_MAX_PIPE] = {
    (volatile PIPECTR *)&USB201.DCPCTR,
    (volatile PIPECTR *)&USB201.PIPE1CTR,
    (volatile PIPECTR *)&USB201.PIPE2CTR,
    (volatile PIPECTR *)&USB201.PIPE3CTR,
    (volatile PIPECTR *)&USB201.PIPE4CTR,
    (volatile PIPECTR *)&USB201.PIPE5CTR,
    (volatile PIPECTR *)&USB201.PIPE6CTR,
    (volatile PIPECTR *)&USB201.PIPE7CTR,
    (volatile PIPECTR *)&USB201.PIPE8CTR,
    (volatile PIPECTR *)&USB201.PIPE9CTR,
    (volatile PIPECTR *)&USB201.PIPEACTR,
    (volatile PIPECTR *)&USB201.PIPEBCTR,
    (volatile PIPECTR *)&USB201.PIPECCTR,
    (volatile PIPECTR *)&USB201.PIPEDCTR,
    (volatile PIPECTR *)&USB201.PIPEECTR,
    (volatile PIPECTR *)&USB201.PIPEFCTR
};

static volatile uint16_t *pFIFOSEL[USB_MAX_FIFO] =
{
    (volatile uint16_t *)&USB201.CFIFOSEL,
    (volatile uint16_t *)&USB201.D0FIFOSEL,
    (volatile uint16_t *)&USB201.D1FIFOSEL
};

static volatile uint16_t *pFIFOCTR[USB_MAX_FIFO] =
{
    (volatile uint16_t *)&USB201.CFIFOCTR,
    (volatile uint16_t *)&USB201.D0FIFOCTR,
    (volatile uint16_t *)&USB201.D1FIFOCTR
};

static volatile unsigned char *pFIFO[USB_MAX_FIFO] =
{
    (volatile unsigned char *)&USB201.CFIFO.UINT8[HH],
    (volatile unsigned char *)&USB201.D0FIFO.UINT8[HH],
    (volatile unsigned char *)&USB201.D1FIFO.UINT8[HH]
};

static uint16_t *DEVADD[USB_MAX_DEVADD] = {
    (uint16_t *)&USB201.DEVADD0,
    (uint16_t *)&USB201.DEVADD1,
    (uint16_t *)&USB201.DEVADD2,
    (uint16_t *)&USB201.DEVADD3,
    (uint16_t *)&USB201.DEVADD4,
    (uint16_t *)&USB201.DEVADD5,
    (uint16_t *)&USB201.DEVADD6,
    (uint16_t *)&USB201.DEVADD7,
    (uint16_t *)&USB201.DEVADD8,
    (uint16_t *)&USB201.DEVADD9,
    (uint16_t *)&USB201.DEVADDA
};

unsigned char *rd_ptr[USB_MAX_PIPE];
volatile int rd_cnt[USB_MAX_PIPE];
volatile int rd_end[USB_MAX_PIPE];
volatile int rd_err[USB_MAX_PIPE];

unsigned char *wt_ptr[USB_MAX_PIPE];
volatile int wt_cnt[USB_MAX_PIPE];
volatile int wt_end[USB_MAX_PIPE];

static uint16_t MaxPacket[USB_MAX_PIPE] = {
    64, 512, 512, 512, 512, 512, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

volatile int PipeFlag[USB_MAX_PIPE];

//#ifdef USBH_DEBUG_dump_int_regs
static void dump_int_regs(unsigned char *s_func, int f_delay)
{
    if (s_func)
        debug_printf("[%s]\r\n", s_func);
    debug_printf("sts0=%04X sts1=%04X enb0=%04X enb1=%04X\r\n",
            USB201.INTSTS0, USB201.INTSTS1, USB201.INTENB0, USB201.INTENB1);
    debug_printf("brdy=%04X nrdy=%04X bemp=%04X\r\n",
            USB201.BRDYENB, USB201.NRDYENB, USB201.BEMPENB);
    if (f_delay)
        udelay(100000);
}

static void dump_pipe_regs(unsigned char *s_func, int f_delay)
{
    uint16_t tmp;
    if (s_func)
        debug_printf("[%s]\r\n", s_func);
    debug_printf("psel=%04X p0ctr=%04X p1ctr=%04X p2ctr=%04X\r\n",
            USB201.PIPESEL, USB201.DCPCTR, USB201.PIPE1CTR, USB201.PIPE2CTR);
    tmp = USB201.PIPESEL;
    USB201.PIPESEL = 1;
    debug_printf("P1 MAX:%04X CFG:%04X BUF:%04X\r\n", USB201.PIPEMAXP, USB201.PIPECFG, USB201.PIPEBUF);
    USB201.PIPESEL = 2;
    debug_printf("P2 MAX:%04X CFG:%04X BUF:%04X\r\n", USB201.PIPEMAXP, USB201.PIPECFG, USB201.PIPEBUF);
    USB201.PIPESEL = 3;
    debug_printf("P3 MAX:%04X CFG:%04X BUF:%04X\r\n", USB201.PIPEMAXP, USB201.PIPECFG, USB201.PIPEBUF);
    USB201.PIPESEL = tmp;
    if (f_delay)
        udelay(100000);
}

static void dump_fifo_regs(unsigned char *s_func, int f_delay)
{
    if (s_func)
        debug_printf("[%s]\r\n", s_func);
    debug_printf("cfifosel=%04X d0fifosel=%04X d1fifosel=%04X\r\n",
            USB201.CFIFOSEL, USB201.D0FIFOSEL, USB201.D1FIFOSEL);
    debug_printf("cfifoctr=%04X d0fifoctr=%04X d1fifoctr=%04X\r\n",
            USB201.CFIFOCTR, USB201.D0FIFOCTR, USB201.D1FIFOCTR);
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
        MaxPacket[0] = 64;
        MaxPacket[1] = 512;
        MaxPacket[2] = 512;
        MaxPacket[3] = 512;
        MaxPacket[4] = 512;
    } else if (speed == SPEED_FULL) {    // Full Speed
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


void usb_delay_xms (uint32_t msec)
{
    udelay(msec*1000);
}

static void usb_int_brdy_enable(int pipe)
{
    uint16_t tmp = USB201.INTENB0;
    USB201.INTENB0 = (uint16_t)(tmp & (uint16_t)~(M_BEMPE|M_NRDYE|M_BRDYE));
    USB201.BRDYENB |= (1 << pipe);
    USB201.INTENB0 = tmp;
}

static void usb_int_brdy_disable(int pipe)
{
    uint16_t tmp = USB201.INTENB0;
    USB201.INTENB0 = (uint16_t)(tmp & (uint16_t)~(M_BEMPE|M_NRDYE|M_BRDYE));
    USB201.BRDYENB &= ~(1 << pipe);
    USB201.INTENB0 = tmp;
}

static void usb_int_nrdy_enable(int pipe)
{
    uint16_t tmp = USB201.INTENB0;
    USB201.INTENB0 = (uint16_t)(tmp & (uint16_t)~(M_BEMPE|M_NRDYE|M_BRDYE));
    USB201.NRDYENB |= (1 << pipe);
    USB201.INTENB0 = tmp;
}

static void usb_int_nrdy_disable(int pipe)
{
    uint16_t tmp = USB201.INTENB0;
    USB201.INTENB0 = (uint16_t)(tmp & (uint16_t)~(M_BEMPE|M_NRDYE|M_BRDYE));
    USB201.NRDYENB &= ~(1 << pipe);
    USB201.INTENB0 = tmp;
}

static void usb_int_bemp_enable(int pipe)
{
    uint16_t tmp = USB201.INTENB0;
    USB201.INTENB0 = (uint16_t)(tmp & (uint16_t)~(M_BEMPE|M_NRDYE|M_BRDYE));
    USB201.BEMPENB |= (1 << pipe);
    USB201.INTENB0 = tmp;
}

static void usb_int_bemp_disable(int pipe)
{
    uint16_t tmp = USB201.INTENB0;
    USB201.INTENB0 = (uint16_t)(tmp & (uint16_t)~(M_BEMPE|M_NRDYE|M_BRDYE));
    USB201.BEMPENB &= ~(1 << pipe);
    USB201.INTENB0 = tmp;
}

void usbh_ctrl_int_enable(void)
{
    uint16_t buf;

    buf  = USB201.INTENB0;
    buf |= (M_BEMP|M_NRDY|M_BRDY);
    USB201.INTENB0 = buf;

    buf  = USB201.INTENB1;
    buf |= (M_SIGN|M_SACK);
    USB201.INTENB1 = buf;

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
#ifdef USBH_DEBUG_FIFOW
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
            ret = 1;    /* null packect */
        if ((count % (int)MaxPacket[pipe]) != 0)
            ret = 1;    /* short packect */
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
#ifdef USBH_DEBUG_FIFOR
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
    uint16_t bempsts = USB201.BEMPSTS;
    uint16_t bempenb = USB201.BEMPENB;
    if ((USB201.BEMPSTS & M_PIPE0BEMP) && (USB201.BEMPENB & M_PIPE0BEMPE)) {
        USB201.BEMPSTS = ~0x0001;
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
        USB201.BEMPSTS = ~bempsts;
        for (pipe = 1; pipe < USB_MAX_PIPE; pipe++) {
            int mask = (1 << pipe);
            if ((bempsts & mask) && (bempenb & mask)) {
                USB201.PIPESEL = pipe;
                if ((USB201.PIPECFG & DIR_M) == DIR_R_M) {
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
                USB201.PIPESEL = pipe;
            }
        }
    }
}

static void usbh_handle_nrdy(void)
{
    int pipe;
    uint16_t nrdysts = USB201.NRDYSTS;
    uint16_t nrdyenb = USB201.NRDYENB;
    if ((USB201.NRDYSTS & M_PIPE0NRDY) && (USB201.NRDYENB & M_PIPE0NRDYE)) {
        USB201.NRDYSTS = ~0x0001;
        usb_int_nrdy_disable(PIPE0);
        intsts0_nrdy = 1;
    } else {
        USB201.NRDYSTS = ~nrdysts;
        for (pipe = 1; pipe < USB_MAX_PIPE; pipe++) {
            int mask = (1 << pipe);
            if ((nrdysts & mask) && (nrdyenb & mask)) {
                //uint16_t pid = pPIPECTR[pipe]->BIT.PID;
                //if ((pid == PID_STALL_1) || (pid ==PID_STALL_2)) {
                //    PipeFlag[pipe] = PIPE_STALL;
                //    debug_printf("TA\n");
                //} else {
                rd_err[pipe] ++;
                if (rd_err[pipe] > 3) {
                    usb_int_nrdy_disable(pipe);
                    intsts0_nrdy = 1;
                    PipeFlag[pipe] = PIPE_NORES;
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
    uint16_t brdysts = USB201.BRDYSTS;
    uint16_t brdyenb = USB201.BRDYENB;
    if ((USB201.BRDYSTS & M_PIPE0BRDY) && (USB201.BRDYENB & M_PIPE0BRDYE)) {
        USB201.BRDYSTS = ~0x0001;
#ifdef USBH_POLLING
        usb_int_brdy_disable(PIPE0);
        intsts0_brdy = 1;
#else
        USB201.PIPESEL = (uint16_t)PIPE0;
        if ((USB201.PIPECFG & DIR_M) == DIR_R_M) {
            usbh_fifo2buf(PIPE0);
            if (rd_end[PIPE0] == 1) {
                usbh_pipe_set_pidnak(PIPE0);
                usb_int_brdy_disable(PIPE0);
            }
        } else {
            usbh_buf2fifo(PIPE0);
        }
        intsts0_brdy = 1;
        USB201.PIPESEL = 0;
#endif
    } else {
        USB201.BRDYSTS = ~brdysts;
        for (pipe = 1; pipe < USB_MAX_PIPE; pipe++) {
            int mask = (1 << pipe);
            if ((brdysts & mask) && (brdyenb & mask)) {
#ifdef USBH_POLLING
                intsts0_brdy = 1;
#else
                USB201.PIPESEL = (uint16_t)pipe;
                if ((USB201.PIPECFG & DIR_M) == DIR_R_M) {
                    usbh_fifo2buf(pipe);
                    if (rd_end[pipe] == 1) {
                        usbh_pipe_set_pidnak(pipe);
                        usb_int_brdy_disable(pipe);
                    }
                } else {
                    usbh_buf2fifo(pipe);
                }
                intsts0_brdy = 1;
                USB201.PIPESEL = 0;
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

//void __attribute__ ((interrupt)) INT_Excep_USB0_USBI0(void)
void USBInterruptHandler(void)
{
    uint16_t intsts0, intsts1;
    uint16_t intenb0, intenb1;
    uint16_t brdysts, nrdysts, bempsts;
    uint16_t brdyenb, nrdyenb, bempenb;

    uint16_t savepipe1,savepipe2;
    uint16_t tmp;

    savepipe1 = USB201.CFIFOSEL;
    savepipe2 = USB201.PIPESEL;

    intsts0 = USB201.INTSTS0;
    intsts1 = USB201.INTSTS1;
    intenb0 = USB201.INTENB0;
    intenb1 = USB201.INTENB1;

    if( (intsts1 & M_BCHG) && (intenb1 & M_BCHGE) ) {
        USB201.INTSTS1 = (uint16_t)~M_BCHG;
        USB201.INTENB1 &= (uint16_t)~M_BCHGE;
#ifdef USBH_DEBUG_INT
        debug_printf("BC\n");
#endif
    }
    else if( (intsts1 & M_SACK) && (intenb1 & M_SACK) ) {
        USB201.INTSTS1 = (uint16_t)~M_SACK;
#ifdef USBH_DEBUG_INT
        debug_printf("SA\n");
#endif
        intsts1_sack = 1;
    }
    else if( (intsts1 & M_SIGN) && (intenb1 & M_SIGN) ) {
        USB201.INTSTS1 = (uint16_t)~M_SIGN;
#ifdef USBH_DEBUG_INT
        debug_printf("SI\n");
#endif
    }
    else if( ((intsts1&M_DTCH)==M_DTCH) && ((intenb1&M_DTCHE)==M_DTCHE) ){
        USB201.INTSTS1 = (uint16_t)~M_DTCH;
        USB201.INTENB1 &= (uint16_t)~M_DTCHE;
#ifdef USBH_DEBUG_INT
        debug_printf("DT\n");
#endif
    }
    else if( ((intsts1&M_ATTCH)==M_ATTCH) && ((intenb1&M_ATTCHE)==M_ATTCHE) ){
        USB201.INTSTS1 = (uint16_t)~M_ATTCH;
        USB201.INTENB1 &= (uint16_t)~M_ATTCHE;
#ifdef USBH_DEBUG_INT
        debug_printf("AT\n");
#endif
    }
    else if( (intsts0 & intenb0 & (M_BEMP|M_NRDY|M_BRDY)) ) {
        brdysts = USB201.BRDYSTS;
        nrdysts = USB201.NRDYSTS;
        bempsts = USB201.BEMPSTS;
        brdyenb = USB201.BRDYENB;
        nrdyenb = USB201.NRDYENB;
        bempenb = USB201.BEMPENB;

        if( (intsts0 & M_BRDY) && (intenb0 & M_BRDY) && (brdysts & brdyenb) ) {
            usbh_handle_brdy();
#if defined(USBH_DEBUG_INT) || defined(USBH_DEBUG_INT_BRDY)
            debug_printf("BR\n");
#endif
        }
        else if( (intsts0 & M_BEMP) && (intenb0 & M_BEMP) && (bempsts & bempenb) ) {
            usbh_handle_bemp();
#if defined(USBH_DEBUG_INT) || defined(USBH_DEBUG_INT_BEMP)
            debug_printf("BE\n");
#endif
        }
        else if( (intsts0 & M_NRDY) && (intenb0 & M_NRDY) && (nrdysts & nrdyenb) ) {
            usbh_handle_nrdy();
#if defined(USBH_DEBUG_INT) || defined(USBH_DEBUG_INT_NRDY)
            debug_printf("NR\n");
#endif
        }
    } else {
    }

    tmp = USB201.CFIFOSEL;
    tmp &= (uint16_t)~(M_CURPIPE);
    tmp |= (uint16_t)(savepipe1 & M_CURPIPE);
    USB201.CFIFOSEL = tmp;
    USB201.PIPESEL = savepipe2;
}

static void usbh_DEVADD_set_speed(int n)
{
    uint16_t tmp = (*DEVADD[n]) & 0xff3f;
    switch (usb_speed) {
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

static void usbh_DEVADD_init(void)
{
    int i;
    for (i = 0; i < USB_MAX_DEVADD; i++) {
        *DEVADD[i] = 0;
    }
}

void usbh_int_init(void)
{
    USB201.INTENB0 |= (volatile uint16_t)(M_BEMP | M_NRDYE | M_BRDYE);
    //    USB201.INTENB1 |= (volatile uint16_t)(M_BCHGE | M_DTCHE | M_ATTCHE | M_SIGNE | M_SACKE);

    USB201.BEMPENB |= (volatile uint16_t)M_PIPE0BEMPE;     // pipe 0 bemp interrupt enable
    USB201.BRDYENB &= (volatile uint16_t)~M_PIPE0BRDYE;    // pipe 0 brdy inerrrupt disable
}

static inline void usbh_port_config(void)
{
    // P3_9: GPD34: USB1_OC
    // P3_8: GPD35: USB1_EN
    /* P3_8(USB0_EN) */
    GPIOP3      &= ~0x0100;         /* Outputs low level */
    GPIOPMC3    &= ~0x0100;         /* Port mode */
    GPIOPM3     &= ~0x0100;         /* Output mode */

    // P11_1    BSEL0
    // P11_2    BSEL1
    // P11_3    HSTSEL
    GPIOP11     &= ~0x000e;         /* Outputs low level */
    GPIOPMC11   &= ~0x000e;         /* Port mode */
    GPIOPM11    &= ~0x000e;         /* Output mode */

    // P11_0    nWIFIRST
    GPIOP11     |=  0x0001;         /* Outputs high level */
    GPIOPMC11   &= ~0x0001;         /* Port mode */
    GPIOPM11    &= ~0x0001;         /* Output mode */

    udelay(100000);

    GPIOP11     &= ~0x0001;         /* Outputs low level */

    udelay(100000);

    GPIOP11     |=  0x0001;         /* Outputs high level */

    udelay(500000);

}

static inline void usbh_int_disable(void)
{
    USB201.INTENB0 = 0;
    USB201.INTENB1 = 0;
}

static inline void usbh_int_level(unsigned char level)
{
    // ToDo
    //ICU.IER[0x04].BIT.IEN1  = 0;   /* D0FIFO0 disable */
    //ICU.IER[0x04].BIT.IEN2  = 0;   /* D1FIFO0 disable */
    //ICU.IER[0x04].BIT.IEN3  = 1;   /* USBI0 enable */
    //ICU.IPR[33].BYTE = 0x00;    /* D0FIFO0 */
    //ICU.IPR[34].BYTE = 0x00;    /* D1FIFO0 */
    //ICU.IPR[35].BYTE = level;   /* USBI0 */
}

static void usbh_wait_line_statble(void)
{
    unsigned char s0, s1;
    do {
        s0 =  (USB201.SYSSTS0 & 0x3);
        udelay(50000);
        s1 =  (USB201.SYSSTS0 & 0x3);
    } while (s0 != s1);
}

static void usbh_pipe_clr_sq(int pipe);

void usbh_module_init(void)
{
    uint16_t pipe;
    volatile uint16_t *pPIPECFG = (volatile uint16_t *)&USB201.PIPECFG;
    volatile uint16_t *pPIPEMAXP = (volatile uint16_t *)&USB201.PIPEMAXP;
    volatile uint16_t *pPIPEBUF = (volatile uint16_t *)&USB201.PIPEBUF;

    usbh_DEVADD_init();
    USB200.SYSCFG0 &= ~M_DCFM;      // 1: host, 0: function
    USB200.SYSCFG0 &= ~M_UCKSEL;    // usb clcok sel only for channel 0
    USB200.SYSCFG0 |= M_USBE;       // usb clock enable

    if (usb_speed == SPEED_HI)
        USB201.SYSCFG0 |= M_HSE;        // HI SPEED
    USB201.SYSCFG0 |= M_DCFM;       // 1: host, 0: function
    USB201.SYSCFG0 |= M_DRPD;       // pull down enable
    USB201.SYSCFG0 &= ~M_DPRPU;       // pull down enable
    usbh_wait_line_statble();
    USB201.SYSCFG0 |= M_USBE;       // usb clock enable
    USB201.SOFCFG = 0x0020;
    usbh_pipe_clr_sq(BULK_IN_PIPENO);
    usbh_pipe_clr_sq(BULK_OUT_PIPENO);
    usbh_pipe_clr_sq(INT_IN_PIPENO);
    usbh_pipe_clr_sq(INT_OUT_PIPENO);
    for (pipe = 1; pipe <= 2; pipe++) {
        USB201.PIPESEL = pipe;
        RegW16(pPIPEMAXP, (uint16_t)MaxPacket[pipe], S_PIPEMAXP_MXPS, M_PIPEMAXP_MXPS);
        RegW16(pPIPEMAXP, (uint16_t)1, S_PIPEMAXP_DEVSEL, M_PIPEMAXP_DEVSEL);
        RegW16(pPIPEBUF, (uint16_t)7, S_PIPEBUF_BUFSIZE, M_PIPEBUF_BUFSIZE);
        RegW16(pPIPEBUF, (uint16_t)pipe*8, S_PIPEBUF_BUFNMB, M_PIPEBUF_BUFNMB);
    }
    USB201.PIPESEL = 0;
}

#define USB_HOST_BITUPLLE               (0x0002u)
#define USB_HOST_BITUCKSEL              (0x0004u)
#define USB_HOST_BITBWAIT               (0x003fu)

#define M_SYSCFG_USBE   (0x0001u)
#define M_SYSCFG_UPLLE  (0x0002u)
#define M_SYSCFG_UCKSEL (0x0004u)
#define M_SYSCFG_DPRPU  (0x0010u)
#define M_SYSCFG_DRPD   (0x0020u)
#define M_SYSCFG_DCFM   (0x0040u)
#define M_SYSCFG_HSE    (0x0080u)

#define S_SYSCFG_USBE   (0)
#define S_SYSCFG_UPLLE  (1)
#define S_SYSCFG_UCKSEL (2)
#define S_SYSCFG_DPRPU  (4)
#define S_SYSCFG_DRPD   (5)
#define S_SYSCFG_DCFM   (6)
#define S_SYSCFG_HSE    (7)

#define M_SUSPMODE_SUSPM    (0x4000u)
#define S_SUSPMODE_SUSPM    (14)

void usbh_reset_module(unsigned char clockmode)
{
    if (RegR16(&USB200.SYSCFG0, S_SYSCFG_UPLLE, M_SYSCFG_UPLLE) == 1) {
        if ((USB200.SYSCFG0 & USB_HOST_BITUCKSEL) != clockmode) {
            RegW16(&USB201.SUSPMODE, 0, S_SUSPMODE_SUSPM, M_SUSPMODE_SUSPM);
            RegW16(&USB200.SUSPMODE, 0, S_SUSPMODE_SUSPM, M_SUSPMODE_SUSPM);
            USB201.SYSCFG0 = 0;
            USB200.SYSCFG0 = 0;
            USB200.SYSCFG0 = (USB_HOST_BITUPLLE | clockmode);
            usb_delay_xms(1);
            RegW16(&USB200.SUSPMODE, 1, S_SUSPMODE_SUSPM, M_SUSPMODE_SUSPM);
            RegW16(&USB201.SUSPMODE, 1, S_SUSPMODE_SUSPM, M_SUSPMODE_SUSPM);
        } else {
            RegW16(&USB201.SUSPMODE, 0, S_SUSPMODE_SUSPM, M_SUSPMODE_SUSPM);
            usb_delay_xms(1);
            RegW16(&USB201.SUSPMODE, 1, S_SUSPMODE_SUSPM, M_SUSPMODE_SUSPM);
        }
    } else {
        RegW16(&USB201.SUSPMODE, 0, S_SUSPMODE_SUSPM, M_SUSPMODE_SUSPM);
        RegW16(&USB200.SUSPMODE, 0, S_SUSPMODE_SUSPM, M_SUSPMODE_SUSPM);
        USB201.SYSCFG0 = 0;
        USB200.SYSCFG0 = 0;
        USB200.SYSCFG0 = (USB_HOST_BITUPLLE | clockmode);
        usb_delay_xms(1);
        RegW16(&USB200.SUSPMODE, 1, S_SUSPMODE_SUSPM, M_SUSPMODE_SUSPM);
        RegW16(&USB201.SUSPMODE, 1, S_SUSPMODE_SUSPM, M_SUSPMODE_SUSPM);
    }
    USB201.BUSWAIT = (uint16_t)(USB_HOST_BUSWAIT_05 & USB_HOST_BITBWAIT);
}

#if 0
void usbf_reset_module (uint16_t clockmode)
{
    if (RegR16(&USB200.SYSCFG0, S_SYSCFG_UPLLE, M_SYSCFG_UPLLE) == 1) {
        if ((USB200.SYSCFG0 & USB_HOST_BITUCKSEL) != clockmode) {
            RegW16(&USB200.SUSPMODE, 0, S_SUSPMODE_SUSPM, M_SUSPMODE_SUSPM);
            USB200.SYSCFG0 = 0;
            USB200.SYSCFG0 = (USB_HOST_BITUPLLE | clockmode);
            usb_delay_xms(1);
            RegW16(&USB200.SUSPMODE, 1, S_SUSPMODE_SUSPM, M_SUSPMODE_SUSPM);
        } else {
            RegW16(&USB200.SUSPMODE, 0, S_SUSPMODE_SUSPM, M_SUSPMODE_SUSPM);
            usb_delay_xms(1);
            RegW16(&USB201.SUSPMODE, 1, S_SUSPMODE_SUSPM, M_SUSPMODE_SUSPM);
        }
    } else {
        RegW16(&USB200.SUSPMODE, 0, S_SUSPMODE_SUSPM, M_SUSPMODE_SUSPM);
        USB200.SYSCFG0 = 0;
        USB200.SYSCFG0 = (USB_HOST_BITUPLLE | clockmode);
        usb_delay_xms(1);
        RegW16(&USB200.SUSPMODE, 1, S_SUSPMODE_SUSPM, M_SUSPMODE_SUSPM);
    }
    USB200.BUSWAIT = (uint16_t)(USB_HOST_BUSWAIT_05 & USB_HOST_BITBWAIT);
}
#endif

void usbh_init(void)
{
    API_FUNC_ENTER();
    //GIC_DisableIRQ(USBI1_IRQn);
    CPG.STBCR7 &= ~(CPG_STBCR7_MSTP71 | CPG_STBCR7_MSTP70);
    usbh_port_config();
    usbh_int_disable();
    usbh_reset_module(0);  // USBFCLOCK_X1_48MHZ = 0
    usbh_int_level(USBH_INT_LEVEL);
    usbh_module_init();
    usbh_int_init();
    //usbh_pipe_init();
    InterruptHandlerRegister(USBI1_IRQn, (IRQHandler)USBInterruptHandler);
    GIC_SetPriority(USBI1_IRQn, 5);
    GIC_EnableIRQ(USBI1_IRQn);
#ifdef USBH_DEBUG_dump_int_regs
    dump_int_regs(__func__, 1);
#endif
    API_FUNC_EXIT();
}

#ifdef USBH_USE_TRANSACTION
static void usbh_pipe_set_counter(int pipe, int size)
{
    int count = 0;
    if (size  == 0) {
        return;
    }
    if ((size % (int)MaxPacket[pipe]) == 0) {
        count =  size / (int)MaxPacket[pipe];
    } else {
        count =  size / (int)MaxPacket[pipe] + 1;
    }
    switch (pipe) {
    case 1:
        USB201.PIPE1TRE.BIT.TRCLR = 1;
        USB201.PIPE1TRN = count;
        USB201.PIPE1TRE.BIT.TRENB = 1;
        break;
    case 2:
        USB201.PIPE2TRE.BIT.TRCLR = 1;
        USB201.PIPE2TRN = count;
        USB201.PIPE2TRE.BIT.TRENB = 1;
        break;
    case 3:
        USB201.PIPE3TRE.BIT.TRCLR = 1;
        USB201.PIPE3TRN = count;
        USB201.PIPE3TRE.BIT.TRENB = 1;
        break;
    case 4:
        USB201.PIPE4TRE.BIT.TRCLR = 1;
        USB201.PIPE4TRN = count;
        USB201.PIPE4TRE.BIT.TRENB = 1;
        break;
    case 5:
        USB201.PIPE5TRE.BIT.TRCLR = 1;
        USB201.PIPE5TRN = count;
        USB201.PIPE5TRE.BIT.TRENB = 1;
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
        USB201.PIPE1TRE.BIT.TRENB = 0;
        USB201.PIPE1TRE.BIT.TRCLR = 1;
        break;
    case 2:
        USB201.PIPE2TRE.BIT.TRENB = 0;
        USB201.PIPE2TRE.BIT.TRCLR = 1;
        break;
    case 3:
        USB201.PIPE3TRE.BIT.TRENB = 0;
        USB201.PIPE3TRE.BIT.TRCLR = 1;
        break;
    case 4:
        USB201.PIPE4TRE.BIT.TRENB = 0;
        USB201.PIPE4TRE.BIT.TRCLR = 1;
        break;
    case 5:
        USB201.PIPE5TRE.BIT.TRENB = 0;
        USB201.PIPE5TRE.BIT.TRCLR = 1;
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
        pipectr = (volatile PIPECTR *)&USB.DCPCTR;
    else {
        pipectr = (volatile PIPECTR *)&USB201.PIPE1CTR;
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
        while (pipectr->BIT.PBUSY == 1);
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

#define M_CURPIPE   0x000f
#ifdef PIPE_CLEAR_FIFO
static void usbh_pipe_clear_fifo(int pipe)
{
    if ((USB201.CFIFOSEL & M_CURPIPE) == pipe)
        USB201.CFIFOSEL &= ~M_CURPIPE;
    if ((USB201.D0FIFOSEL & M_CURPIPE) == pipe)
        USB201.D0FIFOSEL &= ~M_CURPIPE;
    if ((USB201.D1FIFOSEL & M_CURPIPE) == pipe)
        USB201.D1FIFOSEL &= ~M_CURPIPE;
}
#endif

static void usbh_pipe_set_ep(int pipe, int ep, int dir, int attr, int size, int addr)
{
    usbh_pipe_set_pidnak(pipe);
    USB201.PIPESEL = pipe;
    USB201.PIPECFG = 0;
    RegW16((volatile uint16_t *)&USB201.PIPECFG, (uint16_t)ep, (uint16_t)0, (uint16_t)0x000f);
    RegW16((volatile uint16_t *)&USB201.PIPECFG, (uint16_t)dir, (uint16_t)4, (uint16_t)0x0010);
    RegW16((volatile uint16_t *)&USB201.PIPECFG, (uint16_t)0, (uint16_t)9, (uint16_t)0x0200);
#ifdef USBH_USE_TRANSACTION
    if (dir == DIR_R) {
        USB201.PIPECFG.BIT.BFRE = 1;          // Interrupt
        USB201.PIPECFG.BIT.SHTNAK = 1;
    } else
        USB201.PIPECFG.BIT.BFRE = 0;          // Interrupt
#else
    RegW16((volatile uint16_t *)&USB201.PIPECFG, (uint16_t)0, (uint16_t)10, (uint16_t)0x0400);
    if (dir == DIR_R) {
        RegW16((volatile uint16_t *)&USB201.PIPECFG, (uint16_t)1, (uint16_t)7, (uint16_t)0x0080);
    }
#endif
    if (attr == PIPE_BULK) {
        RegW16((volatile uint16_t *)&USB201.PIPECFG, (uint16_t)PIPE_BULK, (uint16_t)14, (uint16_t)0xc000);
        //pipectr->BIT.SQCLR = 1;
    } else if (attr == PIPE_INT) {
        RegW16((volatile uint16_t *)&USB201.PIPECFG, (uint16_t)PIPE_INT, (uint16_t)14, (uint16_t)0xc000);
    } else if (attr == PIPE_ISO) {
        RegW16((volatile uint16_t *)&USB201.PIPECFG, (uint16_t)PIPE_ISO, (uint16_t)14, (uint16_t)0xc000);
    }
    RegW16((volatile uint16_t *)&USB201.PIPEMAXP, (uint16_t)MaxPacket[pipe], (uint16_t)0, (uint16_t)0x03ff);
    RegW16((volatile uint16_t *)&USB201.PIPEMAXP, (uint16_t)addr, (uint16_t)12, (uint16_t)0xf000);
    USB201.PIPESEL = 0;                // Unselect Pipe
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
            USB201.DCPCFG |= 0x0010;    //DIR_W
        else
            USB201.DCPCFG &= ~0x0010;   //DIR_R;
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

#if 0
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
    } else
        if (buf != NULL)
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
#endif

static void usbh_attach(void)
{
    API_FUNC_ENTER();
    usbh_ctrl_int_enable();
    USB201.INTSTS1 &= ~M_BCHG;
    USB201.INTENB1 &= ~M_BCHGE;
    USB201.INTSTS1 &= ~M_ATTCH;
    USB201.INTENB1 |= M_ATTCHE;
    USB201.INTSTS1 &= ~M_DTCH;
    USB201.INTENB1 |= M_DTCHE;
    API_FUNC_EXIT();
}

void ul_host_config(int conn_num)
{
    API_FUNC_ENTER();
    usbh_init();
    usbh_attach();
    API_FUNC_EXIT();
}

#define M_USBRST    0x0040
#define M_UACT      0x0010

void ul_host_bus_reset(int conn_num)
{
    uint16_t tmp, i;
    API_FUNC_ENTER();
    USB201.DVSTCTR0 |= M_USBRST;
    USB201.DVSTCTR0 &= ~M_UACT;
    udelay(20000);
    USB201.DVSTCTR0 |= M_UACT;
    USB201.DVSTCTR0 &= ~M_USBRST;
    udelay(10000);
    USB201.INTSTS1 = ~0x0800;

    for (i = 0, tmp = 0x0004;  i < 5; ++i ) {
        tmp = (USB201.DVSTCTR0 & 0x7);
        if ((tmp & 0x0004) == 0x0004) {
            udelay(20000);
        } else{
            break;
        }
    }
    if (tmp == 3)
        usb_speed = SPEED_HI;
    else if (tmp == 2)
        usb_speed = SPEED_FULL;
    else
        usb_speed = SPEED_LOW;
    usb_set_max_packet_size(usb_speed);
#ifdef USBH_DEBUG_DUMP_INT_REGS
    dump_int_regs(__func__, 1);
#endif
#ifdef USBH_DEBUG_SPEED
    debug_printf("SP=%d\r\n", usb_speed);
#endif
    API_FUNC_EXIT();
    return;
}

//int wait_int_event(int conn_num,en_int_status s) {
//  return USBH_ERROR;
//}

int ul_speed_configuration(int conn_num)
{
    API_FUNC_ENTER();
    API_FUNC_EXIT();
    return usb_speed;
}

uint16_t get_max_packet_size(int pipe)
{
    API_FUNC_ENTER();
    API_FUNC_EXIT();
    return (int)MaxPacket[pipe];
}

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
#ifdef USBH_DEBUG_ERR_TO
            debug_printf("TO\r\n");
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
    USB201.PIPESEL =  PIPE0;
    usbh_pipe_clr_buf(pipe);
    //if (rd_cnt[pipe] != 0) {
    //    ret = -1;
    //}
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
    USB201.PIPESEL =  PIPE0;
    usbh_pipe_clr_buf(pipe);
    if (wt_cnt[pipe] != 0)
        ret = -1;
    return ret;
}
#define M_TRNENSEL  0x0100

en_usb_status ul_control_setup_transaction(int conn_num,st_usb_device *dev,st_usb_setup_data *sd)
{
    int timeout;
    uint16_t max_packet_size = get_max_packet_size(PIPE0);
    int addr = dev->info.address;
#ifdef USBH_DEBUG_WAIT
    udelay(500000);
#endif
#ifdef USBH_DEBUG_CS
    debug_printf("CS\r\n");
#endif
    API_FUNC_ENTER();
    if (usb_speed == SPEED_LOW) {
        USB201.SOFCFG |= M_TRNENSEL;
    } else {
        USB201.SOFCFG &= ~M_TRNENSEL;
    }
    usbh_DEVADD_set_speed(addr);
    intsts1_sack = 0;
    USB201.DCPMAXP = (uint16_t)((uint16_t)(addr << 12) + max_packet_size);
    USB201.INTSTS1 = (uint16_t)~(M_SACK | M_SIGN);  /* Status Clear */
#ifdef USBH_DEBUG_dump_int_regs
    dump_int_regs(__func__, 1);
#endif
    USB201.USBREQ = ((uint16_t)sd->BYTE.bRequest << 8) + (uint16_t)sd->BYTE.bmRequestType;
    USB201.USBVAL = ((uint16_t)sd->BYTE.wValue[1] << 8) + (uint16_t)sd->BYTE.wValue[0];
    USB201.USBINDX = ((uint16_t)sd->BYTE.wIndex[1] << 8) + (uint16_t)sd->BYTE.wIndex[0];
    USB201.USBLENG = ((uint16_t)sd->BYTE.wLength[1] << 8) + (uint16_t)sd->BYTE.wLength[0];
    USB201.DCPCTR = M_SUREQ;
#ifdef USBH_DEBUG_SETUP
    debug_printf("ST %2x %2x %2x %2x\r\n", USB201.USBREQ, USB201.USBVAL, USB201.USBINDX, USB201.USBLENG);
#endif
    timeout = 4000;
    while (1) {
        if (intsts1_sack == 1)
            break;
        udelay(500);
        if (timeout-- == 0) {
#ifdef USBH_DEBUG_ERR
            debug_printf("* [%s] timeout\r\n", __func__);
#endif
            return USBH_ERROR;
        }
    }
    API_FUNC_EXIT();
    return USBH_OK;
}

en_usb_status ul_control_in_transaction(int conn_num,st_usb_device *dev,int size,unsigned char *data)
{
    int ret;
    int pipe = PIPE0;
#ifdef USBH_DEBUG_CI
    debug_printf("CI%d\r\n", size);
#endif
    API_FUNC_ENTER();
    usbh_pipe_set_pidnak(pipe);
    USB201.DCPCFG &= ~0x0010;   //DIR_R;
    USB201.DCPCTR = M_SQSET;
    ret = rd_transfer(pipe, conn_num, dev, size, data);
    API_FUNC_EXIT();
    if (ret < 0)
        return USBH_ERROR;
    return USBH_OK;
}

en_usb_status ul_control_out_transaction(int conn_num,st_usb_device *dev,int size, unsigned char *data)
{
    int ret;
    int pipe = PIPE0;
#ifdef USBH_DEBUG_CO
    debug_printf("CO%d\r\n", size);
#endif
    API_FUNC_ENTER();
    usbh_pipe_set_pidnak(pipe);
    USB201.DCPCFG |= 0x0010;    //DIR_W
    USB201.DCPCTR = M_SQSET;
    ret = wt_transfer(pipe, conn_num, dev, size, data);
    API_FUNC_EXIT();
    if (ret < 0)
        return USBH_ERROR;
    return USBH_OK;
}

en_usb_status ul_control_ack_out_transaction(int conn_num,st_usb_device *dev)
{
    int ret;
    int pipe = PIPE0;
    API_FUNC_ENTER();
    usbh_pipe_set_pidnak(pipe);
    USB201.DCPCFG |= 0x0010;    //DIR_W
    USB201.DCPCTR = M_SQSET;
    ret = wt_transfer(pipe, conn_num, dev, 0, 0);
    API_FUNC_EXIT();
    if (ret < 0)
        return USBH_ERROR;
    return USBH_OK;
}

en_usb_status ul_control_ack_in_transaction(int conn_num,st_usb_device *dev)
{
    int ret;
    int pipe = PIPE0;
    API_FUNC_ENTER();
    usbh_pipe_set_pidnak(pipe);
    USB201.DCPCFG &= ~0x0010;   //DIR_R;
    USB201.DCPCTR = M_SQSET;
    ret = rd_transfer(pipe, conn_num, dev, 0, 0);
    API_FUNC_EXIT();
    if (ret < 0)
        return USBH_ERROR;
    return USBH_OK;
}

// interrupt transfer(one or more transactions)
en_usb_status ul_interrupt_in_transfer(int conn_num,st_usb_device *dev,int *size,unsigned char *data)
{
    int ret;
    int addr = dev->info.address;
    int pipe = INT_IN_PIPENO;
    API_FUNC_ENTER();
#ifdef USBH_DEBUG_II
    debug_printf("II%d P:%d A:%d E:%d\r\n", size, pipe, addr, dev->info.interface[dev->info.cur_interface].ea_in);
#endif
    usbh_pipe_set_pidnak(pipe);
    usbh_pipe_set_ep(pipe, (int)dev->info.interface[dev->info.cur_interface].ea_in, DIR_R, PIPE_INT, (int)MaxPacket[pipe], addr);
    //usbh_pipe_clr_sq(pipe);
    ret = rd_transfer(pipe, conn_num, dev, size, data);
    udelay(20000);
#ifdef USBH_DEBUG_BI
    debug_printf("II(%d) ret=%d\r\n", size - rd_cnt[pipe], ret);
#endif
    API_FUNC_EXIT();
    if (ret < 0)
        return USBH_ERROR;
    return USBH_OK;
}

en_usb_status ul_interrupt_out_transfer(int conn_num,st_usb_device *dev,int size,unsigned char* data)
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

//#ifdef USBH_USE_MSD
// bulk transfer(one or more transactions)
en_usb_status ul_bulk_out_transfer(int conn_num,st_usb_device *dev, int size, unsigned char* data)
{
#ifdef USBH_DEBUG_WAIT
    udelay(500000);
#endif
    int ret;
    int addr = dev->info.address;
    int pipe = BULK_OUT_PIPENO;
    int ep =  dev->info.interface[dev->info.cur_interface].ea_out;
    API_FUNC_ENTER();
#ifdef USBH_DEBUG_BO
    debug_printf("BO%d P:%d A:%d E:%d\r\n",size, pipe, addr, ep);
#endif
    usbh_DEVADD_set_speed(addr);
    usbh_pipe_set_pidnak(pipe);
    usbh_pipe_set_ep(pipe, (int)ep, DIR_W, PIPE_BULK, (int)MaxPacket[pipe], addr);
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
}

en_usb_status ul_bulk_in_transfer(int conn_num,st_usb_device *dev, int size, unsigned char* data)
{
    int ret;
    int addr = dev->info.address;
    int pipe = BULK_IN_PIPENO;
    int ep = dev->info.interface[dev->info.cur_interface].ea_in;
    API_FUNC_ENTER();
#ifdef USBH_DEBUG_BI
    debug_printf("BI%d P:%d A:%d E:%d\r\n", size, pipe, addr, ep);
#endif
    usbh_DEVADD_set_speed(addr);
    usbh_pipe_set_pidnak(pipe);
    usbh_pipe_set_ep(pipe, (int)ep, DIR_R, PIPE_BULK, (int)MaxPacket[pipe], addr);
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
}
//#endif

void ul_hw_init(void) {
    rza1h_init();
    ul_host_config(0);        /* opencores USB host configuration */
    ul_host_bus_reset(0);     /* bus reset */
}

void ul_hw_device_setup(int conn_num,st_usb_device *dev,int adrs) {
    /* this function is called from um_device_setup */
    MPRINT("ul_hw_device_setup called\n");
#ifdef WIFI
#endif
    //usbh_DEVADD_set_speed(adrs);
    //usbh_pipe_set_pidnak(BULK_OUT_PIPENO);
    //usbh_pipe_set_ep(BULK_OUT_PIPENO, (int)dev->info.interface[dev->info.cur_interface].ea_out, DIR_W, PIPE_BULK, (int)MaxPacket[pipe], adrs);
    //usbh_pipe_set_pidnak(BULK_IN_PIPENO);
    //usbh_pipe_set_ep(BULK_IN_PIPENO, (int)dev->info.interface[dev->info.cur_interface].ea_in, DIR_R, PIPE_BULK, (int)MaxPacket[pipe], adrs);
}
