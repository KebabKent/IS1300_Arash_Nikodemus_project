#ifndef TRAFFIC_STATE_H
#define TRAFFIC_STATE_H

#include <DTO/lights_state.h>
#include <DTO/input_state.h>

void State_Init(void);

LightsState_t* Return_LightsState(void);

InputState_t* Return_InputState(void);

void Set_Pl_StatePassiveUp(void);
void Set_Pl_StateWaitingUp(void);
void Set_Pl_StateWalkingUp(void);
void Set_Pl_StatePassiveLeft(void);
void Set_Pl_StateWaitingLeft(void);
void Set_Pl_StateWalkingLeft(void);

void Set_Tl_StateVerG(void);
void Set_Tl_StateHorG(void);
void Set_Tl_StateVerO(void);
void Set_Tl_StateHorO(void);
void Set_Tl_StateVerR(void);
void Set_Tl_StateHorR(void);


#endif
