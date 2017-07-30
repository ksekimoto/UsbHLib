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

#ifndef USBH_HW_H_
#define USBH_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(OPENCORES)
#include "usbh_hw_reg.h"
#endif
#include "usbh_struct.h"

#define USB_MAXCHILDREN			8	/* This is arbitrary */

#ifdef CONFIG_USB_KEYBOARD
#include <stdio_dev.h>
extern unsigned char new[];
#endif

#define FM3_USBH_CONNECT_TIMEOUT	500		/* 500ms */
#define FM3_USBH_TOKEN_TIMEOUT		100000	/* 100000 count */
#define FM3_USBH_RETRY_MAX			500		/* 500 loops */
#define FM3_USBH_TOUT_RETRY_MAX  	100		/* 100ms */
#define FM3_USBH_NAK_RETRY_MAX  	300		/* 300ms */
#define FM3_USBH_NUL_RETRY_MAX  	50		/* 50ms */
#define DEFAULT_MAX_PKS	64

/* USB directions */
#define USB_DIR_OUT           0
#define USB_DIR_IN            0x80

#define OK(x)   len = (x); break
#define RH_GET_STATUS           0x0080
#define RH_CLEAR_FEATURE        0x0100
#define RH_SET_FEATURE          0x0300
#define RH_SET_ADDRESS      0x0500
#define RH_GET_DESCRIPTOR   0x0680
#define RH_SET_DESCRIPTOR       0x0700
#define RH_GET_CONFIGURATION    0x0880
#define RH_SET_CONFIGURATION    0x0900
#define RH_GET_STATE            0x0280
#define RH_GET_INTERFACE        0x0A80
#define RH_SET_INTERFACE        0x0B00
#define RH_SYNC_FRAME           0x0C80

#define USBH_EP_DISABLE 0
#define USBH_EP_ENABLE  1
#define USBH_EP_TYPE_BULK   2
#define USBH_EP_TYPE_INT    3
#define USBH_EP_DIR_IN      0
#define USBH_EP_DIR_OUT     1

#define USBH_VBUS_IO_VALID_LEVEL    ((uint8_t)0U)
#define USBH_MH_EP_INIT         ((uint16_t)0x4000)
#define USBH_MH_EOF_TIME        ((uint16_t)0x02C9)
#define USBH_MH_PKS_INIT        ((uint8_t)0x40)
#define USBH_MH_HRTIMER10_INIT  ((uint16_t)0xFFFF)
#define USBH_MH_HRTIMER2_INIT   ((uint8_t)0x03)
#define USBH_MH_EOF_TIME        ((uint16_t)0x02C9)

#define bFM3_USBETHERNETCLK_UCCR_UCEN0      *((volatile unsigned int*)(0x426C0000UL))
#define bFM3_USBETHERNETCLK_UCCR_UCSEL0     *((volatile unsigned int*)(0x426C0004UL))
#define bFM3_USBETHERNETCLK_UPCR1_UPLLEN    *((volatile unsigned int*)(0x426C0080UL))
#define bFM3_USBETHERNETCLK_UPCR1_UPINC     *((volatile unsigned int*)(0x426C0084UL))
#define bFM3_USBETHERNETCLK_UP_STR_UPRDY    *((volatile unsigned int*)(0x426C0280UL))
#define bFM3_USBETHERNETCLK_UPINT_ENR_UPCSE *((volatile unsigned int*)(0x426C0300UL))
#define bFM3_USBETHERNETCLK_USBEN0_USBEN0   *((volatile unsigned int*)(0x426C0600UL))
#define bFM3_CLK_GATING_CKEN2_USBCK0        *((volatile unsigned int*)(0x42782400UL))

#define UPCR2_UPOWT_INIT_VALUE  (0x07)  /* initial value of UPCR2 register's UPOWT bits    */
#define UPCR3_UPLLK_INIT_VALUE  (0x00)  /* initial value of UPCR3 register's UPLLK bits    */
#define UPCR4_UPLLN_INIT_VALUE  (0x3B)  /* initial value of UPCR4 register's UPLLN bits    */
#define UPCR5_UPLLN_INIT_VALUE  (0x04)  /* initial value of UPCR4 register's UPLLM bits    */

#define USBH_MH_HANDSHAKE_ACK         (0x00)  /* ACK   */
#define USBH_MH_HANDSHAKE_NAK         (0x01)  /* NAK   */
#define USBH_MH_HANDSHAKE_STALL       (0x02)  /* STALL */
#define USBH_MH_HANDSHAKE_NULL        (0x03)  /* NULL  */
/* token type */
#define USBH_MH_TOKEN_SETUP           (1)     /* SETUP token */
#define USBH_MH_TOKEN_IN              (2)     /* IN token    */
#define USBH_MH_TOKEN_OUT             (3)     /* OUT token   */
#define USBH_MH_TOKEN_SOF             (4)     /* SOF token   */

