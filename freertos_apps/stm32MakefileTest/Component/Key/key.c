#include"key.h"

/** GPIO中断回调
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
uint8_t key_0_State = 0;
uint8_t key_1_State = 0;
uint8_t key_2_State = 0;

void Key_CallBack(uint16_t GPIO_Pin)
{
	printf("GPIO_Pin : %d \r\n");
	if(Key_0 == GPIO_Pin){
		HAL_Delay(10);
		if(HAL_GPIO_ReadPin(Key_GPIO_Port, Key_0) == GPIO_PIN_RESET){
			//Button 0 Push Down
			printf("Key 0 Down \r\n");
		}
	}

	if(Key_1 == GPIO_Pin){
		HAL_Delay(10);
		if(HAL_GPIO_ReadPin(Key_GPIO_Port, Key_1) == GPIO_PIN_RESET){
			//Button 1 Push Down
			printf("Key 1 Down \r\n");
		}
	}

	if(Key_2 == GPIO_Pin){
		HAL_Delay(10);
		if(HAL_GPIO_ReadPin(Key_GPIO_Port, Key_2) == GPIO_PIN_RESET){
			//Button 2 Push Down
			printf("Key 2 Down \r\n");
		}
	}

	HAL_Delay(100);	
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
}
