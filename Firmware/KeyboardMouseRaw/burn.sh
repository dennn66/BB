#!/usr/bin/env sh
avrdude -C/usr/share/arduino/hardware/tools/avrdude.conf -v -v -v -v -patmega32u4 -cavr109 -P/dev/ttyACM4 -b57600 -D -V -Uflash:w:./KeyboardMouseRaw.hex:i

