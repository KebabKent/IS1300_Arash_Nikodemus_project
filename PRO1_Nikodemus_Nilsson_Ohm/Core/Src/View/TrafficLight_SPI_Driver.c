/**
******************************************************************************
* @file    TrafficLight_SPI_Driver.c
* @author  [Your Name]
* @version 1.0
* @date    [Current Date]
* @brief   Hardware Abstraction Layer for Traffic Light Shift Registers.
*
* This file handles the translation of the abstract "System Model" (States)
* into concrete "View" data (Bytes/Bits) that are sent to the hardware
* via SPI. It manages the specific mapping of which LED corresponds to
* which bit in the Shift Register chain.
******************************************************************************
*/
#include <DTO/lights_state.h>
#include "stdint.h"
#include "stdbool.h"
#include "main.h"
#include "View/TrafficLight_SPI_Driver.h"
#include "Model/traffic_state.h"
#include "spi.h"

/**
 * @brief Sets the bits for a standard Traffic Light (Red/Yellow/Green).
 *
 * Maps the abstract state (RED, ORANGE, GREEN) to the specific bits in the
 * byte array. It uses bit-shifting to select the correct color based on
 * the base position of the light.
 *
 * @param data  Pointer to the 3-byte SPI data array.
 * @param bit   The base bitmask for the Red light (e.g., 0x1).
 * @param ofst  The bit offset to shift the mask down to fit into the byte.
 * @param state Pointer to the current state of the traffic light (0=Red, 1=Orange, 2=Green).
 * @return void
 */
void set_Traffic_Light(uint8_t* data, uint32_t bit, uint8_t ofst, TrafficLightState* state) {
	if (*state == 0) {
		*data |= bit >> ofst;
	} else if (*state == 1) {
		*data |= (bit << 1) >> ofst;
	} else {
		*data |= (bit << 2) >> ofst;
	}
}

/**
 * @brief Sets the bits for a Pedestrian Light (Red/Green).
 *
 * Similar to the traffic light function but handles only two states.
 *
 * @param data  Pointer to the 3-byte SPI data array.
 * @param bit   The base bitmask for the Red light.
 * @param ofst  The bit offset to shift the mask.
 * @param state Pointer to the pedestrian light state (0=Red, 1=Green).
 * @return void
 */
void set_Pedestrian_Light(uint8_t* data, uint32_t bit, uint8_t ofst, PedestrianLightState* state) {
	if (*state == 0) {
		*data |= bit >> ofst;
	} else {
		*data |= (bit << 1) >> ofst;
	}
}

/**
 * @brief Toggles a specific bit to blink the Pedestrian Blue indicator.
 *
 * Uses the system tick (HAL_GetTick) to determine if the light should be
 * On or Off based on the toggle frequency.
 *
 * @param data            Pointer to the 3-byte SPI data array.
 * @param bit             The bitmask for the blue light.
 * @param ofst            The bit offset to shift the mask.
 * @param state           Pointer to the boolean trigger (true = blinking enabled).
 * @param toggleFrequenzy Pointer to the blinking speed in milliseconds.
 * @return void
 */
void toggle_Pedestrian_Blue(uint8_t* data, uint32_t bit, uint8_t ofst, bool* state, uint16_t* toggleFrequenzy) {
	if (*state == true) {
		if ((HAL_GetTick() / *toggleFrequenzy) % 2 == 0) {
			*data |= (bit >> ofst);
		}
	}
}

/**
 * @brief Decodes the entire System State into raw SPI bytes.
 *
 * This function acts as the "Mapper." It takes the high-level state of
 * every traffic light, pedestrian light, and blinking indicator, and
 * places the correct bits into the 3-byte array that represents the
 * physical Shift Registers.
 *
 * @param state Pointer to the global LightsState_t model.
 * @param data  Pointer to the 3-byte array to be filled.
 * @return void
 */
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

/**
 * @brief Transmits the current state to the hardware via SPI.
 *
 * This is the main interface function for the View task. It calls the
 * decoder to get the raw bytes, then manages the Latch pin (STCP) and
 * SPI transmission to update the physical LEDs.
 *
 * @param lightsState Pointer to the global state model.
 * @return void
 */
void Set_TrafficLights(LightsState_t* lightsState) {
	uint8_t data[3];
	Decode_State(lightsState, data);

    HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi3, data, 3, 100);
    HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_SET);
}


