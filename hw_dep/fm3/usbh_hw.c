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
#if 0
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
#endif

//#define FM3_USB_DEBUG
//#define FM3_USB_DEBUG_HUB
//#define FM3_USB_DEBUG_PACKET
//#define FM3_USB_DEBUG_PACKET_INT
//#define FM3_USB_DEBUG_API_FUNC
//#define FM3_USB_DEBUG_TOKEN
//#define FM3_USB_DEBUG_DUMP_REGS
//#define FM3_USB_DEBUG_TOKEN_ERR
//#define FM3_USB_DEBUG_ERR

#ifdef FM3_USB_DEBUG
#define dprintk(fmt, ...)	debug_printf("%s(): "fmt, __func__, ## __VA_ARGS__)
#else
#define	dprintk(fmt, ...)
#endif

#ifdef FM3_USB_DEBUG_API_FUNC
#define API_FUNC_ENTER()	debug_printf("%s enter\n", __func__)
#define API_FUNC_EXIT()		debug_printf("%s exit\n", __func__)
#else
#define API_FUNC_ENTER()
#define API_FUNC_EXIT()
#endif

#ifdef FM3_USB_DEBUG_FUNC
#define FUNC_ENTER()	debug_printf("%s enter\n", __func__)
#define FUNC_EXIT()		debug_printf("%s exit\n", __func__)
#else
#define FUNC_ENTER()
#define FUNC_EXIT()
#endif

#define EP_IN 0
#define EP_OUT 1
#define TOGGLE0 0
#define TOGGLE1 1
#define EP_SETUP 0

volatile int usb_speed = 1;
volatile int transfer_busy = 0;
volatile int transfer_start = 0;
volatile int busreset_done = 0;

FM3_USB_TypeDef *fm3_usb = FM3_USB1;

#ifdef FM3_USB_DEBUG_DUMP_REGS
void dump_regs(void)
{
    uint8_t hcnt0;
    uint8_t hcnt1;
    uint8_t hirq;
    uint8_t herr;
    uint8_t hstate;
    uint8_t hfcomp;
    uint8_t hrtimer;
    uint8_t hadr;
    uint8_t heof;
    uint8_t hframe;
    uint8_t htoken;

    hcnt0 = fm3_usb->HCNT0;
    hcnt1 = fm3_usb->HCNT1;
    hirq = fm3_usb->HIRQ;
    herr = fm3_usb->HERR;
    hstate = fm3_usb->HSTATE;
    hfcomp = fm3_usb->HFCOMP;
    hrtimer = fm3_usb->HRTIMER;
    hadr = fm3_usb->HADR;
    heof = fm3_usb->HEOF;
    hframe = fm3_usb->HFRAME;
    htoken = fm3_usb->HTOKEN;

    debug_printf("HCNT0:%02x HCNT1:%0x HIRQ:%0x HERR:%0x HSTATE:%0x HTOKEN:%0x\n", hcnt0, hcnt1, hirq, herr, hstate, htoken);
}
#endif

static inline void fm3_usbh_clear_fifo(void)
{
    fm3_usb->EP2S_f.BFINI = 1;  // initialize fifo
    fm3_usb->EP1S_f.BFINI = 1;  // initialize fifo
}

static inline void fm3_usbh_set_fifo(void)
{
    fm3_usb->EP2S_f.BFINI = 0;  // initialize fifo
    fm3_usb->EP1S_f.BFINI = 0;  // initialize fifo
}

void fm3_usbh_clock_init(void)
{
    //bFM3_CLK_GATING_CKEN2_USBCK0 = 1;
    bFM3_USBETHERNETCLK_UCCR_UCEN1 = 0; /* disable USB clock */
    while (bFM3_USBETHERNETCLK_UCCR_UCEN1 != 0)
        ; /* wait for USB clock stop */
    bFM3_USBETHERNETCLK_UPCR1_UPLLEN = 0; /* disable USB-PLL clock */
    bFM3_USBETHERNETCLK_UCCR_UCSEL1 = 1; /* select PLL macro clock */
    bFM3_USBETHERNETCLK_UPCR1_UPINC = 0; /* select main clock as input clock */
    /* select clock stabilization time */
    FM3_USBETHERNETCLK->UPCR2 = UPCR2_UPOWT_INIT_VALUE;
    /* USB-PLL=Fin*N/K -> 96MHz=4MHz*24/1 */
    /* USB-PLL clock configuration register(K) initialize */
    FM3_USBETHERNETCLK->UPCR3 = UPCR3_UPLLK_INIT_VALUE;       // 4MHz:0, 16MHz:0
    /* USB-PLL clock configuration register(N) initialize */
    FM3_USBETHERNETCLK->UPCR4 = UPCR4_UPLLN_INIT_VALUE;      // 4MHz:60, 16Mz:24
    /* USB-PLL clock configuration register(N) initialize */
    FM3_USBETHERNETCLK->UPCR5 = UPCR5_UPLLN_INIT_VALUE;       // 4MHz:5,  16Mz:6
    bFM3_USBETHERNETCLK_UPINT_ENR_UPCSE = 0; /* USB-PLL clock stabilize interrupt disable  */
    bFM3_USBETHERNETCLK_UPCR1_UPLLEN = 1; /* enable USB-PLL clock */
    while (bFM3_USBETHERNETCLK_UP_STR_UPRDY == 0)
        ; /* wait for USB-PLL clock ready */
    bFM3_USBETHERNETCLK_UCCR_UCEN1 = 1; /* enable USB clock */
    /* wait for 5 cycle */
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
    bFM3_USBETHERNETCLK_USBEN1_USBEN1 = 1; /* enable USB controller */
    return;
}

