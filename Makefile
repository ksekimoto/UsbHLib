#######################################################################
# Makefile for UsbHLib
#
# Supports 
#   Fujitsu FM3/FM4
#   Renesas RX62N/RX63N/RX64M/RZA1H
#   ST Micro STM32F4, STM32L4
#
# Need to configure
#	TARGET: 
#   LINC: gcc library directory
#   GCCDIR: gcc executable directory
#######################################################################

#######################################################################
# debug flag
#######################################################################
ifndef OPTIMIZE
OPTIMIZE = 0
#OPTIMIZE = 1
endif

#######################################################################
#  Select minimum modules
#######################################################################
# comment for selecting all modules
# Uncomment for selecting minimum modules
#MIM_MODULE = 1

#######################################################################
# Target application
#######################################################################
ifndef TARGET
#TARGET = simple_gamepad
#TARGET = simple_keyboard
#TARGET = simple_mouse
#TARGET = simple_msd_bot
#TARGET = simple_bp3595
TARGET = simple_usb_check
endif

#######################################################################
# Build directory
#######################################################################
ifndef BUILD_DIR
BUILD_DIR = build
endif

#######################################################################
# Tools
#######################################################################
MKDIR = bin/mkdir
SED = bin/sed

#######################################################################
# CPU configuration
#######################################################################
ifndef CONFIG_CPU
#CONFIG_CPU = FM3
#CONFIG_CPU = FM4
#CONFIG_CPU = RX62N
#CONFIG_CPU = RX63N
#CONFIG_CPU = RX64M
#CONFIG_CPU = RZA1H
CONFIG_CPU = SH2A
#CONFIG_CPU = STM32F4
#CONFIG_CPU = STM32L4
endif

ifeq ($(CONFIG_CPU),FM3)
#MB9BF506N = 1
MB9BF618T = 1
BUILD = $(BUILD_DIR)/fm3
endif

ifeq ($(CONFIG_CPU),FM4)
MB9BF568R = 1
BUILD = $(BUILD_DIR)/fm4
endif

ifeq ($(CONFIG_CPU),RX62N)
RX62N = 1
BUILD = $(BUILD_DIR)/rx62n
endif

ifeq ($(CONFIG_CPU),RX63N)
RX63N = 1
BUILD = $(BUILD_DIR)/rx63n
endif

ifeq ($(CONFIG_CPU),RX64M)
RX64M = 1
BUILD = $(BUILD_DIR)/rx64m
endif

ifeq ($(CONFIG_CPU),RZA1H)
RZA1H = 1
BUILD = $(BUILD_DIR)/rza1h
endif

ifeq ($(CONFIG_CPU),SH2A)
SH2A = 1
BUILD = $(BUILD_DIR)/sh2a
endif

##### STM32F4

ifeq ($(CONFIG_CPU),STM32F4)

STM32F4 = 1
BUILD = $(BUILD_DIR)/stm32f4

ifndef BOARD
#BOARD = NUCLEO_F401RE
BOARD = NUCLEO_F446RE
endif

ifeq ($(BOARD),NUCLEO_F401RE)
DEVICE = STM32F401xE
endif
ifeq ($(BOARD),NUCLEO_F446RE)
DEVICE = STM32F446xx
endif

endif

##### STM32L4

ifeq ($(CONFIG_CPU),STM32L4)

STM32L4 = 1
BUILD = $(BUILD_DIR)/stm32l4

ifndef BOARD
BOARD = NUCLEO_L476RG
endif

ifeq ($(BOARD),NUCLEO_L476RG)
DEVICE = STM32L476xx
endif

endif

#######################################################################
# Define programs and commands.
#######################################################################
ifneq (,$(findstring $(CONFIG_CPU),FM3))
#GCCDIR=c:/cross/arm-2010q1-188/bin
#GCCVER=4.4.1
#GCCLIB=thumb2
GCCDIR=C:/cross/gcc-arm-none-eabi-4_9-2014q4/bin
GCCDIR=C:/cross/gcc-arm-none-eabi-4_9-2014q4/bin
GCCVER=4.9.3
GCCLIB=thumb2
#GCCDIR=C:/cross/arm-2013.11-24/bin
#GCCVER=4.8.1
#GCCLIB=thumn2
#GCCDIR=C:/cross/arm-2013q4/bin
#GCCVER=4.8.3
#GCCLIB=armv7-m
CC = $(GCCDIR)/arm-none-eabi-gcc
CPP = $(GCCDIR)/arm-none-eabi-g++
LD = $(GCCDIR)/arm-none-eabi-ld
AS = $(GCCDIR)/arm-none-eabi-as
OBJCOPY = $(GCCDIR)/arm-none-eabi-objcopy
OBJDUMP = $(GCCDIR)/arm-none-eabi-objdump
SIZE = $(GCCDIR)/arm-none-eabi-size
NM = $(GCCDIR)/arm-none-eabi-nm
REMOVE = bin/rm
CPUTYPE = cortex-m3 -mthumb
endif

