#ifndef __PWM__H
#define __PWM__H

#include"main.h"
#include"tim.h"

#define PWM_TIM		TIM9

#define delay_ms	HAL_Delay

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
void PWM_SpeedSet(uint32_t _rpm);
void PWM_TIM_SetPeriod(uint32_t _period);
void PWM_LocationSet(uint32_t _period);

#endif
