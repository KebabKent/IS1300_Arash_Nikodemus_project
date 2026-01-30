/**
  ******************************************************************************
  * @file    test_Potentiometer_Controller.c
  * @author  [Your Name]
  * @version 1.0
  * @date    [Current Date]
  * @brief   Integration test for Potentiometer and PWM logic.
  *
  * This file contains the test runner to verify that the ADC correctly reads
  * the potentiometer value and updates the Timer PWM duty cycle.
  ******************************************************************************
  */
#include "Controller/test_Potentiometer_Controller.h"
#include "Controller/Potentiometer_Controller.h"
#include "spi.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim3;

/**
 * @brief Integration Test for Potentiometer Dimming.
 *
 * This function initiates the test sequence.
 * @return void
 */
void Test_Potentiometer(void) {
	test_Read_Potentiometer();
}

/**
 * @brief Loop Test for Visual Verification.
 *
 * This function loops for 10 seconds. During this time:
 * 1. It calls the real Read_Potentiometer() function.
 * 2. That function reads the ADC and updates the PWM timer.
 * 3. You should manually turn the knob and watch the LED brightness change.
 *
 * @return void
 */
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