ifneq (,$(findstring $(CONFIG_CPU),RX62N RX63N RX64M))
GCCDIR=C:/cross/rx-elf-gcc-4.9.4-m/bin
GCCVER=4.9.4
#GCCDIR=C:/cross/kpit_gcc_rx_1203/bin
#GCCVER=4.7-GNURX_v12.03
#GCCDIR=C:/cross/kpit_gcc_rx_1501/bin
#GCCVER=4.8-GNURX_v15.01
#GCCDIR=C:/cross/kpit_gcc_rx_1600/bin
#GCCVER=4.8.0.201602-GNURX
CC = $(GCCDIR)/rx-elf-gcc
CPP = $(GCCDIR)/rx-elf-g++
LD = $(GCCDIR)/rx-elf-ld
AS = $(GCCDIR)/rx-elf-as
OBJCOPY = $(GCCDIR)/rx-elf-objcopy
OBJDUMP = $(GCCDIR)/rx-elf-objdump
SIZE = $(GCCDIR)/rx-elf-size
NM = $(GCCDIR)/rx-elf-nm
#REMOVE = $(GCCDIR)/rm -f
REMOVE = bin/rm
endif

ifneq (,$(findstring $(CONFIG_CPU),RZA1H))
ifdef KPIT
GCCDIR=C:/cross/arm-rz-eabi/bin
GCCVER=4.8-GNUARM-RZ_v14.01
CC = $(GCCDIR)/arm-rz-eabi-gcc
CPP = $(GCCDIR)/arm-rz-eabi-g++
LD = $(GCCDIR)/arm-rz-eabi-ld
AS = $(GCCDIR)/arm-rz-eabi-as
OBJCOPY = $(GCCDIR)/arm-rz-eabi-objcopy
OBJDUMP = $(GCCDIR)/arm-rz-eabi-objdump
SIZE = $(GCCDIR)/arm-rz-eabi-size
NM = $(GCCDIR)/arm-rz-eabi-nm
REMOVE = bin/rm
MAKE = bin/make
CPU = -mcpu=cortex-a9 -mthumb -mthumb-interwork -marm -march=armv7-a -mfpu=vfpv3 -mfloat-abi=hard -mno-unaligned-access 
else
GCCDIR=C:/cross/gcc-arm-none-eabi-4_9-2014q4/bin
GCCVER=4.9.3
GCCLIB=
CC = $(GCCDIR)/arm-none-eabi-gcc
CPP = $(GCCDIR)/arm-none-eabi-g++
LD = $(GCCDIR)/arm-none-eabi-ld
AS = $(GCCDIR)/arm-none-eabi-as
OBJCOPY = $(GCCDIR)/arm-none-eabi-objcopy
OBJDUMP = $(GCCDIR)/arm-none-eabi-objdump
SIZE = $(GCCDIR)/arm-none-eabi-size
NM = $(GCCDIR)/arm-none-eabi-nm
REMOVE = bin/rm
#MAKE = bin/make
CPU = -mcpu=cortex-a9 -mthumb -mthumb-interwork -marm -march=armv7-a -mfpu=neon -mfloat-abi=softfp -mno-unaligned-access -mstructure-size-boundary=8
endif
endif

ifneq (,$(findstring $(CONFIG_CPU),SH2 SH2A))
GCCDIR=C:/cross/sh-elf-gcc-4.9.4-m/bin
GCCVER=4.9.4
#GCCDIR=C:/cross/kpit_gcc_sh_1202/bin
#GCCVER=4.7-GNUSH_v12.02
CC = $(GCCDIR)/sh-elf-gcc
CPP = $(GCCDIR)/sh-elf-g++
LD = $(GCCDIR)/sh-elf-ld
AS = $(GCCDIR)/sh-elf-as
OBJCOPY = $(GCCDIR)/sh-elf-objcopy
OBJDUMP = $(GCCDIR)/sh-elf-objdump
SIZE = $(GCCDIR)/sh-elf-size
NM = $(GCCDIR)/sh-elf-nm
#REMOVE = $(GCCDIR)/rm -f
REMOVE = bin/rm
endif

ifneq (,$(findstring $(CONFIG_CPU),STM32F4 STM32L4 FM4))
GCCDIR=C:/cross/gcc-arm-none-eabi-4_9-2014q4/bin
GCCVER=4.9.3
GCCLIB=
CC = $(GCCDIR)/arm-none-eabi-gcc
CPP = $(GCCDIR)/arm-none-eabi-g++
LD = $(GCCDIR)/arm-none-eabi-ld
AS = $(GCCDIR)/arm-none-eabi-as
OBJCOPY = $(GCCDIR)/arm-none-eabi-objcopy
OBJDUMP = $(GCCDIR)/arm-none-eabi-objdump
SIZE = $(GCCDIR)/arm-none-eabi-size
NM = $(GCCDIR)/arm-none-eabi-nm
REMOVE = bin/rm
#MAKE = bin/make
#CPU = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mno-unaligned-access -mstructure-size-boundary=8
CPUTYPE = cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
endif