void fm3_usbh_out_set_fifo_size(uint16_t size)
{
    fm3_usb->EP2S_f.BFINI = 1;
    fm3_usb->EP2C = ((fm3_usb->EP2C & 0xFF80) | size);
    fm3_usb->EP2C_f.DIR = 1;
    fm3_usb->EP2S_f.BFINI = 0;
}

void fm3_usbh_in_set_fifo_size(uint16_t size)
{
    fm3_usb->EP1S_f.BFINI = 1;
    fm3_usb->EP1C = ((fm3_usb->EP1C & 0xFE00) | size);
    fm3_usb->EP1C_f.DIR = 0;
    fm3_usb->EP1S_f.BFINI = 0;
}

void fm3_usbh_read_fifo(unsigned char *buffer, int len)
{
    uint32_t i;
    if (buffer != NULL) {
        for (i = 0; i < len; i++) {
            if ((i & 0x01) == 0x00) {
                *buffer = fm3_usb->EP1DTL;
            } else {
                *buffer = fm3_usb->EP1DTH;
            }
            buffer++;
        }
    }
}

void fm3_usbh_write_fifo(unsigned char *buffer, int len)
{
    uint32_t i;
    for (i = 0; i < len; i++) {
        if ((i & 0x01) == 0x00) {
            fm3_usb->EP2DTL = *buffer;
        } else {
            fm3_usb->EP2DTH = *buffer;
        }
        buffer++;
    }
}

static inline void fm3_usbh_clear_out_ep_drq(void)
{
    fm3_usb->EP2S_f.DRQ = 0;
}

static inline void fm3_usbh_clear_in_ep_drq(void)
{
    fm3_usb->EP1S_f.DRQ = 0;
}

static void fm3_usbh_set_token(uint8_t addr, uint8_t toggle, uint8_t token, uint8_t ep)
{
    uint8_t reg;
    reg = (token << 4) | (ep & 0x0f);
    if (toggle)
        reg |= 0x80;
    else
        reg &= ~0x80;
    fm3_usb->HADR = addr;
    fm3_usb->HTOKEN = reg;
#ifdef FM3_USB_DEBUG_TOKEN
    debug_printf("A:%d T:%d K:%d: E:%d\n", addr, toggle, token, ep);
#endif
}

/* The function works only if interrupt is disabled. */
static int fm3_usbh_is_device_connected(void)
{
    int timeout = FM3_USBH_CONNECT_TIMEOUT;
    while (fm3_usb->HIRQ_f.CNNIRQ) {
        if (timeout-- <= 0) {
            return 0;
        }
    }
    return 1;
}

void fm3_usbh_force_tmode(int mode, int fmessage)
{
    fm3_usb->UDCC_f.RST = 1;
    if (mode == 1) {
        usb_speed = 1;
        fm3_usb->HSTATE_f.ALIVE = 0;
        if (fmessage)
            debug_printf("Changed to FULL SPEED\n");
    } else {
        usb_speed = 0;
        fm3_usb->HSTATE_f.ALIVE = 1;
        if (fmessage)
            debug_printf("Changed to LOW SPEED\n");
    }
    fm3_usb->HSTATE_f.CLKSEL = usb_speed;
    while (fm3_usb->HSTATE_f.CLKSEL != usb_speed)
        ;
    fm3_usb->UDCC_f.RST = 0;
    udelay(10000);
    fm3_usbh_set_token(0, 0, USBH_MH_TOKEN_SOF, 0);
}

