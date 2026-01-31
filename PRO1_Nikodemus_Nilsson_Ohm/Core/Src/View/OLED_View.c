/**
  ******************************************************************************
  * @file    OLED_View.c
  * @author  [Your Name]
  * @version 1.0
  * @date    [Current Date]
  * @brief   OLED Display Output for Traffic Light System.
  *
  * This file handles all graphical output to the SSD1306 OLED display.
  * It translates the system state (timer delays) into visual progress bars
  * and labels, implementing the "View" component of the MVC architecture.
  ******************************************************************************
  */
#include <DTO/lights_state.h>
#include "stdint.h"
#include "main.h"
#include "View/TrafficLight_SPI_Driver.h"
#include "Model/traffic_state.h"

#include "ssd1306.h"
#include "ssd1306_fonts.h"

/**
 * @brief Renders a vertical progress bar for a specific timer delay.
 *
 * This helper function draws a labeled rectangle on the OLED screen. It calculates
 * the height of the filled portion based on the ratio of the current remaining
 * delay to the standard (maximum) delay time.
 *
 * @param text               Pointer to a string label (e.g., "P1") to display below the bar.
 * @param pos                The X-coordinate starting position of the bar on the screen (0-127).
 * @param delay              The current remaining time for this specific timer.
 * @param standardDelayTime  The total duration of this timer (used for scaling).
 * @return void
 */
void set_delay(char text[], uint8_t pos, uint16_t delay, uint16_t standardDelayTime) {
	if (standardDelayTime == 0) return;

	uint8_t scaledRemainingTime = (50 * delay) / standardDelayTime;

	ssd1306_SetCursor(pos, 54);
	ssd1306_WriteString(text, Font_7x10, Black);

	ssd1306_DrawRectangle(pos + 3, 1, pos + 10, 51, Black);
	ssd1306_FillRectangle(pos + 3, 51 - scaledRemainingTime, pos + 10, 51, Black);
}

/**
 * @brief Updates the full OLED screen with the current system state.
 *
 * This function clears the screen buffer and redraws all eight status bars
 * corresponding to the Horizontal and Vertical traffic light timers (Pedestrian,
 * Walking, Green, Red). It then pushes the buffer to the physical display.
 *
 * @param state Pointer to the global LightsState_t model containing current delay values.
 * @return void
 */
void update_OLED(LightsState_t* state) {
	ssd1306_Fill(White);

	set_delay("P1", 0, state->Vertical_Traffic_Light_State.Delays.pedestrianDelay, state->Standard_Delay_Times.pedestrianDelay);
	set_delay("W1", 16, state->Vertical_Traffic_Light_State.Delays.walkingDelay, state->Standard_Delay_Times.walkingDelay);

	set_delay("P2", 32, state->Horizontal_Traffic_Light_State.Delays.pedestrianDelay, state->Standard_Delay_Times.pedestrianDelay);
	set_delay("W2", 48, state->Horizontal_Traffic_Light_State.Delays.walkingDelay, state->Standard_Delay_Times.walkingDelay);

	set_delay("G1", 64, state->Vertical_Traffic_Light_State.Delays.greenDelay, state->Standard_Delay_Times.greenDelay);
	set_delay("R1", 80, state->Vertical_Traffic_Light_State.Delays.redDelay, state->Standard_Delay_Times.redDelay);

	set_delay("G2", 96, state->Horizontal_Traffic_Light_State.Delays.greenDelay, state->Standard_Delay_Times.greenDelay);
	set_delay("R2", 112, state->Horizontal_Traffic_Light_State.Delays.redDelay, state->Standard_Delay_Times.redDelay);

	ssd1306_UpdateScreen();
}

