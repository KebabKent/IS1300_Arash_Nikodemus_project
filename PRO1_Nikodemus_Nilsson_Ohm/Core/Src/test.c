/**
  ******************************************************************************
  * @file    test.c
  * @author  [Your Name]
  * @version 1.0
  * @date    [Current Date]
  * @brief   Main test runner for the Traffic Light application.
  *
  * This file acts as the central hub for the Test-Driven Development (TDD)
  * workflow. It aggregates all unit and integration tests from the Model,
  * View, and Controller layers into a single execution flow.
  ******************************************************************************
  */
#include "test.h"

#include "Controller/test_InputController.h"
#include "Controller/test_Potentiometer_Controller.h"

#include "Model/test_delay.h"
#include "Model/test_FSM.h"
#include "Model/test_traffic_state.h"

#include "View/test_oled.h"
#include "View/test_lights.h"

/**
 * @brief  Executes the master test suite for the entire system.
 *
 * This function calls the individual test runners for each module in a specific
 * sequence. It allows for full system verification before the main scheduler starts.
 *
 * @note   This function is called from main.c only when RUN_TEST_PROGRAM is defined.
 *
 * @return void
 */
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