int fm3_usbh_set_tmode(int fmessage)
{
    if (fm3_usb->HSTATE_f.TMODE == 1) {
        usb_speed = 1;
        fm3_usb->HSTATE_f.ALIVE = 0;
        if (fmessage)
            debug_printf("FULL SPEED detected\n");
    } else {
        usb_speed = 0;
        fm3_usb->HSTATE_f.ALIVE = 1;
        if (fmessage)
            debug_printf("LOW SPEED detected\n");
    }
    fm3_usb->HSTATE_f.CLKSEL = usb_speed;
    while (fm3_usb->HSTATE_f.CLKSEL != usb_speed)
        ;
    return usb_speed;
}

void fm3_usbh_init_sub(void)
{
    transfer_busy = 0;
    /* ToDO set USB host interrupt level */
    /* initialize IO port related VBUS */
    //bFM3_GPIO_PDOR6_P62 = (~USBH_VBUS_IO_VALID_LEVEL);
    //bFM3_GPIO_DDR6_P62 = 1;
    fm3_usb->HCNT_f.HOST = 0;
    fm3_usb->UDCC_f.HCONX = 1; /* set host mode */
    fm3_usb->UDCC_f.RST = 1; /* reset USB function */

    fm3_usb->EP1C_f.EPEN = 0;
    fm3_usb->EP2C_f.EPEN = 0;
    fm3_usbh_clear_fifo();
    fm3_usb->EP1C = 0x4000 | (uint16_t)USBH_MH_PKS_INIT;
    fm3_usb->EP2C = 0x4000 | (uint16_t)USBH_MH_PKS_INIT;
    fm3_usb->EP1C_f.DIR = 0; /* endpoint1 for IN-direction transfer */
    fm3_usb->EP2C_f.DIR = 1; /* endpoint2 for OUT-direction transfer */
    fm3_usb->EP1C_f.EPEN = 1;
    fm3_usb->EP2C_f.EPEN = 1;

    fm3_usb->HFCOMP = 0;
    fm3_usb->HRTIMER0 = 0;
    fm3_usb->HRTIMER1 = 0;
    fm3_usb->HRTIMER2 = 0;
    fm3_usb->HSTATE_f.CSTAT = 0;
    fm3_usb->HSTATE_f.ALIVE = 0;

    fm3_usb->HADR = 0; /* set host address to 0 */
    fm3_usb->HCNT_f.HOST = 1; /* set host mode */
    while (fm3_usb->HCNT_f.HOST != 1)
        ;

    fm3_usbh_force_tmode(0, 0);
    if (fm3_usbh_is_device_connected()) {
        fm3_usbh_set_tmode(1);
    }
    fm3_usb->HIRQ = 0;
    fm3_usb->HEOF = USBH_MH_EOF_TIME;
    fm3_usb->HCNT_f.RETRY = 0; /* do not retry the operation when an error occurs     */
    fm3_usbh_clear_fifo();
    fm3_usbh_set_fifo();

    fm3_usb->UDCC_f.RST = 0; /* cancel reset condition of USB function */
    fm3_usb->HCNT_f.SOFSTEP = 1; /* generate an interrupt request during SOF processing */
    fm3_usb->HCNT_f.CANCEL = 0; /* continue with the token in the EOF area             */

    fm3_usb->HCNT_f.RWKIRE = 1; /* remote wake up interrupt */
    fm3_usb->HCNT_f.CMPIRE = 1; /* token completion interrupt */
    fm3_usb->HCNT_f.CNNIRE = 1; /* connection interrupt of USB device */
    fm3_usb->HCNT_f.DIRE = 1; /* disconnection interrupt of USB device */
    fm3_usb->HCNT_f.SOFIRE = 1; /* SOF token interrupt */
    /* ToDo DMA init */
    /* ToDo NVIC init */
}

void fm3_usbh_busreset(void)
{
    volatile unsigned int timeout = 10000;

    if (fm3_usb->HSTATE_f.CSTAT == 1) {
        fm3_usb->HADR = 0;
        fm3_usbh_clear_fifo();
        fm3_usbh_set_fifo();
        fm3_usb->HCNT_f.URST = 1;
        while (timeout-- > 0) {
            asm volatile ("nop");
        }
    } else {
        //fm3_usb_init_sub();
    }
}

int fm3_usbh_init(void)
{
    __disable_irq();
    fm3_usbh_clock_init();
    //FM3_GPIO->PFR8 |= 0x00000003;
    //FM3_GPIO->PFR8 |= 0x0000000C;
    bFM3_GPIO_SPSR_USB1C = 1;
    fm3_usbh_init_sub();
    NVIC_EnableIRQ(USB1F_USB1H_IRQn);
    //bFM3_GPIO_PDOR6_P62 = USBH_VBUS_IO_VALID_LEVEL;
    __enable_irq();
    return 0;
}

