#include"pwm.h"

uint32_t pwm_count = 0;

void PWM_Init(){

}

void PWM_SetMode(PWM_Mode_t _mode){
	if(_mode == PWM_OPM){
		Set_OnePulseMode();	
	} else if(_mode == PWM_NOR){
		Set_NormalMode();
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
	}
}

uint32_t PWM_LocationSet(uint32_t _period){
	/* test var here */
	uint32_t count = 0;
	/* test var end */
	for(int i = 0; i < _period; i++){
		__HAL_TIM_ENABLE(&htim9);
		HAL_TIM_OnePulse_Start(&htim9, TIM_CHANNEL_1);
		++count;
	}

	return count;
}

void PWM_SpeedSet(uint32_t _rpm){
	uint32_t freq = 10000 * _rpm;
	uint32_t arr = 84000000 * 84 * freq;
	TIM9->ARR = arr;
	HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
}

void PWM_TIM_SetPeriod(uint32_t _period){
	TIM9->ARR = _period;
}
