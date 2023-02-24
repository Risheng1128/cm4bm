/**
 * @file    usart.h
 * @brief   This file contains some macros of GPIOD and USART3
 */

#pragma once
#include <stdint.h>

#define DEFAULT_F_CLK 8000000U
#define BAUDRATE_38400 38400U

/* AHB peripheral clock enable register */
#define RCC_AHBENR *(volatile uint32_t *) 0x40021014U
/* APB1 peripheral clock enable register */
#define RCC_APB1ENR *(volatile uint32_t *) 0x4002101CU

/* GPIO port mode register */
#define GPIOD_MODE *(volatile uint32_t *) 0x48000C00U
/* GPIO alternate function high register */
#define GPIOD_AFRH *(volatile uint32_t *) 0x48000C24U
/* control register 1 */
#define USART3_CR1 *(volatile uint32_t *) 0x40004800U
/* baudrate register */
#define USART3_BRR *(volatile uint32_t *) 0x4000480CU
/* Request register */
#define USART3_RQR *(volatile uint32_t *) 0x40004818U
/* Interrupt and status register */
#define USART3_ISR *(volatile uint32_t *) 0x4000481CU
/* Receive data register */
#define USART3_RDR *(volatile uint32_t *) 0x40004824U
/* Transmit data register */
#define USART3_TDR *(volatile uint32_t *) 0x40004828U

void USART_Init();
void USART_SendData(uint8_t *pTxBuffer, uint8_t len);
uint8_t USART_ReceiveData();