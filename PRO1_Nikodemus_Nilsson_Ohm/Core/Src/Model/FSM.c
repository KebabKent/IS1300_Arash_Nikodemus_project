#include <DTO/input_state.h>
#include <DTO/lights_state.h>

#include "stdint.h"
#include "stdbool.h"
#include "main.h"
#include "Model/traffic_state.h"
#include "Model/FSM.h"
#include "Model/delay.h"

#define DT_MS 10 //the constant I am assuming right now for the peroid of the fsm, if any update is to take place we just gotta fix it here 

typedef enum {
	State1 = 0,
	State11 = 1,
	State12 = 2,
	State2 = 3,
	State21 = 4,
	State22 = 5,
	State3 = 6,
	State31 = 7,
	State32 = 8,
	State33 = 9
} currentState;

currentState lastState;
currentState currState;

void state1() {
	LightsState_t* lightsState = Return_LightsState();

	lightsState->Horizontal_Traffic_Light_State.Light_State_LU = RED_Tr;
	lightsState->Horizontal_Traffic_Light_State.Light_State_RD = RED_Tr;
	lightsState->Horizontal_Traffic_Light_State.Pdst_State = RED_Pd;

	lightsState->Vertical_Traffic_Light_State.Light_State_LU = GREEN_Tr;
	lightsState->Vertical_Traffic_Light_State.Light_State_RD = GREEN_Tr;
	lightsState->Vertical_Traffic_Light_State.Pdst_State = RED_Pd;
}

void state11() {
	LightsState_t* lightsState = Return_LightsState();

	lightsState->Horizontal_Traffic_Light_State.Light_State_LU = RED_Tr;
	lightsState->Horizontal_Traffic_Light_State.Light_State_RD = RED_Tr;
	lightsState->Horizontal_Traffic_Light_State.Pdst_State = RED_Pd;

	lightsState->Vertical_Traffic_Light_State.Light_State_LU = RED_Tr;
	lightsState->Vertical_Traffic_Light_State.Light_State_RD = GREEN_Tr;
	lightsState->Vertical_Traffic_Light_State.Pdst_State = GREEN_Pd;
}

void state12() {
	LightsState_t* lightsState = Return_LightsState();

	lightsState->Horizontal_Traffic_Light_State.Light_State_LU = RED_Tr;
	lightsState->Horizontal_Traffic_Light_State.Light_State_RD = RED_Tr;
	lightsState->Horizontal_Traffic_Light_State.Pdst_State = GREEN_Pd;

	lightsState->Vertical_Traffic_Light_State.Light_State_LU = RED_Tr;
	lightsState->Vertical_Traffic_Light_State.Light_State_RD = GREEN_Tr;
	lightsState->Vertical_Traffic_Light_State.Pdst_State = RED_Pd;
}

void state2() {
	LightsState_t* lightsState = Return_LightsState();

	lightsState->Horizontal_Traffic_Light_State.Light_State_LU = GREEN_Tr;
	lightsState->Horizontal_Traffic_Light_State.Light_State_RD = GREEN_Tr;
	lightsState->Horizontal_Traffic_Light_State.Pdst_State = RED_Pd;

	lightsState->Vertical_Traffic_Light_State.Light_State_LU = RED_Tr;
	lightsState->Vertical_Traffic_Light_State.Light_State_RD = RED_Tr;
	lightsState->Vertical_Traffic_Light_State.Pdst_State = RED_Pd;
}

void state21() {
	LightsState_t* lightsState = Return_LightsState();

	lightsState->Horizontal_Traffic_Light_State.Light_State_LU = GREEN_Tr;
	lightsState->Horizontal_Traffic_Light_State.Light_State_RD = RED_Tr;
	lightsState->Horizontal_Traffic_Light_State.Pdst_State = RED_Pd;

	lightsState->Vertical_Traffic_Light_State.Light_State_LU = RED_Tr;
	lightsState->Vertical_Traffic_Light_State.Light_State_RD = RED_Tr;
	lightsState->Vertical_Traffic_Light_State.Pdst_State = GREEN_Pd;
}

