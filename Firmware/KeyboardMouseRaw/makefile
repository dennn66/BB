#
#             LUFA Library
#     Copyright (C) Dean Camera, 2014.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

#BOARD        = STK526

#MCU          = at90usb162
#ARCH         = AVR8
#BOARD        = USBDONGLE
#F_CPU        = 8000000

MCU          = atmega32u4 
ARCH         = AVR8
BOARD        = LEONARDO 
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = KeyboardMouseRaw
#SRC          = $(TARGET).c Descriptors.c n3310.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)cd //
SRC          = $(TARGET).c Descriptors.c  tmk_core/ps2_mouse.c tmk_core/debug.c tmk_core/print.c tmk_core/ps2_busywait.c  tmk_core/ps2_io_avr.c tmk_core/host.c tmk_core/avr/xprintf.S tmk_core/avr/timer.c  $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)
LUFA_PATH    = ../lufa-LUFA-140928/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/
LD_FLAGS     =
AVRDUDE_PROGRAMMER = arduino
AVRDUDE_VERBOSE = -v -v -v -v
AVRDUDE_BAUDRATE = 57600
AVRDUDE_PORT = com8    # programmer connected to serial device
AVRDUDE_FLAGS = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) -b$(AVRDUDE_BAUDRATE)

# com1 = serial port. Use lpt1 to connect to parallel port.
# Default target
all:

# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
include $(LUFA_PATH)/Build/lufa_cppcheck.mk
include $(LUFA_PATH)/Build/lufa_doxygen.mk
include $(LUFA_PATH)/Build/lufa_dfu.mk
include $(LUFA_PATH)/Build/lufa_hid.mk
include $(LUFA_PATH)/Build/lufa_avrdude.mk
include $(LUFA_PATH)/Build/lufa_atprogram.mk
