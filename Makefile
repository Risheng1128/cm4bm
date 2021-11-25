# ------------------------------------------------
# Generic Makefile
# 
# Author: Ri-Sheng Chen
# ------------------------------------------------

############################################################################
# 設定COMPILER指令
############################################################################
# 目標編譯檔案
TARGET = main

# 編譯檔案資料夾
BUILD_DIR = Debug

# C Source 使用到的.c檔
C_SOURCES = \
Src/$(TARGET).c  \
Src/myusart.c	 \
Src/syscalls.c   \
Src/sysmem.c     \

# ASM sources
ASM_SOURCES = Startup/startup_stm32f303zetx.s

# 使用編譯器
COMPILER = arm-none-eabi-
CC = $(COMPILER)gcc

# Specify the language of the following input files.
AS = $(COMPILER)gcc -x assembler-with-cpp

# 用法: arm-none-eabi-size [option(s)] [file(s)]
# 功能: Displays the sizes of sections inside binary files
SZ = $(COMPILER)size

############################################################################
# 設定CFLAGS
############################################################################

# CPU
# -mcpu=name 指定ARM處理器的型號，如“cortex-m4”等
CPU = -mcpu=cortex-m4

# FPU
# -mfpu=name 指定硬體FPU型號，如“vfpv2”等
FPU = -mfpu=fpv4-sp-d16

# float-abi
# -mfloat-abi=name 指定要使用的浮點ABI，可以是 "hard" 或 "soft"
FLOAT-ABI = -mfloat-abi=hard

# MCU
# -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
# 用途: use hardware floating point
# -mthumb 使用Thumb指令集
MCU = -mthumb $(CPU) $(FLOAT) $(FPU)

C_DEFS = \
-DUSE_HAL_DRIVER \
-DSTM32F303xE

AS_INCLUDES = 

# C includes 函式庫.h 路徑
C_INCLUDES = \
-IInc		 \

# compile gcc flags
# -Og Optimize for debug
# -Wall 編譯後顯示所有警告
# -fdata-sections 在編譯的時候把每個資料作為一個section。其中每個sections名與function或data名保持一致，能夠減小生成檔案的大小
# -ffunction-sections 在編譯的時候把每個函式作為一個section
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) -O0 -Wall -fdata-sections -ffunction-sections
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) -O0 -Wall -fdata-sections -ffunction-sections

# Debug
# -g 輸出除錯資訊
# -g -gdwarf-2 生成 gdb 除錯資訊 格式為 dward-2
CFLAGS += -g -gdwarf-2

# Generate dependency information
# -MMD 用以避免產生系統標頭檔案，如 <stdint.h>
# -MF program.o.d 表示將 program.c 的 dependency 輸出至 program.o.d
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F303ZETX_FLASH.ld

# libraries
# -lc -lm -lnosys (c lib、math lib、nosys lib，用於驗證連結是否有效)
LIBS = -lc -lm -lnosys

# -specs=nano.specs "Link"時使用 newlib（libc），減少大量檔案大小。
# -Wl,-Map=xxxxxxxx.map, --cref 生成連結對映列表， --cref 輸出交叉引用列表
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR):
	mkdir $@

.PHONY: disassembly load upload clean
disassembly: $(BUILD_DIR)/$(TARGET).elf
	$(COMPILER)objdump.exe -d $^ > $(BUILD_DIR)/$(TARGET).S
	$(COMPILER)objdump.exe -d $^ > $(TARGET).cdasm

load: 
	openocd -f board/st_nucleo_f3.cfg

upload:
	openocd -f interface/stlink.cfg -f target/stm32f3x.cfg -c " program $(BUILD_DIR)/$(TARGET).elf verify exit "

clean:
	@-rm -r $(BUILD_DIR)

-include $(wildcard $(BUILD_DIR)/*.d)
