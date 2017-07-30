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

#include "usbh_timer.h"
#include "usbh_hw.h"
#include "usbh_config.h"
#include "_usbh_def.h"
#include "usbh_ioreq.h"
#include "debug_printf.h"

//#define USB_DEBUG_CS
//#define USB_DEBUG_BO
//#define USB_DEBUG_BI
//#define USB_DEBUG_IO
//#define USB_DEBUG_II

extern USBH_HandleTypeDef hUsbHostFS;
USBH_HandleTypeDef *phost = &hUsbHostFS;
volatile static int usb_speed = SPEED_FULL;
static uint8_t pipe_io = 0xff;
static uint8_t pipe_ii = 0xff;
static uint8_t pipe_bo = 0xff;
static uint8_t pipe_bi = 0xff;

// PID
#define DATA0 0x03
#define DATA1 0x0b
#define ACK   0x02
#define STALL 0x0e
#define NAK   0x0a

static void set_data_toggle_bit(int conn_num, st_usb_device *dev, uint8_t ep, uint8_t toggle)
{
    if (ep & 0x80)
        dev->info.interface[dev->info.cur_interface].in_data_type = toggle;
    else
        dev->info.interface[dev->info.cur_interface].out_data_type = toggle;
}

static uint8_t get_data_toggle_bit(int conn_num, st_usb_device *dev, uint8_t ep)
{
    if (ep & 0x80)
        return (uint8_t)dev->info.interface[dev->info.cur_interface].in_data_type;
    else
        return (uint8_t)dev->info.interface[dev->info.cur_interface].out_data_type;
}

static void toggle_data_toggle_bit(int conn_num, st_usb_device *dev, uint8_t ep)
{
    uint8_t toggle;
    if (ep & 0x80) {
        toggle = dev->info.interface[dev->info.cur_interface].in_data_type;
        dev->info.interface[dev->info.cur_interface].in_data_type ^= toggle;
    } else {
        toggle = dev->info.interface[dev->info.cur_interface].out_data_type;
        dev->info.interface[dev->info.cur_interface].out_data_type ^= toggle;
    }
}

void ul_hw_device_setup(int conn_num, st_usb_device *dev, int adrs)
{
    /* this function is called from um_device_setup */
    uint8_t ep;
    uint8_t type;
    uint16_t max;
#ifdef USBH_DEBUG_DS_CB
    MPRINT("ul_hw_device_setup called\n");
#endif
    phost->device.address = (uint8_t)adrs;
    phost->device.is_connected = 1;
    switch(usb_speed) {
    case SPEED_FULL:
        phost->device.speed = USBH_SPEED_FULL;
        break;
    case SPEED_HI:
        phost->device.speed = USBH_SPEED_HIGH;
        break;
    default:
        phost->device.speed = USBH_SPEED_LOW;
        break;
    }
    USBH_OpenPipe(phost, phost->Control.pipe_in, 0x80, phost->device.address, phost->device.speed, USBH_EP_CONTROL, phost->Control.pipe_size);
    USBH_OpenPipe(phost, phost->Control.pipe_out, 0x00, phost->device.address, phost->device.speed, USBH_EP_CONTROL, phost->Control.pipe_size);
    ep = (uint8_t)(dev->info.interface[dev->info.cur_interface].ea_in | 0x80);
    set_data_toggle_bit(conn_num, dev, ep, 0);
    ep = (uint8_t)(dev->info.interface[dev->info.cur_interface].ea_out);
    set_data_toggle_bit(conn_num, dev, ep, 0);
    if (um_is_msd_bot(dev)) {
        type = USB_EP_TYPE_BULK;
        ep = (uint8_t)dev->info.interface[dev->info.cur_interface].ea_out;
        max = (uint16_t)(dev->info.interface[dev->info.cur_interface].ea_out_max_packet_size);
        pipe_bo = USBH_AllocPipe(phost, ep);
        USBH_OpenPipe(phost, pipe_bo, ep, (uint8_t)adrs, phost->device.speed, type, max);
        debug_printf("BO P:%d A:%d E:%x\r\n", pipe_bo, adrs, ep);
        ep = (uint8_t)(dev->info.interface[dev->info.cur_interface].ea_in | 0x80);
        max = (uint16_t)(dev->info.interface[dev->info.cur_interface].ea_in_max_packet_size);
        pipe_bi = USBH_AllocPipe(phost, ep);
        USBH_OpenPipe(phost, pipe_bi, ep, (uint8_t)adrs, phost->device.speed, type, max);
        debug_printf("BI P:%d A:%d E:%x\r\n", pipe_bi, adrs, ep);
        USBH_LL_SetToggle(phost, pipe_bo, 0);
        USBH_LL_SetToggle(phost, pipe_bi, 0);
    }
    if (um_is_hid(dev, dev->info.cur_interface)) {
        type = USB_EP_TYPE_INTR;
        max = (uint16_t)(dev->info.interface[dev->info.cur_interface].ea_out_max_packet_size);
        if (max != 0) {
            ep = (uint8_t)dev->info.interface[dev->info.cur_interface].ea_out;
            pipe_io = USBH_AllocPipe(phost, ep);
            USBH_OpenPipe(phost, pipe_io, ep, (uint8_t)adrs, phost->device.speed, type, max);
            debug_printf("IO P:%d A:%d E:%x\r\n", pipe_io, adrs, ep);
            USBH_LL_SetToggle(phost, pipe_io, 0);
        }
        max = (uint16_t)(dev->info.interface[dev->info.cur_interface].ea_in_max_packet_size);
        if (max != 0) {
            ep = (uint8_t)(dev->info.interface[dev->info.cur_interface].ea_in | 0x80);
            pipe_ii = USBH_AllocPipe(phost, ep);
            USBH_OpenPipe(phost, pipe_ii, ep, (uint8_t)adrs, phost->device.speed, type, max);
            debug_printf("II P:%d A:%d E:%x\r\n", pipe_ii, adrs, ep);
            USBH_LL_SetToggle(phost, pipe_ii, 0);
        }
    }
}