//void __attribute__ ((interrupt)) fm3_usbh_int(void)
void __attribute__ ((interrupt)) USB1_Handler(void)
{
#ifdef FM3_USB_DEBUG
    uint8_t hcnt0;
    uint8_t hcnt1;
    uint8_t hirq;
    uint8_t herr;
    uint8_t hstate;
    uint8_t hfcomp;
    uint8_t hrtimer;
    uint8_t hadr;
    uint8_t heof;
    uint8_t hframe;
    uint8_t htoken;

    hcnt0 = fm3_usb->HCNT0;
    hcnt1 = fm3_usb->HCNT1;
    hirq = fm3_usb->HIRQ;
    herr = fm3_usb->HERR;
    hstate = fm3_usb->HSTATE;
    hfcomp = fm3_usb->HFCOMP;
    hrtimer = fm3_usb->HRTIMER;
    hadr = fm3_usb->HADR;
    heof = fm3_usb->HEOF;
    hframe = fm3_usb->HFRAME;
    htoken = fm3_usb->HTOKEN;
#endif
    if (fm3_usb->HIRQ_f.DIRQ == 1) {
        // Disconnection Interrupt Request
        fm3_usb->HIRQ_f.DIRQ = 0;
        // ToDo: clear all endpoints
    }
    if (fm3_usb->HIRQ_f.CNNIRQ == 1) {
        // Connection Interrupt Request
        fm3_usb->HIRQ_f.CNNIRQ = 0;
        if (fm3_usb->HIRQ_f.URIRQ == 0) {
            // USB Bus Reset Interrupt Request
            // URIRQ 0: in bus reset
            fm3_usb->UDCC_f.RST = 1;
            fm3_usbh_set_tmode(0);
            fm3_usb->UDCC_f.RST = 0;
            fm3_usbh_busreset();
        } else {
            // URIRQ 1: bus reset done.
            fm3_usb->HIRQ_f.URIRQ = 0;
            fm3_usbh_set_tmode(0);
            fm3_usbh_set_token(0, 0, USBH_MH_TOKEN_SOF, 0);
            busreset_done = 1;
        }
    }
    if (fm3_usb->HIRQ_f.URIRQ == 1) {
        fm3_usb->HIRQ_f.URIRQ = 0;
        busreset_done = 1;
    }
    if (fm3_usb->HIRQ_f.SOFIRQ == 1) {
        fm3_usb->HIRQ_f.SOFIRQ = 0;
    }
    if (fm3_usb->HIRQ_f.CMPIRQ == 1) {
        int i = 1000;
        unsigned char dummy = fm3_usb->HERR;
        fm3_usb->HIRQ_f.CMPIRQ = 0;
        while (((fm3_usb->HTOKEN & 0x70) != 0) && (i--)) {
            asm volatile ("nop");
        }
        transfer_busy = 0;
    }
}

int usb_lowlevel_init(void)
{
    return fm3_usbh_init();
}

int usb_lowlevel_stop(void)
{
    fm3_usb->UDCC_f.RST = 1; /* reset USB function */
    //bFM3_GPIO_PDOR6_P62 = (~USBH_VBUS_IO_VALID_LEVEL);
    return 0;
}

void ul_host_config(int conn_num)
{
    API_FUNC_ENTER();
    fm3_usbh_init();
    API_FUNC_EXIT();
}

#ifdef USE_MODIFIED_CORE
void ul_change_conn(int n) {
    /* STOP SOF */
    //USB_TX_SOF_ENABLE_REG(n) = 0;
    //ul_timer(1);
    //USB_HOST_SLAVE_CONTROL_REG(n) = n;
    //ul_timer(1);
    /* START SOF */
    //USB_TX_SOF_ENABLE_REG(n) = 1;
    //ul_timer(1);
}
#endif

void ul_host_bus_reset(int conn_num)
{
    int timeout = 10000;
    API_FUNC_ENTER();
    fm3_usbh_busreset();
    while (timeout-- > 0) {
        if (busreset_done == 1)
            break;
        udelay(100);
    }
    fm3_usbh_set_tmode(1);
    API_FUNC_EXIT();
}

//int wait_int_event(int conn_num,en_int_status s) {
//  return USBH_ERROR;
//}

int ul_speed_configuration(int conn_num)
{
    return fm3_usbh_set_tmode(0);
}

void full_speed_mode(int conn_num)
{
    API_FUNC_ENTER();
    fm3_usbh_force_tmode(1, 1);
    API_FUNC_EXIT();
}

void hub_low_speed_mode(int conn_num)
{
    API_FUNC_ENTER();
    fm3_usbh_force_tmode(0, 1);
    API_FUNC_EXIT();
}

int usb_maxpacket(struct usb_device *dev, unsigned long pipe)
{
    if (usb_speed == 1)
        return 64;
    else
        return 8;
}

