#ifndef __A6_X4CONNECTOR__H
#define __A6_X4CONNECTOR__H

#include"gpio.h"
#include"stdio.h"

#define X4_Connector_PortE	GPIOE

#define X4_Connector_Pin_6  GPIO_PIN_6
#define X4_Connector_Pin_4  GPIO_PIN_5
#define X4_Connector_Pin_31 GPIO_PIN_0
#define X4_Connector_Pin_6  GPIO_PIN_6

#define X4_Connector_PortF   	GPIOF

#define X4_Connector_Pin_37 GPIO_PIN_1
#define X4_Connector_Pin_35 GPIO_PIN_0
#define X4_Connector_Pin_9  GPIO_PIN_3
#define X4_Connector_Pin_8  GPIO_PIN_2

#define X4_Direction_Pin	X4_Connector_Pin_6
#define X4_PWM_Pin		X4_Connector_Pin_4
#define X4_NegativeForbit_Pin 	X4_Connector_Pin_8
#define X4_PostiveForbit_Pin 	X4_Connector_Pin_9
#define X4_ServoAlarmOutput_Pin X4_Connector_Pin_37
#define X4_ServoReadyOutput_Pin X4_Connector_Pin_35
#define X4_ServoAlarmClear_Pin  X4_Connector_Pin_31

//Define X4_Pin_Write and X4_Pin_Read
#define X4_Pin_Write HAL_GPIO_WritePin
#define X4_Pin_Read  HAL_GPIO_ReadPin

//A6 X4 Connector Struct Define
typedef struct{
	uint8_t ServoAlarmOutput;
	uint8_t ServoReadyOutput;	
}X4_Connector;

void X4_ConnectorInit(X4_Connector *__x4_connector);
void X4_ClearAlarm();
void X4_PostiveForbit();
void X4_NegativeForbit();
#endif 
