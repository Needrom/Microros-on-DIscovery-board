/*
 * retarget printf
 */

#include "retarget.h"

UART_HandleTypeDef * printf_uart = NULL;

int Retarget_Init(UART_HandleTypeDef *_huart){
	printf_uart = _huart;
}

int __io_putchar(int ch)
{
  uint8_t c[1];
  c[0] = ch & 0x00FF;
  if (printf_uart != NULL){
     HAL_UART_Transmit(printf_uart, &c[0], 1, 10);
  }
  return ch;
}

int _write(int file,char *ptr, int len)
{
  int DataIdx;
  for(DataIdx= 0; DataIdx< len; DataIdx++){
  __io_putchar(*ptr++);
  }
  return len;
}

