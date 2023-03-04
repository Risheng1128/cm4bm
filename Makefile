# Generic Makefile
OUT ?= build
BIN ?= main
# only the filename of target
BIN_NAME = $(notdir $(BIN))
BIN_DIR = $(dir $(BIN))

C_SRC += syscalls.c \
	    usart.c \
	    $(BIN_NAME).c
ASM_SRC = startup.s
LDSCRIPT = src/link.ld

TOOLCHAIN = arm-none-eabi-
CC = $(TOOLCHAIN)gcc
SZ = $(TOOLCHAIN)size

MCU = -mthumb -mcpu=cortex-m4
C_INC += -Iinc
CFLAGS = $(MCU) $(C_INC) -O0 -Wall
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) -lc -lm -lnosys

OBJS = $(patsubst %.c, $(OUT)/%.o, $(C_SRC))
OBJS += $(patsubst %.s, $(OUT)/%.o, $(ASM_SRC))

all: $(OUT)/$(BIN_NAME)
$(OUT)/%.o: $(BIN_DIR)%.c
	$(CC) -c $(CFLAGS) $< -o $@
$(OUT)/%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@
$(OUT)/%.o: src/%.s
	$(CC) -c $(CFLAGS) $< -o $@
$(OUT)/$(BIN_NAME): $(OUT) $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SZ) $@
$(OUT):
	mkdir $@

.PHONY: disassembly load upload clean
disassembly: $(OUT)/$(BIN)
	$(TOOLCHAIN)objdump -d $^ > $(OUT)/$(BIN_NAME).S
debug:
	openocd -f board/st_nucleo_f3.cfg
upload:
	openocd -f interface/stlink-v2-1.cfg -f target/stm32f3x.cfg -c " program $(OUT)/$(BIN_NAME) verify exit "
clean:
	-@rm -r $(OUT)