void cnv_udev(struct fm_usb_device *udev, st_usb_device *dev)
{
    udev->devnum = dev->info.address;
    udev->toggle[0] = dev->toggle[0];
    udev->toggle[1] = dev->toggle[1];
    if (usb_speed == 1) {
        udev->maxpacketsize = 3;
        udev->speed = 0;
    } else {
        udev->maxpacketsize = 0;
        udev->speed = 1;
    }
}

#ifdef FM3_USB_DEBUG_PACKET
int usb_send_packet(char *fn, st_usb_device *dev, unsigned long pipe, int token, int toggle, unsigned char *buffer, int len, int *status)
#else
int usb_send_packet(st_usb_device *dev, unsigned long pipe, int token, int toggle, unsigned char *buffer, int len, int *status)
#endif
{
    int ep;
    volatile int timeout;
    unsigned char herr;
    unsigned char handshake;
    int ret = len;
    int error = 0;
    int addr = dev->info.address;
    *status = USBH_OK;

    ep = usb_pipeendpoint(pipe);
#ifdef FM3_USB_DEBUG_PACKET
    debug_printf("%s addr=%d pipe=%0lx ep=%d token=%d len=%3d dir=%ld toggle=%d\n",
            fn, addr, pipe, ep, token, len, usb_pipeout(pipe), toggle);
#endif
    while (error < FM3_USBH_RETRY_MAX) {
        fm3_usb->HERR = 0;
        if (transfer_busy == 0) {
            fm3_usb->HCNT_f.CMPIRE = 0;
            fm3_usb->HCNT_f.SOFIRE = 0;
            if (usb_pipeout(pipe)) {
                if (len) {
                    fm3_usbh_out_set_fifo_size(len);
                    fm3_usbh_in_set_fifo_size(DEFAULT_MAX_PKS);
                    fm3_usbh_write_fifo(buffer, len);
                }
                fm3_usbh_clear_out_ep_drq();
            } else {
                fm3_usbh_in_set_fifo_size(usb_maxpacket(dev, pipe));
                fm3_usbh_out_set_fifo_size(DEFAULT_MAX_PKS);
            }
            fm3_usbh_set_token(addr, toggle, token, ep);
            transfer_busy = 1;
            fm3_usb->HCNT_f.CMPIRE = 1;
            fm3_usb->HCNT_f.SOFIRE = 1;
            timeout = FM3_USBH_TOKEN_TIMEOUT;
            while (transfer_busy == 1) {
                if (timeout-- == 0) {
#ifdef FM3_USB_DEBUG_TOKEN_ERR
                    debug_printf("Err: token timeout\n");
#endif
                    transfer_busy = 0;
                    break;
                }
            }
        } else {
            udelay(100);
            error++;
            continue;
        }
        herr = fm3_usb->HERR;
        if (fm3_usb->HERR_f.LSTOF == 1) {
            fm3_usb->HERR_f.LSTOF = 0;	// clear error;
#ifdef FM3_USB_DEBUG_TOKEN_ERR
                    debug_printf("L.");
#endif
        } else if (fm3_usb->HERR_f.TOUT == 1) {
            fm3_usb->HERR_f.TOUT = 0;	// clear error;
#ifdef FM3_USB_DEBUG_TOKEN_ERR
                    debug_printf("T.");
#endif
            udelay(100);
            error++;
            if (error == FM3_USBH_TOUT_RETRY_MAX) {
                ret = USBH_TIMEOUT;
                break;
            }
        } else if (fm3_usb->HERR_f.RERR == 1) {
            fm3_usb->HERR_f.RERR = 0;	// clear error;
#ifdef FM3_USB_DEBUG_TOKEN_ERR
                    debug_printf("R.");
#endif
        } else if (fm3_usb->HERR_f.TGERR == 1) {
            fm3_usb->HERR_f.TGERR = 0;	// clear error;
#ifdef FM3_USB_DEBUG_TOKEN_ERR
                    debug_printf("G.");
#endif
        }
        handshake = fm3_usb->HERR & 0x03;
        fm3_usb->HERR = 0x03;  // clear error
        if (handshake == USBH_MH_HANDSHAKE_ACK) {
            if (usb_pipein(pipe)) {
                len = FM3_USB1->EP1S & 0x01FF;
                if (len) {
                    fm3_usbh_read_fifo(buffer, len);
                    fm3_usbh_clear_in_ep_drq();
                }
                ret = len;
            } else
                ret = len;
            break;
        } else if (handshake == USBH_MH_HANDSHAKE_NAK) {
            if (error > FM3_USBH_NAK_RETRY_MAX) {
                *status = USBH_NAK;
                ret = -1;
                break;
            }
            udelay(1000);
        } else if (handshake == USBH_MH_HANDSHAKE_STALL) {
            *status = USBH_STALL;
            ret = -1;
            break;
        } else {
            if (error > FM3_USBH_NUL_RETRY_MAX) {
#ifdef FM3_USB_DEBUG_TOKEN_ERR
                debug_printf("N.");
#endif
                *status = USBH_ERROR;
                ret = -1;
                break;
            }
            udelay(1000);
        }
        error++;
    }
    if (error >= FM3_USBH_RETRY_MAX) {
#ifdef FM3_USB_DEBUG_TOKEN_ERR
        debug_printf("Err: handshake HERR:%x\n", herr);
#endif
        *status = USBH_ERROR;
        ret = -1;
    }
//fm3_send_packet_exit:
    return ret;
}

