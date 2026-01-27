#include "DTO/input_state.h"
#include "stdint.h"
#include "stdbool.h"
#include "main.h"
#include "Model/traffic_state.h"

extern ADC_HandleTypeDef hadc1;

void Read_Potentiometer() {
	InputState_t* state = Return_InputState();
	uint32_t sum = 0;

	// Take 5 samples and average them
	for (int i = 0; i < 5; i++) {
		HAL_ADC_Start(&hadc1);
		if (HAL_ADC_PollForConversion(&hadc1, 5) == HAL_OK) {
			sum += HAL_ADC_GetValue(&hadc1);
		}
	}

	// Store the average (divide by 5)
	state->PotiValue = sum / 5;

	HAL_ADC_Stop(&hadc1);
}
