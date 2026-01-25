#include "DTO/input_state.h"
#include "stdint.h"
#include "stdbool.h"
#include "main.h"
#include "Model/traffic_state.h"


static uint8_t lastBtnLeft = GPIO_PIN_SET;
static uint8_t lastBtnUp   = GPIO_PIN_SET;


void readMomentaryButton(GPIO_TypeDef* Port, uint16_t Pin, uint8_t* lastState, bool* outFlag) {
    uint8_t currentState = HAL_GPIO_ReadPin(Port, Pin);
    *outFlag = false;

    if (*lastState == GPIO_PIN_SET && currentState == GPIO_PIN_RESET) {
        *outFlag = true;
    }
    *lastState = currentState;
}

void readToggleSwitch(GPIO_TypeDef* Port, uint16_t Pin, bool* outState) {
    if (HAL_GPIO_ReadPin(Port, Pin) == GPIO_PIN_RESET) {
        *outState = true;
    } else {
        *outState = false;
    }
}

// --- MAIN INPUT CONTROLLER ---
void readAndSetInputsState() {
	InputState_t* state;
	state = Return_InputState();

    readMomentaryButton(PL1_Switch_GPIO_Port, PL1_Switch_Pin, &lastBtnLeft, &state->Button_Pressed_Left);
    readMomentaryButton(PL2_Switch_GPIO_Port, PL2_Switch_Pin, &lastBtnUp,   &state->Button_Pressed_Up);

    readToggleSwitch(TL1_Car_GPIO_Port, TL1_Car_Pin, &state->Car_Pesent_Left);
    readToggleSwitch(TL2_Car_GPIO_Port, TL2_Car_Pin, &state->Car_Pesent_Down);
    readToggleSwitch(TL3_Car_GPIO_Port, TL3_Car_Pin, &state->Car_Pesent_Right);
    readToggleSwitch(TL4_Car_GPIO_Port, TL4_Car_Pin, &state->Car_Pesent_Up);
}
