# cm4bm
The ARM Cortex-M4 processor bare metal project with UART implementation
- Device: [NUCLEO-F303ZE](https://www.st.com/en/evaluation-tools/nucleo-f303ze.html)

## Build and Verify
`cm4bm` dependents on some third-party packages to build code and burn into processor like [ARM GNU Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain) and [stlink](https://github.com/stlink-org/stlink). Therefore, before using the project, we need to install these packages.

Install [ARM GNU Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain):
```shell
$ sudo apt-get update
$ sudo apt-get -y install gcc-arm-none-eabi
```

Install [stlink](https://github.com/stlink-org/stlink):
```shell
$ sudo apt-get install git cmake libusb-1.0-0-dev
$ git clone https://github.com/stlink-org/stlink && cd stlink
$ cmake .
$ make
$ cd bin && sudo cp st-* /usr/local/bin
$ cd ../lib && sudo cp *.so* /lib32
$ cd ../config/udev/rules.d/ && sudo cp 49-stlinkv* /etc/udev/rules.d/
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
In `cm4bm`, it uses [OpenOCD](https://openocd.org/) to open the debug mode of processor. We can use the following command:
```shell
$ make debug
```

After typing the command, the next step is opening gdb to connect to rhe openocd.
```shell
$ arm-none-eabi-gdb
```

Connect to the openocd:
```shell
$ target remote localhost:3333
```

## Test
Install [putty](https://www.putty.org/) to communicate with the board:
```shell
$ sudo apt-get install putty
```

Result:
```
Input a data!!
255
data = 255
Input a data!!
```

## GPIO setting
Set `PD8` and `PD9` as USART3 TX and USART3 TX respectively.

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
- [NUCLEO-F303ZE](https://www.st.com/en/evaluation-tools/nucleo-f303ze.html)
- [OpenOCD](https://openocd.org/)
- [stlink](https://github.com/stlink-org/stlink)
- [Use ARM Cortex-M4 on Linux](https://hackmd.io/@Risheng/rkVSOVLwF)