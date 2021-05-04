/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include "allocators.h"
#include "cmsis_os.h"

#include <rcl/rcl.h>
#include <rmw_microxrcedds_c/config.h>
#include <ucdr/microcdr.h>
#include <uxr/client/client.h>
#include <lwip.h>
#include <stdio.h>

#include <rmw_microxrcedds_c/config.h>
#include "microros_transports.h"
#include "usart.h"
#include "retarget.h"
#include "../../Component/Key/key.h"
#include "../../Component/PWM/pwm.h"
#include "../../Component/Motor_App/motor_app.h"

#include "tim.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
void CheckGPIO();
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern struct netif gnetif;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1500 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);
  bool availableNetwork = false;

#ifdef RMW_UXRCE_TRANSPORT_CUSTOM 
  availableNetwork = true; 
  rmw_uros_set_custom_transport( 
    true, 
    (void *) &huart1, 
    freertos_serial_open, 
    freertos_serial_close, 
    freertos_serial_write, 
    freertos_serial_read); 
  printf("Init uxr serial \r\n");
#elif defined(RMW_UXRCE_TRANSPORT_UDP)
//#elif defined(MICRO_XRCEDDS_UDP)
  printf("Ethernet Initialization \r\n");

  printf("Waiting for IP \r\n");
  int retries = 0;
  
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);

  while(gnetif.ip_addr.addr == 0 && retries < 10){
  	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);
	osDelay(500);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET);
	retries++;
  };

  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET);

  availableNetwork = (gnetif.ip_addr.addr != 0);
  if(availableNetwork){
  	printf("IP: %s \r\n", ip4addr_ntoa(&(gnetif.ip_addr)));
  } else {
  	printf("Impossible to retrieve an IP \n");
  }
#endif
  printf("endif----- \r\n");

  rcl_allocator_t freeRTOS_allocator = rcutils_get_zero_initialized_allocator();
  freeRTOS_allocator.allocate = __freertos_allocate;
  freeRTOS_allocator.deallocate = __freertos_deallocate;
  freeRTOS_allocator.reallocate = __freertos_reallocate;
  freeRTOS_allocator.zero_allocate = __freertos_zero_allocate;
  
  if(!rcutils_set_default_allocator(&freeRTOS_allocator)){
  	printf("Error on default allocators (line %d)\n", __LINE__);
  }
  else {
  	printf("freeRTOS allocator success \r\n");
  }

  /* the second thread */
  osThreadAttr_t attributes_second;
  memset(&attributes_second, 0x0, sizeof(osThreadAttr_t));
  attributes_second.name = "motor_app";
  attributes_second.stack_size = 5000;
  attributes_second.priority = (osPriority_t)osPriorityNormal;
  osThreadNew(Motor_App, NULL, &attributes_second);
  osDelay(500);

  osThreadAttr_t attributes;
  memset(&attributes, 0x0, sizeof(osThreadAttr_t));
  attributes.name = "microROS_app";
  attributes.stack_size = 25000;
  attributes.priority = (osPriority_t)osPriorityNormal1;
  printf("before appMain \r\n");
  osThreadNew(appMain, NULL, &attributes);
//  Retarget_Init(&huart1);
  osDelay(500);
  char ptrTaskList[500];
  vTaskList(ptrTaskList);
  printf("**********************************\n");
  printf("Task  State   Prio    Stack    Num\n");
  printf("**********************************\n");
  printf(ptrTaskList);
  printf("**********************************\n");

  TaskHandle_t xHandle;
  xHandle = xTaskGetHandle("microROS_app");

  PWM_SetMode(PWM_NOR); 
  
  HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);

  int eRunningStateCount = 0;
  int eSuspendedStateCount = 0;
  int eBlockedStateCount = 0;
  for(;;)
  {
//    printf("into the printf \r\n");
     
      //GPIO test in CheckGPIO();
    osDelay(100);
//    CheckGPIO();
  //Check for the Task detail.
//  osDelay(3000);
//  PrintfTaskList(ptrTaskList);

    //Check eTaskGetState
//    if(eTaskGetState(xHandle) == eRunning){
//    	//counting running
//	eRunningStateCount++;
//    } else if(eTaskGetState(xHandle) == eSuspended){
//    	eSuspendedStateCount++;
//    } else if(eTaskGetState(xHandle) == eBlocked){
//    	eBlockedStateCount++;
//    }
//

//    char *timeRunStat[500];
//    if(timerFlag == 1){
//    	timerFlag = 0;
//	vTaskGetRunTimeStats(timeRunStat);
//    	printf("task \t\t time \t\t duty \t\t");
//	printf("%s \r\n", timeRunStat);
//    }
//

//    if (eTaskGetState(xHandle) != eSuspended && availableNetwork && eTaskGetState(xHandle) != eRunning)
//    {
//    	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);    
//	osDelay(150);
//	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET);
//	osDelay(150);	
//	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);    
//	osDelay(150);
//	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET);
//	osDelay(150);
//    } else {
//        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);    
//	osDelay(1000);
//	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_RESET);
//	osDelay(1000);	
//	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);    
//	osDelay(1000);
//	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET);
//	osDelay(1000);
//    } 


  }

  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )

{

    printf("Task %s stack OverFlow \r\n", pcTaskName);

}

int PrintfTaskList(char * _ptrTaskList){
  
  if(_ptrTaskList == NULL){
  	return -1;
  }
	
  osDelay(3000);
  vTaskList(_ptrTaskList);
  printf("**********************************\n");
  printf("Task  State   Prio    Stack    Num\n");
  printf("**********************************\n");
  printf(_ptrTaskList);
  printf("**********************************\n");

  return 0;
}

void CheckGPIO(){
	//printf("PE2: %d \r\n", HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2));
	//printf("PE3: %d \r\n", HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3));
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, GPIO_PIN_SET);
	osDelay(1000);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_2, GPIO_PIN_RESET);
	osDelay(1000);

}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
