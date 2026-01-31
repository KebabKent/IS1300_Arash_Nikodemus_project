#include "Controller/test_InputController.h"
#include "Controller/InputController.h"
#include "stdint.h"
#include "stdbool.h"
#include "spi.h"
#include "main.h"
#include "Model/traffic_state.h"

void Test_Input(void) {
	test_readMomentaryButton();
	test_readToggleSwitch();
	test_readAndSetInputsState();
}

void test_readMomentaryButton() {
    static uint8_t lastBtnLeft = GPIO_PIN_SET;
    static uint8_t lastBtnUp   = GPIO_PIN_SET;

    bool btnLeft = false;
    bool btnUp = false;

    uint8_t dataOff[3] = {0,0,0};
    uint8_t dataOn[3] = {0,0,0x1};

    uint32_t startTime = HAL_GetTick();
    const uint32_t testDuration = 10000;

    while ((HAL_GetTick() - startTime) < testDuration) {
        readMomentaryButton(PL1_Switch_GPIO_Port, PL1_Switch_Pin, &lastBtnLeft, &btnLeft);
        readMomentaryButton(PL2_Switch_GPIO_Port, PL2_Switch_Pin, &lastBtnUp, &btnUp);

        if (btnLeft || btnUp) {
            HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);
            HAL_SPI_Transmit(&hspi3, dataOn, 3, 100);
            HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_SET);

            HAL_Delay(200);

            HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);
            HAL_SPI_Transmit(&hspi3, dataOff, 3, 100);
            HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_SET);
        }
        HAL_Delay(10);
    }
}

void test_readToggleSwitch() {
    uint8_t data[3] = {0,0,0};

    bool carLeft = false;
	bool carUp = false;
	bool carRight = false;
	bool carDown = false;


    uint32_t startTime = HAL_GetTick();
    const uint32_t testDuration = 10000;

    while ((HAL_GetTick() - startTime) < testDuration) {
    	data[0] = 0;
    	data[1] = 0;
    	data[2] = 0;

    	readToggleSwitch(TL1_Car_GPIO_Port, TL1_Car_Pin, &carLeft);
    	readToggleSwitch(TL2_Car_GPIO_Port, TL2_Car_Pin, &carDown);
    	readToggleSwitch(TL3_Car_GPIO_Port, TL3_Car_Pin, &carRight);
    	readToggleSwitch(TL4_Car_GPIO_Port, TL4_Car_Pin, &carUp);

    	if (carLeft) {
    		data[2] |= 0x1;
    	}

    	if (carDown) {
			data[1] |= 0x1;
		}

    	if (carRight) {
			data[0] |= 0x1;
		}

    	if (carUp) {
			data[0] |= 0x8;
		}


		HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&hspi3, data, 3, 100);
		HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_SET);

        HAL_Delay(10);
    }

	data[0] = 0;
	data[1] = 0;
	data[2] = 0;


	HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, data, 3, 100);
	HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_SET);
}

/**
 * @brief Integration test for the high-level input controller and global state.
 * * This test verifies that readAndSetInputsState() correctly updates the global
 * InputState_t structure by reading physical GPIO pins (buttons and switches).
 * * Logic:
 * 1. Retrieves a pointer to the global InputState via Return_InputState().
 * 2. Runs a loop for 10 seconds where it polls inputs.
 * 3. Maps boolean flags from the global state to specific bits in a 3-byte
 * SPI buffer (data[0-2]).
 * 4. Transmits the buffer to Shift Registers to provide visual LED feedback.
 * * Visual Mapping:
 * - data[2] bit 3 (0x8): Pedestrian Left Button (Momentary)
 * - data[1] bit 3 (0x8): Pedestrian Up Button (Momentary)
 * - data[2] bit 0 (0x1): Car Present Left (Toggle)
 * - data[1] bit 0 (0x1): Car Present Down (Toggle)
 * - data[0] bit 0 (0x1): Car Present Right (Toggle)
 * - data[0] bit 3 (0x8): Car Present Up (Toggle)
 * * @note Momentary button LEDs (data[2]/data[1] bit 0x8) will only flash
 * briefly per press due to the edge-detection logic.
 */
void test_readAndSetInputsState() {
    InputState_t* state = Return_InputState();

    uint8_t data[3] = {0, 0, 0};
    uint32_t startTime = HAL_GetTick();
    const uint32_t testDuration = 10000;

    while ((HAL_GetTick() - startTime) < testDuration) {
        readAndSetInputsState();

        if (state->Button_Pressed_Left) data[2] |= 0x8;
        if (state->Button_Pressed_Up)   data[1] |= 0x8;

        if (state->Car_Pesent_Left)  data[2] |= 0x1;
        if (state->Car_Pesent_Down)  data[1] |= 0x1;
        if (state->Car_Pesent_Right) data[0] |= 0x1;
        if (state->Car_Pesent_Up)    data[0] |= 0x8;

        HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);
        HAL_SPI_Transmit(&hspi3, data, 3, 100);
        HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_SET);

        HAL_Delay(10);
    }


    data[0] = 0;
	data[1] = 0;
	data[2] = 0;

    HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi3, data, 3, 100);
    HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_SET);
}
