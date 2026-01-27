#include <DTO/input_state.h>
#include <DTO/lights_state.h>

#include "stdint.h"
#include "stdbool.h"
#include "main.h"
#include "Model/traffic_state.h"
#include "Model/FSM.h"
#include "Model/delay.h"

#define DT_MS 10 //the constant I am assuming right now for the peroid of the fsm, if any update is to take place we just gotta fix it here 

uint8_t count = 0;
uint8_t count2 = 0;

static ped_state_t pedLeftState = PED_PASSIVE;
static ped_state_t pedUpState = PED_PASSIVE;
static active_ped_t activePed = ACTIVE_NONE;
static bool initDone = false;


static void setPedOutputs(void)
{
	Set_Pl_StatePassiveLeft();//Basically no need for the if else, because these 2 are the default states for both pedestrains, if we are not waiting, or walking then it must be red
	Set_Pl_StatePassiveUp();

	if (pedLeftState == PED_WAITING) {
		Set_Pl_StateWaitingLeft(); 
	}
	else if (pedLeftState == PED_WALKING) {
		Set_Pl_StateWalkingLeft(); 
	}

	if (pedUpState == PED_WAITING) {
		Set_Pl_StateWaitingUp(); 
	}
	else if (pedUpState == PED_WALKING) {
		Set_Pl_StateWalkingUp(); 
	}
}

static void PedLeft_Tick(LightsState_t* lights, InputState_t* input)
{
	switch (pedLeftState)
	{
		case PED_PASSIVE:
			if (input->Button_Pressed_Left) {
				pedLeftState = PED_WAITING;
				Delay_Start(TIMER_PED_LEFT_WAIT, lights->Standard_Delay_Times.pedestrianDelay);
			}
		break;


		case PED_WAITING:
			if (Delay_IsDone(TIMER_PED_LEFT_WAIT)) {//Update to add, if the trafic light is green, we will remain in wait, i have not added this yet
				if (activePed == ACTIVE_NONE) {
					activePed = ACTIVE_LEFT;
					pedLeftState = PED_WALKING;
					Delay_Start(TIMER_PED_LEFT_WALK, lights->Standard_Delay_Times.walkingDelay);
				}
			}
		break;


		case PED_WALKING:
			if (Delay_IsDone(TIMER_PED_LEFT_WALK)) {
				pedLeftState = PED_PASSIVE;
				if (activePed == ACTIVE_LEFT) {
					activePed = ACTIVE_NONE;
				}
			}
		break;
	}
}

static void PedUp_Tick(LightsState_t* lights, InputState_t* input)
{
	switch (pedUpState)
	{
		case PED_PASSIVE:
			if (input->Button_Pressed_Up) {
				pedUpState = PED_WAITING;
				Delay_Start(TIMER_PED_UP_WAIT, lights->Standard_Delay_Times.pedestrianDelay);
			}
		break;


		case PED_WAITING:
			if (Delay_IsDone(TIMER_PED_UP_WAIT)) {
				if (activePed == ACTIVE_NONE) {
					activePed = ACTIVE_UP;
					pedUpState = PED_WALKING;
					Delay_Start(TIMER_PED_UP_WALK, lights->Standard_Delay_Times.walkingDelay);
				}
			}
		break;


		case PED_WALKING:
			if (Delay_IsDone(TIMER_PED_UP_WALK)) {
				pedUpState = PED_PASSIVE;
				if (activePed == ACTIVE_UP) {
					activePed = ACTIVE_NONE;
				}
			}
		break;
	}
}


void readAndSet(InputState_t* inputState) {
	LightsState_t* lightsState = Return_LightsState();


	if (!initDone) {
		initDone = true;
		Delay_Init();
		
		pedLeftState = PED_PASSIVE;
		pedUpState = PED_PASSIVE;
		activePed = ACTIVE_NONE;

		Set_Pl_StatePassiveLeft();
		Set_Pl_StatePassiveUp();
	}

	Delay_Tick(DT_MS);

	PedLeft_Tick(lightsState, inputState);
	PedUp_Tick(lightsState, inputState);

	setPedOutputs();

	lightsState->Horizontal_Traffic_Light_State.Delays.pedestrianDelay = Delay_Remaining(0);
	lightsState->Horizontal_Traffic_Light_State.Delays.walkingDelay = Delay_Remaining(1);

	lightsState->Vertical_Traffic_Light_State.Delays.pedestrianDelay = Delay_Remaining(2);
	lightsState->Vertical_Traffic_Light_State.Delays.walkingDelay = Delay_Remaining(3);




}




