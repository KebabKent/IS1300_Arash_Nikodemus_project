#ifndef TRAFFIC_STATE_H
#define TRAFFIC_STATE_H


typedef enum {
	RED = 0,
	ORANGE,
	GREEN
} TrafficLightState;

typedef struct {
	TrafficLightState Traffic_Light_State;
	uint32_t orangeDelay;

} TrafficLight;

typedef struct {
	TrafficLight Horizontal_Traffic_Light_State;
	TrafficLight Vertical_Traffic_Light_State;

	uint32_t pedestrianDelay;
	uint32_t walkingDelay;
	uint32_t greenDelay;
	uint32_t redDelay;

	uint32_t PotentiometerValue;

} SystemState_t;
