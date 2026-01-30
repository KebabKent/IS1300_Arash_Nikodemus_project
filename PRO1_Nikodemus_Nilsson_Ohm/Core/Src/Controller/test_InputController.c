#include "Controller/test_InputController.h"
#include "Controller/InputController.h"
#include "stdint.h"

void Test_Input(void) {
	test_readMomentaryButton();
	test_readToggleSwitch();
	test_readAndSetInputsState();
}

// readMomentaryButton(GPIO_TypeDef* Port, uint16_t Pin, uint8_t* lastState, bool* outFlag)
void test_readMomentaryButton() {
	InputState_t state;


	uint32_t startTime = HAL_GetTick();
	const uint32_t testDuration = 10000;


	while ((HAL_GetTick() - startTime) < testDuration) {

		HAL_Delay(100);
	}


    readMomentaryButton(PL1_Switch_GPIO_Port, PL1_Switch_Pin, &lastBtnLeft, &state->Button_Pressed_Left);
    readMomentaryButton(PL2_Switch_GPIO_Port, PL2_Switch_Pin, &lastBtnUp,   &state->Button_Pressed_Up);

}

// void readToggleSwitch(GPIO_TypeDef* Port, uint16_t Pin, bool* outState)
void test_readToggleSwitch() {

}

void test_readAndSetInputsState() {

}
