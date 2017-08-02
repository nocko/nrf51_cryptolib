# List all source files the application uses.
APPLICATION_SRCS = $(notdir $(wildcard ./*.c))

PROJECT_NAME = nrf51_cryptolib
VERSION = 0.1

DEVICE = NRF51

#USE_SOFTDEVICE = s110

SDK_PATH = $(HOME)/devel/nrf-sdk/10.0/

TEMPLATE_PATH = ./template/

HOST_CC = gcc # Used when running tests on host
CFLAGS = -Os -Wall -Werror -g
LDFLAGS = -Os -g

include $(TEMPLATE_PATH)Makefile
