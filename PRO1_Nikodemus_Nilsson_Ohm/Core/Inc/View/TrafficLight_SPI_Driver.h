#ifndef TRAFFICLIGHT_SPI_DRIVER
#define TRAFFICLIGHT_SPI_DRIVER

#ifdef __cplusplus
extern "C" {
#endif

#include <DTO/lights_state.h>

typedef struct {
	uint8_t data[3];
} spiData;

void set_Traffic_Light(uint8_t* data, uint32_t bit, uint8_t ofst, TrafficLightState* state);
void set_Pedestrian_Light(uint8_t* data, uint32_t bit, uint8_t ofst, PedestrianLightState* state);

void toggle_Pedestrian_Blue(uint8_t* data, uint32_t bit, uint8_t ofst, bool* state, uint16_t* toggleFrequenzy);

spiData Decode_State(LightsState_t* state);

void Set_TrafficLights(LightsState_t* state);

#ifdef __cplusplus
}
#endif

#endif
