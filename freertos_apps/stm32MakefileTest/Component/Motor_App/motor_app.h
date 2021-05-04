#ifndef __MOTOR_APP__H
#define __MOTOR_APP__H

#include "FreeRTOS.h"
#include "../PWM/pwm.h"
#include <math.h>
#include "tim.h"

#define REDUCTION_RARIO		120

#define Motor_SetLoc(x)		PWM_LocationSet(x)
#define Motor_SetDir(x)		X4_MotorDir(x)

///* ToDo: Not finish yet, Need to Finish compile. */
//typedef struct X4Unit{
//	double current_pos;
//	double exp;
//}X4Uint_t;


void Motor_App(void);
void Motor_SetExp(double _exp);
void Motor_GetPos(void);

#endif