#######################################################################
# C header include path
#######################################################################
CDEFS =

CINCS = .
CINCS += core
CINCS += config_sample/$(TARGET)
CINCS += hw_dep/common
ifeq ($(CONFIG_CPU),FM3)
CINCS += hw_dep/fm3
CINCS += hw_dep/fm3/common
endif
ifeq ($(CONFIG_CPU),FM4)
CINCS += hw_dep/fm4
CINCS += hw_dep/fm4/common
endif
ifeq ($(CONFIG_CPU),RX62N)
CINCS += hw_dep/rx62n
endif
ifeq ($(CONFIG_CPU),RX63N)
CINCS += hw_dep/rx63n
endif
ifeq ($(CONFIG_CPU),RX64M)
CINCS += hw_dep/rx64m
endif
ifeq ($(CONFIG_CPU),RZA1H)
CINCS += hw_dep/rza1h
CINCS += hw_dep/rza1h/cmsis
CINCS += hw_dep/rza1h/cmsis/TARGET_RENESAS/TARGET_RZ_A1H/
CINCS += hw_dep/rza1h/cmsis/TARGET_RENESAS/TARGET_RZ_A1H/inc
CINCS += hw_dep/rza1h/cmsis/TARGET_RENESAS/TARGET_RZ_A1H/inc/iobitmasks
CINCS += hw_dep/rza1h/cmsis/TARGET_RENESAS/TARGET_RZ_A1H/inc/iodefines
CINCS += hw_dep/TARGET_RZ_A1H/usb1
endif
ifeq ($(CONFIG_CPU),SH2A)
CINCS += hw_dep/sh2a/
endif
ifeq ($(CONFIG_CPU),STM32F4)
CINCS += hw_dep/stm32f4/
CINCS += hw_dep/stm32f4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
CINCS += hw_dep/stm32f4/Drivers/CMSIS/Include
CINCS += hw_dep/stm32f4/Drivers/STM32F4xx_HAL_Driver/Inc
CINCS += hw_dep/stm32f4/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
#CINCS += hw_dep/stm32f4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
CINCS += hw_dep/stm32f4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
#CINCS += hw_dep/stm32f4/Middlewares/Third_Party/FatFs/src
#CINCS += hw_dep/stm32f4/Middlewares/Third_Party/FatFs/src/drivers
CINCS += hw_dep/stm32f4/Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Inc
CINCS += hw_dep/stm32f4/Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc
CINCS += hw_dep/stm32f4/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc
CINCS += hw_dep/stm32f4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
CINCS += hw_dep/stm32f4/Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Inc
endif
ifeq ($(CONFIG_CPU),STM32L4)
CINCS += hw_dep/stm32l4/
CINCS += hw_dep/stm32l4/Drivers/CMSIS/Device/ST/STM32L4xx/Include
CINCS += hw_dep/stm32l4/Drivers/CMSIS/Include
CINCS += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Inc
CINCS += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy
#CINCS += hw_dep/stm32l4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
CINCS += hw_dep/stm32l4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
#CINCS += hw_dep/stm32l4/Middlewares/Third_Party/FatFs/src
#CINCS += hw_dep/stm32l4/Middlewares/Third_Party/FatFs/src/drivers
CINCS += hw_dep/stm32l4/Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Inc
CINCS += hw_dep/stm32l4/Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc
CINCS += hw_dep/stm32l4/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc
CINCS += hw_dep/stm32l4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
CINCS += hw_dep/stm32l4/Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Inc
endif

ifeq ($(TARGET),simple_bp3595)
CINCS += example/ping/
endif
#######################################################################
# C flags.
#######################################################################

