# Generic Makefile
OUT ?= build
BIN ?= main

# c source file
INPUT_C_SRC ?= src/main.c
C_SRC = src/syscalls.c \
	src/usart.c \
	src/startup.c \
	$(INPUT_C_SRC)

INPUT_C_INC ?=
C_INC = -Iinc \
	$(INPUT_C_INC)

# linker script
LDSCRIPT = src/link.ld

VPATH  = $(dir $(C_SRC))

# object file
OBJS  = $(patsubst %.c, $(OUT)/%.o, $(notdir $(C_SRC)))

# toolchain
TOOLCHAIN = arm-none-eabi-
CC = $(TOOLCHAIN)gcc
SZ = $(TOOLCHAIN)size

# toolchain options
MCU = -mcpu=cortex-m4
CFLAGS = $(MCU) -mthumb $(C_INC) -O0 -Wall -g
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) -lc -lm -lnosys

all: $(OUT)/$(BIN)
# compile
$(OUT)/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# link
$(OUT)/$(BIN): $(OUT) $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SZ) $@

$(OUT):
	mkdir $@

.PHONY: disassembly load upload clean
disassembly: $(OUT)/$(BIN)
	$(TOOLCHAIN)objdump -d $^ > $(OUT)/$(BIN).S

upload:
	openocd -f interface/stlink-v2-1.cfg -f target/stm32f3x.cfg -c " program $(OUT)/$(BIN) verify exit "

clean:
	-@rm -r $(OUT)
