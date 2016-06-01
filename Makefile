# List all source files the application uses.
APPLICATION_SRCS = $(notdir $(wildcard ./*.c))

PROJECT_NAME = cmac_aes128_test
VERSION = 0.1

DEVICE = NRF51

#USE_SOFTDEVICE = s110

SDK_PATH = $(HOME)/devel/nrf-sdk/10.0/

TEMPLATE_PATH = ./template/

CFLAGS = -Os -Wall -Werror -flto -g
LDFLAGS = -Os -flto -g

include $(TEMPLATE_PATH)Makefile