en_usb_status ul_control_setup_transaction(int conn_num, st_usb_device *dev, st_usb_setup_data *sd)
{
    struct fm_usb_device fm_dev;
    int status;
    int ret;
    cnv_udev(&fm_dev, dev);
    unsigned long pipe = usb_sndctrlpipe(&fm_dev, EP_SETUP);
    API_FUNC_ENTER();
    usb_settoggle(dev, EP_SETUP, EP_OUT, TOGGLE0);
#ifdef FM3_USB_DEBUG_PACKET
    ret = usb_send_packet("CS:", dev, pipe, USBH_MH_TOKEN_SETUP, TOGGLE0, (unsigned char *)&sd->BYTE, sizeof(sd->BYTE), &status);
#else
    ret = usb_send_packet(dev, pipe, USBH_MH_TOKEN_SETUP, TOGGLE0, (unsigned char *)&sd->BYTE, sizeof(sd->BYTE), &status);
#endif
    dev->pipe = pipe;
    if (ret != sizeof(sd->BYTE)) {
#ifdef FM3_USB_DEBUG_ERR
        debug_printf("%s err\n", __func__);
#endif
        ret = status;
    } API_FUNC_EXIT();
    return USBH_OK;
}

en_usb_status ul_control_in_transaction(int conn_num, st_usb_device *dev, int size, unsigned char *data)
{
    struct fm_usb_device fm_dev;
    int status;
    int ret;
    int done = 0;
    cnv_udev(&fm_dev, dev);
    unsigned long pipe = usb_rcvctrlpipe(&fm_dev, EP_SETUP);
    int ep = usb_pipeendpoint(pipe);
    int max = usb_maxpacket(dev, pipe);
    API_FUNC_ENTER();
    usb_settoggle(dev, ep, usb_pipeout(pipe), TOGGLE1);
    while (done < size) {
#ifdef FM3_USB_DEBUG_PACKET
        ret = usb_send_packet("CI:", dev, pipe, USBH_MH_TOKEN_IN, usb_gettoggle(dev, ep, usb_pipeout(pipe)), data + done, max > size - done? size - done : max, &status);
#else
        ret = usb_send_packet(dev, pipe, USBH_MH_TOKEN_IN, usb_gettoggle(dev, ep, usb_pipeout(pipe)), data + done,
                max > size - done ? size - done : max, &status);
#endif
        usb_dotoggle(dev, ep, usb_pipeout(pipe));
        if (ret < 0) {
#ifdef FM3_USB_DEBUG_ERR
            if (status != USBH_NAK)
            debug_printf("%s err=%d\n", __func__, status);
#endif
            return status;
        }
        done += ret;
        if (ret < max)
            break;
    }
    dev->pipe = pipe;
    API_FUNC_EXIT();
    return USBH_OK;
}

en_usb_status ul_control_out_transaction(int conn_num, st_usb_device *dev, int size, unsigned char *data)
{
    struct fm_usb_device fm_dev;
    int status;
    int ret;
    int done = 0;
    cnv_udev(&fm_dev, dev);
    unsigned long pipe = usb_sndctrlpipe(&fm_dev, EP_SETUP);
    int ep = usb_pipeendpoint(pipe);
    int max = usb_maxpacket(dev, pipe);
    API_FUNC_ENTER();
    usb_settoggle(dev, ep, usb_pipeout(pipe), TOGGLE1);
    while (done < size) {
#ifdef FM3_USB_DEBUG_PACKET
        ret = usb_send_packet("CO:", dev, pipe, USBH_MH_TOKEN_OUT, usb_gettoggle(dev, ep, usb_pipeout(pipe)), data + done, max > size - done? size - done : max, &status);
#else
        ret = usb_send_packet(dev, pipe, USBH_MH_TOKEN_OUT, usb_gettoggle(dev, ep, usb_pipeout(pipe)), data + done,
                max > size - done ? size - done : max, &status);
#endif
        usb_dotoggle(dev, ep, usb_pipeout(pipe));
        if (ret < 0) {
#ifdef FM3_USB_DEBUG_ERR
            if (status != USBH_NAK)
            debug_printf("%s err=%d\n", __func__, status);
#endif
            return status;
        }
        done += ret;
    }
    dev->pipe = pipe;
    API_FUNC_EXIT();
    return USBH_OK;
}

