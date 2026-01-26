#include <DTO/input_state.h>
#include <DTO/lights_state.h>

#include "stdint.h"
#include "stdbool.h"
#include "main.h"
#include "Model/traffic_state.h"

uint8_t count = 0;
uint8_t count2 = 0;


void readAndSet(void) {
	LightsState_t* lightsState = Return_LightsState();
	InputState_t* inputState = Return_InputState();

	if (inputState->Button_Pressed_Left) {
		count++;

		if (count % 6 == 0) {
			Set_Pl_StatePassiveUp();
		} else if (count % 6 == 1) {
			Set_Pl_StateWaitingUp();
		} else if (count % 6 == 2) {
			Set_Pl_StateWalkingUp();
		} else if (count % 6 == 3) {
			Set_Pl_StatePassiveLeft();
		} else if (count % 6 == 4) {
			Set_Pl_StateWaitingLeft();
		} else if (count % 6 == 5) {
			Set_Pl_StateWalkingLeft();
		}
	}

	if (inputState->Button_Pressed_Up) {
		count2++;

		if (count2 % 5 == 0) {
			Set_Tl_StateVerG_HorR();
		} else if (count2 % 5 == 1) {
			Set_Tl_StateVerR_HorG();
		} else if (count2 % 5 == 2) {
			Set_Tl_StateVerO_HorO();
		} else if (count2 % 5 == 3) {
			Set_Tl_StateVerR_HorO();
		} else if (count2 % 5 == 4) {
			Set_Tl_StateVerO_HorR();
		}
	}

	if (inputState->Car_Pesent_Left) {
		if (lightsState->Horizontal_Traffic_Light_State.Delays.pedestrianDelay > 0)
			lightsState->Horizontal_Traffic_Light_State.Delays.pedestrianDelay -= 1;
	}

}


