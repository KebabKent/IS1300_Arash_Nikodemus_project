#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include "main.h"
#include "stdint.h"
#include "stdbool.h"

void readMomentaryButton(GPIO_TypeDef* Port, uint16_t Pin, uint8_t* lastState, bool* outFlag);

void readToggleSwitch(GPIO_TypeDef* Port, uint16_t Pin, bool* outState);

void readAndSetInputsState();


#endif
