
#include "Controller/test_Potentiometer_Controller.h"
#include "Controller/Potentiometer_Controller.h"
#include "spi.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim3;


void Test_Potentiometer(void) {
	test_Read_Potentiometer();
}


void test_Read_Potentiometer(void) {
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

	uint32_t startTime = HAL_GetTick();
	const uint32_t testDuration = 10000;

	uint8_t data[3] = {0xFF, 0xFF, 0xFF};

	while ((HAL_GetTick() - startTime) < testDuration) {
		Read_Potentiometer();

		HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&hspi3, data, 3, 100);
		HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_SET);
		HAL_Delay(100);
	}

	data[0] = 0;
	data[1] = 0;
	data[2] = 0;


	HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, data, 3, 100);
	HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_SET);

	uint32_t standardStrength = 0;
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, standardStrength);

}