void state22() {
	LightsState_t* lightsState = Return_LightsState();

	lightsState->Horizontal_Traffic_Light_State.Light_State_LU = RED_Tr;
	lightsState->Horizontal_Traffic_Light_State.Light_State_RD = GREEN_Tr;
	lightsState->Horizontal_Traffic_Light_State.Pdst_State = GREEN_Pd;

	lightsState->Vertical_Traffic_Light_State.Light_State_LU = RED_Tr;
	lightsState->Vertical_Traffic_Light_State.Light_State_RD = RED_Tr;
	lightsState->Vertical_Traffic_Light_State.Pdst_State = RED_Pd;
}

void state3() {
	LightsState_t* lightsState = Return_LightsState();

	lightsState->Horizontal_Traffic_Light_State.Light_State_LU = ORANGE_Tr;
	lightsState->Horizontal_Traffic_Light_State.Light_State_RD = ORANGE_Tr;
	lightsState->Horizontal_Traffic_Light_State.Pdst_State = RED_Pd;

	lightsState->Vertical_Traffic_Light_State.Light_State_LU = ORANGE_Tr;
	lightsState->Vertical_Traffic_Light_State.Light_State_RD = ORANGE_Tr;
	lightsState->Vertical_Traffic_Light_State.Pdst_State = RED_Pd;
}

void state31() {
	LightsState_t* lightsState = Return_LightsState();

	lightsState->Horizontal_Traffic_Light_State.Light_State_LU = RED_Tr;
	lightsState->Horizontal_Traffic_Light_State.Light_State_RD = RED_Tr;
	lightsState->Horizontal_Traffic_Light_State.Pdst_State = RED_Pd;

	lightsState->Vertical_Traffic_Light_State.Light_State_LU = ORANGE_Tr;
	lightsState->Vertical_Traffic_Light_State.Light_State_RD = GREEN_Tr;
	lightsState->Vertical_Traffic_Light_State.Pdst_State = RED_Pd;
}

void state32() {
	LightsState_t* lightsState = Return_LightsState();

	lightsState->Horizontal_Traffic_Light_State.Light_State_LU = GREEN_Tr;
	lightsState->Horizontal_Traffic_Light_State.Light_State_RD = ORANGE_Tr;
	lightsState->Horizontal_Traffic_Light_State.Pdst_State = RED_Pd;

	lightsState->Vertical_Traffic_Light_State.Light_State_LU = RED_Tr;
	lightsState->Vertical_Traffic_Light_State.Light_State_RD = RED_Tr;
	lightsState->Vertical_Traffic_Light_State.Pdst_State = RED_Pd;
}

void state33() {
	LightsState_t* lightsState = Return_LightsState();

	lightsState->Horizontal_Traffic_Light_State.Light_State_LU = ORANGE_Tr;
	lightsState->Horizontal_Traffic_Light_State.Light_State_RD = GREEN_Tr;
	lightsState->Horizontal_Traffic_Light_State.Pdst_State = RED_Pd;

	lightsState->Vertical_Traffic_Light_State.Light_State_LU = RED_Tr;
	lightsState->Vertical_Traffic_Light_State.Light_State_RD = RED_Tr;
	lightsState->Vertical_Traffic_Light_State.Pdst_State = RED_Pd;
}


void passiveState(InputState_t* input) {
	LightsState_t* lightsState = Return_LightsState();

	if (!input->Car_Pesent_Down && !input->Car_Pesent_Left && !input->Car_Pesent_Up && !input->Car_Pesent_Right) {
		if (currState == State1 && Delay_IsDone(TIMER_CAR_UP_GREEN)) {
			lastState = State1;
			currState = State3;
			Delay_Start(TIMER_CAR_UP_ORANGE, lightsState->Standard_Delay_Times.orangeDelay);
		}

		if (currState == State2 && Delay_IsDone(TIMER_CAR_LEFT_GREEN)) {
			lastState = State2;
			currState = State3;
			Delay_Start(TIMER_CAR_LEFT_ORANGE, lightsState->Standard_Delay_Times.orangeDelay);
		}

		if (currState == State3 && Delay_IsDone(TIMER_CAR_UP_ORANGE) && Delay_IsDone(TIMER_CAR_LEFT_ORANGE)) {
			if (lastState == State1) {
				currState = State2;
				Delay_Start(TIMER_CAR_LEFT_GREEN, lightsState->Standard_Delay_Times.greenDelay);
			} else if (lastState == State2) {
				currState = State1;
				Delay_Start(TIMER_CAR_UP_GREEN, lightsState->Standard_Delay_Times.greenDelay);
			}
		}
	}
}

