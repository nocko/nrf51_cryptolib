TOOLCHAIN_PREFIX ?= arm-none-eabi

AS = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-as
CC = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-gcc
LD = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-gcc
OBJCOPY = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-objcopy
OBJDUMP = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-objdump
SIZE = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-size
GDB = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)-gdb

START_CODE ?= startup_nrf51.s
SYSTEM_FILE ?= system_nrf51.c

ifdef DEV_BOARD
    CFLAGS += -DDEV_BOARD
endif

ifeq ($(USE_SOFTDEVICE), s110)
    CFLAGS += -DS110
    USE_BLE = 1
endif

ifeq ($(USE_SOFTDEVICE), s120)
    CFLAGS += -DS120
    USE_BLE = 1
endif

ifeq ($(USE_SOFTDEVICE), s310)
    USE_BLE = 1
    USE_ANT = 1
    CFLAGS += -DS310
endif

LIBRARY_PATHS += $(TEMPLATE_PATH)
LIBRARY_PATHS += $(SDK_PATH)components/toolchain/gcc/
LIBRARY_PATHS += $(SDK_PATH)components/toolchain/
LIBRARY_PATHS += $(wildcard $(SDK_PATH)components/libraries/*/)

LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/ble_flash/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/clock/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/common/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/config/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/delay/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/gpiote/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/hal/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/lpcomp/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/ppi/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/pstorage/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/qdec/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/radio_config/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/rng/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/rtc/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/sdio/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/spi_master/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/spi_slave/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/swi/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/timer/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/twi_master/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/uart/
LIBRARY_PATHS += $(SDK_PATH)/components/drivers_nrf/wdt/

LIBRARY_PATHS += $(wildcard $(SDK_PATH)components/device/)
LIBRARY_PATHS += $(wildcard $(SDK_PATH)components/serialization/*/)

ifdef USE_BLE
    LIBRARY_PATHS += $(wildcard $(SDK_PATH)components/ble/*/)
    LIBRARY_PATHS += $(wildcard $(SDK_PATH)components/ble/ble_services/*/)
    CFLAGS += -DBLE_STACK_SUPPORT_REQD
endif

ifdef USE_ANT
    CFLAGS += -DANT_STACK_SUPPORT_REQD
endif

ifdef USE_SOFTDEVICE
    CFLAGS += -DSOFTDEVICE_PRESENT
    LIBRARY_PATHS += $(SDK_PATH)components/softdevice/common/softdevice_handler/
    LIBRARY_PATHS += $(SDK_PATH)components/softdevice/$(USE_SOFTDEVICE)/headers/
else
    LIBRARY_PATHS += $(wildcard $(SDK_PATH)components/drivers_nrf/nrf_soc_nosd/)
    USE_SOFTDEVICE = blank
endif

LINKER_SCRIPT ?= gcc_nrf51_$(USE_SOFTDEVICE).ld
OUTPUT_NAME ?= $(addsuffix _$(USE_SOFTDEVICE)_$(VERSION), $(PROJECT_NAME))

ifdef USE_EXT_SENSORS
    LIBRARY_PATHS += $(wildcard $(SDK_PATH)components/ext_sensors/*/)
endif

LIBRARY_INCLUDES = $(addprefix -I,$(LIBRARY_PATHS))

VPATH = $(LIBRARY_PATHS)

OUTPUT_PATH = _build/

CPUFLAGS += -mthumb -mcpu=cortex-m0 -mabi=aapcs -mfloat-abi=soft
ifdef BOARD
CFLAGS += -DBOARD=$(BOARD)
endif
CFLAGS += -std=gnu99 -c $(CPUFLAGS) -Wall -D$(DEVICE) $(LIBRARY_INCLUDES) -MD
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums

# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m0
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys
LDFLAGS += -Wl,-Map=$(OUTPUT_PATH)$(OUTPUT_NAME).Map

HEX = $(OUTPUT_PATH)$(OUTPUT_NAME).hex
ELF = $(OUTPUT_PATH)$(OUTPUT_NAME).elf
BIN = $(OUTPUT_PATH)$(OUTPUT_NAME).bin

SRCS = $(SYSTEM_FILE) $(notdir $(APPLICATION_SRCS))
OBJS = $(addprefix $(OUTPUT_PATH), $(SRCS:.c=.o)) $(addprefix $(OUTPUT_PATH),$(APPLICATION_LIBS))
DEPS = $(addprefix $(OUTPUT_PATH), $(SRCS:.c=.d))
SRCS_AS = $(START_CODE)
OBJS_AS = $(addprefix $(OUTPUT_PATH), $(SRCS_AS:.s=.os))

JLINK_OPTIONS = -device nrf51822 -if swd -speed 1000

all: $(OBJS) $(OBJS_AS) $(HEX)

rebuild: clean all

bootloader: clean
	make -C bootloader

combined: bootloader all
	srec_cat \
          softdevice/softdevice.hex -intel \
          bootloader/_build/sdk10_bootloader_s110.hex -intel -exclude 0x3FC00 0x3FC04 \
          _build/hiro_s110_$(VERSION).hex -intel \
          -generate 0x3FC00 0x3FC04 -const-le 0x01 4 \
          -output bulk-program/$(PROJECT_NAME)-combined-$(VERSION).hex -intel
	ln bulk-program/$(PROJECT_NAME)-combined-$(VERSION).hex bulk-program/$(PROJECT_NAME)-combined.hex

# combined: rebuild
# 	srec_cat \
#           softdevice/softdevice.hex -intel \
#           _build/$(PROJECT_NAME)_$(USE_SOFTDEVICE)_$(VERSION).hex -intel \
#           -output bulk-program/combined.hex -intel
# 	ln -s bulk-program/combined.hex bulk-program/$(PROJECT_NAME)-$(VERSION)-combined.hex

ifeq ($(OS),Windows_NT)
include $(TEMPLATE_PATH)Makefile.windows
else
include $(TEMPLATE_PATH)Makefile.posix
endif

$(HEX): $(OBJS) $(OBJS_AS)
	$(LD) $(LDFLAGS) $(OBJS_AS) $(OBJS) -o $(ELF)
	$(OBJCOPY) -Oihex $(ELF) $(HEX)
	$(OBJCOPY) -Obinary $(ELF) $(BIN)
	$(SIZE) $(ELF)

size: $(ELF)
	$(SIZE) $(ELF)

$(OUTPUT_PATH)%.o: %.c
	$(MAKE_BUILD_FOLDER)
	$(CC) $(LDFLAGS) $(CFLAGS) $< -o $@

$(OUTPUT_PATH)%.os: %.s
	$(MAKE_BUILD_FOLDER)
	$(AS) $< -o $@

-include $(DEPS)

.PHONY: all clean rebuild size
