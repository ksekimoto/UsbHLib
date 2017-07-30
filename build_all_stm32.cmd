@rem build_all 2>&1| tee build_all.log

make clean CONFIG_CPU=STM32F4
make CONFIG_CPU=STM32F4 TARGET=simple_gamepad
make CONFIG_CPU=STM32F4 TARGET=simple_keyboard
make CONFIG_CPU=STM32F4 TARGET=simple_mouse
make CONFIG_CPU=STM32F4 TARGET=simple_msd_bot
make CONFIG_CPU=STM32F4 TARGET=simple_usb_check