#ifdef FM3_USB_DEBUG
#define PDEBUG(level, fmt, args...) \
        printf(fmt, ## args)
/*
 if (debug >= (level)) printf("[%s:%d] " fmt, \
        __PRETTY_FUNCTION__, __LINE__ , ## args)
 */
#else
#define PDEBUG(level, fmt, args...) do {} while(0)
#endif

/* "pipe" definitions */

#define PIPE_ISOCHRONOUS    0
#define PIPE_INTERRUPT      1
#define PIPE_CONTROL        2
#define PIPE_BULK           3
#define PIPE_DEVEP_MASK     0x0007ff00

struct fm_usb_device {
    int devnum; /* Device number on USB bus */
    int speed; /* full/low/high */
    char mf[32]; /* manufacturer */
    char prod[32]; /* product */
    char serial[32]; /* serial number */

    /* Maximum packet size; one of: PACKET_SIZE_* */
    int maxpacketsize;
    /* one bit for each endpoint ([0] = IN, [1] = OUT) */
    unsigned int toggle[2];
    /* endpoint halts; one bit per endpoint # & direction;
     * [0] = IN, [1] = OUT
     */
    unsigned int halted[2];
    int epmaxpacketin[16]; /* INput endpoint specific maximums */
    int epmaxpacketout[16]; /* OUTput endpoint specific maximums */

    int configno; /* selected config number */
    //struct usb_device_descriptor descriptor; /* Device Descriptor */
    //struct usb_config config; /* config descriptor */
    int have_langid; /* whether string_langid is valid yet */
    int string_langid; /* language ID for strings */
    int (*irq_handle)(struct usb_device *dev);
    unsigned long irq_status;
    int irq_act_len; /* transfered bytes */
    void *privptr;
    /*
     * Child devices -  if this is a hub device
     * Each instance needs its own set of data structures.
     */
    unsigned long status;
    int act_len; /* transfered bytes */
    int maxchild; /* Number of ports if hub */
    int portnr;
    struct usb_device *parent;
    struct usb_device *children[USB_MAXCHILDREN];
};

/*
 * Calling this entity a "pipe" is glorifying it. A USB pipe
 * is something embarrassingly simple: it basically consists
 * of the following information:
 *  - device number (7 bits)
 *  - endpoint number (4 bits)
 *  - current Data0/1 state (1 bit)
 *  - direction (1 bit)
 *  - speed (2 bits)
 *  - max packet size (2 bits: 8, 16, 32 or 64)
 *  - pipe type (2 bits: control, interrupt, bulk, isochronous)
 *
 * That's 18 bits. Really. Nothing more. And the USB people have
 * documented these eighteen bits as some kind of glorious
 * virtual data structure.
 *
 * Let's not fall in that trap. We'll just encode it as a simple
 * unsigned int. The encoding is:
 *
 *  - max size:		bits 0-1	(00 = 8, 01 = 16, 10 = 32, 11 = 64)
 *  - direction:	bit 7		(0 = Host-to-Device [Out],
 *					(1 = Device-to-Host [In])
 *  - device:		bits 8-14
 *  - endpoint:		bits 15-18
 *  - Data0/1:		bit 19
 *  - speed:		bit 26		(0 = Full, 1 = Low Speed, 2 = High)
 *  - pipe type:	bits 30-31	(00 = isochronous, 01 = interrupt,
 *					 10 = control, 11 = bulk)
 *
 * Why? Because it's arbitrary, and whatever encoding we select is really
 * up to us. This one happens to share a lot of bit positions with the UHCI
 * specification, so that much of the uhci driver can just mask the bits
 * appropriately.
 */
/* Create various pipes... */
#define create_pipe(dev,endpoint) \
		(((dev)->devnum << 8) | (endpoint << 15) | \
		((dev)->speed << 26) | (dev)->maxpacketsize)
#define default_pipe(dev) ((dev)->speed << 26)

#define usb_sndctrlpipe(dev, endpoint)	((PIPE_CONTROL << 30) | \
					 create_pipe(dev, endpoint))
#define usb_rcvctrlpipe(dev, endpoint)	((PIPE_CONTROL << 30) | \
					 create_pipe(dev, endpoint) | \
					 USB_DIR_IN)
#define usb_sndisocpipe(dev, endpoint)	((PIPE_ISOCHRONOUS << 30) | \
					 create_pipe(dev, endpoint))
#define usb_rcvisocpipe(dev, endpoint)	((PIPE_ISOCHRONOUS << 30) | \
					 create_pipe(dev, endpoint) | \
					 USB_DIR_IN)
#define usb_sndbulkpipe(dev, endpoint)	((PIPE_BULK << 30) | \
					 create_pipe(dev, endpoint))
#define usb_rcvbulkpipe(dev, endpoint)	((PIPE_BULK << 30) | \
					 create_pipe(dev, endpoint) | \
					 USB_DIR_IN)
#define usb_sndintpipe(dev, endpoint)	((PIPE_INTERRUPT << 30) | \
					 create_pipe(dev, endpoint))
