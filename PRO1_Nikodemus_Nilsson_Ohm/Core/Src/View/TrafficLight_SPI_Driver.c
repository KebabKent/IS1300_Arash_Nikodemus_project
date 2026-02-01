
#include <DTO/lights_state.h>
#include "stdint.h"
#include "stdbool.h"
#include "main.h"
#include "View/TrafficLight_SPI_Driver.h"
#include "Model/traffic_state.h"
#include "spi.h"


void set_Traffic_Light(uint8_t* data, uint32_t bit, uint8_t ofst, TrafficLightState* state) {
	if (*state == 0) {
		*data |= bit >> ofst;
	} else if (*state == 1) {
		*data |= (bit << 1) >> ofst;
	} else {
		*data |= (bit << 2) >> ofst;
	}
}


void set_Pedestrian_Light(uint8_t* data, uint32_t bit, uint8_t ofst, PedestrianLightState* state) {
	if (*state == 0) {
		*data |= bit >> ofst;
	} else {
		*data |= (bit << 1) >> ofst;
	}
}


void toggle_Pedestrian_Blue(uint8_t* data, uint32_t bit, uint8_t ofst, bool* state, uint16_t* toggleFrequenzy) {
	if (*state == true) {
		if ((HAL_GetTick() / (*toggleFrequenzy / 2)) % 2 == 0) {
			*data |= (bit >> ofst);
		}
	}
}


void Decode_State(LightsState_t* state, uint8_t* data) {
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;

	// Horizontal
	set_Traffic_Light(&data[2], 0x1, 0, &state->Horizontal_Traffic_Light_State.Light_State_LU);
	set_Traffic_Light(&data[0], 0x10000, 16, &state->Horizontal_Traffic_Light_State.Light_State_RD);
	set_Pedestrian_Light(&data[2], 0x8, 0, &state->Horizontal_Traffic_Light_State.Pdst_State);

	// Vertical
	set_Traffic_Light(&data[0], 0x80000, 16, &state->Vertical_Traffic_Light_State.Light_State_LU);
	set_Traffic_Light(&data[1], 0x100, 8, &state->Vertical_Traffic_Light_State.Light_State_RD);
	set_Pedestrian_Light(&data[1], 0x800, 8, &state->Vertical_Traffic_Light_State.Pdst_State);

	toggle_Pedestrian_Blue(&data[2], 0x20, 0, &state->Horizontal_Traffic_Light_State.toggle, &state->toggleFrequenzy);
	toggle_Pedestrian_Blue(&data[1], 0x2000, 8, &state->Vertical_Traffic_Light_State.toggle, &state->toggleFrequenzy);
}


void Set_TrafficLights(LightsState_t* lightsState) {
	uint8_t data[3];
	Decode_State(lightsState, data);

    HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi3, data, 3, 100);
    HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_SET);
}


