/*
 * retarget printf
 */

#include"main.h"
#include"usart.h"

int Retarget_Init(UART_HandleTypeDef *_huart);
int __io_putchar(int ch);
int _write(int file, char *ptr, int len);
