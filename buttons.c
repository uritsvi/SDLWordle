#include <stddef.h>
#include <stdlib.h>
#include <memory.h>

#include "my_malloc.h"

#include "buttons.h"
#include "list.h"

struct rect
{
	int x_min;
	int x_max;
	int y_min;
	int y_max;
};

struct buttons
{
	struct list* buttons;
};

struct button
{
	button_call_back call_back;
	void** data;

	Render sprite;
	Graphics graphics;

	struct rect rect;
	
	Vec2 last_position;
	Vec2 last_scale;
};

struct list* buttons_objects = NULL;

struct buttons* get_buttons_object(Buttons buttons)
{
	return (struct buttons*)get_cell_by_index(buttons_objects, buttons)->data;
}
struct button* get_button(struct buttons* buttons , Button button)
{
	return (struct button*)get_cell_by_index(buttons->buttons, button)->data;
}

struct rect create_rect(Vec2 position , Vec2 scale)
{
	struct rect rect = { .x_min = position.x - (scale.x / 2) ,
						 .x_max = position.x + (scale.x / 2) , 
						 .y_min = position.y - (scale.y / 2) , 
						 .y_max = position.y + (scale.y / 2) };
	return rect;
}


Buttons create_buttons()
{
	if(buttons_objects == NULL)
	{
		buttons_objects = create_list();
	}

	struct buttons* buttons = (struct buttons*)my_malloc(sizeof(struct buttons));
	buttons->buttons = create_list();

	add_object_to_list(buttons, buttons_objects);

	return get_new_object_index(buttons_objects);
}
void destroy_buttons(Buttons buttons)
{
	destroy_list_and_data(buttons_objects);
}

void buttons_on_mose_click(Vec2 position)
{
	for(int curent_buttons_object_index = 0; curent_buttons_object_index< get_list_len(buttons_objects); curent_buttons_object_index++)
	{
		struct buttons* buttons_object = get_buttons_object(curent_buttons_object_index);
		for(int curent_button_index = 0; curent_button_index < get_list_len(buttons_object->buttons); curent_button_index++)
		{
			struct button* button = get_button(buttons_object, curent_button_index);

			Vec2 button_sprite_curent_position = get_sprite_position(button->sprite, button->graphics);
			Vec2 button_sprite_curent_scale = get_sprite_sclae(button->sprite, button->graphics);

			if(memcmp(&button_sprite_curent_position , &button->last_position , sizeof(Vec2)) != 0 ||
			memcmp(&button_sprite_curent_scale , &button->last_scale , sizeof(Vec2) != 0))
			{
				button->rect = create_rect(button_sprite_curent_position, button_sprite_curent_scale);
			}

			if(position.x >= button->rect.x_min && position.x <= button->rect.x_max
			   && position.y >= button->rect.y_min && position.y <= button->rect.y_max)
			{
				button->call_back(button->data);
			}
		}
	}
}

Button create_button_from_sprite(Render sprite, Graphics graphics, Buttons buttons)
{
	struct button* button = (struct button*)my_malloc(sizeof(struct button));

	Vec2 sprite_position = get_sprite_position(sprite, graphics);
	Vec2 sprite_scale = get_sprite_sclae(sprite, graphics);

	button->data = NULL;
	button->call_back = NULL;
	button->last_position = sprite_position;
	button->last_scale = sprite_scale;
	button->rect = create_rect(sprite_position, sprite_scale);
	button->sprite = sprite;
	button->graphics = graphics;

	struct buttons* m_buttons = get_buttons_object(buttons);
	add_object_to_list(button, m_buttons->buttons);
	
	return get_new_object_index(m_buttons->buttons);
}
void destroy_button(Button button , Buttons buttons)
{
	struct buttons* m_buttons = get_buttons_object(buttons);
	struct button* m_button = get_button(m_buttons, button);

	remove_object_from_list(m_buttons->buttons, button);

	free(m_button);
}

void set_button_callback(Button button, Buttons buttons, button_call_back call_back , void** data)
{
	struct buttons* m_buttons = get_buttons_object(buttons);
	struct button* m_button = get_button(m_buttons, button);

	m_button->data = data;
	m_button->call_back = call_back;
}