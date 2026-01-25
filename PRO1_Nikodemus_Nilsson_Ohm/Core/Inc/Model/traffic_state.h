#ifndef TRAFFIC_STATE_H
#define TRAFFIC_STATE_H

#include <DTO/lights_state.h>
#include <DTO/input_state.h>

void State_Init(void);

LightsState_t* Return_LightsState(void);

InputState_t* Return_InputState(void);



#endif
