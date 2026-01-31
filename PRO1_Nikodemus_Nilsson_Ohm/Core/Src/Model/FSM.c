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
	EMPTY = 0,
	WAITING = 1,
	WALKING = 2
} PedState;

typedef enum {
	State1 = 0,
	State11 = 1,
	State12 = 2,
	State2 = 3,
	State21 = 4,
	State22 = 5,
	State3 = 6
} currentState;

currentState lastState;
currentState currState;

PedState pedestrianUp;
PedState pedestrianLeft;


void Ped_Tick(LightsState_t* lights, InputState_t* input)
{
}

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

	if (input->Car_Pesent_Down || input->Car_Pesent_Up) {
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
			if (lastState == State2) {
				currState = State1;
				Delay_Start(TIMER_CAR_UP_GREEN, lightsState->Standard_Delay_Times.greenDelay);
			}
		}
	}
}

void carLeftOrRight(InputState_t* input) {
	LightsState_t* lightsState = Return_LightsState();

	if (input->Car_Pesent_Left || input->Car_Pesent_Right) {
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
			if (lastState == State1) {
				currState = State2;
				Delay_Start(TIMER_CAR_LEFT_GREEN, lightsState->Standard_Delay_Times.greenDelay);

			}
		}
	}
}

void Car_Tick(LightsState_t* lights, InputState_t* input)
{
	passiveState(input);
	carUpOrDown(input);

	LightsState_t* lightsState = Return_LightsState();


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
	}
}

void readAndSet(InputState_t* inputState) {
	LightsState_t* lightsState = Return_LightsState();

	Delay_Tick(DT_MS);

	Ped_Tick(lightsState, inputState);

	Car_Tick(lightsState, inputState);

	updateState();

	lightsState->Horizontal_Traffic_Light_State.Delays.pedestrianDelay = Delay_Remaining(TIMER_PED_LEFT_WAIT);
	lightsState->Horizontal_Traffic_Light_State.Delays.walkingDelay = Delay_Remaining(TIMER_PED_LEFT_WALK);
	lightsState->Horizontal_Traffic_Light_State.Delays.greenDelay = Delay_Remaining(TIMER_CAR_LEFT_GREEN);
	lightsState->Horizontal_Traffic_Light_State.Delays.redDelay = Delay_Remaining(TIMER_CAR_LEFT_RED);

	lightsState->Vertical_Traffic_Light_State.Delays.pedestrianDelay = Delay_Remaining(TIMER_PED_UP_WAIT);
	lightsState->Vertical_Traffic_Light_State.Delays.walkingDelay = Delay_Remaining(TIMER_PED_UP_WALK);
	lightsState->Vertical_Traffic_Light_State.Delays.greenDelay = Delay_Remaining(TIMER_CAR_UP_GREEN);
	lightsState->Vertical_Traffic_Light_State.Delays.redDelay = Delay_Remaining(TIMER_CAR_UP_RED);
}

void FSM_Init() {
	LightsState_t* lightsState = Return_LightsState();

	currState = State1;

	pedestrianUp = EMPTY;
	pedestrianLeft = EMPTY;

	Delay_Start(TIMER_CAR_UP_GREEN, lightsState->Standard_Delay_Times.greenDelay);
}

