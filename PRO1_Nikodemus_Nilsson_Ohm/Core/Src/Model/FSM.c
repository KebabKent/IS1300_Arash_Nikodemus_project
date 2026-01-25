#include <DTO/input_state.h>
#include <DTO/lights_state.h>

#include "stdint.h"
#include "stdbool.h"
#include "main.h"
#include "Model/traffic_state.h"

void readAndSet(void) {
	LightsState_t* lightsState = Return_LightsState();
	InputState_t* inputState = Return_InputState();

	if (inputState->Button_Pressed_Left) {
		lightsState->Horizontal_Traffic_Light_State.Pdst_State = 0;
		lightsState->Horizontal_Traffic_Light_State.toggle = true;
	}

	if (inputState->Car_Pesent_Left) {
		if (lightsState->Horizontal_Traffic_Light_State.Delays.pedestrianDelay > 0)
			lightsState->Horizontal_Traffic_Light_State.Delays.pedestrianDelay -= 1;
	}

}


