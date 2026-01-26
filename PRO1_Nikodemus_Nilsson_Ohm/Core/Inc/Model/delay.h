#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>
#include <stdbool.h>

typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;

typedef enum {
    TIMER_PED_LEFT_WAIT,
    TIMER_PED_LEFT_WALK,

    TIMER_PED_UP_WAIT,
    TIMER_PED_UP_WALK,

    TIMER_CAR_ORANGE,

    TIMER_COUNT
} TimerId;

void Delay_Init(void);
void Delay_Tick(uint32_t dt_ms);

void Delay_Start(TimerId id, uint32_t duration_ms);
bool Delay_IsDone(TimerId id);
uint32_t Delay_Remaining(TimerId id);

#endif