CFLAGS = -g
CFLAGS += -MMD -MP
CFLAGS += $(CDEFS)
ifeq ($(OPTIMIZE),1)
CFLAGS += -Os
else
CFLAGS += -O0
endif
CFLAGS += $(patsubst %,-I%,$(CINCS))
CFLAGS += -w
#CFLAGS += -Wall
CFLAGS += -DUSBHLIB
CFLAGS += -fno-rtti -fno-exceptions 
ifeq ($(MIN_MODULE),1)
ifeq ($(TARGET),simple_msd_bot)
CFLAGS += -DUSBH_USE_HUB
CFLAGS += -DUSBH_USE_MSD
endif
ifeq ($(TARGET),simple_gamepad)
CFLAGS += -DUSBH_USE_HUB
CFLAGS += -DUSBH_USE_HID
CFLGGS += -DUSBH_USE_GAMEPAD
endif
ifeq ($(TARGET),simple_keyboard)
CFLAGS += -DUSBH_USE_HUB
CFLAGS += -DUSBH_USE_HID
CFLGGS += -DUSBH_USE_KEYBOARD
endif
ifeq ($(TARGET),simple_mouse)
CFLAGS += -DUSBH_USE_HUB
CFLAGS += -DUSBH_USE_HID
CFLGGS += -DUSBH_USE_MOUSE
endif
ifeq ($(TARGET),simple_bp3595)
CFLAGS += -DUSBH_USE_HUB
CFLAGS += -DUSBH_USE_MSD
CFLAGS += -DUSBH_USE_CDC
endif
else
CFLAGS += -DUSBH_USE_GAMEPAD
CFLAGS += -DUSBH_USE_KEYBOARD
CFLAGS += -DUSBH_USE_MOUSE
CFLAGS += -DUSBH_USE_HUB
CFLAGS += -DUSBH_USE_HID
CFLAGS += -DUSBH_USE_MSD
CFLAGS += -DUSBH_USE_CDC
endif
ifdef MB9BF568R
CFLAGS += -DCONFIG_FM4
CFLAGS += -DMB9BF568R
CFLAGS += -DKS_MB9BF568
endif
ifdef MB9BF618T
CFLAGS += -DCONFIG_FM3
CFLAGS += -DMB9BF618T
endif
ifdef MB9BF506N
CFLAGS += -DCONFIG_FM3
CFLAGS += -DMB9BF506N
endif
ifdef RX62N
CFLAGS += -DRX62N
CFLAGS += -DCONFIG_RX62N
endif
ifdef RX63N
CFLAGS += -DRX63N
CFLAGS += -DCONFIG_RX63N
endif
ifdef RX64M
CFLAGS += -DRX64M
CFLAGS += -DCONFIG_RX64M
endif
ifdef RZA1H
CFLAGS += -DRZA1H
CFLAGS += -DCONFIG_RZA1H
CFLAGS += -g -fno-common -fmessage-length=0 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers
CFLAGS += -fno-exceptions -fno-builtin -ffunction-sections -fdata-sections -fno-delete-null-pointer-checks -fomit-frame-pointer
endif
ifdef SH2A
CFLAGS += -DSH2A
CFLAGS += -DCONFIG_SH2A
CFLAGS += -DBIGENDIAN
endif
ifdef STM32F4
CFLAGS += -DSTM32F4
CFLAGS += -DCONFIG_STM32F4
CFLAGS += -D$(DEVICE)
CFLAGS += -DUSE_HAL_DRIVER
CFLAGS += -D__weak="__attribute__((weak))"
CFLAGS += -D__packed="__attribute__((__packed__))"
#ifeq ($(BOARD),NUCLEO_F446RE)
#CFLAGS += -DSTM32F446xx
#endif
#ifeq ($(BOARD),NUCLEO_F401E)
#CFLAGS += -DSTM32F401xE
#endif
endif
ifdef STM32L4
CFLAGS += -DSTM32L4
CFLAGS += -DCONFIG_STM32L4
CFLAGS += -D$(DEVICE)
CFLAGS += -DUSE_HAL_DRIVER
CFLAGS += -D__weak="__attribute__((weak))"
CFLAGS += -D__packed="__attribute__((__packed__))"
endif
ifeq ($(TARGET),simple_bp3595)
CFLAGS += -DWIFI_B3595
endif

