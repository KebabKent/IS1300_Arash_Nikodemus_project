#ifndef FSM_H
#define FSM_H

#include <DTO/input_state.h>
#include <DTO/lights_state.h>

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

void readAndSet(InputState_t* inputState);

void FSM_Init(void);

#endif
