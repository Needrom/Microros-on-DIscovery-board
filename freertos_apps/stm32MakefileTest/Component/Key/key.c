#include"key.h"

/** GPIO中断回调
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
uint8_t key_0_State = 0;
uint8_t key_1_State = 0;
uint8_t key_2_State = 0;

void __weak Key_2_Callback(){
	
}

void __weak Key_1_Callback(){

}

void __weak Key_0_Callback(){

}

void Key_CallBack(uint16_t GPIO_Pin)
{
    HAL_Delay(100);      
    switch(GPIO_Pin)
    {
        case GPIO_PIN_0:
//            if(WK_UP==1) 
//            {
//				LED1=!LED1;				
//				LED0=!LED1;
//            }
            break;
        case GPIO_PIN_2:
            if(ReadPin(Key_2) == 0)  
	    {
                Key_2_Callback();
		//printf("Pin 2 Down \r\n");   
            }
            break;
        case GPIO_PIN_3:
            if(ReadPin(Key_1)==0)  
	    {
		Key_1_Callback();
               	//printf("Pin 3 Down \r\n");
            }
            break;
        case GPIO_PIN_4:
            if(ReadPin(Key_0)==0)  
            {
		Key_0_Callback();
		//printf("Pin 4 Down \r\n");           
	    }
            break;
    }
}
