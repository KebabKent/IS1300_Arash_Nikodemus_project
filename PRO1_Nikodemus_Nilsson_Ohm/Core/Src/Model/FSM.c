#include <DTO/input_state.h>
#include <DTO/lights_state.h>

#include "stdint.h"
#include "stdbool.h"
#include "main.h"
#include "Model/traffic_state.h"
#include "Model/FSM.h"
#include "Model/delay.h"

#define DT_MS 10 //the constant I am assuming right now for the peroid of the fsm, if any update is to take place we just gotta fix it here 

int firsttime=0;

uint8_t count = 0;
uint8_t count2 = 0;

static ped_state_t pedLeftState = PED_PASSIVE;
static ped_state_t pedUpState = PED_PASSIVE;
static active_ped_t activePed = ACTIVE_NONE;
static bool initDone = false;

static bool pedUpDue   = false; 
static bool pedLeftDue = false;

typedef enum {
    CAR_R = 0,
    CAR_O = 1,
    CAR_G = 2
} car_color_t;

static car_color_t carUpState   = CAR_G;
static car_color_t carLeftState = CAR_R;


static void setPedOutputs(void)
{
	if (pedLeftState == PED_WAITING) {
		Set_Pl_StateWaitingLeft(); 
	}
	else if (pedLeftState == PED_WALKING) {
		Set_Pl_StateWalkingLeft(); 
	} else {
		Set_Pl_StatePassiveLeft();
	}

	if (pedUpState == PED_WAITING) {
		Set_Pl_StateWaitingUp(); 
	}
	else if (pedUpState == PED_WALKING) {
		Set_Pl_StateWalkingUp(); 
	} else {
		Set_Pl_StatePassiveUp();
	}
}

