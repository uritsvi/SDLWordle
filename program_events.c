#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "program_events.h"

struct program_events* m_events;

void program_events_init(struct program_events* events) 
{
	m_events = events;
}

void poll_and_handle_program_events() 
{
	m_events->set_curent_key_pressed(0);

	if (m_events == NULL) 
	{
		printf("mising events");
		exit(-1);
	}

	SDL_Event _event;

	SDL_PollEvent(&_event);

	switch (_event.type)
	{
	case SDL_QUIT: m_events->quit(); break;

	case SDL_KEYDOWN: m_events->set_curent_key_pressed(SDL_GetKeyFromScancode(_event.key.keysym.scancode)); break;

	case SDL_MOUSEBUTTONDOWN: m_events->on_mose_clicked(vec2(_event.button.x, _event.button.y)); break;

	default:return;
	break;
	}
}
