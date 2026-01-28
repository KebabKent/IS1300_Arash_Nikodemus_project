#include "DTO/input_state.h"
#include "stdint.h"
#include "stdbool.h"
#include "main.h"
#include "Model/traffic_state.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim3;

void Read_Potentiometer() {
	uint32_t sum = 0;

	for (int i = 0; i < 5; i++) {
		HAL_ADC_Start(&hadc1);
		if (HAL_ADC_PollForConversion(&hadc1, 5) == HAL_OK) {
			sum += HAL_ADC_GetValue(&hadc1);
		}
	}

	HAL_ADC_Stop(&hadc1);

	uint32_t pwm_val = 4095 - (sum / 5);

	if (pwm_val > 4095) pwm_val = 4095;

	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pwm_val);
}
