# UsbHLib

## Overview
This UsbHLib repository is based on the following site.
- [usbhlib_top](https://www.kice.tokyo/open-source/usbhlib_top.html, "UsbHLib")

## UsbHLib original readme.txt

USB Host C Library
 release 2013/10/31 version 0.659
  - PS3 Game Controller tested
  - PS/PS2 Game Converter tested

 release 2013/11/08 version 0.667
  - Error handling during hub port device enumeration.
    After the port reset, "CurrentConnectStatus" changes
    from "A device is present on this port" to "No device is present"
    even though the device is still attached.

 release 2014/03/07 version 0.670
  - GetDescriptor(STRING) bugfix
    initialize wLength=4 before getting iProduct string.

 release 2014/09/18 version 0.700
  - String Descriptor UNICODE character display bugfix.
  - add ul_hw_init function for hardware-dependent initialization.
  - add ul_hw_device_setup functon, which is called from um_device_setup.
    This is for hardware configuration depending on the endpoint type.
  - add callback functions. these are called when a device is attached
    to the HUB, or detached from the HUB.
    the callback functions are enabled if USBH_USE_HUB_CALLBACK is defined.
  - config_sample/microblaze is deleted.
  - hw_dep sub-directory structure is changed.

 release 2015/03/08 version 0.703
  - Mass-Storage device setup modification.
    Only the first pair of Bulk-In and Bulk-Out Endpoints is used.
    Other Endpoint types(Interrupt and Isochronous) are ignored.
    
