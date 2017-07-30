//
// Copyright (c) 2014, Kentaro Sekimoto
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

#include <stdio.h>
#include "usbh_hw_dep_include.h"
#include "usbh_env.h"

#define ENDP_ATTR_CONT  0
#define ENDP_ATTR_ISO   1
#define ENDP_ATTR_BULK  2
#define ENDP_ATTR_INTR  3

#define HID_PROTO_NO 0
#define HID_PROTO_KEYBOARD 1
#define HID_PROTO_MOUSE 2

extern st_usbh_env usbh_env;
static int conn_num;
static st_usb_device *dev;
static found = 0;
#define BUF_MAX 256
static unsigned char buf[BUF_MAX];

void print_speed(int conn_num)
{
    int speed = ul_speed_configuration(conn_num);
    MPRINT("Speed: ");
    switch (speed) {
    case 0:
        MPRINT("Low\n");
        break;
    case 1:
        MPRINT("Full\n");
        break;
    case 2:
        MPRINT("High\n");
        break;
    default:
        MPRINT("Not detected\n");
        break;
    }
}

void print_desc_string(int conn_num, st_usb_device *dev, unsigned char *name, int index)
{
    int i;
    st_usb_setup_data sd;
    en_usb_status status = USBH_OK;
    sd.BYTE.bmRequestType = 0x80;
    sd.BYTE.bRequest     = 6;
    sd.BYTE.wValue[0] = 0;      // descriptor index (0: lang id)
    sd.BYTE.wValue[1] = 3;      // descriptor type (3: string)
    sd.BYTE.wIndex[0] = 0;      // language id or 0
    sd.BYTE.wIndex[1] = 0;
    sd.BYTE.wLength[0] = 4;
    sd.BYTE.wLength[1] = 0;
    status = um_control_in_transfer(conn_num, dev, &sd, buf);
    sd.BYTE.wIndex[0] = buf[2]; /* Language ID */
    sd.BYTE.wIndex[1] = buf[3];
    if (dev->descs.device.PACKED[index] != 0) {
        sd.BYTE.wValue[0] = (unsigned char)dev->descs.device.PACKED[index];
        sd.BYTE.wValue[1] = 3;      // descriptor type (3: string)
        status = um_control_in_transfer(conn_num, dev, &sd, buf);
        sd.BYTE.wLength[0] = buf[0]; /* actual size */
        status = um_control_in_transfer(conn_num, dev, &sd, buf);
        MPRINT("%s: ", name);
        for (i = 2; i < buf[0]; i += 2)
            MPRINT("%c",buf[i]);  // UNICODE characters
        MPRINT("\n");
    } else {
        MPRINT("%s: Not specified\n", name);
    }
}

unsigned short get_vendor(int conn_num, st_usb_device *dev)
{
    return *(unsigned short *)&dev->descs.device.BYTE.idVendor[0];
}

unsigned short get_product(int conn_num, st_usb_device *dev)
{
    return *(unsigned short *)&dev->descs.device.BYTE.idProduct[0];
}

void ep_enum(int conn_cum, st_usb_device *dev, int type)
{
    int i, j;
    st_endpoint_descriptor *endp;
    unsigned char epaddr;
    unsigned short size;

    for (j = 0; j < dev->descs.config.BYTE.bNumInterfaces; j++) {
        for (i = 0; i < dev->descs.interface[j].BYTE.bNumEndpoints; i++) {
            endp = &dev->descs.interface[j].endp[i];
            if ((endp->BYTE.bmAttributes & 0x3) == type) {
                epaddr = (unsigned char)endp->BYTE.bEndpointAddress;
                size = (unsigned short)endp->BYTE.wMaxPacketSize[0] + ((unsigned short)endp->BYTE.wMaxPacketSize[1] << 8);
                if (epaddr & 0x80) {
                    /* IN Endpoint */
                    MPRINT("EP In : %02x Size: %04d\n", epaddr, size);
                } else {
                    /* OUT Endpoint */
                    MPRINT("EP Out: %02x Size: %04d\n", epaddr, size);
                }
            }
        }
    }
}