static void PedLeft_Tick(LightsState_t* lights, InputState_t* input)
{
	switch (pedLeftState)
	{
		case PED_PASSIVE:
			if (input->Button_Pressed_Left) {
        		pedLeftDue = false;
				pedLeftState = PED_WAITING;
				Delay_Start(TIMER_PED_LEFT_WAIT, lights->Standard_Delay_Times.pedestrianDelay);
			}
		break;


		case PED_WAITING:
			if (Delay_IsDone(TIMER_PED_LEFT_WAIT)) {//Update to add, if the trafic light is green, we will remain in wait, i have not added this yet
				pedLeftDue = true;
			}
				
			if (pedLeftDue && (activePed == ACTIVE_NONE) && (carLeftState == CAR_R)) {
					activePed = ACTIVE_LEFT;
					pedLeftState = PED_WALKING;
					pedLeftDue = false;

					Delay_Start(TIMER_PED_LEFT_WALK, lights->Standard_Delay_Times.walkingDelay);
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
        		pedUpDue = false;
				pedUpState = PED_WAITING;
				Delay_Start(TIMER_PED_UP_WAIT, lights->Standard_Delay_Times.pedestrianDelay);
			}
		break;


		case PED_WAITING:
			if (Delay_IsDone(TIMER_PED_UP_WAIT)) {
					pedUpDue = true;
			}

			if (pedUpDue && (activePed == ACTIVE_NONE) && (carUpState == CAR_R)) {
					activePed = ACTIVE_UP;
					pedUpState = PED_WALKING;
					pedUpDue = false;
					Delay_Start(TIMER_PED_UP_WALK, lights->Standard_Delay_Times.walkingDelay);
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

static void setCarOutputs(void)
{
        if (carUpState == CAR_R && pedUpState == PED_WALKING) {
        Set_Tl_StateVerUpR_DownG();
    }
    else {
        if (carUpState == CAR_G)      Set_Tl_StateVerG();
        else if (carUpState == CAR_O) Set_Tl_StateVerO();
        else                          Set_Tl_StateVerR();
    }

    
    if (carLeftState == CAR_R && pedLeftState == PED_WALKING) {
        Set_Tl_StateHorLeftR_RightG();
    }
    else {
        if (carLeftState == CAR_G)      Set_Tl_StateHorG();
        else if (carLeftState == CAR_O) Set_Tl_StateHorO();
        else                            Set_Tl_StateHorR();
    }
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

typedef enum {
    PHASE_VER_GREEN,
    PHASE_HOR_GREEN,
    PHASE_SWITCH_TO_VER,
    PHASE_SWITCH_TO_HOR
} car_phase_t;

static car_phase_t carPhase = PHASE_VER_GREEN;

static bool prevUpCars   = false;
static bool prevLeftCars = false;
static inline void Timer_Stop(TimerId id) { Delay_Start(id, 0); }


static bool upRedWaitArmed = false;
static bool leftRedWaitArmed = false;

static void Car_Tick(LightsState_t* lights, InputState_t* input)
{
    bool upCars   = input->Car_Pesent_Up   || input->Car_Pesent_Down;
    bool leftCars = input->Car_Pesent_Left || input->Car_Pesent_Right;

	bool leftCanRightTurnOnRed = (carLeftState == CAR_R) && (pedUpState   == PED_WALKING);
	bool upCanRightTurnOnRed   = (carUpState   == CAR_R) && (pedLeftState == PED_WALKING);

    bool leftCarsWaiting = leftCars && !leftCanRightTurnOnRed;
    bool upCarsWaiting   = upCars   && !upCanRightTurnOnRed;

	bool needHorGreen_forPedUp   = (pedUpDue   || activePed == ACTIVE_UP);  
	bool needVerGreen_forPedLeft = (pedLeftDue || activePed == ACTIVE_LEFT);

	if (needHorGreen_forPedUp && needVerGreen_forPedLeft) {
    	if (activePed == ACTIVE_LEFT) needHorGreen_forPedUp = false;
   		else{needVerGreen_forPedLeft = false;}
	}

	if (needHorGreen_forPedUp) {
		if (carPhase == PHASE_VER_GREEN) {
        carPhase = PHASE_SWITCH_TO_HOR;
    }
	}
	else if (needVerGreen_forPedLeft) {
		if (carPhase == PHASE_HOR_GREEN) {
       	 carPhase = PHASE_SWITCH_TO_VER;
    	}
	
	}

	if (needHorGreen_forPedUp && carPhase == PHASE_HOR_GREEN) {
    	prevUpCars = upCarsWaiting;    	
		prevLeftCars = leftCarsWaiting;
    	return;
	}

	if (needVerGreen_forPedLeft && carPhase == PHASE_VER_GREEN) {
    	prevUpCars = upCarsWaiting;    	
		prevLeftCars = leftCarsWaiting;
    	return;
	}

    bool upArrived   = (upCarsWaiting   && !prevUpCars);
    bool leftArrived = (leftCarsWaiting && !prevLeftCars);
    uint32_t redDelayMax = lights->Standard_Delay_Times.redDelay;
    static uint32_t prevIdleRem = 0;


    if (carPhase == PHASE_SWITCH_TO_HOR) {
        bool upDone   = Up_GreenToRed(lights);     
        bool leftDone = Left_RedToGreen(lights);   

        if (upDone && leftDone) {
            carPhase = PHASE_HOR_GREEN;
			Delay_Start(TIMER_CAR_LEFT_GREEN, lights->Standard_Delay_Times.greenDelay);
        	Timer_Stop(TIMER_CAR_UP_GREEN);
            Timer_Stop(TIMER_CAR_UP_RED);
            Timer_Stop(TIMER_CAR_IDLE_SWAP);
			upRedWaitArmed = false;     
			leftRedWaitArmed = false;   
            prevIdleRem = 0;
        }

        prevUpCars = upCarsWaiting;        
		prevLeftCars = leftCarsWaiting;
        return;
    }

    if (carPhase == PHASE_SWITCH_TO_VER) {
        bool leftDone = Left_GreenToRed(lights);   
        bool upDone   = Up_RedToGreen(lights);    

        if (upDone && leftDone) {
            carPhase = PHASE_VER_GREEN;
			Delay_Start(TIMER_CAR_UP_GREEN, lights->Standard_Delay_Times.greenDelay);
        	Timer_Stop(TIMER_CAR_LEFT_GREEN);
            Timer_Stop(TIMER_CAR_LEFT_RED);
            Timer_Stop(TIMER_CAR_IDLE_SWAP);
			upRedWaitArmed = false;
			leftRedWaitArmed = false;
            prevIdleRem = 0;
        }

        prevUpCars = upCarsWaiting;        
		prevLeftCars = leftCarsWaiting;
        return;
    }

    // IDLE MODE
	if (!upCars && !leftCars) {
    uint32_t rem = Delay_Remaining(TIMER_CAR_IDLE_SWAP);
    if (prevIdleRem > 0 && rem == 0) {
        if (carPhase == PHASE_VER_GREEN) carPhase = PHASE_SWITCH_TO_HOR;
        else                            carPhase = PHASE_SWITCH_TO_VER;

        Timer_Stop(TIMER_CAR_LEFT_RED);
        Timer_Stop(TIMER_CAR_UP_RED);
    }
    if (rem == 0) {
        Delay_Start(TIMER_CAR_IDLE_SWAP, lights->Standard_Delay_Times.greenDelay);
        rem = Delay_Remaining(TIMER_CAR_IDLE_SWAP);
    }

    prevIdleRem = rem;

    prevUpCars = upCarsWaiting;    
	prevLeftCars = leftCarsWaiting;
    return;
    }

	else {

        Timer_Stop(TIMER_CAR_IDLE_SWAP);
        prevIdleRem = 0;
    }


    if (carPhase == PHASE_VER_GREEN) {
	bool minGreenRightDone = Delay_IsDone(TIMER_CAR_UP_GREEN);

        if (leftArrived && !upCarsWaiting) {
            carPhase = PHASE_SWITCH_TO_HOR;
            Timer_Stop(TIMER_CAR_LEFT_RED);
			leftRedWaitArmed = false;

			 prevUpCars = upCarsWaiting;        	
			 prevLeftCars = leftCarsWaiting;
       		 return;

        }

        if (leftCarsWaiting  && upCarsWaiting && leftRedWaitArmed!= true) {
            Delay_Start(TIMER_CAR_LEFT_RED, redDelayMax);
			leftRedWaitArmed = true;
        }

		if (leftCarsWaiting  && upCarsWaiting && leftRedWaitArmed== true) {
			if(Delay_IsDone(TIMER_CAR_LEFT_RED) && minGreenRightDone){
			carPhase = PHASE_SWITCH_TO_HOR;
			leftRedWaitArmed = false;

			 prevUpCars = upCarsWaiting;        	
			 prevLeftCars = leftCarsWaiting;
			//Delay_Start(TIMER_CAR_UP_RED, redDelayMax);
			upRedWaitArmed= true;
       		 return;
			}

        }

        if (!leftCarsWaiting ) {
            Timer_Stop(TIMER_CAR_LEFT_RED);
			leftRedWaitArmed = false;

        }

        if (leftCarsWaiting  && leftRedWaitArmed && Delay_IsDone(TIMER_CAR_LEFT_RED)) {
    		carPhase = PHASE_SWITCH_TO_HOR;
    		Timer_Stop(TIMER_CAR_LEFT_RED);
    		leftRedWaitArmed = false;
			prevUpCars = upCarsWaiting;        	
			prevLeftCars = leftCarsWaiting;
        	return;
		}
        if ( minGreenRightDone && !upCarsWaiting && leftCarsWaiting ) {
            carPhase = PHASE_SWITCH_TO_HOR;
            Timer_Stop(TIMER_CAR_LEFT_RED);
			leftRedWaitArmed = false;
			prevUpCars = upCarsWaiting; 	        
			prevLeftCars = leftCarsWaiting;
   	    	return;


        }

    } 
	
	else if (carPhase == PHASE_HOR_GREEN) {
	bool minGreenLeftDone = Delay_IsDone(TIMER_CAR_LEFT_GREEN);

        if (upArrived && !leftCarsWaiting ) {
            carPhase = PHASE_SWITCH_TO_VER;
            Timer_Stop(TIMER_CAR_UP_RED);
			upRedWaitArmed = false;
        	prevUpCars = upCarsWaiting;        	
			prevLeftCars = leftCarsWaiting;
        	return;

        }

        if (upCarsWaiting && leftCarsWaiting  && upRedWaitArmed!=true) {
            Delay_Start(TIMER_CAR_UP_RED, redDelayMax);
			upRedWaitArmed = true;
        }


		 if (upCarsWaiting && leftCarsWaiting  && upRedWaitArmed==true && minGreenLeftDone) {
			if(Delay_IsDone(TIMER_CAR_UP_RED)){
			carPhase = PHASE_SWITCH_TO_VER;
			upRedWaitArmed = false;
        	prevUpCars = upCarsWaiting;        	
			prevLeftCars = leftCarsWaiting;
			//Delay_Start(TIMER_CAR_LEFT_RED, redDelayMax);
			leftRedWaitArmed= true;
        	return;
			}}
        

        if (!upCarsWaiting) {
            Timer_Stop(TIMER_CAR_UP_RED);
			upRedWaitArmed = false;
        }

        if (upCarsWaiting && upRedWaitArmed && Delay_IsDone(TIMER_CAR_UP_RED)) {
    		carPhase = PHASE_SWITCH_TO_VER;
    		Timer_Stop(TIMER_CAR_UP_RED);
    		upRedWaitArmed = false;
			prevUpCars = upCarsWaiting;        	
			prevLeftCars = leftCarsWaiting;
        	return;
		}

        if (minGreenLeftDone &&!leftCarsWaiting  && upCarsWaiting) {
            carPhase = PHASE_SWITCH_TO_VER;
            Timer_Stop(TIMER_CAR_UP_RED);
			upRedWaitArmed = false;
 	       	prevUpCars = upCarsWaiting;    	    
			prevLeftCars = leftCarsWaiting;
        	return;

        }
    }

    prevUpCars = upCarsWaiting;    
	prevLeftCars = leftCarsWaiting;
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
		pedUpDue = false;
		pedLeftDue = false;

		Set_Pl_StatePassiveLeft();
		Set_Pl_StatePassiveUp();

		carPhase     = PHASE_VER_GREEN;
        carUpState   = CAR_G;
        carLeftState = CAR_R;

		Delay_Start(TIMER_CAR_UP_GREEN, lightsState->Standard_Delay_Times.greenDelay);
		Timer_Stop(TIMER_CAR_LEFT_GREEN);

		Delay_Start(TIMER_CAR_UP_ORANGE,    0);
        Delay_Start(TIMER_CAR_LEFT_ORANGE,  0);
		Delay_Start(TIMER_CAR_UP_GREEN, lightsState->Standard_Delay_Times.greenDelay);
        Delay_Start(TIMER_CAR_LEFT_GREEN,   0);

        Delay_Start(TIMER_CAR_IDLE_SWAP,    0);
        Delay_Start(TIMER_CAR_LEFT_RED,  0);
        Delay_Start(TIMER_CAR_UP_RED,  0);

		prevUpCars   = false;
        prevLeftCars = false;
	}

	Delay_Tick(DT_MS);

	PedLeft_Tick(lightsState, inputState);
	PedUp_Tick(lightsState, inputState);

	setPedOutputs();

	Car_Tick(lightsState, inputState);
	setCarOutputs();


	lightsState->Horizontal_Traffic_Light_State.Delays.pedestrianDelay = Delay_Remaining(TIMER_PED_LEFT_WAIT);
	lightsState->Horizontal_Traffic_Light_State.Delays.walkingDelay = Delay_Remaining(TIMER_PED_LEFT_WALK);
	lightsState->Horizontal_Traffic_Light_State.Delays.greenDelay = Delay_Remaining(TIMER_CAR_LEFT_GREEN);
	lightsState->Horizontal_Traffic_Light_State.Delays.redDelay = Delay_Remaining(TIMER_CAR_LEFT_RED);

	lightsState->Vertical_Traffic_Light_State.Delays.pedestrianDelay = Delay_Remaining(TIMER_PED_UP_WAIT);
	lightsState->Vertical_Traffic_Light_State.Delays.walkingDelay = Delay_Remaining(TIMER_PED_UP_WALK);
	lightsState->Vertical_Traffic_Light_State.Delays.greenDelay = Delay_Remaining(TIMER_CAR_UP_GREEN);
	lightsState->Vertical_Traffic_Light_State.Delays.redDelay = Delay_Remaining(TIMER_CAR_UP_RED);
}

