MCU          = atmega16u2
ARCH         = AVR8
BOARD        = USER
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TARGET       = hid-joystick
LUFA_PATH    = ../LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -IConfig/
CFLAGS       = -DBOARD=BOARD_$(BOARD)
LD_FLAGS     =
SRC          = Joystick.c \
			   Descriptors.c \
			   $(LUFA_SRC_USB) \
			   $(LUFA_SRC_USBCLASS)

# Default target
all:

# We also want to remove obj directory
clean: deepClean
deepClean:
	rm -rf obj

# Include LUFA-specific DMBS extension modules
DMBS_LUFA_PATH = $(LUFA_PATH)/Build/LUFA
include $(DMBS_LUFA_PATH)/lufa-sources.mk
include $(DMBS_LUFA_PATH)/lufa-gcc.mk

# Include common DMBS build system modules
DMBS_PATH = $(LUFA_PATH)/Build/DMBS/DMBS
include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/gcc.mk
