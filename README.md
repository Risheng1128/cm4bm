# CM4 Project
###### tags: `ARM Cortex-M`
使用NUCLEO-F303ZE完成的專案，並實作printf及scanf功能

## 檔案編排
```
ARM-Template-Project
|    Makefile
|    README.md
|    STM32F303.svd
|    STM32F303ZETX_FLASH.ld
|    st_nucleo_f3.cfg
|────Inc
|    |    myusart.h
|
|────Src
|    |    main.c 
|    |    myusart.c
|    |    syscall.c
|    |    sysmem.c
|
|────Startup
|    |    startup_stm32f303zetx.s

```

## 使用設備
- NUCLEO-F303ZE
  1. [官方文件規格介紹](https://www.st.com/en/evaluation-tools/nucleo-f303ze.html)
     ![](https://i.imgur.com/xnKAENl.png)
  2. 核心為: STM32F303ZE
     [官方文件連結](https://www.st.com/en/microcontrollers-microprocessors/stm32f303ze.html)

- 需要工具
  1. [ST-Link(和板子連接)](https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-utilities/stsw-link009.html?dl=NCWMPz8nopJ5khffu%2F9QmQ%3D%3D%2CWweBOQKqZ%2Fh88%2BZkYv6600%2BzYxzrVLpvTUDLmq2eRG7lpwFzjoue0R2T%2FIhPrxz5shDoc2Iw%2FpMlGzkS5d%2B7cDojQPuWTbCMGjy7i4mRoDYG8omxLndyTs6vk9JuiWwwo6iFG5gODbBgUZE9XYj6MzT27BETsC%2FIlGdOMpyOw01kOaztq2FnADZ2T%2Fdozp3FqjWcF7BoC2kA3jX%2FWibrHNy4bIuRL4s4JsMXRBWmjDNxvYbhhCm3h7bESHdDNz6mJV1PitQm3a1X2DfqRbg%2FlIQ3iR0QVkIkj9A3ZMDCD%2FRmyu75S5hGTsJKi0zE3pG8&uid=AbAAPrnhg03EDrR2hPt2MKBBSrNbNCYh)

  2. arm-none-eabi-gcc (Essential compiler driver)
     [arm-none-eabi-gcc載點](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
     [各種GCC指令總攬](https://gcc.gnu.org/onlinedocs/gcc-9.3.0/gcc/)

  3. [OpenOCD](https://openocd.org/) (用來Debug及燒入)
     [OpenOCD for windows載點](https://gnutoolchains.com/arm-eabi/openocd/)
  
## 目標: 使板子可以使用 printf() 及 scanf() 函式
### GPIO設定
-  設定PD8及PD9(兩者為USART3 Tx/Rx)
   1. PD8: 
      - GPIOD_MODE:      Alternate function mode
      - GPIOD_OSPEEDR:   Low Speed
      - GPIOD_PUPDR:     No Pull-up & Pull-down
      - GPIOD_OTYPER:    Push-Pull
      - GPIOD_AFRH:      AF7 (USART3 Tx)
   2. PD9: 
      - GPIOD_MODE:      Alternate function mode
      - GPIOD_OSPEEDR:   Low Speed
      - GPIOD_PUPDR:     No Pull-up & Pull-down
      - GPIOD_OTYPER:    Push-Pull
      - GPIOD_AFRH:      AF7 (USART3 Rx)

### USART設定
1. 使用 USART3，因為USART3連接到Virual COM port
   1. PD8 設為 USART3 TX
   2. PD9 設為 USART3 RX
   ![](https://i.imgur.com/59O5lGb.png)
   
2. USART3基本設定
   - F_CLK:           8MHz (STM32F303ZE預設clock)
   - USART3_Mode:     Tx/Rx Enable
   - USART3_Parity:   Parity disable
   - USART3_StopBits: 1
   - USART3_WordLen:  8
   - USART3_Baud:     38400
   - USART3_HW_FLOW:  None
   - Oversampling:    16

### 修改 printf 及 scanf system call
1. printf
   ```c=
    int _write(int file, char *ptr, int len)
    {
	    MYUSART_SendData((uint8_t*)ptr, len);
	    MYUSART_SendData((uint8_t*)"\r", 1);
	    return len;
    }
    ```

2. scanf
   ```c=
    int _read(int file, char *ptr, int len) 
    {
	    for (int i = 0; i < len; i++) {
		    *ptr = (char)MYUSART_ReceiveData();
		    if(*ptr == '\r') break; /* read Enter */
		    MYUSART_SendData((uint8_t*)ptr++, 1);
	    }
	    MYUSART_SendData((uint8_t*)"\n\r", 2);
	    return len;
    }
   ```
   
### 使用步驟
> make
```shell
$ make
mkdir Debug
arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F303xE -IInc -O0 -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"Debug/main.d" -Wa,-a,-ad,-alms=Debug/main.lst ./Src/main.c -o Debug/main.o
arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F303xE -IInc -O0 -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"Debug/myusart.d" -Wa,-a,-ad,-alms=Debug/myusart.lst ./Src/myusart.c -o Debug/myusart.o
arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F303xE -IInc -O0 -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"Debug/syscalls.d" -Wa,-a,-ad,-alms=Debug/syscalls.lst ./Src/syscalls.c -o Debug/syscalls.o
arm-none-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F303xE -IInc -O0 -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"Debug/sysmem.d" -Wa,-a,-ad,-alms=Debug/sysmem.lst ./Src/sysmem.c -o Debug/sysmem.o
arm-none-eabi-gcc -x assembler-with-cpp -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32F303xE -IInc -O0 -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF"Debug/startup_stm32f303zetx.d" ./Startup/startup_stm32f303zetx.s -o Debug/startup_stm32f303zetx.o
arm-none-eabi-gcc Debug/main.o Debug/myusart.o Debug/syscalls.o Debug/sysmem.o   Debug/startup_stm32f303zetx.o   -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -specs=nano.specs -TSTM32F303ZETX_FLASH.ld  -lc -lm -lnosys -Wl,-Map=Debug/main.map,--cref -Wl,--gc-sections -o Debug/main.elf
arm-none-eabi-size Debug/main.elf
   text    data     bss     dec     hex filename
   4196     108    1588    5892    1704 Debug/main.elf
```
  
> make upload
```shell
$ make upload
openocd -f interface/stlink.cfg -f target/stm32f3x.cfg -c " program Debug/main.elf verify exit "
xPack OpenOCD x86_64 Open On-Chip Debugger 0.11.0+dev (2021-12-07-17:33)
Licensed under GNU GPL v2
For bug reports, read
        http://openocd.org/doc/doxygen/bugs.html
Info : auto-selecting first available session transport "hla_swd". To override use 'transport select <transport>'.
Info : The selected transport took over low-level target control. The results might differ compared to plain JTAG/SWD
Info : DEPRECATED target event trace-config; use TPIU events {pre,post}-{enable,disable}
Info : clock speed 1000 kHz
Info : STLINK V2J38M27 (API v2) VID:PID 0483:374B
Info : Target voltage: 3.245115
Info : stm32f3x.cpu: Cortex-M4 r0p1 processor detected
Info : stm32f3x.cpu: target has 6 breakpoints, 4 watchpoints
Info : starting gdb server for stm32f3x.cpu on 3333
Info : Listening on port 3333 for gdb connections
Info : Unable to match requested speed 1000 kHz, using 950 kHz
Info : Unable to match requested speed 1000 kHz, using 950 kHz
target halted due to debug-request, current mode: Thread
xPSR: 0x01000000 pc: 0x08000508 msp: 0x20010000
Info : Unable to match requested speed 8000 kHz, using 4000 kHz
Info : Unable to match requested speed 8000 kHz, using 4000 kHz
** Programming Started **
Info : device id = 0x10036446
Info : flash size = 512kbytes
** Programming Finished **
** Verify Started **
** Verified OK **
shutdown command invoked

```

## 結果(在putty上顯示)
   ![](https://i.imgur.com/EfIvBQZ.png)

## [Eclipse: Error: init mode failed (unable to connect to the target)](https://stackoverflow.com/questions/57628401/eclipse-error-init-mode-failed-unable-to-connect-to-the-target)

送電異常導致Flash memory內部亂掉

[解決方法use IDE](https://ithelp.ithome.com.tw/articles/10280961)
- Use [STLinkUtility](https://www.st.com/en/development-tools/stsw-link004.html)
- Use [Linux](https://www.gushiciku.cn/pl/aCuQ/zh-tw)

開啟STLinkUtility

>按下插頭連接板子
![](https://i.imgur.com/JK9piLH.png)

如果無法連接
**Target -> Settings -> Mode -> Connect under reset.**
再次連接

<font color=#ff00>**Utility 開著的時候不能upload**</font>

>橡皮擦清除Flash memory即可
![](https://i.imgur.com/0hZH7Xa.png)