void ul_host_config(int conn_num)
{
    INFOPRINT("\nUSB Configuration connector %d...\n",conn_num);
}

void ul_host_bus_reset(int conn_num)
{
    phost->gState = HOST_IDLE;
    phost->EnumState = ENUM_IDLE;
    phost->RequestState = CMD_SEND;
    phost->Timer = 0;

    phost->Control.state = CTRL_SETUP;
    phost->Control.errorcount = 0;
    phost->device.address = 0;

    //phost->Control.pipe_size = 8;

    USBH_LL_ResetPort(phost);
    USBH_Delay(100);

    usb_speed = USBH_LL_GetSpeed(phost);
    switch(usb_speed) {
    case USBH_SPEED_FULL:
        //phost->Control.pipe_size = 64;
        //phost->device.speed = USBH_SPEED_FULL;
        usb_speed = SPEED_FULL;
        debug_printf("FULL SPEED\r\n");
        break;
        //case USBH_SPEED_HIGH:
        //    phost->Control.pipe_size = 64;
        //    phost->device.speed = USBH_SPEED_HIGH;
        //    usb_speed = SPEED_HI;
        //    debug_printf("HIGH SPEED\r\n");
        //    break;
    default:
        phost->Control.pipe_size = 8;
        phost->device.speed = USBH_SPEED_LOW;
        debug_printf("LOW SPEED\r\n");
        usb_speed = SPEED_LOW;
        break;
    }

    phost->Control.pipe_out = USBH_AllocPipe(phost, 0x00);
    phost->Control.pipe_in = USBH_AllocPipe(phost, 0x80);
    /* Open Control pipes */
    USBH_OpenPipe(phost, phost->Control.pipe_in, 0x80, phost->device.address, phost->device.speed,
    USBH_EP_CONTROL, phost->Control.pipe_size);

    /* Open Control pipes */
    USBH_OpenPipe(phost, phost->Control.pipe_out, 0x00, phost->device.address, phost->device.speed,
    USBH_EP_CONTROL, phost->Control.pipe_size);
}

int wait_int_event(int conn_num, en_int_status s)
{
    return USBH_ERROR;
}

int ul_speed_configuration(int conn_num)
{
    return usb_speed;
}

void full_speed_mode(int conn_num)
{
    usb_speed = SPEED_FULL;
}

void hub_low_speed_mode(int conn_num)
{
    usb_speed = SPEED_LOW;
}

en_usb_status ul_control_setup_transaction(int conn_num, st_usb_device *dev, st_usb_setup_data *sd)
{
    en_usb_status status = USBH_OK;
    USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
    USBH_LL_SetToggle(phost, phost->Control.pipe_out, 0);
    USBH_CtlSendSetup(phost, (uint8_t *)sd, phost->Control.pipe_out);
    do {
        URB_Status = USBH_LL_GetURBState(phost, phost->Control.pipe_out);
        if (URB_Status == USBH_URB_ERROR) {
            USBH_LL_Stop(phost);
            status = USBH_ERROR;
            break;
        }
        if (URB_Status == USBH_URB_STALL) {
            status = USBH_STALL;
            break;
        }
        if (URB_Status == USBH_URB_NOTREADY) {
            status = USBH_NAK;
            break;
        }
    } while (URB_Status == USBH_URB_IDLE);
#ifdef USB_DEBUG_CS
    if (status != USBH_OK)
    debug_printf("CS%d\r\n", status);
#endif
    return status;
}

