#include <DTO/lights_state.h>
#include "stdint.h"
#include "main.h"
#include "View/TrafficLight_SPI_Driver.h"
#include "Model/traffic_state.h"

#include "ssd1306.h"
#include "ssd1306_fonts.h"

void set_delay(char text[], uint8_t pos, uint16_t delay, uint16_t standardDelayTime) {
	if (standardDelayTime == 0) return;

	uint8_t scaledRemainingTime = (50 * delay) / standardDelayTime;

	ssd1306_SetCursor(pos, 54);
	ssd1306_WriteString(text, Font_7x10, Black);

	ssd1306_DrawRectangle(pos + 3, 1, pos + 10, 51, Black);
	ssd1306_FillRectangle(pos + 3, 51 - scaledRemainingTime, pos + 10, 51, Black);
}

void update_OLED() {

	LightsState_t* state;
	state = Return_LightsState();


	ssd1306_Fill(White);

	set_delay("P1", 0, state->Horizontal_Traffic_Light_State.Delays.pedestrianDelay, state->Standard_Delay_Times.pedestrianDelay);
	set_delay("W1", 16, state->Horizontal_Traffic_Light_State.Delays.walkingDelay, state->Standard_Delay_Times.walkingDelay);

	set_delay("P2", 32, state->Vertical_Traffic_Light_State.Delays.pedestrianDelay, state->Standard_Delay_Times.pedestrianDelay);
	set_delay("W2", 48, state->Vertical_Traffic_Light_State.Delays.walkingDelay, state->Standard_Delay_Times.walkingDelay);

	set_delay("G1", 64, state->Horizontal_Traffic_Light_State.Delays.greenDelay, state->Standard_Delay_Times.greenDelay);
	set_delay("R1", 80, state->Horizontal_Traffic_Light_State.Delays.redDelay, state->Standard_Delay_Times.redDelay);

	set_delay("G2", 96, state->Vertical_Traffic_Light_State.Delays.greenDelay, state->Standard_Delay_Times.greenDelay);
	set_delay("R2", 112, state->Vertical_Traffic_Light_State.Delays.redDelay, state->Standard_Delay_Times.redDelay);

	ssd1306_UpdateScreen();
}

