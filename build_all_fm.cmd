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
