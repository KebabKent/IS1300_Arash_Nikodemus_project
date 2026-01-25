#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include <DTO/input_state.h>

void readButtonLeft(bool* buttonPressed);

void readButtonUp(void);

void readSwitchLeft(void);

void readSwitchUp(void);

void readSwitchRight(void);

void readSwitchDown(void);

void readAndSetInputsState();


#endif
