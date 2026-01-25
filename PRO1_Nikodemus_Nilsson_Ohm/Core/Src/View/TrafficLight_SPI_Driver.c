#include <DTO/lights_state.h>
#include "stdint.h"
#include "stdbool.h"
#include "main.h"
#include "View/TrafficLight_SPI_Driver.h"

#include "spi.h"

//Deadbits: 6,7, 14,15, 22,23

//Left TL
//Led1 = 0x1
//Led2 = 0x2
//Led3 = 0x4

//Left PL
//Led13_14 = 0x8
//Led15_16 = 0x10
//Led17_18 = 0x20

//Down TL
//Led4 = 0x100
//Led5 = 0x200
//Led6 = 0x400

//Up PL
//Led19_20 = 0x800
//Led21_22 = 0x1 000
//Led23_24 = 0x2 000

//Right TL
//Led7 = 0x10 000
//Led8 = 0x20 000
//Led9 = 0x40 000

//Up TL
//Led10 = 0x80 000
//Led11 = 0x100 000
//Led12 = 0x200 000

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
		if ((HAL_GetTick() / *toggleFrequenzy) % 2 == 0) {
			*data |= (bit >> ofst);
		}
	}
}

spiData Decode_State(LightsState_t* state) {
	spiData temp;

	temp.data[0] = 0;
	temp.data[1] = 0;
	temp.data[2] = 0;

	// Horizontal
	set_Traffic_Light(&temp.data[2], 0x1, 0, &state->Horizontal_Traffic_Light_State.Light_State_LU);
	set_Traffic_Light(&temp.data[0], 0x10000, 16, &state->Horizontal_Traffic_Light_State.Light_State_RD);
	set_Pedestrian_Light(&temp.data[2], 0x8, 0, &state->Horizontal_Traffic_Light_State.Pdst_State);

	// Vertical
	set_Traffic_Light(&temp.data[0], 0x80000, 16, &state->Vertical_Traffic_Light_State.Light_State_LU);
	set_Traffic_Light(&temp.data[1], 0x100, 8, &state->Vertical_Traffic_Light_State.Light_State_RD);
	set_Pedestrian_Light(&temp.data[1], 0x800, 8, &state->Vertical_Traffic_Light_State.Pdst_State);

	toggle_Pedestrian_Blue(&temp.data[2], 0x20, 0, &state->Horizontal_Traffic_Light_State.toggle, &state->toggleFrequenzy);
	toggle_Pedestrian_Blue(&temp.data[1], 0x2000, 8, &state->Vertical_Traffic_Light_State.toggle, &state->toggleFrequenzy);

	return temp;
}


void Set_TrafficLights(LightsState_t* state) {
	spiData spiData;
	spiData = Decode_State(state);

    // 1. Lower Latch
    HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);

    // 2. Transmit 3 Bytes
    HAL_SPI_Transmit(&hspi3, spiData.data, 3, 100);

    // 3. Raise Latch
    HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_SET);
}