#######################################################################
# CSRC
#######################################################################
CSRC = example/$(TARGET).c
CSRC += core/usbh_base.c
CSRC += core/usbh_env.c
CSRC += core/usbh_hid.c
CSRC += core/usbh_hub.c
CSRC += core/usbh_user_api.c
ifeq ($(TARGET),simple_msd_bot)
CSRC += core/usbh_msd_bot.c
CSRC += core/ff.c
#CSRC += core/diskio.c
endif
ifeq ($(TARGET),simple_bp3595)
CSRC += example/ping/arp.c
CSRC += example/ping/eth.c
CSRC += example/ping/icmp.c
CSRC += example/ping/ping.c
endif
CSRC += hw_dep/common/debug_printf.c
CSRC += hw_dep/common/tinylib.c
CSRC += hw_dep/common/vsnprintf.c
ifeq ($(CONFIG_CPU),FM3)
CSRC += hw_dep/fm3/common/core_cm3.c
ifdef MB9BF618T
CSRC += hw_dep/fm3/common/system_mb9bf61x.c
endif
ifdef MB9BF506N
CSRC += hw_dep/fm3/common/system_mb9bf50x.c
endif
CSRC += hw_dep/fm3/fm3_bootstrap.c
CSRC += hw_dep/fm3/fm3_init.c
CSRC += hw_dep/fm3/fm3_timer.c
CSRC += hw_dep/fm3/fm3_uart.c
CSRC += hw_dep/fm3/usbh_timer.c
CSRC += hw_dep/fm3/usbh_hw.c
endif
ifeq ($(CONFIG_CPU),FM4)
CSRC += hw_dep/fm4/common/core_cm3.c
ifdef MB9BF568R
CSRC += hw_dep/fm4/common/system_mb9abxxx.c
endif
CSRC += hw_dep/fm4/fm4_bootstrap.c
CSRC += hw_dep/fm4/fm4_init.c
CSRC += hw_dep/fm4/fm4_timer.c
CSRC += hw_dep/fm4/fm4_uart.c
CSRC += hw_dep/fm4/usbh_timer.c
CSRC += hw_dep/fm4/usbh_hw.c
endif
ifeq ($(CONFIG_CPU),RX62N)
CSRC += hw_dep/rx62n/interrupt_handlers.c
CSRC += hw_dep/rx62n/vector_table.c
CSRC += hw_dep/rx62n/rx62n_bootstrap.c
CSRC += hw_dep/rx62n/rx62n_init.c
CSRC += hw_dep/rx62n/rx62n_sci.c
CSRC += hw_dep/rx62n/rx62n_timer.c
CSRC += hw_dep/rx62n/usbh_timer.c
CSRC += hw_dep/rx62n/usbh_hw.c
endif
ifeq ($(CONFIG_CPU),RX63N)
CSRC += hw_dep/rx63n/interrupt_handlers.c
CSRC += hw_dep/rx63n/vector_table.c
CSRC += hw_dep/rx63n/rx63n_bootstrap.c
CSRC += hw_dep/rx63n/rx63n_init.c
CSRC += hw_dep/rx63n/rx63n_sci.c
CSRC += hw_dep/rx63n/rx63n_timer.c
CSRC += hw_dep/rx63n/usbh_timer.c
CSRC += hw_dep/rx63n/usbh_hw.c
endif
ifeq ($(CONFIG_CPU),RX64M)
CSRC += hw_dep/rx64m/interrupt_handlers.c
CSRC += hw_dep/rx64m/vector_table.c
CSRC += hw_dep/rx64m/rx64m_bootstrap.c
CSRC += hw_dep/rx64m/rx64m_init.c
CSRC += hw_dep/rx64m/rx64m_sci.c
CSRC += hw_dep/rx64m/rx64m_timer.c
CSRC += hw_dep/rx64m/usbh_timer.c
CSRC += hw_dep/rx64m/usbh_hw.c
endif
ifeq ($(CONFIG_CPU),RZA1H)
CSRC += hw_dep/rza1h/rza1h_bootstrap.c
CSRC += hw_dep/rza1h/rza1h_init.c
CSRC += hw_dep/rza1h/rza1h_timer.c
CSRC += hw_dep/rza1h/rza1h_sci.c
CSRC += hw_dep/rza1h/usbh_timer.c
CSRC += hw_dep/rza1h/usbh_hw.c
CSRC += hw_dep/rza1h/cmsis/TARGET_RENESAS/TARGET_RZ_A1H/cmsis_nvic.c
CSRC += hw_dep/rza1h/cmsis/TARGET_RENESAS/TARGET_RZ_A1H/gic.c
CSRC += hw_dep/rza1h/cmsis/TARGET_RENESAS/TARGET_RZ_A1H/mbed_sf_boot.c
CSRC += hw_dep/rza1h/cmsis/TARGET_RENESAS/TARGET_RZ_A1H/mmu_Renesas_RZ_A1.c
CSRC += hw_dep/rza1h/cmsis/TARGET_RENESAS/TARGET_RZ_A1H/nvic_wrapper.c
CSRC += hw_dep/rza1h/cmsis/TARGET_RENESAS/TARGET_RZ_A1H/pl310.c
CSRC += hw_dep/rza1h/cmsis/TARGET_RENESAS/TARGET_RZ_A1H/RZ_A1_Init.c
CSRC += hw_dep/rza1h/cmsis/TARGET_RENESAS/TARGET_RZ_A1H/rza_io_regrw.c
CSRC += hw_dep/rza1h/cmsis/TARGET_RENESAS/TARGET_RZ_A1H/system_MBRZA1H.c
endif
ifeq ($(CONFIG_CPU),SH2A)
CSRC += hw_dep/sh2a/interrupt_handlers.c
CSRC += hw_dep/sh2a/vector_table.c
CSRC += hw_dep/sh2a/sh2a_bootstrap.c
CSRC += hw_dep/sh2a/sh2a_init.c
CSRC += hw_dep/sh2a/sh2a_sci.c
CSRC += hw_dep/sh2a/sh2a_timer.c
CSRC += hw_dep/sh2a/usbh_timer.c
CSRC += hw_dep/sh2a/usbh_hw.c
endif
ifeq ($(CONFIG_CPU),STM32F4)
CSRC += hw_dep/stm32f4/stm32f4_bootstrap.c
CSRC += hw_dep/stm32f4/stm32f4_init.c
CSRC += hw_dep/stm32f4/stm32f4_sci.c
CSRC += hw_dep/stm32f4/stm32f4_timer.c
CSRC += hw_dep/stm32f4/stm32f4xx_hal_msp.c
CSRC += hw_dep/stm32f4/stm32f4xx_it.c
CSRC += hw_dep/stm32f4/usb_host.c
CSRC += hw_dep/stm32f4/usbh_conf.c
CSRC += hw_dep/stm32f4/usbh_timer.c
CSRC += hw_dep/stm32f4/usbh_hw.c
CSRC += hw_dep/stm32f4/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c
CSRC += hw_dep/stm32f4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
CSRC += hw_dep/stm32f4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
CSRC += hw_dep/stm32f4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
CSRC += hw_dep/stm32f4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
CSRC += hw_dep/stm32f4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
CSRC += hw_dep/stm32f4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
CSRC += hw_dep/stm32f4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
CSRC += hw_dep/stm32f4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c
CSRC += hw_dep/stm32f4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
CSRC += hw_dep/stm32f4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
CSRC += hw_dep/stm32f4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
CSRC += hw_dep/stm32f4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
CSRC += hw_dep/stm32f4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
CSRC += hw_dep/stm32f4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
CSRC += hw_dep/stm32f4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c
#CSRC += hw_dep/stm32f4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.c
#CSRC += hw_dep/stm32f4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.c
#CSRC += hw_dep/stm32f4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.c
CSRC += hw_dep/stm32f4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c
CSRC += hw_dep/stm32f4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c
CSRC += hw_dep/stm32f4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c
CSRC += hw_dep/stm32f4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c
#CSRC += hw_dep/stm32f4/Middlewares/Third_Party/FatFs/src/diskio.c
#CSRC += hw_dep/stm32f4/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c
#CSRC += hw_dep/stm32f4/Middlewares/Third_Party/FatFs/src/ff.c
#CSRC += hw_dep/stm32f4/Middlewares/Third_Party/FatFs/src/drivers/usbh_diskio.c
#CSRC += hw_dep/stm32f4/Middlewares/Third_Party/FatFs/src/option/syscall.c
endif
ifeq ($(CONFIG_CPU),STM32L4)
CSRC += hw_dep/stm32l4/stm32l4_bootstrap.c
CSRC += hw_dep/stm32l4/stm32l4_init.c
CSRC += hw_dep/stm32l4/stm32l4_sci.c
CSRC += hw_dep/stm32l4/stm32l4_timer.c
CSRC += hw_dep/stm32l4/stm32l4xx_hal_msp.c
CSRC += hw_dep/stm32l4/stm32l4xx_it.c
CSRC += hw_dep/stm32l4/usb_host.c
CSRC += hw_dep/stm32l4/usbh_conf.c
CSRC += hw_dep/stm32l4/usbh_timer.c
CSRC += hw_dep/stm32l4/usbh_hw.c
CSRC += hw_dep/stm32l4/system_stm32l4xx.c
CSRC += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c
#CSRC += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c
CSRC += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c
CSRC += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c
CSRC += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c
CSRC += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c
CSRC += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c
CSRC += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_hcd.c
CSRC += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c
CSRC += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c
CSRC += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c
CSRC += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c
CSRC += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c
CSRC += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c
CSRC += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c
CSRC += hw_dep/stm32l4/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_usb.c
#CSRC += hw_dep/stm32l4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.c
#CSRC += hw_dep/stm32l4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.c
#CSRC += hw_dep/stm32l4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.c
CSRC += hw_dep/stm32l4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c
CSRC += hw_dep/stm32l4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c
CSRC += hw_dep/stm32l4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c
CSRC += hw_dep/stm32l4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c
#CSRC += hw_dep/stm32l4/Middlewares/Third_Party/FatFs/src/diskio.c
#CSRC += hw_dep/stm32l4/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c
#CSRC += hw_dep/stm32l4/Middlewares/Third_Party/FatFs/src/ff.c
#CSRC += hw_dep/stm32l4/Middlewares/Third_Party/FatFs/src/drivers/usbh_diskio.c
#CSRC += hw_dep/stm32l4/Middlewares/Third_Party/FatFs/src/option/syscall.c
endif

