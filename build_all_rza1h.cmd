@rem build_all 2>&1| tee build_all.log

make clean CONFIG_CPU=RZA1H
make CONFIG_CPU=RZA1H TARGET=simple_gamepad
make CONFIG_CPU=RZA1H TARGET=simple_keyboard
make CONFIG_CPU=RZA1H TARGET=simple_mouse
make CONFIG_CPU=RZA1H TARGET=simple_msd_bot
make CONFIG_CPU=RZA1H TARGET=simple_usb_check
