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
				else if ((activePed== ACTIVE_UP) & (Delay_IsDone(TIMER_PED_LEFT_WAIT)))
				{
					Delay_Start(TIMER_PED_LEFT_WAIT, Delay_Remaining(3));
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
				else if ((activePed== ACTIVE_LEFT) & (Delay_IsDone(TIMER_PED_UP_WAIT)))
				{
					Delay_Start(TIMER_PED_UP_WAIT, Delay_Remaining(1));
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


/*CAR CODE*/
typedef enum {
    CAR_R = 0,
    CAR_O = 1,
    CAR_G = 2
} car_color_t;

static car_color_t carUpState   = CAR_G;  
static car_color_t carLeftState = CAR_R;

static void setCarOutputs(void)
{

    if (carUpState == CAR_G)      Set_Tl_StateVerG();
    else if (carUpState == CAR_O) Set_Tl_StateVerO();
    else                         Set_Tl_StateVerR();

    if (carLeftState == CAR_G)      Set_Tl_StateHorG();
    else if (carLeftState == CAR_O) Set_Tl_StateHorO();
    else                           Set_Tl_StateHorR();
}




static bool Up_GreenToRed(LightsState_t* lights)
{
    
    if (carUpState == CAR_G) {
        carUpState = CAR_O;
        Delay_Start(TIMER_CAR_UP_ORANGE, lights->Standard_Delay_Times.orangeDelay);
        return false;
    }

   
    if (carUpState == CAR_O && Delay_IsDone(TIMER_CAR_UP_ORANGE)) {
        carUpState = CAR_R;
        return true;
    }

    return false; 
}

static bool Up_RedToGreen(LightsState_t* lights)
{
    if (carUpState == CAR_R) {
        carUpState = CAR_O;
        Delay_Start(TIMER_CAR_UP_ORANGE, lights->Standard_Delay_Times.orangeDelay);
        return false;
    }

    if (carUpState == CAR_O && Delay_IsDone(TIMER_CAR_UP_ORANGE)) {
        carUpState = CAR_G;
        return true;
    }

    return false;
}

static bool Left_GreenToRed(LightsState_t* lights)
{
    if (carLeftState == CAR_G) {
        carLeftState = CAR_O;
        Delay_Start(TIMER_CAR_LEFT_ORANGE, lights->Standard_Delay_Times.orangeDelay);
        return false;
    }

    if (carLeftState == CAR_O && Delay_IsDone(TIMER_CAR_LEFT_ORANGE)) {
        carLeftState = CAR_R;
        return true;
    }

    return false;
}

static bool Left_RedToGreen(LightsState_t* lights)
{
    if (carLeftState == CAR_R) {
        carLeftState = CAR_O;
        Delay_Start(TIMER_CAR_LEFT_ORANGE, lights->Standard_Delay_Times.orangeDelay);
        return false;
    }

    if (carLeftState == CAR_O && Delay_IsDone(TIMER_CAR_LEFT_ORANGE)) {
        carLeftState = CAR_G;
        return true;
    }

    return false;
}


static void Car_Tick(LightsState_t* lights, InputState_t* input)
{
	bool upCars   = input->Car_Pesent_Up   || input->Car_Pesent_Down;
	bool leftCars = input->Car_Pesent_Left || input->Car_Pesent_Right;

	bool noPedActivity =
    	(!input->Button_Pressed_Up) &&
    	(!input->Button_Pressed_Left) &&
		(pedUpState == PED_PASSIVE) &&
    	(pedLeftState == PED_PASSIVE) &&
    	(activePed == ACTIVE_NONE);


	bool passiveMode = (!upCars && !leftCars && noPedActivity);


	if(passiveMode)
	{
		if (carUpState == CAR_G && carLeftState == CAR_R)
    	{
        if (!Delay_IsDone(TIMER_CAR_UP_GREEN)) return;
        Up_GreenToRed(lights);
        return;
    }

    if (carLeftState == CAR_R && carUpState != CAR_G) 
    {
        if (Up_GreenToRed(lights)){Left_RedToGreen(lights);}
        return;
    }

    if (carUpState == CAR_R && carLeftState != CAR_G) 
    {
        if (Left_RedToGreen(lights)){Delay_Start(TIMER_CAR_LEFT_GREEN, lights->Standard_Delay_Times.greenDelay);}
        return;}

    if (carLeftState == CAR_G && carUpState == CAR_R)
    {
        if (!Delay_IsDone(TIMER_CAR_LEFT_GREEN)) return;
        Left_GreenToRed(lights);
        return;
    }

	if (carUpState == CAR_R && carLeftState != CAR_G)  
    {
        if (Left_GreenToRed(lights)){Up_RedToGreen(lights);}
        return;
    }

    if (carLeftState == CAR_R && carUpState != CAR_G)  
    {
        if (Up_RedToGreen(lights)){Delay_Start(TIMER_CAR_UP_GREEN, lights->Standard_Delay_Times.greenDelay);}
        return;
    }

    carUpState   = CAR_G;
    carLeftState = CAR_R;
    Delay_Start(TIMER_CAR_UP_GREEN, lights->Standard_Delay_Times.greenDelay);

	}
}

/*CAR CODE*/

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

		carUpState   = CAR_G;   
    	carLeftState = CAR_R;

		Set_Tl_StateVerG();
    	Set_Tl_StateHorR();

		Delay_Start(TIMER_CAR_UP_GREEN, lightsState->Standard_Delay_Times.greenDelay);

	}

	Delay_Tick(DT_MS);

	PedLeft_Tick(lightsState, inputState);
	PedUp_Tick(lightsState, inputState);

	setPedOutputs();

	Car_Tick(lightsState, inputState);
	setCarOutputs();


	lightsState->Horizontal_Traffic_Light_State.Delays.pedestrianDelay = Delay_Remaining(0);
	lightsState->Horizontal_Traffic_Light_State.Delays.walkingDelay = Delay_Remaining(1);

	lightsState->Vertical_Traffic_Light_State.Delays.pedestrianDelay = Delay_Remaining(2);
	lightsState->Vertical_Traffic_Light_State.Delays.walkingDelay = Delay_Remaining(3);




}




