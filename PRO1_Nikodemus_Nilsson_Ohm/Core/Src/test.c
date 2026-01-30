#include "test.h"

#include "Controller/test_InputController.h"
#include "Controller/test_Potentiometer_Controller.h"

#include "Model/test_delay.h"
#include "Model/test_FSM.h"
#include "Model/test_traffic_state.h"

#include "View/test_oled.h"
#include "View/test_lights.h"

void Test_program(void)
{
	Test_OLED();
	Test_Lights();
	Test_Traffic_State();
	Test_FSM();
	Test_Delay();
	Test_Potentiometer();
	Test_Input();
}