void hid_enum(int conn_num, st_usb_device *dev)
{
    unsigned char pt;
    int ip;
    int i;
    for (i = 0; i < dev->descs.config.BYTE.bNumInterfaces; i++) {
        if (um_is_hid(dev, i)) {
            ip = (int)dev->descs.interface[i].BYTE.bInterfaceProtocol;
            switch (ip) {
            case HID_PROTO_NO:
                if (um_is_ps3_gc(dev)) {
                    MPRINT("Device (%02x): PS3 gamepad\n", ip);
                } else {
                    MPRINT("Device (%02x): Unknown\n", ip);
                }
                break;
            case HID_PROTO_KEYBOARD:
                MPRINT("Device (%02x): Keyboard\n", ip);
                break;
            case HID_PROTO_MOUSE:
                MPRINT("Device (%02x): Mouse\n", ip);
                break;
            default:
                MPRINT("Device (%02x): Unknown\n", ip);
                break;
            }
        }
    }
}

void interface_enum(int conn_num, st_usb_device *dev)
{
    int ic = (int)(dev->descs.interface[dev->info.cur_interface].BYTE.bInterfaceClass);
    switch ((ic)) {
    case UCC_AUDIO:
        MPRINT("Device Class (%02x): Audio\n", ic);
        hid_enum(conn_num, dev);
        break;
    case UCC_CDC_CONTROL:
        MPRINT("Device Class (%02x): CDC Control\n", ic);
        break;
    case UCC_HID:
        MPRINT("Device Class (%02x): HID\n", ic);
        hid_enum(conn_num, dev);
        break;
    case UCC_PHYSICAL:
        MPRINT("Device Class (%02x): Physical\n", ic);
        break;
    case UCC_IMAGE:
        MPRINT("Device Class (%02x): Image\n", ic);
        break;
    case UCC_PRINTER:
        MPRINT("Device Class (%02x): Printer\n", ic);
        break;
    case UCC_MASS_STORAGE:
        MPRINT("Device Class (%02x): Mass Storage\n", ic);
        break;
    case UCC_HUB:
        MPRINT("Device Class (%02x): HUB\n", ic);
        break;
    case UCC_CDC_DATA:
        MPRINT("Device Class (%02x): CDC Data\n", ic);
        break;
    case UCC_SMART_CARD:
        MPRINT("Device Class (%02x): Smart Card\n", ic);
        break;
    case UCC_CONTENT_SECURITY:
        MPRINT("Device Class (%02x): Content Security\n", ic);
        break;
    case UCC_VIDEO:
        MPRINT("Device Class (%02x): Video\n", ic);
        break;
    case UCC_HEALTH_CARE:
        MPRINT("Device Class (%02x): Health Care\n", ic);
        break;
    case UCC_AUDIO_VIDEO:
        MPRINT("Device Class (%02x): Audio Video\n", ic);
        break;
    case UCC_BILLBOARD :
        MPRINT("Device Class (%02x): Billboard\n", ic);
        break;
    case UCC_USB_TYPE_C:
        MPRINT("Device Class (%02x): USB Type C\n", ic);
        break;
    case UCC_DIAGNOSTIC:
        MPRINT("Device Class (%02x): Diagostic\n", ic);
        break;
    case UCC_WIRELESS:
        MPRINT("Device Class (%02x): Wireless\n", ic);
        break;
    case UCC_MISC:
        MPRINT("Device Class (%02x): MISC\n", ic);
        break;
    case UCC_APPLICATION:
        MPRINT("Device Class (%02x): Application\n", ic);
        break;
    case UCC_VENDOR:
        MPRINT("Device Class (%02x): Vendor\n", ic);
        break;
    default:
        MPRINT("Device Class (%02x): Unknown\n", ic);
        break;
    }
}

int main()
{
    uh_init();

    MPRINT("\n===== Simple USB Device Check =====\n");

    conn_num = 0;
    dev = usbh_env.conn[conn_num].p_root;
    dev->info.cur_interface = 0;
    print_desc_string(conn_num, dev, "iManufacturer", 14);
    print_desc_string(conn_num, dev, "iProduct", 15);
    print_speed(conn_num);
    interface_enum(conn_num, dev);
    MPRINT("VID:%04x\n", get_vendor(conn_num, dev));
    MPRINT("PID:%04x\n", get_product(conn_num, dev));
    MPRINT("ISO Transfer\n");
    ep_enum(conn_num, dev, (int)ENDP_ATTR_ISO);
    MPRINT("Bulk Transfer\n");
    ep_enum(conn_num, dev, (int)ENDP_ATTR_BULK);
    MPRINT("Interrupt Transfer\n");
    ep_enum(conn_num, dev, (int)ENDP_ATTR_INTR);

    MPRINT("===================================\n");

    while (1) {}
}

