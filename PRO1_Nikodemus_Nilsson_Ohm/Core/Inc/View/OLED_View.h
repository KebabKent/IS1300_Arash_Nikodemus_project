#ifndef OLED_VIEW
#define OLED_VIEW

#ifdef __cplusplus
extern "C" {
#endif

#include <DTO/lights_state.h>

void set_delay(char text[], uint8_t pos, uint16_t delay, uint16_t standardDelayTime);

void update_OLED(LightsState_t* state);

#ifdef __cplusplus
}
#endif

#endif
