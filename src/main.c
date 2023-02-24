/**
 * @file    main.c
 * @brief   This file is a example
 */

#include <stdio.h>
#include "usart.h"

int main(void)
{
    USART_Init();
    int data;
    while (1) {
        printf("input a data!!\n");
        scanf("%d", &data);
        printf("data = %d\n", data);
        fflush(stdin); /* clear buffer */
    }
    return 0;
}