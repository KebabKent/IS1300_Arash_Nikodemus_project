#include <DTO/lights_state.h>
#include "stdint.h"
#include "stdbool.h"
#include "main.h"
#include "Model/traffic_state.h"


LightsState_t lightState;
InputState_t inputState;


void State_Init(void) {
	lightState.toggleFrequenzy = 500;

	// Standard delay times
	lightState.Standard_Delay_Times.pedestrianDelay = 10000;
	lightState.Standard_Delay_Times.walkingDelay = 10000;
	lightState.Standard_Delay_Times.greenDelay = 10000;
	lightState.Standard_Delay_Times.orangeDelay = 10000;
	lightState.Standard_Delay_Times.redDelay = 10000;

	// Horizontal
	// Lights
	lightState.Horizontal_Traffic_Light_State.Light_State_LU = RED_Tr;
	lightState.Horizontal_Traffic_Light_State.Light_State_RD = RED_Tr;
	lightState.Horizontal_Traffic_Light_State.Pdst_State = RED_Pd;
	// Delays
	lightState.Horizontal_Traffic_Light_State.Delays.pedestrianDelay = 10000;
	lightState.Horizontal_Traffic_Light_State.Delays.walkingDelay = 10000;
	lightState.Horizontal_Traffic_Light_State.Delays.greenDelay = 10000;
	lightState.Horizontal_Traffic_Light_State.Delays.orangeDelay = 10000;
	lightState.Horizontal_Traffic_Light_State.Delays.redDelay = 10000;

	lightState.Horizontal_Traffic_Light_State.toggle = false;

	// Vertical
	// Lights
	lightState.Vertical_Traffic_Light_State.Light_State_LU = GREEN_Tr;
	lightState.Vertical_Traffic_Light_State.Light_State_RD = GREEN_Tr;
	lightState.Vertical_Traffic_Light_State.Pdst_State = RED_Pd;
	// Delays
	lightState.Vertical_Traffic_Light_State.Delays.pedestrianDelay = 10000;
	lightState.Vertical_Traffic_Light_State.Delays.walkingDelay = 10000;
	lightState.Vertical_Traffic_Light_State.Delays.greenDelay = 10000;
	lightState.Vertical_Traffic_Light_State.Delays.orangeDelay = 5000;
	lightState.Vertical_Traffic_Light_State.Delays.redDelay = 10000;

	lightState.Vertical_Traffic_Light_State.toggle = false;


	// Inputs
	inputState.Button_Pressed_Left = false;
	inputState.Button_Pressed_Up = false;

	inputState.Car_Pesent_Left = false;
	inputState.Car_Pesent_Up = false;
	inputState.Car_Pesent_Right = false;
	inputState.Car_Pesent_Down = false;
}

LightsState_t* Return_LightsState(void) {
	return &lightState;
}

InputState_t* Return_InputState(void) {
	return &inputState;
}

void Set_Pl_StatePassiveUp(void) {
	lightState.Vertical_Traffic_Light_State.Pdst_State = RED_Pd;
	lightState.Vertical_Traffic_Light_State.toggle = false;
}


void Set_Pl_StatePassiveLeft(void) {
	lightState.Horizontal_Traffic_Light_State.Pdst_State = RED_Pd;
	lightState.Horizontal_Traffic_Light_State.toggle = false;

}

void Set_Pl_StateWaitingUp(void) {
	lightState.Vertical_Traffic_Light_State.Pdst_State = RED_Pd;
	lightState.Vertical_Traffic_Light_State.toggle = true;
}

void Set_Pl_StateWalkingUp(void) {
	lightState.Vertical_Traffic_Light_State.Pdst_State = GREEN_Pd;
	lightState.Vertical_Traffic_Light_State.toggle = false;
}

void Set_Pl_StateWaitingLeft(void) {
	lightState.Horizontal_Traffic_Light_State.Pdst_State = RED_Pd;
	lightState.Horizontal_Traffic_Light_State.toggle = true;
}

void Set_Pl_StateWalkingLeft(void) {
	lightState.Horizontal_Traffic_Light_State.Pdst_State = GREEN_Pd;
	lightState.Horizontal_Traffic_Light_State.toggle = false;
}









void Set_Tl_StateVerG(void) {
	lightState.Vertical_Traffic_Light_State.Light_State_LU = GREEN_Tr;
	lightState.Vertical_Traffic_Light_State.Light_State_RD = GREEN_Tr;
}

void Set_Tl_StateHorG(void) {
	lightState.Horizontal_Traffic_Light_State.Light_State_LU = GREEN_Tr;
	lightState.Horizontal_Traffic_Light_State.Light_State_RD = GREEN_Tr;
}

void Set_Tl_StateVerO(void) {
	lightState.Vertical_Traffic_Light_State.Light_State_LU = ORANGE_Tr;
	lightState.Vertical_Traffic_Light_State.Light_State_RD = ORANGE_Tr;
}

void Set_Tl_StateHorO(void) {
	lightState.Horizontal_Traffic_Light_State.Light_State_LU = ORANGE_Tr;
	lightState.Horizontal_Traffic_Light_State.Light_State_RD = ORANGE_Tr;
}

void Set_Tl_StateVerR(void) {
	lightState.Vertical_Traffic_Light_State.Light_State_LU = RED_Tr;
	lightState.Vertical_Traffic_Light_State.Light_State_RD = RED_Tr;
}

void Set_Tl_StateHorR(void) {
	lightState.Horizontal_Traffic_Light_State.Light_State_LU = RED_Tr;
	lightState.Horizontal_Traffic_Light_State.Light_State_RD = RED_Tr;
}

