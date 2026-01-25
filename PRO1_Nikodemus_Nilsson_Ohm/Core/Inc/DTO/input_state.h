#ifndef INPUT_STATE_H
#define INPUT_STATE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {

	bool Button_Pressed_Left;
	bool Button_Pressed_Up;

	bool Car_Pesent_Left;
	bool Car_Pesent_Up;
	bool Car_Pesent_Right;
	bool Car_Pesent_Down;
} InputState_t;

#endif
