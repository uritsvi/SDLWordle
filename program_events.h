#ifndef __PROGRAM__EVENTS__HANDLER__
#define __PROGRAM__EVENTS__HANDLER__

#include "vec2.h"

struct program_events
{
	void(*set_curent_key_pressed)(char key);
	void(*quit)();
	void(*on_mose_clicked)(Vec2 position);
};

void program_events_init(struct program_events* events);
void poll_and_handle_program_events();

#endif // !__PROGRAM__EVENTS__HANDLER__
