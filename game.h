#ifndef __GAME__
#define __GAME__

#include "graphics.h"
#include "buttons.h"

#define WIDTH 600
#define HIGHT 800

void set_game_graphics(Graphics graphics);
void set_game_buttons(Buttons buttons);

void start_game();
void update_game();

#endif // !__GAME__
