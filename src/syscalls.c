/**
 * @file  syscalls.c
 * @brief system calls file
 */

#include <sys/stat.h>
#include "usart.h"
extern int __io_putchar(int ch) __attribute__((weak));
extern int __io_getchar(void) __attribute__((weak));

int _write(int file, char *ptr, int len)
{
    USART_SendData((uint8_t *) ptr, len);
    USART_SendData((uint8_t *) "\r", 1);
    return len;
}

int _read(int file, char *ptr, int len)
{
    for (int i = 0; i < len; i++) {
        *ptr = (char) USART_ReceiveData();
        /* read Enter */
        if (*ptr == '\r')
            break;
        USART_SendData((uint8_t *) ptr++, 1);
    }
    USART_SendData((uint8_t *) "\n\r", 2);
    return len;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

int _close(int file)
{
    return -1;
}

int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    return 1;
}
