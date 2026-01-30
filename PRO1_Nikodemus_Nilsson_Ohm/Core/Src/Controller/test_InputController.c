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

// readMomentaryButton(GPIO_TypeDef* Port, uint16_t Pin, uint8_t* lastState, bool* outFlag)
void test_readMomentaryButton() {
	static uint8_t lastBtnLeft = GPIO_PIN_SET;
	static uint8_t lastBtnUp   = GPIO_PIN_SET;

	bool btnLeft = false;
	bool btnUp = false;

	uint32_t startTime = HAL_GetTick();
	const uint32_t testDuration = 10000;


	while ((HAL_GetTick() - startTime) < testDuration) {
		readMomentaryButton(PL1_Switch_GPIO_Port, PL1_Switch_Pin, &lastBtnLeft, &btnLeft);
		readMomentaryButton(PL2_Switch_GPIO_Port, PL2_Switch_Pin, &lastBtnUp, &btnUp);
		HAL_Delay(100);
	}

	uint8_t data[3] = {0,0,0};

	if (btnLeft) data[2] = 0x8;
	if (btnLeft) data[1] = 0x800;


	HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, data, 3, 100);
	HAL_GPIO_WritePin(SR_STCP_GPIO_Port, SR_STCP_Pin, GPIO_PIN_SET);


}

// void readToggleSwitch(GPIO_TypeDef* Port, uint16_t Pin, bool* outState)
void test_readToggleSwitch() {

}

void test_readAndSetInputsState() {

}
