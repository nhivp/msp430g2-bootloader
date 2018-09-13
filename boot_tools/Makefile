#
# Makefile script for compiling
#

GCC = gcc
BUILD_DIR = build
SRCS = $(wildcard *.c)
OBJECTS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))
TARGET = $(BUILD_DIR)/serial_flash

CFLAGS = -g -Wall

all: credir $(TARGET)

$(TARGET): $(OBJECTS)
	$(GCC) $(CFLAGS) $^ -o $@

$(OBJECTS): $(BUILD_DIR)/%.o : %.c
	$(GCC) $(CFLAGS) -c $< -o $@

clean :
	rm -rf build

credir:
	mkdir -p build

.PHONY: clean credir