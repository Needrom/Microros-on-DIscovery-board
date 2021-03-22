#ifndef __PWM__H
#define __PWM__H

#include"tim.h"

#define PWM_TIM		TIM9

//define PWM struct

typedef enum{
	PWM_OPM = 0,
	PWM_NOR,
}PWM_Mode_t;

typedef struct{
	uint16_t mode;
	uint16_t rpm;
	uint16_t CPPM;
}PWM;

void PWM_Init();
void PWM_SetMode(PWM_Mode_t _mode);

#endif