#######################################################################
# ASRC
#######################################################################
ifdef MB9BF568R
ASRC = hw_dep/fm4/startup_fm4.s
endif
ifdef MB9BF618T
ASRC = hw_dep/fm3/startup_fm3.s
endif
ifdef MB9BF506N
ASRC = hw_dep/fm3/startup_fm3.s
endif
ifdef RX62N
ASRC = hw_dep/rx62n/startup_rx62n.s
endif
ifdef RX63N
ASRC = hw_dep/rx63n/startup_rx63n.s
endif
ifdef RX64M
ASRC = hw_dep/rx64m/startup_rx64m.s
endif
ifdef RZA1H
ASRC = hw_dep/rza1h/startup_rza1h.s
ASRC += hw_dep/rza1h/cmsis/TOOLCHAIN_GCC/TARGET_CORTEX_A/cache.s
#ASRC += hw_dep/rza1h/irqfiq_handler.s
endif
ifdef SH2A
ASRC = hw_dep/sh2a/startup_sh2a.s
endif
ifdef STM32F4
ifeq ($(BOARD),NUCLEO_F446RE)
ASRC = hw_dep/stm32f4/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f446xx.s
endif
ifeq ($(BOARD),NUCLEO_F401RE)
ASRC = hw_dep/stm32f4/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f401xe.s
endif
endif
ifdef STM32L4
ASRC = hw_dep/stm32l4/Drivers/CMSIS/Device/ST/STM32L4xx/Source/Templates/gcc/startup_stm32l476xx.s
endif