#define usb_rcvintpipe(dev, endpoint)	((PIPE_INTERRUPT << 30) | \
					 create_pipe(dev, endpoint) | \
					 USB_DIR_IN)
#define usb_snddefctrl(dev)		((PIPE_CONTROL << 30) | \
					 default_pipe(dev))
#define usb_rcvdefctrl(dev)		((PIPE_CONTROL << 30) | \
					 default_pipe(dev) | \
					 USB_DIR_IN)

/* The D0/D1 toggle bits */
#define usb_gettoggle(dev, ep, out) (((dev)->toggle[out] >> ep) & 1)
#define usb_dotoggle(dev, ep, out)  ((dev)->toggle[out] ^= (1 << ep))
#define usb_settoggle(dev, ep, out, bit) ((dev)->toggle[out] = \
						((dev)->toggle[out] & \
						 ~(1 << ep)) | ((bit) << ep))

/* Endpoint halt control/status */
#define usb_endpoint_out(ep_dir)	(((ep_dir >> 7) & 1) ^ 1)
#define usb_endpoint_halt(dev, ep, out) ((dev)->halted[out] |= (1 << (ep)))
#define usb_endpoint_running(dev, ep, out) ((dev)->halted[out] &= ~(1 << (ep)))
#define usb_endpoint_halted(dev, ep, out) ((dev)->halted[out] & (1 << (ep)))

#define usb_packetid(pipe)	(((pipe) & USB_DIR_IN) ? USB_PID_IN : \
				 USB_PID_OUT)

#define usb_pipeout(pipe)	((((pipe) >> 7) & 1) ^ 1)
#define usb_pipein(pipe)	(((pipe) >> 7) & 1)
#define usb_pipedevice(pipe)	(((pipe) >> 8) & 0x7f)
#define usb_pipe_endpdev(pipe)	(((pipe) >> 8) & 0x7ff)
#define usb_pipeendpoint(pipe)	(((pipe) >> 15) & 0xf)
#define usb_pipedata(pipe)	(((pipe) >> 19) & 1)
#define usb_pipespeed(pipe)	(((pipe) >> 26) & 3)
#define usb_pipeslow(pipe)	(usb_pipespeed(pipe) == USB_SPEED_LOW)
#define usb_pipetype(pipe)	(((pipe) >> 30) & 3)
#define usb_pipeisoc(pipe)	(usb_pipetype((pipe)) == PIPE_ISOCHRONOUS)
#define usb_pipeint(pipe)	(usb_pipetype((pipe)) == PIPE_INTERRUPT)
#define usb_pipecontrol(pipe)	(usb_pipetype((pipe)) == PIPE_CONTROL)
#define usb_pipebulk(pipe)	(usb_pipetype((pipe)) == PIPE_BULK)

// host int status
typedef enum {
    IS_TRANS_DONE = 1,
    IS_RESUME = 2,
    IS_CONNECTION_EVENT = 4,
    IS_SOF_SENT = 8
}en_int_status;

// rx status
typedef enum {
    RS_CRC_ERROR = 0x01,
    RS_BIT_STUFF_ERROR = 0x02,
    RS_OVERFLOW = 0x04,
    RS_TIME_OUT = 0x08,
    RS_NAK_RXED = 0x10,
    RS_STALL_RXED = 0x20,
    RS_ACK_RXED = 0x40,
    RS_DATA_SEQUENCE = 0x80
}en_rx_status;

/* OpenCores usbhostslave API */
void ul_host_config(int conn_num);
void ul_host_bus_reset(int conn_num);
int ul_speed_configuration(int conn_num);
en_usb_status ul_control_setup_transaction(int conn_num,st_usb_device *dev,st_usb_setup_data *sd);
en_usb_status ul_control_in_transaction(int conn_num,st_usb_device *dev,int size, unsigned char *data);
en_usb_status ul_control_out_transaction(int conn_num,st_usb_device *dev,int size, unsigned char *data);
en_usb_status ul_control_ack_out_transaction(int conn_num,st_usb_device *dev);
en_usb_status ul_control_ack_in_transaction(int conn_num,st_usb_device *dev);
/* interrupt transfer(one or more transactions) */
en_usb_status ul_interrupt_in_transfer(int conn_num,st_usb_device *dev,int *size,unsigned char *data);
en_usb_status ul_interrupt_out_transfer(int conn_num,st_usb_device *dev,int size,unsigned char *data);
/* bulk transfer(one or more transactions) */
#ifdef USBH_USE_MSD
en_usb_status ul_bulk_out_transfer(int conn_num,st_usb_device *dev, int size, unsigned char* data);
en_usb_status ul_bulk_in_transfer(int conn_num,st_usb_device *dev, int size, unsigned char* data);
#endif
int wait_int_event(int conn_num,en_int_status s);
void full_speed_mode(int conn_num);
void hub_low_speed_mode(int conn_num);
#ifdef USE_MODIFIED_CORE
void ul_change_conn(int n);
#endif
#ifdef __cplusplus
}
#endif

#endif /* USBH_HW_H_ */
