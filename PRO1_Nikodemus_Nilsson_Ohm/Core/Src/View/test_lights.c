/**
 * @file test_lights.c
 * @brief Test suite for Traffic Light SPI Driver verification.
 *
 * This file contains test functions designed to verify the correct operation
 * of the traffic light hardware driver. It follows the TDD approach
 * outlined in the course manual, ensuring individual driver components
 * (bit-shifting logic) work before integration.
 *
 * @author [Your Name]
 * @version 1.0
 * @date [Current Date]
 */
#include "View/test_lights.h"
#include "View/TrafficLight_SPI_Driver.h"
#include "main.h"
#include "spi.h"

/**
 * @brief Master runner for all Traffic Light tests.
 *
 * Calls all individual unit and integration tests in sequence.
 * Intended to be run once at startup when RUN_TEST_PROGRAM is defined.
 *
 * @return void
 */
void Test_Lights(void) {
	test_set_Traffic_Light();
	test_set_Pedestrian_Light();
	test_toggle_Pedestrian_Blue();
	test_Decode_State();
	test_Set_TrafficLights();

}

/**
 * @brief Verifies the set_Traffic_Light bit-shifting logic.
 *
 * Iterates through all possible Traffic Light states (RED, ORANGE, GREEN)
 * for multiple light instances. It manually constructs the SPI data buffer
 * and transmits it to the shift registers to allow visual verification
 * of the correct color sequence.
 *
 * @return void
 */
void test_set_Traffic_Light() {
	uint8_t data[3];
	TrafficLightState state[3] = {RED_Tr, ORANGE_Tr, GREEN_Tr};

	for (int j = 0; j < 3; j++) {
		data[0] = 0;
		data[1] = 0;
		data[2] = 0;

		set_Traffic_Light(&data[2], 0x1, 0, &state[j]);
		set_Traffic_Light(&data[1], 0x100, 8, &state[j]);
		set_Traffic_Light(&data[0], 0x10000, 16, &state[j]);
		set_Traffic_Light(&data[0], 0x80000, 16, &state[j]);


		HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&hspi3, data, 3, 100);
		HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_SET);

		HAL_Delay(1000);

	}
}

/**
 * @brief Verifies the set_Pedestrian_Light bit-shifting logic.
 *
 * Iterates through Pedestrian states (RED, GREEN) for multiple light
 * instances. Transmits data via SPI for visual verification that
 * the pedestrian LEDs light up correctly without affecting other bits.
 *
 * @return void
 */
void test_set_Pedestrian_Light() {
	uint8_t data[3];
	PedestrianLightState state[3] = {RED_Pd, GREEN_Pd};

	for (int j = 0; j < 2; j++) {
		data[0] = 0;
		data[1] = 0;
		data[2] = 0;

		set_Pedestrian_Light(&data[2], 0x8, 0, &state[j]);
		set_Pedestrian_Light(&data[1], 0x800, 8, &state[j]);

		HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&hspi3, data, 3, 100);
		HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_SET);

		HAL_Delay(1000);

	}
}

/**
 * @brief Verifies the pedestrian blue light toggling logic.
 *
 * Simulates a blinking sequence by calling toggle_Pedestrian_Blue
 * in a loop with delays. Visually confirms that the blue indicator
 * bit is set and cleared at the correct frequency.
 *
 * @return void
 */
void test_toggle_Pedestrian_Blue() {
	uint8_t data[3];
	uint16_t toggleFrequenzy = 500;
	bool toggle = true;

	for (int j = 0; j < 10; j++) {
		data[0] = 0;
		data[1] = 0;
		data[2] = 0;

		toggle_Pedestrian_Blue(&data[2], 0x20, 0, &toggle, &toggleFrequenzy);
		toggle_Pedestrian_Blue(&data[1], 0x2000, 8, &toggle, &toggleFrequenzy);

		HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&hspi3, data, 3, 100);
		HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_SET);

		HAL_Delay(500);
	}
}

/**
 * @brief Logic check for full state decoding.
 *
 * Verifies that the Decode_State function correctly converts a
 * complete LightsState_t model into the correct 3-byte SPI array.
 * This ensures no overlapping bits when multiple lights are active simultaneously.
 *
 * @return void
 */
void test_Decode_State() {
	LightsState_t test_state;
	uint8_t data[3];

	test_state.Horizontal_Traffic_Light_State.Light_State_LU = GREEN_Tr;
	test_state.Horizontal_Traffic_Light_State.Light_State_RD = GREEN_Tr;
	test_state.Vertical_Traffic_Light_State.Light_State_LU = GREEN_Tr;
	test_state.Vertical_Traffic_Light_State.Light_State_RD = GREEN_Tr;

	test_state.Horizontal_Traffic_Light_State.Pdst_State = RED_Pd;
	test_state.Vertical_Traffic_Light_State.Pdst_State = RED_Pd;
	test_state.Horizontal_Traffic_Light_State.toggle = false;
	test_state.Vertical_Traffic_Light_State.toggle = false;

	Decode_State(&test_state, data);

	HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, data, 3, 100);
	HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_SET);

	HAL_Delay(2000);
}

/**
 * @brief Integration test for the full hardware abstraction layer.
 *
 * Simulates real-world traffic scenarios (All Green, All Red, Pedestrian Blink)
 * using the high-level Set_TrafficLights function. This serves as the final
 * verification that the Model correctly controls the View (Hardware) before
 * the FSM takes over.
 *
 * @return void
 */
void test_Set_TrafficLights() {
	LightsState_t state;

	state.Horizontal_Traffic_Light_State.toggle = false;
	state.Vertical_Traffic_Light_State.toggle = false;

	state.Horizontal_Traffic_Light_State.Light_State_LU = GREEN_Tr;
	state.Horizontal_Traffic_Light_State.Light_State_RD = GREEN_Tr;
	state.Vertical_Traffic_Light_State.Light_State_LU = GREEN_Tr;
	state.Vertical_Traffic_Light_State.Light_State_RD = GREEN_Tr;
	state.Horizontal_Traffic_Light_State.Pdst_State = GREEN_Pd;
	state.Vertical_Traffic_Light_State.Pdst_State = GREEN_Pd;

	Set_TrafficLights(&state);
	HAL_Delay(2000);

	state.Horizontal_Traffic_Light_State.Light_State_LU = RED_Tr;
	state.Horizontal_Traffic_Light_State.Light_State_RD = RED_Tr;
	state.Vertical_Traffic_Light_State.Light_State_LU = RED_Tr;
	state.Vertical_Traffic_Light_State.Light_State_RD = RED_Tr;
	state.Horizontal_Traffic_Light_State.Pdst_State = RED_Pd;
	state.Vertical_Traffic_Light_State.Pdst_State = RED_Pd;

	Set_TrafficLights(&state);
	HAL_Delay(2000);

	state.Horizontal_Traffic_Light_State.Pdst_State = GREEN_Pd;
	state.Vertical_Traffic_Light_State.Pdst_State = GREEN_Pd;

	state.Horizontal_Traffic_Light_State.toggle = true;
	state.Vertical_Traffic_Light_State.toggle = true;

	Set_TrafficLights(&state);
	HAL_Delay(2000);

}