#######################################################################
# Assembler flags.
#######################################################################
ASFLAGS = -g
#ASFLAGS = -Wa

#######################################################################
# Linker flags.
#######################################################################
ifneq (,$(findstring $(CONFIG_CPU),FM3))
LINC = $(GCCDIR)/../arm-none-eabi/lib/$(GCCLIB)
LINC += $(GCCDIR)/../lib/gcc/arm-none-eabi/$(GCCVER)/$(GCCLIB)
endif
ifneq (,$(findstring $(CONFIG_CPU),RX62N RX63N RX64M))
LINC = $(GCCDIR)/../rx-elf/lib
LINC += $(GCCDIR)/../lib/gcc/rx-elf/$(GCCVER)
endif
ifneq (,$(findstring $(CONFIG_CPU),RZA1H))
ifdef KPIT
LINC = $(GCCDIR)/../arm-rz-eabi/lib
LINC += $(GCCDIR)/../lib/gcc/arm-rz-eabi/$(GCCVER)
else
LINC = $(GCCDIR)/../arm-none-eabi/lib
LINC += $(GCCDIR)/../lib/gcc/arm-none-eabi/$(GCCVER)/$(GCCLIB)
endif
endif
ifneq (,$(findstring $(CONFIG_CPU),SH2 SH2A))
LINC = $(GCCDIR)/../sh-elf/lib
LINC += $(GCCDIR)/../lib/gcc/sh-elf/$(GCCVER)
endif
ifneq (,$(findstring $(CONFIG_CPU),STM32F4 STM32L4 FM4))
#LINC = $(GCCDIR)/../arm-none-eabi/lib/$(GCCLIB)/fpu
#LINC += $(GCCDIR)/../lib/gcc/arm-none-eabi/$(GCCVER)/$(GCCLIB)/fpu
endif

ifneq (,$(findstring $(CONFIG_CPU),RZA1H))
LDFLAGS = -g -lg -lm -lc -lgcc -lnosys -specs=nosys.specs
endif
ifneq (,$(findstring $(CONFIG_CPU),STM32F4 STM32L4 FM4))
LDFLAGS += -g -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -lm -specs=nosys.specs -specs=nano.specs
endif
ifneq (,$(findstring $(CONFIG_CPU),FM3))
LDFLAGS += -g -mcpu=cortex-m3 -mthumb -lm -specs=nosys.specs -specs=nano.specs
endif
ifneq (,$(findstring $(CONFIG_CPU),RX62N RX63N RX64M SH2A))
LDFLAGS += -g -lm -lc -lgcc -nostartfiles
endif

LDFLAGS += $(patsubst %,-L%,$(LINC))
ifdef MB9BF568R
LDFLAGS += -T hw_dep/fm4/mb9bf568.ld
endif
ifdef MB9BF618T
LDFLAGS += -T hw_dep/fm3/mb9bf618.ld
endif
ifdef MB9BF506N
LDFLAGS += -T hw_dep/fm3/mb9bf506.ld
endif
ifdef RX62N
LDFLAGS += -T hw_dep/rx62n/rx62n.ld
endif
ifdef RX63N
LDFLAGS += -T hw_dep/rx63n/rx63n.ld
endif
ifdef RX64M
LDFLAGS += -T hw_dep/rx64m/rx64m.ld
endif
ifdef RZA1H
LDFLAGS += -T hw_dep/rza1h/rza1h_sf.ld
endif
ifdef SH2A
LDFLAGS += -T hw_dep/sh2a/sh2a.ld
endif
ifdef STM32F4
ifeq ($(BOARD),NUCLEO_F446RE)
LDFLAGS += -T hw_dep/stm32f4/STM32F446RETx_FLASH.ld
endif
ifeq ($(BOARD),NUCLEO_F401RE)
LDFLAGS += -T hw_dep/stm32f4/STM32F401RETx_FLASH.ld
endif
endif
ifdef STM32L4
LDFLAGS += -T hw_dep/stm32l4/STM32L476RG_FLASH.ld
endif