void carUpOrDown(InputState_t* input) {
	LightsState_t* lightsState = Return_LightsState();

	if ((input->Car_Pesent_Down || input->Car_Pesent_Up) && !input->Car_Pesent_Left && !input->Car_Pesent_Right) {
		if (currState == State1) {
			lastState = State1;
			currState = State1;
		}

		if (currState == State2) {
			lastState = State2;
			currState = State3;
			Delay_Start(TIMER_CAR_LEFT_ORANGE, lightsState->Standard_Delay_Times.orangeDelay);
		}

		if (currState == State3 && Delay_IsDone(TIMER_CAR_UP_ORANGE) && Delay_IsDone(TIMER_CAR_LEFT_ORANGE)) {
			currState = State1;
			Delay_Start(TIMER_CAR_UP_GREEN, lightsState->Standard_Delay_Times.greenDelay);
		}
	}
}

void carLeftOrRight(InputState_t* input) {
	LightsState_t* lightsState = Return_LightsState();

	if ((input->Car_Pesent_Left || input->Car_Pesent_Right) && !input->Car_Pesent_Up && !input->Car_Pesent_Down) {
		if (currState == State1) {
			lastState = State1;
			currState = State3;
			Delay_Start(TIMER_CAR_UP_ORANGE, lightsState->Standard_Delay_Times.orangeDelay);
		}

		if (currState == State2) {
			lastState = State2;
			currState = State2;
		}

		if (currState == State3 && Delay_IsDone(TIMER_CAR_UP_ORANGE) && Delay_IsDone(TIMER_CAR_LEFT_ORANGE)) {
			currState = State2;
			Delay_Start(TIMER_CAR_LEFT_GREEN, lightsState->Standard_Delay_Times.greenDelay);
		}
	}
}

void carsMultipleDirections(InputState_t* input) {
	LightsState_t* lightsState = Return_LightsState();

	if ((input->Car_Pesent_Left || input->Car_Pesent_Right) && (input->Car_Pesent_Up || input->Car_Pesent_Down)) {
		if (currState == State1 && !Delay_IsDone(TIMER_CAR_UP_GREEN) && Delay_IsDone(TIMER_CAR_UP_RED)) {
			Delay_Start(TIMER_CAR_UP_RED, lightsState->Standard_Delay_Times.redDelay);
		}

		if (currState == State2 && !Delay_IsDone(TIMER_CAR_LEFT_GREEN) && Delay_IsDone(TIMER_CAR_LEFT_RED)) {
			Delay_Start(TIMER_CAR_LEFT_RED, lightsState->Standard_Delay_Times.redDelay);
		}


		if (currState == State1 && Delay_IsDone(TIMER_CAR_UP_RED)) {
			lastState = State1;
			currState = State3;
			Delay_Start(TIMER_CAR_UP_ORANGE, lightsState->Standard_Delay_Times.orangeDelay);
		}

		if (currState == State2 && Delay_IsDone(TIMER_CAR_LEFT_RED)) {
			lastState = State2;
			currState = State3;
			Delay_Start(TIMER_CAR_LEFT_ORANGE, lightsState->Standard_Delay_Times.orangeDelay);
		}

		if (currState == State3 && Delay_IsDone(TIMER_CAR_UP_ORANGE) && Delay_IsDone(TIMER_CAR_LEFT_ORANGE)) {
			if (lastState == State1) {
				currState = State2;
				Delay_Start(TIMER_CAR_LEFT_GREEN, lightsState->Standard_Delay_Times.greenDelay);
				Delay_Start(TIMER_CAR_UP_RED, lightsState->Standard_Delay_Times.redDelay);
			} else if (lastState == State2) {
				currState = State1;
				Delay_Start(TIMER_CAR_UP_GREEN, lightsState->Standard_Delay_Times.greenDelay);
				Delay_Start(TIMER_CAR_LEFT_RED, lightsState->Standard_Delay_Times.redDelay);
			}
		}
	}
}