en_usb_status ul_control_in_transaction(int conn_num, st_usb_device *dev, int size, unsigned char *data)
{
    en_usb_status status = USBH_OK;
    USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
    USBH_CtlReceiveData(phost, data, size, phost->Control.pipe_in);
    do {
        URB_Status = USBH_LL_GetURBState(phost, phost->Control.pipe_in);
        if (URB_Status == USBH_URB_ERROR) {
            USBH_LL_Stop(phost);
            status = USBH_ERROR;
            break;
        }
        if (URB_Status == USBH_URB_STALL) {
            status = USBH_STALL;
            break;
        }
        if (URB_Status == USBH_URB_NOTREADY) {
            status = USBH_NAK;
            break;
        }
    } while (URB_Status == USBH_URB_IDLE);
#ifdef USB_DEBUG_CS
    if (status != USBH_OK)
    debug_printf("CI%d\r\n", status);
#endif
    return status;
}

en_usb_status ul_control_out_transaction(int conn_num, st_usb_device *dev, int size, unsigned char *data)
{
    en_usb_status status = USBH_OK;
    USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
    USBH_CtlSendData(phost, data, size, phost->Control.pipe_out, 1);
    do {
        URB_Status = USBH_LL_GetURBState(phost, phost->Control.pipe_out);
        if (URB_Status == USBH_URB_ERROR) {
            USBH_LL_Stop(phost);
            status = USBH_ERROR;
            break;
        }
        if (URB_Status == USBH_URB_STALL) {
            status = USBH_STALL;
            break;
        }
        if (URB_Status == USBH_URB_NOTREADY) {
            status = USBH_NAK;
            break;
        }
    } while (URB_Status == USBH_URB_IDLE);
#ifdef USB_DEBUG_CS
    if (status != USBH_OK)
    debug_printf("CO%d\r\n", status);
#endif
    return status;
}

en_usb_status ul_control_ack_out_transaction(int conn_num, st_usb_device *dev)
{
    en_usb_status status = USBH_OK;
    USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
    USBH_LL_SetToggle(phost, phost->Control.pipe_out, 1);
    USBH_CtlSendData(phost, 0, 0, phost->Control.pipe_out, 1);
    do {
        URB_Status = USBH_LL_GetURBState(phost, phost->Control.pipe_out);
        if (URB_Status == USBH_URB_ERROR) {
            USBH_LL_Stop(phost);
            status = USBH_ERROR;
            break;
        }
        if (URB_Status == USBH_URB_STALL) {
            status = USBH_STALL;
            break;
        }
        if (URB_Status == USBH_URB_NOTREADY) {
            status = USBH_NAK;
            break;
        }
    } while (URB_Status == USBH_URB_IDLE);
#ifdef USB_DEBUG_CS
    if (status != USBH_OK)
    debug_printf("AO%d\r\n", status);
#endif
    return status;
}

en_usb_status ul_control_ack_in_transaction(int conn_num, st_usb_device *dev)
{
    en_usb_status status = USBH_OK;
    USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
    USBH_LL_SetToggle(phost, phost->Control.pipe_in, 1);
    USBH_CtlReceiveData(phost, 0, 0, phost->Control.pipe_in);
    do {
        URB_Status = USBH_LL_GetURBState(phost, phost->Control.pipe_in);
        if (URB_Status == USBH_URB_ERROR) {
            USBH_LL_Stop(phost);
            status = USBH_ERROR;
            break;
        }
        if (URB_Status == USBH_URB_STALL) {
            status = USBH_STALL;
            break;
        }
        if (URB_Status == USBH_URB_NOTREADY) {
            status = USBH_NAK;
            break;
        }
    } while (URB_Status == USBH_URB_IDLE);
#ifdef USB_DEBUG_CS
    if (status != USBH_OK)
    debug_printf("AI%d\r\n", status);
#endif
    return status;
}