#######################################################################
# Define all object files.
#######################################################################
COBJ = $(addprefix $(BUILD)/,$(patsubst %.c,%.o,$(CSRC)))
AOBJ = $(addprefix $(BUILD)/,$(patsubst %.s,%.o,$(ASRC)))
CPPOBJ = $(addprefix $(BUILD)/,$(patsubst %.cpp,%.o,$(CPPSRC)))
OBJ = $(COBJ) $(AOBJ) $(CPPOBJ)
#OBJ = $(CSRC:.c=.o) $(ASRC:.s=.o) $(CPPSRC:.cpp=.o)

#######################################################################
# Define all listing files.
#######################################################################
LST = $(ASRC:.s=.lst) $(CSRC:.c=.lst) $(CPPSRC:.cpp=.lst)

#######################################################################
# Combine all necessary flags and optional flags.
# Add target processor to flags.
#######################################################################
ifneq (,$(findstring $(CONFIG_CPU),FM3 FM4 STM32F4 STM32L4))
ALL_CFLAGS = -mcpu=$(CPUTYPE) -I. $(CFLAGS)
ALL_ASFLAGS = -mcpu=$(CPUTYPE) -I. -x assembler-with-cpp $(ASFLAGS)
else
ifneq (,$(findstring $(CONFIG_CPU),RZA1H))
ALL_CFLAGS = -I. $(CFLAGS) $(CPU)
ALL_ASFLAGS = -I. -x assembler-with-cpp $(ASFLAGS) $(CPU)
else
ALL_CFLAGS = -I. $(CFLAGS)
ALL_ASFLAGS = -I. -x assembler-with-cpp $(ASFLAGS)
endif
endif

#######################################################################
#######################################################################

.PHONY: all clean lst size

#all: gccversion build sizeafter
all: build

#build: elf
build: elf hex bin srec
#build: elf hex bin sym lss srec

elf: $(BUILD)/$(TARGET).elf
hex: $(BUILD)/$(TARGET).hex
bin: $(BUILD)/$(TARGET).bin
lss: $(BUILD)/$(TARGET).lss 
sym: $(BUILD)/$(TARGET).sym
srec: $(BUILD)/$(TARGET).srec

stdlib:	lib
	$(MAKE) -C lib

# Display size of file.
FORMAT = ihex
HEXSIZE = $(SIZE) --target=$(FORMAT) $(BUILD)/$(TARGET).hex
ELFSIZE = $(SIZE) -A $(BUILD)/$(TARGET).elf
sizebefore:
	@if [ -f $(BUILD)/$(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); echo; fi

sizeafter:
	@if [ -f $(BUILD)/$(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi

$(BUILD)/$(TARGET).hex: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O $(FORMAT) $< $@

$(BUILD)/$(TARGET).bin : $(BUILD)/$(TARGET).elf
	$(OBJDUMP) -f -p $<
	$(OBJCOPY) -O binary $< $@

$(BUILD)/$(TARGET).srec : $(BUILD)/$(TARGET).elf
	$(OBJCOPY) --srec-forceS3 -O srec $< $@

$(BUILD)/$(TARGET).lss : $(BUILD)/$(TARGET).elf
	$(OBJDUMP) -D $< > $@

$(BUILD)/$(TARGET).sym : $(BUILD)/$(TARGET).elf
	$(NM) -n $< > $@

#ifneq (,$(findstring $(CONFIG_CPU),RZA1H))
$(BUILD)/$(TARGET).elf : $(OBJ)
	$(CPP) $(LDFLAGS) -Wl,-Map=$(BUILD)/$(TARGET).map $(OBJ) -o $@

#else
#%.elf : $(OBJ)
#	$(LD) $(LDFLAGS) -Map=$(BUILD)/$(TARGET).map $(OBJ) -o $@
#
#endif

# Compile: create object files from C source files.
$(BUILD)/%.o : %.cpp
	$(MKDIR) -p $(dir $@)
	$(CPP) -c $(ALL_CFLAGS) $< -o $@ 

# Compile: create object files from C source files.
$(BUILD)/%.o : %.c
	$(MKDIR) -p $(dir $@)
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 

# Compile: create assembler files from C source files.
#%.s : %.c
#	$(CC) -S $(ALL_CFLAGS) $< -o $@

# Assemble: create object files from assembler source files.
$(BUILD)/%.o : %.s
	$(MKDIR) -p $(dir $@)
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

distclean: clean
	$(MAKE) -C clean

clean:
	$(REMOVE) -rf $(BUILD)/*
#	$(REMOVE) $(BUILD)/$(TARGET).hex
#	$(REMOVE) $(BUILD)/$(TARGET).elf
#	$(REMOVE) $(BUILD)/$(TARGET).map
#	$(REMOVE) $(BUILD)/$(TARGET).lss
#	$(REMOVE) $(BUILD)/$(TARGET).sym
#	$(REMOVE) $(BUILD)/$(TARGET).srec
#	$(REMOVE) $(BUILD)/$(OBJ)
#	$(REMOVE) $(BUILD)/$(LST)
	
DEPS = $(CSRC:.c=.d) $(ASRC:.s=.d) $(CPPSRC:.cpp=.d)
-include $(DEPS)
