# cm4bm
The ARM Cortex-M4 processor bare metal project with UART implementation
- Device: [NUCLEO-F303ZE](https://www.st.com/en/evaluation-tools/nucleo-f303ze.html)

## Build
`cm4bm` dependents on some third-party packages to build code and burn into processor like [ARM GNU Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain) and [stlink](https://github.com/stlink-org/stlink). Therefore, before using the project, we need to install these packages.

Install [ARM GNU Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain):
```shell
$ sudo apt-get update
$ sudo apt-get -y install gcc-arm-none-eabi
```

Install [stlink](https://github.com/stlink-org/stlink):
```shell
$ sudo apt-get update
$ sudo apt-get -y install stlink-tools
```

Install [OpenOCD](https://openocd.org/):
```shell
$ sudo apt install openocd
```

Build `cm4bm`:
```shell
$ make
```

Burn the code into processor:
```shell
$ make upload
```

Generate the disassembly file:
```shell
$ make disassembly
```

## Debug
The toolchain downloaded using the command `sudo apt-get -y install gcc-arm-none-eabi` does not include `arm-none-eabi-gdb`. Therefore, it needs to be added separately, which can be downloaded from [Arm GNU Toolchain Downloads](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads). Then. adding `arm-none-eabi-gdb` into path `/usr/bin/`.

In `cm4bm`, it uses [cortex-debug](https://github.com/Marus/cortex-debug) to trace the source code and find the bugs. Before using cortex-debug, the following libraries that are necessaty for `arm-none-eabi-gdb` need to be installed.
```shell
$ sudo apt-get install libncursesw5
$ sudo apt install python3.8
```

## Test
Install [putty](https://www.putty.org/) to communicate with the board:
```shell
$ sudo apt-get install putty
```

## GPIO setting
Set `PD8` and `PD9` as USART3 Tx and USART3 Rx respectively.

- `PD8` setup:
   | GPIO Setting  | Mode                    |
   | ------------- | ----------------------  |
   | GPIOD_MODE    | Alternate function mode |
   | GPIOD_OSPEEDR | Low Speed               |
   | GPIOD_PUPDR   | No Pull-up & Pull-down  |
   | GPIOD_OTYPER  | Push-Pull               |
   | GPIOD_AFRH    | AF7 (USART3 Tx)         |
-  `PD9` setup:
   | GPIO Setting  | Mode                    |
   | ------------- | ----------------------  |
   | GPIOD_MODE    | Alternate function mode |
   | GPIOD_OSPEEDR | Low Speed               |
   | GPIOD_PUPDR   | No Pull-up & Pull-down  |
   | GPIOD_OTYPER  | Push-Pull               |
   | GPIOD_AFRH    | AF7 (USART3 Rx)         |

## USART setup
| `USART3` config | Setting |
| ------------------ | ------- |
| F_CLK              | 8MHz (STM32F303ZE default clock) |
| USART3_Mode        | Tx/Rx Enable   |
| USART3_Parity      | Parity disable |
| USART3_StopBits    | 1     |
| USART3_WordLen     | 8     |
| USART3_Baud        | 38400 |
| USART3_HW_FLOW     | None  |
| Oversampling       | 16    |

## Reference
- [First time to use ARM Cortex-M4 ](https://hackmd.io/@Risheng/cm4bm)
- [STM32F303xB/C/D/E, STM32F303x6/8, STM32F328x8, STM32F358xC, STM32F398xE Reference manual](https://www.st.com/resource/en/reference_manual/dm00043574-stm32f303xb-c-d-e-stm32f303x6-8-stm32f328x8-stm32f358xc-stm32f398xe-advanced-arm-based-mcus-stmicroelectronics.pdf)
- [Cortex-M4 Technical Reference Manual](https://developer.arm.com/documentation/100166/0001/)
- [ARM® v7-M Architecture Reference Manual](https://developer.arm.com/documentation/ddi0403/latest/)
