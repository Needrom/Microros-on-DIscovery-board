#include"motor_app.h"

double motor_pos = 0;
double motor_exp = 0;

void Motor_SetExp(double _exp){
	motor_exp = _exp;
}

double Motor_GetPos(){
	return motor_pos;
}

void Motor_App(){
	int motor_ret = 0;
	double motor_det = 0;

	PWM_SetMode(0);

	while(1){
		/* Don't forget to keep this delay for freertos */
		osDelay(1000);
	
		/* calculate how much pusle we can get to the expected pos. */	
		motor_det = motor_exp - motor_pos;
		printf("%s det now :%d exp: %d pos: %d \r\n", __func__, (int)(motor_det * 100), (int)(motor_exp * 100), (int)(motor_pos) * 100);
		if(0 == (int)motor_det){
			/* Not thing to do, so skip this step */
			continue;
		} else if((int)motor_det > 0) {
			/* Set motor direction positive. */
			Motor_SetDir((uint8_t)1);
		} else {
			/* Set motor direction negative. */
			Motor_SetDir((uint8_t)0);		
		}
		
		/* Send pusle to servo */	
		Generate_PWM((uint32_t)fabs(motor_det));
		motor_pos += motor_det;
	}
}

