@rem build_all 2>&1| tee build_all.log

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
