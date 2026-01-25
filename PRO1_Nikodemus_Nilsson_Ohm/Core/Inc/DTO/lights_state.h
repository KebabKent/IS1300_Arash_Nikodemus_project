#ifndef LIGHTS_STATE_H
#define LIGHTS_STATE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	RED_Tr = 0,
	ORANGE_Tr = 1,
	GREEN_Tr = 2
} TrafficLightState;

typedef enum {
	RED_Pd = 0,
	GREEN_Pd = 1
} PedestrianLightState;

typedef struct {
	uint16_t pedestrianDelay;
	uint16_t walkingDelay;
	uint16_t greenDelay;
	uint16_t orangeDelay;
	uint16_t redDelay;
} Delays;

typedef struct {
	TrafficLightState Light_State_LU;
	TrafficLightState Light_State_RD;

	PedestrianLightState Pdst_State;
	bool toggle;

	Delays Delays;
} TrafficLight;

typedef struct {
	uint16_t toggleFrequenzy;
	Delays Standard_Delay_Times;
	TrafficLight Horizontal_Traffic_Light_State;
	TrafficLight Vertical_Traffic_Light_State;
} LightsState_t;

#endif
