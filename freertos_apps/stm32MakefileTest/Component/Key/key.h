#ifndef __KEY__H
#define __KEY__H
#include"gpio.h"
#include"main.h"

#define Key_GPIO_Port GPIOE

#define Key_0	GPIO_PIN_4
#define Key_1	GPIO_PIN_3
#define Key_2	GPIO_PIN_2

#define ReadPin(pin) HAL_GPIO_ReadPin(Key_GPIO_Port, pin)

//define delay from HAL_Delay
#define delay HAL_Delay

//Key handle
void Key_2_Callback();
void Key_1_Callback();
void Key_0_Callback();

#endif