en_usb_status ul_control_ack_out_transaction(int conn_num, st_usb_device *dev)
{
    struct fm_usb_device fm_dev;
    int status;
    int ret;
    cnv_udev(&fm_dev, dev);
    unsigned long pipe = usb_sndctrlpipe(&fm_dev, EP_SETUP);
    int ep = usb_pipeendpoint(pipe);
    API_FUNC_ENTER();
    //usb_settoggle(dev, ep, !usb_pipeout(pipe), TOGGLE1);
#ifdef FM3_USB_DEBUG_PACKET
    //ret = usb_send_packet("AO:", dev, usb_sndctrlpipe(&fm_dev, ep), usb_gettoggle(dev, ep, !usb_pipeout(pipe)), USBH_MH_TOKEN_OUT, (unsigned char *)0, 0);
    ret = usb_send_packet("AO:", dev, pipe, USBH_MH_TOKEN_OUT, TOGGLE1, (unsigned char *)0, 0, &status);
#else
    ret = usb_send_packet(dev, pipe, USBH_MH_TOKEN_OUT, TOGGLE1, (unsigned char *)0, 0, &status);
#endif
    API_FUNC_EXIT();
    if (ret < 0) {
#ifdef FM3_USB_DEBUG_ERR
        if (status != USBH_NAK)
        debug_printf("%s err=%d\n", __func__, status);
#endif
        return status;
    }
    return USBH_OK;
}

en_usb_status ul_control_ack_in_transaction(int conn_num, st_usb_device *dev)
{
    struct fm_usb_device fm_dev;
    int status;
    int ret;
    cnv_udev(&fm_dev, dev);
    //unsigned long pipe = dev->pipe;
    unsigned long pipe = usb_rcvctrlpipe(&fm_dev, EP_SETUP);
    int ep = usb_pipeendpoint(pipe);
    API_FUNC_ENTER();
    //usb_settoggle(dev, ep, !usb_pipeout(pipe), TOGGLE1);
#ifdef FM3_USB_DEBUG_PACKET
    //ret = usb_send_packet("AI:", dev, pipe, TOGGLE1, USBH_MH_TOKEN_IN, (unsigned char *)0, 0);
    ret = usb_send_packet("AI:", dev, pipe, USBH_MH_TOKEN_IN, TOGGLE1, (unsigned char *)0, 0, &status);
#else
    ret = usb_send_packet(dev, pipe, USBH_MH_TOKEN_IN, TOGGLE1, (unsigned char *)0, 0, &status);
#endif
    API_FUNC_EXIT();
    if (ret < 0) {
#ifdef FM3_USB_DEBUG_ERR
        if (status != USBH_NAK)
        debug_printf("%s err=%d\n", __func__, status);
#endif
        return status;
    }
    return USBH_OK;
}

// interrupt transfer(one or more transactions)
en_usb_status ul_interrupt_in_transfer(int conn_num, st_usb_device *dev, int *size, unsigned char *data)
{
    struct fm_usb_device fm_dev;
    int status;
    int ret;
    int ep = dev->info.interface[dev->info.cur_interface].ea_in;
    cnv_udev(&fm_dev, dev);
    unsigned long pipe = usb_rcvintpipe(&fm_dev, ep);
    int max = usb_maxpacket(&fm_dev, pipe);
    API_FUNC_ENTER();
#ifdef FM3_USB_DEBUG_PACKET_INT
    ret = usb_send_packet("II:", dev, pipe, USBH_MH_TOKEN_IN, usb_gettoggle(dev, ep, 0), data, max, &status);
#else
    ret = usb_send_packet(dev, pipe, USBH_MH_TOKEN_IN, usb_gettoggle(dev, ep, 0), data, max, &status);
#endif
    usb_dotoggle(dev, ep, 0);
    dev->pipe = pipe;
    if (ret < 0) {
#ifdef FM3_USB_DEBUG_ERR
        if (status != USBH_NAK)
        debug_printf("%s err=%d\n", __func__, status);
#endif
        *size = 0;
        return status;
    }
    *size = ret;
    API_FUNC_EXIT();
    return USBH_OK;
}

