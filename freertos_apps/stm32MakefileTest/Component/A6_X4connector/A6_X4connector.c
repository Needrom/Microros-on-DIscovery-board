/*
 * Creator: Mego
 * Date: 20210316
 * Brief: API to connector the A6_X4connetor
 */

#include"A6_X4connector.h"

X4_Connector *x4_connector;

void X4_ConnectorInit(X4_Connector *__x4_connector){
	if(__x4_connector == NULL){
		return -1;
	}
	
	x4_connector = __x4_connector;
	
	return 0;
}

void X4_ConnectorState_Tmr(){
	
}

void X4_NegativeForbit(){
	X4_Pin_Write(GPIOE, X4_NegativeForbit_Pin, GPIO_PIN_SET);
} 

void X4_PostiveForbit(){
	X4_Pin_Write(GPIOE, X4_PostiveForbit_Pin, GPIO_PIN_SET);
}

void X4_ClearAlarm(){
	X4_Pin_Write(GPIOE, X4_ServoAlarmClear_Pin, GPIO_PIN_SET);
}

/* GPIO中断回调
 * @brief  EXTI line detection callbacks.
 * @param  GPIO_Pin: Specifies the pins connected EXTI line
 * @retval None
 */
void X4_ConnectorCallBack(uint16_t GPIO_Pin){
   
    if(X4_Connector_Pin_37 == GPIO_Pin){
    	HAL_Delay(10);
	if(HAL_GPIO_ReadPin(X4_Connector_PortF, X4_Connector_Pin_37) == GPIO_PIN_RESET){
		printf("pin 37 get signal \r\n");
	}
    }
    
    if(X4_Connector_Pin_35 == GPIO_Pin){
    	HAL_Delay(10);
	if(HAL_GPIO_ReadPin(X4_Connector_PortF, X4_Connector_Pin_37) == GPIO_PIN_RESET){
		printf("pin 35 get signal \r\n");
	}
    }
}
