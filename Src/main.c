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
 * uint32_t *pClkCtrlreg = (uint32_t)0x40021014U; // p clock control register
 * uint32_t *pPortModeReg = (uint32_t)0x48000400U;
 * uint32_t *pPortOutReg = (uint32_t)0x48000414U;
 */
// using define to make code clean
#define RCC_AHBENR *(volatile uint32_t *)(0x40021014U)	// AHB peripheral clock enable register, 0x40021000(RCC base address) + 0x14(offset)
#define GPIOB_MODER *(volatile uint32_t *)(0x48000400U) // GPIOB base address(0x48000400) + 0x00(offset)
#define GPIOC_MODER *(volatile uint32_t *)(0x48000800U) // GPIOC base address(0x48000000) + 0x00(offset)
#define GPIOB_ODR *(volatile uint32_t *)(0x48000414U)	// GPIOB base address + 0x14(offset)
#define GPIOC_IDR *(volatile uint32_t *)(0x48000810U)	// GPIOC base address + 0x10(offset)

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

	// 1. enable the clock for GPIOB and GPIOA peripheral in the AHB2
	RCC_AHBENR |= (1 << 18); // means	0x00040000U, set IOPB EN to 1(datasheet)
	RCC_AHBENR |= (1 << 19); // Open IOPC(Set IOPC EN to 1)
	// 2. configure the mode of the IO pin as output
	// Configuring PA0 as input, because we are reading data from it.
	// The LED is on pin7 -> confirm 14th -> 1, 15th -> 0
	// a. clear the 14th and 15th bit positions (CLEAR)
	GPIOB_MODER &= ~(3 << 14); // means 0xFFFF3FFFU, set 14th and 15th to 0 (datasheet)
							   // 3 means 11 in binary
							   // also can write ~(1 << 14) & ~(1 << 15)
	// b make 14th bit position as 1 (SET)
	GPIOB_MODER |= (1 << 14); // means 0x00004000U, set 14th to 1 (datasheet)

	GPIOC_MODER &= ~(3 << 13); // set MODER0 to 00 (input mode)

	while (1) { 
		// read datasheet and find out that GPIO only has 0~15th position, from 16th to 31th is reserved(useless).
		uint16_t pinStatus = (uint16_t)GPIOC_IDR & (1 << 13); // bt_press -> 2000, 0010 0000 0000 0000
		if (pinStatus) {
			// 3. SET 7th bit of the output data register to make I/O pin-7 HIGH
			GPIOB_ODR |= (1 << 7); // means 0x0080
			printf("LED ON\n");
		} else {
			// Turn the Led off
			GPIOB_ODR &= ~(1 << 7); // set the postion to 0
			printf("LED OFF\n");
		}
	}
	return 0;
}
