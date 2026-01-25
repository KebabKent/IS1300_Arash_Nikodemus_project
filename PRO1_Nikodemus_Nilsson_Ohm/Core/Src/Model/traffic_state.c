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
	lightState.Horizontal_Traffic_Light_State.Light_State_LU = 0;
	lightState.Horizontal_Traffic_Light_State.Light_State_RD = 0;
	lightState.Horizontal_Traffic_Light_State.Pdst_State = 1;
	// Delays
	lightState.Horizontal_Traffic_Light_State.Delays.pedestrianDelay = 10000;
	lightState.Horizontal_Traffic_Light_State.Delays.walkingDelay = 10000;
	lightState.Horizontal_Traffic_Light_State.Delays.greenDelay = 10000;
	lightState.Horizontal_Traffic_Light_State.Delays.orangeDelay = 10000;
	lightState.Horizontal_Traffic_Light_State.Delays.redDelay = 10000;

	lightState.Horizontal_Traffic_Light_State.toggle = false;

	// Vertical
	// Lights
	lightState.Vertical_Traffic_Light_State.Light_State_LU = 2;
	lightState.Vertical_Traffic_Light_State.Light_State_RD = 2;
	lightState.Vertical_Traffic_Light_State.Pdst_State = 0;
	// Delays
	lightState.Vertical_Traffic_Light_State.Delays.pedestrianDelay = 10000;
	lightState.Vertical_Traffic_Light_State.Delays.walkingDelay = 10000;
	lightState.Vertical_Traffic_Light_State.Delays.greenDelay = 10000;
	lightState.Vertical_Traffic_Light_State.Delays.orangeDelay = 10000;
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

void Set_LightsState(void) {

}


LightsState_t* Return_LightsState(void) {
	return &lightState;
}

void Set_InputState(void) {

}

InputState_t* Return_InputState(void) {
	return &inputState;
}

