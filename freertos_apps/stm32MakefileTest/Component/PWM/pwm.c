#include"pwm.h"

void PWM_Init(){

}

void PWM_SetMode(PWM_Mode_t _mode){
	if(_mode == PWM_OPM){
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
	} else if(_mode == PWM_NOR){
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
	}
}

