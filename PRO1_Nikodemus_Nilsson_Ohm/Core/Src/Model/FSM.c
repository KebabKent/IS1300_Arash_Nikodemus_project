#include <DTO/input_state.h>
#include <DTO/lights_state.h>

#include "stdint.h"
#include "stdbool.h"
#include "main.h"
#include "Model/traffic_state.h"
#include "Model/delay.h"


#define DT_MS 10 //the constant I am assuming right now for the peroid of the fsm, if any update is to take place we just gotta fix it here 

uint8_t count = 0;
uint8_t count2 = 0;

/*Arash Code*/
typedef enum {
	PED_PASSIVE = 0,
	PED_WAITING = 1,
	PED_WALKING = 2
} ped_state_t;

typedef enum {//we need sth to keep track if one of the pedestrains is active so we can then force the other guy to waiting, im vibin with enum
	ACTIVE_NONE = 0,
	ACTIVE_LEFT,
	ACTIVE_UP
} active_ped_t;

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


/*Arash Code*/




void readAndSet(void) {
	LightsState_t* lightsState = Return_LightsState();
	InputState_t* inputState = Return_InputState();



	/*arash code*/

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

}
	/*Not sure if we need this because I havent tested the buttons yet, but to ensure the button press is ont time event we could have this OHM */
	/*
	input->Button_Pressed_Left = false;
	input->Button_Pressed_Up = false;
	*/



	/*arash code*/

	/*if (inputState->Button_Pressed_Left) {
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
	}*/

	/*if (inputState->Button_Pressed_Up) {
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
	}*/

	



