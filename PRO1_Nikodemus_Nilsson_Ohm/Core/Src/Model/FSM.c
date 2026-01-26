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
/*task 2 update*/

typedef enum {
CAR_RED = 0,
CAR_ORANGE = 1,
CAR_GREEN = 2
} car_color_t;


static car_color_t carVer = CAR_GREEN;
static car_color_t carHor = CAR_RED;


static bool HasVerticalCars(InputState_t* in)
{
	return (in->Car_Pesent_Up || in->Car_Pesent_Down);
}


static bool HasHorizontalCars(InputState_t* in)
{
	return (in->Car_Pesent_Left || in->Car_Pesent_Right);
}


static void setCarOutputs(void)
{
	if (carVer == CAR_GREEN && carHor == CAR_RED) {
		Set_Tl_StateVerG_HorR();
	}
	else if (carVer == CAR_RED && carHor == CAR_GREEN) {
		Set_Tl_StateVerR_HorG();
	}
	else if (carVer == CAR_ORANGE && carHor == CAR_ORANGE) {
		Set_Tl_StateVerO_HorO();
	}
	else if (carVer == CAR_RED && carHor == CAR_ORANGE) {
		Set_Tl_StateVerR_HorO();
	}
	else if (carVer == CAR_ORANGE && carHor == CAR_RED) {
		Set_Tl_StateVerO_HorR();
	}
	else {
		Set_Tl_StateVerO_HorO();
	}
}


static void Car_Tick(LightsState_t* lights, InputState_t* input)
{
    bool vCars = (input->Car_Pesent_Up || input->Car_Pesent_Down);
    bool hCars = (input->Car_Pesent_Left || input->Car_Pesent_Right);


    if (carVer == CAR_GREEN && carHor == CAR_RED)
    {
        if (Delay_IsDone(TIMER_CAR_GREEN_VER))
        {
            if (hCars || !vCars) // If no car in vertical but car in horizontal we 
            {
                carVer = CAR_ORANGE;
                carHor = CAR_RED;
                Delay_Start(TIMER_CAR_ORANGE_VER, lights->Standard_Delay_Times.orangeDelay);
            }
            else
            {
                Delay_Start(TIMER_CAR_GREEN_VER, lights->Standard_Delay_Times.greenDelay);
            }
        }
        return;
    }


    if (carVer == CAR_ORANGE && carHor == CAR_RED)
    {
        if (Delay_IsDone(TIMER_CAR_ORANGE_VER))
        {
            carVer = CAR_RED;
            carHor = CAR_ORANGE; 
            Delay_Start(TIMER_CAR_RED_VER, lights->Standard_Delay_Times.redDelay);
        }
        return;
    }


    if (carVer == CAR_RED && carHor == CAR_ORANGE)
    {
        if (Delay_IsDone(TIMER_CAR_RED_VER))
        {
            carVer = CAR_RED;
            carHor = CAR_GREEN;
            Delay_Start(TIMER_CAR_GREEN_HOR, lights->Standard_Delay_Times.greenDelay);
        }
        return;
    }


    if (carVer == CAR_RED && carHor == CAR_GREEN)
    {
        if (Delay_IsDone(TIMER_CAR_GREEN_HOR))
        {
            if (vCars || !hCars)
            {
                carHor = CAR_ORANGE;
                carVer = CAR_RED;
                Delay_Start(TIMER_CAR_ORANGE_HOR, lights->Standard_Delay_Times.orangeDelay);
            }
            else
            {
                Delay_Start(TIMER_CAR_GREEN_HOR, lights->Standard_Delay_Times.greenDelay);
            }
        }
        return;
    }


    if (carVer == CAR_RED && carHor == CAR_ORANGE)
    {
        if (Delay_IsDone(TIMER_CAR_ORANGE_HOR))
        {
            carHor = CAR_RED;
            carVer = CAR_ORANGE;
            Delay_Start(TIMER_CAR_RED_HOR, lights->Standard_Delay_Times.redDelay);
        }
        return;
    }


    if (carVer == CAR_ORANGE && carHor == CAR_RED)
    {
        if (Delay_IsDone(TIMER_CAR_RED_HOR))
        {
            carVer = CAR_GREEN;
            carHor = CAR_RED;
            Delay_Start(TIMER_CAR_GREEN_VER, lights->Standard_Delay_Times.greenDelay);
        }
        return;
    }

    carVer = CAR_GREEN;
    carHor = CAR_RED;
    Delay_Start(TIMER_CAR_GREEN_VER, lights->Standard_Delay_Times.greenDelay);
}
/*task 2 update*/




void readAndSet(void) {
	LightsState_t* lightsState = Return_LightsState();
	InputState_t* inputState = Return_InputState();



	/*arash code*/

	if (!initDone) {//first time initialization
		initDone = true;
		Delay_Init();
		
		pedLeftState = PED_PASSIVE;
		pedUpState = PED_PASSIVE;
		activePed = ACTIVE_NONE;

		Set_Pl_StatePassiveLeft();
		Set_Pl_StatePassiveUp();
		
		
		carVer = CAR_GREEN;
		carHor = CAR_RED;

		Car_Tick(lightsState, inputState);
		setCarOutputs();
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

	



