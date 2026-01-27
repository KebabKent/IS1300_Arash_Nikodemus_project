#include "Model/delay.h"
#include "stdint.h"
#include "stdbool.h"
#include "main.h"

static int32_t timers[TIMER_COUNT];


void Delay_Init(void)
{
    for(int i = 0; i < TIMER_COUNT; i++){
    timers[i] = 0;
    }
}


void Delay_Tick(uint32_t dt_ms){

    for(int i = 0; i < TIMER_COUNT; i++){
    if (timers[i] > 0){
        timers[i] -= (int32_t)dt_ms;
        if (timers[i] < 0) timers[i] = 0;
        }
    }
}


void Delay_Start(TimerId id, uint32_t duration_ms)
{
    timers[id] = (int32_t)duration_ms;
}


bool Delay_IsDone(TimerId id)
{
    return (timers[id] == 0);
}


uint32_t Delay_Remaining(TimerId id)
{
    return (timers[id] > 0) ? (uint32_t)timers[id] : 0;
}
