#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "my_malloc.h"

#include "game_manager.h"
#include "game.h"
#include "graphics.h"
#include "program_events.h"
#include "input_system.h"
#include "buttons.h"

Graphics_Window window;
Graphics graphics;

Buttons buttons;

void start_game_manager()
{
	struct program_events* events = (struct program_events*)my_malloc(sizeof(struct program_events));

	events->quit = handle_quit;
	events->set_curent_key_pressed = set_curent_key_pressed;
	events->on_mose_clicked = buttons_on_mose_click;

	program_events_init(events);

	window = create_game_window(WIDTH, HIGHT);
	graphics = create_graphics(window);
	
	buttons = create_buttons();

	set_game_graphics(graphics);
	set_game_buttons(buttons);

	start_game();
	
	while (true)
	{
		poll_and_handle_program_events();
		
		draw_game(graphics);
		update_game();
	}
}

void handle_quit() 
{
	destroy_graphics(graphics);
	destroy_game_window(window);

	destroy_buttons(buttons);

	exit(0);
}