#ifndef __BUTTONS__
#define __BUTTONS__

#include "graphics.h" 

typedef Object Buttons;
typedef Object Button;
typedef void(*button_call_back)(void** data);

Buttons create_buttons();
void destroy_buttons(Buttons buttons);

void buttons_on_mose_click(Vec2 position);

Button create_button_from_sprite(Render sprite , Graphics graphics , Buttons buttons);
void destroy_button(Button button, Buttons buttons);

void set_button_callback(Button button,Buttons buttons , button_call_back call_back , void** data);

#endif // !__BUTTONS__