//USBH_OK = 0x00,
//USBH_TIMEOUT = 0x08,
//USBH_NAK = 0x10,
//USBH_STALL = 0x20,
//USBH_ERROR = 0xff
// interrupt transfer(one or more transactions)
en_usb_status ul_interrupt_in_transfer(int conn_num, st_usb_device *dev, int *size, unsigned char *data)
{
    int timeout;
    en_usb_status status = USBH_OK;
    USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
    HCD_HCStateTypeDef state = -1;
    uint8_t ep = dev->info.interface[dev->info.cur_interface].ea_in | 0x80;
    uint16_t max = (int)(dev->info.interface[dev->info.cur_interface].ea_in_max_packet_size);
    //USBH_OpenPipe(phost, pipe_ii, ep, (uint8_t) (dev->info.address), phost->device.speed, USB_EP_TYPE_INTR, max);
    //USBH_LL_SetToggle(phost, pipe_ii, get_data_toggle_bit(conn_num, dev, ep));
    uint16_t len = (uint16_t)*size;
    uint16_t done = 0;
    int nak_count = 0;
    len = (max > (uint16_t)*size - done ? (uint16_t)*size - done : max);
    USBH_InterruptReceiveData(phost, (uint8_t *)(data + done), (uint8_t)len, (uint8_t)pipe_ii);
    while ((URB_Status = USBH_LL_GetURBState(phost, pipe_ii)) == URB_IDLE)
        ;
    len = 0;
    switch((int)URB_Status) {
    case URB_DONE:
        state = HAL_HCD_HC_GetState((HCD_HandleTypeDef *)phost->pData, pipe_ii);
        switch((int)state) {
        case HC_XFRC:
            len = HAL_HCD_HC_GetXferCount((HCD_HandleTypeDef *)phost->pData, pipe_ii);
            //toggle_data_toggle_bit(conn_num, dev, ep);
            break;
        case HC_NAK:
            status = USBH_NAK;
            break;
        }
        break;
    default:
        status = USBH_STALL;
        break;
    }
    done += len;
    *size = (int)done;
#ifdef USB_DEBUG_II
    debug_printf("II%d P:%d A:%d E:%x S:%d\r\n", len, pipe_ii, dev->info.address, ep, state);
    if (status != USBH_OK)
    debug_printf("II(%d) ret=%d\r\n", len, status);
#endif
    //if (status != USBH_NAK)
    //    debug_printf("II(%d) ret=%d\r\n", len, status);
    return status;
}

en_usb_status ul_interrupt_out_transfer(int conn_num, st_usb_device *dev, int size, unsigned char* data)
{
    int timeout;
    en_usb_status status = USBH_OK;
    USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
    uint8_t ep = dev->info.interface[dev->info.cur_interface].ea_out;
    uint16_t max = (int)(dev->info.interface[dev->info.cur_interface].ea_out_max_packet_size);
    USBH_OpenPipe(phost, pipe_io, ep, (uint8_t)(dev->info.address), phost->device.speed, USB_EP_TYPE_INTR, max);
    USBH_LL_SetToggle(phost, pipe_io, 0);
#ifdef USB_DEBUG_IO
    debug_printf("IO%d P:%d A:%d E:%x\r\n", size, pipe_io, dev->info.address, ep);
#endif
    uint16_t done = 0;
    uint16_t len = 0;
    int nak_count = 0;
    while (done < size) {
        len = (max > (uint16_t)size - done ? (uint16_t)size - done : max);
        timeout = 2000;
        USBH_InterruptSendData(phost, data + (int)done, len, pipe_io);
        do {
            URB_Status = USBH_LL_GetURBState(phost, pipe_io);
            if (URB_Status == USBH_URB_DONE)
                break;
            if (URB_Status == USBH_URB_ERROR) {
                status = USBH_ERROR;
                goto ul_interrupt_out_transfer_exit;
            }
            if (URB_Status == USBH_URB_STALL) {
                status = USBH_STALL;
                goto ul_interrupt_out_transfer_exit;
            }
            if (timeout-- == 0) {
                status = USBH_TIMEOUT;
                goto ul_interrupt_out_transfer_exit;
            }
            HAL_Delay(2);
        } while (URB_Status == USBH_URB_IDLE);
        done += len;
    }
    ul_interrupt_out_transfer_exit:
#ifdef USB_DEBUG_IO
    if (status != USBH_OK)
    debug_printf("IO(%d) ret=%d\r\n", done, status);
#endif
    return status;
}