void Car_Tick(InputState_t* input)
{
	if (input->Button_Pressed_Left || Delay_IsDone(TIMER_PED_LEFT_WAIT) || Delay_IsDone(TIMER_PED_LEFT_WALK) ||
			input->Button_Pressed_Up || Delay_IsDone(TIMER_PED_UP_WAIT) || Delay_IsDone(TIMER_PED_UP_WALK)) {
		passiveState(input);
		carUpOrDown(input);
		carLeftOrRight(input);
		carsMultipleDirections(input);
	}
}

void pedUp(InputState_t* input) {
	LightsState_t* lightsState = Return_LightsState();

	if (input->Button_Pressed_Up) {
		lightsState->Vertical_Traffic_Light_State.toggle = true;

		if (currState == State1 && Delay_IsDone(TIMER_PED_UP_WAIT)) {
			lastState = State1;
			currState = State31;
			Delay_Start(TIMER_CAR_UP_ORANGE, lightsState->Standard_Delay_Times.orangeDelay);
			Delay_Start(TIMER_PED_UP_WAIT, lightsState->Standard_Delay_Times.pedestrianDelay);
		}

	}

}

void pedLeft(InputState_t* input) {
	LightsState_t* lightsState = Return_LightsState();

	if (input->Button_Pressed_Left) {
		lightsState->Horizontal_Traffic_Light_State.toggle = true;

	}

}


void Ped_Tick(InputState_t* input)
{
	pedUp(input);
	pedLeft(input);

}

void updateState() {
	switch (currState) {
		case State1:
			state1();
			break;

		case State11:
			state11();
			break;

		case State12:
			state12();
			break;

		case State2:
			state2();
			break;

		case State21:
			state21();
			break;

		case State22:
			state22();
			break;

		case State3:
			state3();
			break;

		case State31:
			state31();
			break;

		case State32:
			state32();
			break;

		case State33:
			state33();
			break;
	}
}

void readAndSet(InputState_t* inputState) {
	Car_Tick(inputState);

	Ped_Tick(inputState);

	updateState();

	LightsState_t* lightsState = Return_LightsState();
	lightsState->Horizontal_Traffic_Light_State.Delays.pedestrianDelay = Delay_Remaining(TIMER_PED_LEFT_WAIT);
	lightsState->Horizontal_Traffic_Light_State.Delays.walkingDelay = Delay_Remaining(TIMER_PED_LEFT_WALK);
	lightsState->Horizontal_Traffic_Light_State.Delays.greenDelay = Delay_Remaining(TIMER_CAR_LEFT_GREEN);
	lightsState->Horizontal_Traffic_Light_State.Delays.redDelay = Delay_Remaining(TIMER_CAR_LEFT_RED);

	lightsState->Vertical_Traffic_Light_State.Delays.pedestrianDelay = Delay_Remaining(TIMER_PED_UP_WAIT);
	lightsState->Vertical_Traffic_Light_State.Delays.walkingDelay = Delay_Remaining(TIMER_PED_UP_WALK);
	lightsState->Vertical_Traffic_Light_State.Delays.greenDelay = Delay_Remaining(TIMER_CAR_UP_GREEN);
	lightsState->Vertical_Traffic_Light_State.Delays.redDelay = Delay_Remaining(TIMER_CAR_UP_RED);

	Delay_Tick(DT_MS);
}

void FSM_Init() {
	LightsState_t* lightsState = Return_LightsState();

	currState = State1;

	Delay_Start(TIMER_CAR_UP_GREEN, lightsState->Standard_Delay_Times.greenDelay);
}

