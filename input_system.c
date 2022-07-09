#include "input_system.h"

char last_key_pressed = '\0';

void set_curent_key_pressed(char key) // set 0 if any key is not pressed
{
	last_key_pressed = key;
}

char get_key_down() 
{
	return last_key_pressed;
}
