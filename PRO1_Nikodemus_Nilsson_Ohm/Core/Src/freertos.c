/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "semphr.h"
#include "View/TrafficLight_SPI_Driver.h"
#include "View/OLED_View.h"
#include "Model/traffic_state.h"
#include "ssd1306.h"
#include "Model/traffic_state.h"
#include "Model/FSM.h"
#include "Controller/InputController.h"
#include "Controller/Potentiometer_Controller.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
SemaphoreHandle_t outputMutex;
SemaphoreHandle_t inputMutex;

extern TIM_HandleTypeDef htim3;
/* USER CODE END Variables */
/* Definitions for FSM_Task */
osThreadId_t FSM_TaskHandle;
const osThreadAttr_t FSM_Task_attributes = {
  .name = "FSM_Task",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityHigh1,
};
/* Definitions for Input_Task */
osThreadId_t Input_TaskHandle;
const osThreadAttr_t Input_Task_attributes = {
  .name = "Input_Task",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal1,
};
/* Definitions for Output_Task */
osThreadId_t Output_TaskHandle;
const osThreadAttr_t Output_Task_attributes = {
  .name = "Output_Task",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};
/* Definitions for Poti_Task */
osThreadId_t Poti_TaskHandle;
const osThreadAttr_t Poti_Task_attributes = {
  .name = "Poti_Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal1,
};
/* Definitions for OLED_Task */
osThreadId_t OLED_TaskHandle;
const osThreadAttr_t OLED_Task_attributes = {
  .name = "OLED_Task",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void Read_Potentiometer(void);
void Set_TrafficLights(LightsState_t* lightsState);
void update_OLED(LightsState_t* lightsState);
void readAndSet(InputState_t* input);
void readAndSetInputsState(void);
/* USER CODE END FunctionPrototypes */

void StartFSM_Task(void *argument);
void StartInputTask(void *argument);
void StartOutputTask(void *argument);
void StartPotiTask(void *argument);
void StartOLEDTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	State_Init();
	ssd1306_Init();
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */

  outputMutex = xSemaphoreCreateMutex();
  inputMutex = xSemaphoreCreateMutex();

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
  /* creation of FSM_Task */
  FSM_TaskHandle = osThreadNew(StartFSM_Task, NULL, &FSM_Task_attributes);

  /* creation of Input_Task */
  Input_TaskHandle = osThreadNew(StartInputTask, NULL, &Input_Task_attributes);

  /* creation of Output_Task */
  Output_TaskHandle = osThreadNew(StartOutputTask, NULL, &Output_Task_attributes);

  /* creation of Poti_Task */
  Poti_TaskHandle = osThreadNew(StartPotiTask, NULL, &Poti_Task_attributes);

  /* creation of OLED_Task */
  OLED_TaskHandle = osThreadNew(StartOLEDTask, NULL, &OLED_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartFSM_Task */
/**
  * @brief  Function implementing the FSM_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartFSM_Task */
void StartFSM_Task(void *argument)
{
  /* USER CODE BEGIN StartFSM_Task */
  /* Infinite loop */
	InputState_t inputState;

	for(;;)
	{
	  if (xSemaphoreTake(inputMutex, portMAX_DELAY) == pdTRUE)
	  {
		  inputState = *Return_InputState();
		  xSemaphoreGive(inputMutex);
	  }

	  if (xSemaphoreTake(outputMutex, portMAX_DELAY) == pdTRUE)
	  {
		  readAndSet(&inputState);
		  xSemaphoreGive(outputMutex);
	  }

	  osDelay(10);
  }
  /* USER CODE END StartFSM_Task */
}

/* USER CODE BEGIN Header_StartInputTask */
/**
* @brief Function implementing the Input_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartInputTask */
void StartInputTask(void *argument)
{
  /* USER CODE BEGIN StartInputTask */
  for(;;)
  {
	  if (xSemaphoreTake(inputMutex, portMAX_DELAY) == pdTRUE)
	  {
		  readAndSetInputsState();
		  xSemaphoreGive(inputMutex);
	  }

	  osDelay(10);
  }
  /* USER CODE END StartInputTask */
}

/* USER CODE BEGIN Header_StartOutputTask */
/**
* @brief Function implementing the Output_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartOutputTask */
void StartOutputTask(void *argument)
{
  /* USER CODE BEGIN StartOutputTask */
  /* Infinite loop */
	LightsState_t lightsState;

	HAL_GPIO_WritePin(SR_Reset_GPIO_Port, SR_Reset_Pin, GPIO_PIN_SET);

	for(;;)
	{
		if (xSemaphoreTake(outputMutex, portMAX_DELAY) == pdTRUE)
		{
			lightsState = *Return_LightsState();
			xSemaphoreGive(outputMutex);
		}

		Set_TrafficLights(&lightsState);

		osDelay(100);
	}
  /* USER CODE END StartOutputTask */
}

/* USER CODE BEGIN Header_StartPotiTask */
/**
* @brief Function implementing the Poti_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPotiTask */
void StartPotiTask(void *argument)
{
  /* USER CODE BEGIN StartPotiTask */
  /* Infinite loop */
	for(;;)
	{
		Read_Potentiometer();
	  osDelay(100);
	}
  /* USER CODE END StartPotiTask */
}

/* USER CODE BEGIN Header_StartOLEDTask */
/**
* @brief Function implementing the OLED_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartOLEDTask */
void StartOLEDTask(void *argument)
{
  /* USER CODE BEGIN StartOLEDTask */
	/* Infinite loop */
	LightsState_t lightsState;

	for(;;)
	{
		if (xSemaphoreTake(outputMutex, portMAX_DELAY) == pdTRUE)
		{
			lightsState = *Return_LightsState();
			xSemaphoreGive(outputMutex);
		}

		update_OLED(&lightsState);

		osDelay(20);
	}
  /* USER CODE END StartOLEDTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