en_usb_status ul_interrupt_out_transfer(int conn_num, st_usb_device *dev, int size, unsigned char* data)
{
    struct fm_usb_device fm_dev;
    int status;
    int ret;
    int done = 0;
    int ep = dev->info.interface[dev->info.cur_interface].ea_out;
    cnv_udev(&fm_dev, dev);
    unsigned long pipe = usb_sndintpipe(&fm_dev, ep);
    int max = usb_maxpacket(&fm_dev, pipe);
    API_FUNC_ENTER();
    while (done < size) {
#ifdef FM3_USB_DEBUG_PACKET_INT
        ret = usb_send_packet("IO:", dev, pipe, USBH_MH_TOKEN_OUT, usb_gettoggle(dev, ep, usb_pipeout(pipe)), data + done, max > size - done? size - done : max, &status);
#else
        ret = usb_send_packet(dev, pipe, USBH_MH_TOKEN_OUT, usb_gettoggle(dev, ep, usb_pipeout(pipe)), data + done,
                max > size - done ? size - done : max, &status);
#endif
        usb_dotoggle(dev, ep, usb_pipeout(pipe));
        if (ret < 0) {
#ifdef FM3_USB_DEBUG_ERR
            if (status != USBH_NAK)
            debug_printf("%s err=%d\n", __func__, status);
#endif
            return status;
        }
        done += ret;
    }
    dev->pipe = pipe;
    API_FUNC_EXIT();
    return USBH_OK;
}

#ifdef USBH_USE_MSD
// bulk transfer(one or more transactions)
en_usb_status ul_bulk_out_transfer(int conn_num, st_usb_device *dev, int size, unsigned char* data)
{
    struct fm_usb_device fm_dev;
    int status;
    int ret;
    int done = 0;
    int ep = dev->info.interface[dev->info.cur_interface].ea_out;
    cnv_udev(&fm_dev, dev);
    unsigned long pipe = usb_sndbulkpipe(&fm_dev, ep);
    int addr = dev->info.address;
    int max = usb_maxpacket(&fm_dev, pipe);
    //usb_settoggle(dev, ep, 1, 0);
    while (done < size) {
#ifdef FM3_USB_DEBUG_PACKET
        ret = usb_send_packet("BO:", dev, pipe, USBH_MH_TOKEN_OUT, usb_gettoggle(dev, ep, usb_pipeout(pipe)), data + done, max > size - done? size - done : max, &status);
#else
        ret = usb_send_packet(dev, pipe, USBH_MH_TOKEN_OUT, usb_gettoggle(dev, ep, usb_pipeout(pipe)), data + done,
                max > size - done ? size - done : max, &status);
#endif
        usb_dotoggle(dev, ep, usb_pipeout(pipe));
        if (ret < 0) {
#ifdef FM3_USB_DEBUG_ERR
            if (status != USBH_NAK)
            debug_printf("%s err=%d\n", __func__, status);
#endif
            return status;
        }
        done += ret;
    }
    return USBH_OK;
}

en_usb_status ul_bulk_in_transfer(int conn_num, st_usb_device *dev, int size, unsigned char* data)
{
    struct fm_usb_device fm_dev;
    int status;
    int ret;
    int done = 0;
    int ep = dev->info.interface[dev->info.cur_interface].ea_in;
    cnv_udev(&fm_dev, dev);
    unsigned long pipe = usb_rcvbulkpipe(&fm_dev, ep);
    int max = usb_maxpacket(&fm_dev, pipe);
    //usb_settoggle(dev, ep, 0, 0);
    while (done < size) {
#ifdef FM3_USB_DEBUG_PACKET
        ret = usb_send_packet("BI:", dev, pipe, USBH_MH_TOKEN_IN, usb_gettoggle(dev, ep, usb_pipeout(pipe)), data + done, max > size - done? size - done : max, &status);
#else
        ret = usb_send_packet(dev, pipe, USBH_MH_TOKEN_IN, usb_gettoggle(dev, ep, usb_pipeout(pipe)), data + done,
                max > size - done ? size - done : max, &status);
#endif
        usb_dotoggle(dev, ep, usb_pipeout(pipe));
        if (ret < 0) {
#ifdef FM3_USB_DEBUG_ERR
            if (status != USBH_NAK)
            debug_printf("%s err=%d\n", __func__, status);
#endif
            return status;
        }
        if (ret < max)
            break;
        done += ret;
    }
    return USBH_OK;
}
#endif

void ul_hw_init(void)
{
    fm3_init();
    ul_host_config(0); /* opencores USB host configuration */
    ul_host_bus_reset(0); /* bus reset */
}

void ul_hw_device_setup(int conn_num, st_usb_device *dev, int adrs)
{
    /* this function is called from um_device_setup */
    MPRINT("ul_hw_device_setup called\n");
}

