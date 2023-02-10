/**
 * @file    main.c
 * @author  Ri-Sheng Chen
 * @brief   This file is a example
 */

#include <stdio.h>
#include "myusart.h"

int main(void)
{
    MYUSART_Init();
    int data;
    while (1) {
        printf("input a data!!\n");
        scanf("%d", &data);
        printf("data = %d\n", data);
        fflush(stdin); /* Claer buffer */
    }
    return 0;
}