#ifdef USBH_USE_MSD
// bulk transfer(one or more transactions)
en_usb_status ul_bulk_out_transfer(int conn_num, st_usb_device *dev, int size, unsigned char* data)
{
    int timeout;
    en_usb_status status = USBH_OK;
    USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
    uint8_t ep = dev->info.interface[dev->info.cur_interface].ea_out;
    uint16_t max = (int)(dev->info.interface[dev->info.cur_interface].ea_out_max_packet_size);
    //USBH_OpenPipe(phost, pipe_bo, ep, (uint8_t) (dev->info.address), phost->device.speed, USB_EP_TYPE_BULK, max);
    //USBH_LL_SetToggle(phost, pipe_bo, 0);
#ifdef USB_DEBUG_BO
    debug_printf("BO%d P:%d A:%d E:%x\r\n", size, pipe_bo, dev->info.address, ep);
#endif
    uint16_t done = 0;
    uint16_t len = 0;
    int nak_count = 0;
    while (done < size) {
        len = (max > (uint16_t)size - done ? (uint16_t)size - done : max);
        ul_bulk_out_transfer_0: timeout = 2000;
        USBH_BulkSendData(phost, data + (int)done, len, pipe_bo, 1);
        do {
            URB_Status = USBH_LL_GetURBState(phost, pipe_bo);
            if (URB_Status == USBH_URB_DONE)
                break;
            if (URB_Status == USBH_URB_ERROR) {
                status = USBH_ERROR;
                goto ul_bulk_out_transfer_exit;
            }
            if (URB_Status == USBH_URB_STALL) {
                status = USBH_STALL;
                goto ul_bulk_out_transfer_exit;
            }
            //if (URB_Status == USBH_URB_NOTREADY)  {
            //    nak_count++;
            //    if (nak_count >= 500) {
            //        status = USBH_NAK;
            //        goto ul_bulk_out_transfer_exit;
            //    }
            //    HAL_Delay(5);
            //    goto ul_bulk_out_transfer_0;
            //}
            if (timeout-- == 0) {
                status = USBH_TIMEOUT;
                goto ul_bulk_out_transfer_exit;
            }
            HAL_Delay(2);
        } while (URB_Status == USBH_URB_IDLE);
        done += len;
    }
    ul_bulk_out_transfer_exit:
#ifdef USB_DEBUG_BO
    if (status != USBH_OK)
    debug_printf("BO(%d) ret=%d\r\n", done, status);
#endif
    return status;
}

en_usb_status ul_bulk_in_transfer(int conn_num, st_usb_device *dev, int size, unsigned char* data)
{
    int timeout;
    en_usb_status status = USBH_OK;
    USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
    uint8_t ep = (dev->info.interface[dev->info.cur_interface].ea_in | 0x80);
    uint16_t max = (int)(dev->info.interface[dev->info.cur_interface].ea_in_max_packet_size);
    uint16_t done = 0;
    //USBH_OpenPipe(phost, pipe_bi, ep, (uint8_t) (dev->info.address), phost->device.speed, USB_EP_TYPE_BULK, max);
    //USBH_LL_SetToggle(phost, pipe_bi, 0);
    uint16_t len = 0;
    int nak_count = 0;
    while (done < size) {
        len = (max > (uint16_t)size - done ? (uint16_t)size - done : max);
        ul_bulk_in_transfer_0: timeout = 2000;
        USBH_BulkReceiveData(phost, data + (int)done, len, pipe_bi);
        do {
            URB_Status = USBH_LL_GetURBState(phost, pipe_bi);
            if (URB_Status == USBH_URB_DONE) {
                break;
            }
            if (URB_Status == USBH_URB_ERROR) {
                status = USBH_ERROR;
                goto ul_bulk_in_transfer_exit;
            }
            if (URB_Status == USBH_URB_STALL) {
                status = USBH_STALL;
                goto ul_bulk_in_transfer_exit;
            }
            //if (URB_Status == USBH_URB_NOTREADY) {
            //    nak_count++;
            //    if (nak_count >= 500) {
            //        status = USBH_NAK;
            //        goto ul_bulk_in_transfer_exit;
            //    }
            //    HAL_Delay(5);
            //    goto ul_bulk_in_transfer_0;
            //}
            if (timeout-- == 0) {
                status = USBH_TIMEOUT;
                goto ul_bulk_in_transfer_exit;
            }
            HAL_Delay(2);
        } while (URB_Status == USBH_URB_IDLE);
        done += len;
    }
    ul_bulk_in_transfer_exit:
#ifdef USB_DEBUG_BI
    debug_printf("BI%d P:%d A:%d E:%x\r\n", size, pipe_bi, dev->info.address, ep);
    if (status != USBH_OK)
    debug_printf("BI(%d) ret=%d\r\n", done, status);
#endif
    return status;
}

#endif

void ul_hw_init()
{
    stm32l4_init();
    ul_host_config(0);
    ul_host_bus_reset(0);
}
