/**
 * @file test_oled.c
 * @brief Test suite for OLED View verification.
 * @author [Your Name]
 * @version 1.0
 * @date [Current Date]
 */

#include "View/test_oled.h"
#include "View/OLED_View.h"
#include <DTO/lights_state.h>
#include "ssd1306.h"
#include "main.h"

void Test_OLED(void) {
	ssd1306_Init();
	Test_set_delay();
	Test_update_OLED();
}

/**
 * @brief Verifies the progress bar scaling logic.
 * * Manually tests the set_delay function with specific values to verify
 * that the rectangle fill height scales correctly to 50 pixels.
 * @return void
 */
void Test_set_delay(void) {
    ssd1306_Fill(White);

    set_delay("1", 0, 100, 100);
    set_delay("2", 32, 75, 100);
    set_delay("3", 64, 50, 100);
    set_delay("4", 96, 25, 100);

    ssd1306_UpdateScreen();
    HAL_Delay(2000);
}

/**
 * @brief Integration test for the full OLED update function.
 * * Simulates different system states to verify that all 8 progress bars
 * (Pedestrian, Walking, Green, Red for both roads) render in the correct positions.
 * @return void
 */
void Test_update_OLED(void) {
    LightsState_t testState;

    testState.Standard_Delay_Times.pedestrianDelay = 1000;
    testState.Standard_Delay_Times.walkingDelay = 1000;
    testState.Standard_Delay_Times.greenDelay = 1000;
    testState.Standard_Delay_Times.redDelay = 1000;

    testState.Horizontal_Traffic_Light_State.Delays.pedestrianDelay = 500;
    testState.Horizontal_Traffic_Light_State.Delays.walkingDelay = 500;
    testState.Horizontal_Traffic_Light_State.Delays.greenDelay = 500;
    testState.Horizontal_Traffic_Light_State.Delays.redDelay = 500;

    testState.Vertical_Traffic_Light_State.Delays.pedestrianDelay = 1000;
    testState.Vertical_Traffic_Light_State.Delays.walkingDelay = 1000;
    testState.Vertical_Traffic_Light_State.Delays.greenDelay = 1000;
    testState.Vertical_Traffic_Light_State.Delays.redDelay = 1000;

    update_OLED(&testState);
    HAL_Delay(2000);
}

