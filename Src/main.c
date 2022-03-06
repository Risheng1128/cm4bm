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

/* original setting
uint32_t *pClkCtrlreg = (uint32_t)0x40021014U; // p clock control register
uint32_t *pPortModeReg = (uint32_t)0x48000400U;
uint32_t *pPortOutReg = (uint32_t)0x48000414U;
*/
// using define to make code clean
#define RCC_AHBENR *(volatile uint32_t *)(0x40021014U)	// AHB peripheral clock enable register, 0x40021000(RCC base address) + 0x14(offset)
#define GPIOB_MODER *(volatile uint32_t *)(0x48000400U) // GPIOB base address + 0x00(offset)
#define GPIOB_ODR *(volatile uint32_t *)(0x48000414U)	// GPIOB base address + 0x14(offset)

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

	// 1. enable the clock for GPIOB peripheral in the AHB2
	RCC_AHBENR |= (1 << 18); // means	0x00040000U, set IOPB EN to 1(datasheet)

	// 2. configure the mode of the IO pin as output
	// The LED is on pin7 -> confirm 14th -> 1, 15th -> 0
	// a. clear the 14th and 15th bit positions (CLEAR)
	GPIOB_MODER &= (3 << 14); // means 0xFFFF3FFFU, set 14th and 15th to 0 (datasheet)
							  // 3 means 11 in binary
							  // also can write ~(1 << 14) & ~(1 << 15)
	// b make 14th bit position as 1 (SET)
	GPIOB_MODER |= (1 << 14); // means 0x00004000U, set 14th to 1 (datasheet)

	while (1)
	{
		// 3. SET 7th bit of the output data register to make I/O pin-7 HIGH
		GPIOB_ODR |= (1 << 7); // means 0x0080

		for (int i = 0; i < 1000000; i++)
			;

		// Turn the Led off
		GPIOB_ODR &= ~(1 << 7); // set the postion to 0

		for (int i = 0; i < 1000000; i++)
			;
	}
	return 0;
}
