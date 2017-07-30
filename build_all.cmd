@rem build_all 2>&1| tee build_all.log

make clean CONFIG_CPU=FM3
make CONFIG_CPU=FM3 TARGET=simple_gamepad
make CONFIG_CPU=FM3 TARGET=simple_keyboard
make CONFIG_CPU=FM3 TARGET=simple_mouse
make CONFIG_CPU=FM3 TARGET=simple_msd_bot
make CONFIG_CPU=FM3 TARGET=simple_usb_check

make clean CONFIG_CPU=FM4
make CONFIG_CPU=FM4 TARGET=simple_gamepad
make CONFIG_CPU=FM4 TARGET=simple_keyboard
make CONFIG_CPU=FM4 TARGET=simple_mouse
make CONFIG_CPU=FM4 TARGET=simple_msd_bot
make CONFIG_CPU=FM4 TARGET=simple_usb_check

make clean CONFIG_CPU=RX62N
make CONFIG_CPU=RX62N TARGET=simple_gamepad
make CONFIG_CPU=RX62N TARGET=simple_keyboard
make CONFIG_CPU=RX62N TARGET=simple_mouse
make CONFIG_CPU=RX62N TARGET=simple_msd_bot
make CONFIG_CPU=RX62N TARGET=simple_usb_check

make clean CONFIG_CPU=RX63N
make CONFIG_CPU=RX63N TARGET=simple_gamepad
make CONFIG_CPU=RX63N TARGET=simple_keyboard
make CONFIG_CPU=RX63N TARGET=simple_mouse
make CONFIG_CPU=RX63N TARGET=simple_msd_bot
make CONFIG_CPU=RX63N TARGET=simple_usb_check

make clean CONFIG_CPU=RX64M
make CONFIG_CPU=RX64M TARGET=simple_gamepad
make CONFIG_CPU=RX64M TARGET=simple_keyboard
make CONFIG_CPU=RX64M TARGET=simple_mouse
make CONFIG_CPU=RX64M TARGET=simple_msd_bot
make CONFIG_CPU=RX64M TARGET=simple_usb_check

make clean CONFIG_CPU=SH2A
make CONFIG_CPU=SH2A TARGET=simple_gamepad
make CONFIG_CPU=SH2A TARGET=simple_keyboard
make CONFIG_CPU=SH2A TARGET=simple_mouse
make CONFIG_CPU=SH2A TARGET=simple_msd_bot
make CONFIG_CPU=SH2A TARGET=simple_usb_check

make clean CONFIG_CPU=RZA1H
make CONFIG_CPU=RZA1H TARGET=simple_gamepad
make CONFIG_CPU=RZA1H TARGET=simple_keyboard
make CONFIG_CPU=RZA1H TARGET=simple_mouse
make CONFIG_CPU=RZA1H TARGET=simple_msd_bot
make CONFIG_CPU=RZA1H TARGET=simple_usb_check
make CONFIG_CPU=RZA1H TARGET=simple_bp3595

make clean CONFIG_CPU=STM32F4
make CONFIG_CPU=STM32F4 TARGET=simple_gamepad
make CONFIG_CPU=STM32F4 TARGET=simple_keyboard
make CONFIG_CPU=STM32F4 TARGET=simple_mouse
make CONFIG_CPU=STM32F4 TARGET=simple_msd_bot
make CONFIG_CPU=STM32F4 TARGET=simple_usb_check
