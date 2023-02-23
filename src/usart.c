/**
 * @file    myusart.c
 * @author  Ri-Sheng Chen
 * @brief   Functions to enable USART3 on STM32F303ZE
 *
 * @attention
 *   - PD8: USART3 Tx
 *   - PD9: USART3 Rx
 *
 *   ---------- GPIO Set ------------
 *   - GPIOD_MODE:      Alternate function mode
 *   - GPIOD_OSPEEDR:   Low Speed
 *   - GPIOD_PUPDR:     No Pull-up & Pull-down
 *   - GPIOD_OTYPER:    Push-Pull
 *   - GPIOD_AFRH:      AF7 (USART3 Tx/Rx)
 *
 *   ---------- USART3 Set ------------
 *   - F_CLK:           8MHz
 *   - USART3_Mode:     Tx/Rx Enable
 *   - USART3_Parity:   Parity disable
 *   - USART3_StopBits: 1
 *   - USART3_WordLen:  8
 *   - USART3_Baud:     38400
 *   - USART3_HW_FLOW:  None
 *   - Oversampling:    16
 */

#include "usart.h"

void USART_Init()
{
    /**************************** GPIO Set ****************************/
    /* GPIOD RCC enable */
    RCC_AHBENR |= 1 << 20;
    /* PD8 PD9 set alternate function mode */
    GPIOD_MODE &= ~(0xF << 16);
    GPIOD_MODE |= (0x2 << 16) | (0x2 << 18);
    /* PD8: USART3 Tx | PD9: USART3 Rx */
    GPIOD_AFRH &= ~(0xFF << 0);
    GPIOD_AFRH |= 0x77 << 0;

    /**************************** USART Set ****************************/
    /* USART RCC enable */
    RCC_APB1ENR |= 1 << 18;
    /* UE (USART Enable) set */
    USART3_CR1 |= 1 << 0;
    /* Enable Tx/Rx */
    USART3_CR1 |= (1 << 2) | (1 << 3);
    /* Baudrate setup */
    USART3_BRR = DEFAULT_F_CLK / BAUDRATE_38400;
}

void USART_SendData(uint8_t *pTxBuffer, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++) {
        /* Waiting for the transmit data register empty (bit 7 TXE) */
        while (!(USART3_ISR & 0x80))
            ;
        /* Send Data */
        USART3_TDR = (*pTxBuffer++ & 0xFF);
        /* Waiting for the transmit complete (bit 6 TC) */
        while (!(USART3_ISR & 0x40))
            ;
    }
}

uint8_t USART_ReceiveData()
{
    uint8_t data;
    /* Waiting for the transmit data transmit to USART_RDR register (USART_ISR
     * bit 5 RXNE) */
    while (!(USART3_ISR & 0x20))
        ;
    /* Receive Data */
    data = USART3_RDR;
    /* Clear RXNE by USARTx_RQR (bit 3 RXFRQ) */
    USART3_RQR |= 1 << 3;
    return data;
}
