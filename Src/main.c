/**
 ******************************************************************************
 * @file    main.c
 * @author  Kuan-Di Jiang
 * @brief   This file is a example
 ******************************************************************************
 */

#include "myusart.h"
#include <stdio.h>
#include <stdint.h>

int main(void)
{
	MYUSART_Init();
	printf("Connect successful\n"); // needed slash n
	/** Control LED
	 *	Address of the clock control register (AHB2)
	 *  0x40021000(RCC base address) + 0x14(offset)
	 *  = 0x40021014
	 *  Address of the GPIOB mode register (used to control mode)
	 *  0x48000400(GPIOB base address) + 0x00(offset)
	 *  0x48000400
	 *
	 * Address of the GPIOB output data register (used to write)
	 * 0x48000400(GPIOB base address) + 0x14(offset)
	 * 0x48000414
	 *
	 */

	// select uint32_t because all peripheral register are of 32 bits.
	uint32_t *pClkCtrlreg = (uint32_t)0x40021014U; // p clock control register
	uint32_t *pPortModeReg = (uint32_t)0x48000400U;
	uint32_t *pPortOutReg = (uint32_t)0x48000414U;
	// 1. enable the clock for GPIOB peripheral in the AHB2
	*pClkCtrlreg |= 0x00040000U; // set IOPB EN to 1(datasheet)

	// 2. configure the mode of the IO pin as output
	// The LED is on pin7 -> confirm 14th -> 1, 15th -> 0
	// a. clear the 14th and 15th bit positions (CLEAR)
	*pPortModeReg &= 0xFFFF3FFFU; // set 14th and 15th to 0 (datasheet)
	// b make 14th bit position as 1 (SET)
	*pPortModeReg |= 0x00004000U; // set 14th to 1 (datasheet)

	// 3. SET 7th bit of the output data register to make I/O pin-7 HIGH
	*pPortOutReg |= 1 << 7; // 0x0080
	while (1)
	{
		printf("In while\n");
	}
	return 0;
}
