#include"menu.h"
#include"../PWM/pwm.h"
#include"../Key/key.h"
#include"../A6_X4connector/A6_X4connector.h"
#include"tim.h"

int dir = 0;
int mode = 0;
int period = 0;
int startFlag = 0;
//mode Select
//
//@brief: mode 0 NOR, mode 1 OPM
void Key_2_Callback(){
	if(mode == 0){
		dir = (dir + 1) % 2;
		X4_MotorDir(dir);
	} else {
		PWM_LocationSet(5000);
	}	
}

//Freq Change
void Key_1_Callback(){
	period += 100;
	if(period > 1000){
		period = 200;
	}
	TIM9->ARR = period;
}

void Key_0_Callback(){
	startFlag = (startFlag + 1) % 2;
	if(startFlag == 0){
		HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
	} else {
		HAL_TIM_PWM_Stop(&htim9, TIM_CHANNEL_1);
	}
}
