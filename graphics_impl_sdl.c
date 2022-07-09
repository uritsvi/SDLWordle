#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include "my_malloc.h"
#include "graphics.h"
#include "list.h"
#include "color.h"

#define DEFALUT_RENDER_SCALE 100
#define FONT_DEFALUT_SIZE 900

SDL_Color defalut_color = { .r = 0 , .g = 0 , .b = 0 , .a = HIGHEST_COLOR_VALUE };
SDL_Rect defalut_rect = { .x = 0 , .y = 0 , .w = 100 , .h = 100 };

const Vec2 defalut_position = { .x = 50 , .y = 50 };
const Vec2 defalut_scale = { .x = 100 , .y = 100 };


struct graphics_window
{
	int width;
	int hight;

	SDL_Window* window;
};

struct graphics
{
	struct list* sprites;
	struct list* texts;
	struct list* fonts;

	SDL_Renderer* renderer;
};

struct render
{
	bool is_renderrd;

	void** render;
	
	SDL_Rect rect;
	
	Vec2 scale;
	Vec2 absolute_position;
	
	int render_order;

};

struct sprite
{
	SDL_Texture* sdl_texture;
	SDL_Surface* sdl_surface; // for duplication
};

struct font
{
	TTF_Font* ttf_font;
	SDL_Rect src_rect;
};
struct text
{
	char* text;
	struct font* font;

	SDL_Texture* sdl_texture;
	SDL_Surface* sdl_surface; // for duplication

	SDL_Rect src_rect;
};


struct list* graphics_window_list = NULL;
struct list* graphics_list = NULL;


struct graphics* get_graphics(Graphics graphics) 
{
	return (struct graphics*)get_cell_by_index(graphics_list, graphics)->data;
}

///////////////////
// all methods get a graphics struct because all of the methodes already need the graphics struct
//////////////////

struct render* get_sprite_render(struct graphics* graphics , Render sprite)
{
	return (struct render*)get_cell_by_index(graphics->sprites, sprite)->data;
}
struct render* get_text_render(struct graphics* graphics , Render text)
{
	return (struct render*)get_cell_by_index(graphics->texts, text)->data;
}
struct font* get_font(struct graphics* graphics , Font font)
{
	return (struct font*)get_cell_by_index(graphics->fonts, font)->data;
}


struct render* create_render()
{
	struct render* render = (struct render*)my_malloc(sizeof(struct render));
	render->render = NULL;

	render->absolute_position = defalut_position;
	render->scale = defalut_scale;

	render->render_order = DEFALUT_RENDER_ORDER;

	return render;
}


void set_sprite_color(Color color, Graphics graphics , Render sprite)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* render = get_sprite_render(m_graphics, sprite);
	struct sprite* m_sprite = (struct sprite*)render->render;

	SDL_SetTextureAlphaMod(m_sprite->sdl_texture, HIGHEST_COLOR_VALUE);
	SDL_SetTextureColorMod(m_sprite->sdl_texture, color.r, color.g, color.b);
}
void set_sprite_postion(Vec2 position, Graphics graphics, Render sprite)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* render = get_sprite_render(m_graphics, sprite);

	render->absolute_position = position;

	render->rect.x = position.x - (render->scale.x / 2);
	render->rect.y = position.y - (render->scale.y / 2);
}
void set_sprite_scale(Vec2 scale, Graphics graphics, Render sprite)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* render = get_sprite_render(m_graphics, sprite);

	render->scale = scale;

	render->rect.x = render->absolute_position.x - (scale.x / 2);
	render->rect.y = render->absolute_position.y - (scale.y / 2);

	render->rect.w = scale.x;
	render->rect.h = scale.y;
}

void set_sprite_to_not_render(Render sprite, Graphics graphics)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* render = get_sprite_render(m_graphics, sprite);

	render->is_renderrd = false;
}
void set_sprite_to_render(Render sprite, Graphics graphics)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* render = get_sprite_render(m_graphics, sprite);

	render->is_renderrd = true;
}

Vec2 get_sprite_position(Render sprite, Graphics graphics)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* render = get_sprite_render(m_graphics, sprite);

	return render->absolute_position;
}
Vec2 get_sprite_sclae(Render sprite, Graphics graphics)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* render = get_sprite_render(m_graphics, sprite);

	return render->scale;
}

Render dup_sprite(Render sprite, Graphics graphics)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* old_render = get_sprite_render(m_graphics, sprite);
	
	struct sprite* old_sprite = (struct sprite*)old_render->render;
	struct sprite* new_sprite = (struct sprite*)my_malloc(sizeof(struct sprite));
	memcpy(new_sprite, old_render, sizeof(struct sprite));

	struct render* new_render = create_render();
	memcpy(new_render, old_render, sizeof(struct render));
	
	new_render->render = (void**)new_sprite;

	new_sprite->sdl_surface = SDL_DuplicateSurface(old_sprite->sdl_surface);
	new_sprite->sdl_texture = SDL_CreateTextureFromSurface(m_graphics->renderer , new_sprite->sdl_surface);

	add_object_to_list(new_render, m_graphics->sprites);
	return get_new_object_index(m_graphics->sprites);
}

void set_sprite_render_order(int rende_order, Graphics graphics, Render sprite)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* render = get_sprite_render(m_graphics, sprite);


	if (rende_order > HIGHTST_RENDER_ORDER)
	{
		render->render_order = HIGHTST_RENDER_ORDER;
		return;
	}
	else if (rende_order < LOWEST_RENDER_ORDER)
	{
		render->render_order = LOWEST_RENDER_ORDER;
		return;
	}

	render->render_order = rende_order;
}


void set_text_color(Color color, Graphics graphics, Render text)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* render = get_text_render(m_graphics, text);
	struct text* m_text = render->render;

	SDL_Color sdl_color;

	sdl_color.r = color.r;
	sdl_color.g = color.g;
	sdl_color.b = color.b;
	sdl_color.a = HIGHEST_COLOR_VALUE;

	SDL_Surface* surfes = TTF_RenderText_Solid(m_text->font->ttf_font, m_text->text, sdl_color);
	m_text->sdl_texture = SDL_CreateTextureFromSurface(m_graphics->renderer, surfes);

	SDL_FreeSurface(surfes);
}
void set_text_postion(Vec2 position, Graphics graphics, Render text)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* render = get_text_render(m_graphics, text);

	render->absolute_position = position;

	render->rect.x = position.x - (render->scale.x / 2);
	render->rect.y = position.y - (render->scale.y / 2);
}
void set_text_scale(Vec2 scale, Graphics graphics, Render text)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* render = get_text_render(m_graphics, text);

	render->scale = scale;

	render->rect.x = render->absolute_position.x - (scale.x / 2);
	render->rect.y = render->absolute_position.y - (scale.y / 2);

	render->rect.w = scale.x;
	render->rect.h = scale.y;
}

void set_text_to_not_render(Render text, Graphics graphics)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* render = get_text_render(m_graphics, text);

	render->is_renderrd = false;
}
void set_text_to_render(Render text, Graphics graphics)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* render = get_text_render(m_graphics, text);

	render->is_renderrd = true;
}

Render dup_text(Render text, Graphics graphics)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* old_render = get_text_render(m_graphics, text);

	struct render* new_render = create_render();

	struct text* old_text = (struct text*)old_render->render;
	struct text* new_text = (struct text*)my_malloc(sizeof(struct text));

	memcpy(new_render, old_render, sizeof(struct render));
	memcpy(new_text, old_text , sizeof(struct text));

	new_render->render = (void**)new_text;

	SDL_Surface* surfes = SDL_DuplicateSurface(old_text->sdl_surface);
	new_text->sdl_texture = SDL_CreateTextureFromSurface(m_graphics->renderer , surfes);
	new_text->sdl_surface = surfes;
	strcpy(new_text->text, old_text->text);

	add_object_to_list(new_render, m_graphics->texts);
	return get_new_object_index(m_graphics->texts);
}

void set_text_render_order(int rende_order, Graphics graphics, Render text)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* render = get_text_render(m_graphics, text);

	if(rende_order > HIGHTST_RENDER_ORDER)
	{
		render->render_order = HIGHTST_RENDER_ORDER;
		return;
	}
	else if(rende_order < LOWEST_RENDER_ORDER)
	{
		render->render_order = LOWEST_RENDER_ORDER;
		return;
	}

	render->render_order = rende_order;
}

Vec2 get_text_position(Render text, Graphics graphics)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* render = get_text_render(m_graphics, text);

	return render->absolute_position;
}
Vec2 get_text_scale(Render text , Graphics graphics)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* render = get_text_render(m_graphics, text);

	return render->scale;
}

/// fonts /////
Font create_font(const char* path, Graphics graphics , Vec2 min_point_to_draw_letter_from, Vec2 max_point_to_draw_letter_from)
{
	struct graphics* m_graphics = get_graphics(graphics);

	struct font* font = (struct font*)my_malloc(sizeof(struct font));
	
	SDL_Rect src_rect;
	src_rect.x = min_point_to_draw_letter_from.x;
	src_rect.y = min_point_to_draw_letter_from.y;
	
	src_rect.w = max_point_to_draw_letter_from.x;
	src_rect.h = max_point_to_draw_letter_from.y;

	font->ttf_font = TTF_OpenFont(path, FONT_DEFALUT_SIZE);
	font->src_rect = src_rect;

	add_object_to_list(font, m_graphics->fonts);

	return get_new_object_index(m_graphics->fonts);
}
void destroy_font(Font font , Graphics graphics)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct font* m_font = get_font(m_graphics, font);

	remove_object_from_list(m_graphics->fonts, font);

	TTF_CloseFont(m_font->ttf_font);
	free(m_font);
}


/// text //////
Render create_text(const char* text, Graphics graphics, Font font)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct font* m_font = get_font(m_graphics, font);
	struct render* render = create_render();
	struct text* m_text = (struct text*)my_malloc(sizeof(struct text));

	SDL_Surface* surfes = TTF_RenderText_Solid(m_font->ttf_font, text, defalut_color);

	SDL_Rect src_rect;
	src_rect.x = m_font->src_rect.x;
	src_rect.y = m_font->src_rect.y;
	src_rect.w = m_font->src_rect.w * strlen(text);
	src_rect.h = m_font->src_rect.h;

	m_text->text = strdup(text);
	m_text->font = m_font;
	m_text->sdl_texture = SDL_CreateTextureFromSurface(m_graphics->renderer, surfes);
	m_text->sdl_surface = surfes;
	m_text->src_rect = src_rect;

	render->is_renderrd = true;
	render->rect = defalut_rect;
	render->render = (void**)m_text;

	add_object_to_list(render, m_graphics->texts);
	return get_new_object_index(m_graphics->texts);
}
void destroy_text(Render text , Graphics graphics)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct node* cell = get_cell_by_index(m_graphics->texts, text);

	struct render* render = (struct render*)cell->data;
	struct text* m_text = (struct text*)render->render;
	
	SDL_DestroyTexture(m_text->sdl_texture);
	SDL_FreeSurface(m_text->sdl_surface);

	free(m_text);
	free(render);

	cell->data = NULL;	
}


/// texture  ///////	
Render create_sprite(const char* path, Graphics graphics)
{
	struct render* render = create_render();
	struct sprite* sprite = (struct sprite*)my_malloc(sizeof(struct sprite));
	struct graphics* m_graphics = get_graphics(graphics);
	
	render->is_renderrd = true;

	SDL_Surface* surfes = IMG_Load(path);
	
	sprite->sdl_texture = SDL_CreateTextureFromSurface(m_graphics->renderer, surfes);
	sprite->sdl_surface = surfes;

	render->render = (void**)sprite;
	render->rect = defalut_rect;

	add_object_to_list(render, m_graphics->sprites);

	return get_new_object_index(m_graphics->sprites);
}
void destroy_sprite(Render sprite, Graphics graphics)
{
	struct graphics* m_graphics = get_graphics(graphics);
	struct render* m_render = get_sprite_render(m_graphics, sprite);
	struct sprite* m_sprite = (struct sprite*)m_render->render;

	remove_object_from_list(m_graphics->sprites, sprite);

	SDL_DestroyTexture(m_sprite->sdl_texture);
	SDL_FreeSurface(m_sprite->sdl_surface);
}


/// render meneger
Graphics create_graphics(Graphics_Window window)
{
	if (TTF_WasInit() == false)
	{
		TTF_Init();
	}
	if(SDL_WasInit(0) == false)
	{
		SDL_Init(0);
	}

	if (graphics_list == NULL) 
	{
		graphics_list = create_list();
	}

	struct graphics* graphics = (struct graphics*)my_malloc(sizeof(struct graphics));
	
	struct graphics_window* m_window = (struct graphics_window*)get_cell_by_index(graphics_window_list, window)->data;
	SDL_Renderer* renderer = SDL_CreateRenderer(m_window->window, get_list_len(graphics_window_list), SDL_RENDERER_TARGETTEXTURE);

	graphics->sprites = create_list();
	graphics->texts = create_list();
	graphics->fonts = create_list();
	graphics->renderer = renderer;

	add_object_to_list(graphics, graphics_list);

	return get_new_object_index(graphics_list);
}
void destroy_graphics(Graphics graphics)
{
	struct graphics* m_graphics = get_graphics(graphics);

	destroy_list_and_data(m_graphics->sprites);
	destroy_list_and_data(m_graphics->texts);
	destroy_list_and_data(m_graphics->fonts);

	SDL_DestroyRenderer(m_graphics->renderer);

	remove_object_from_list(graphics_list, graphics);
	free(m_graphics);
}

Graphics_Window create_game_window(int width, int hight)
{
	if (graphics_window_list == NULL)
	{
		graphics_window_list = create_list();
	}

	struct graphics_window* window = (struct graphics_window*)my_malloc(sizeof(struct graphics_window));

	window->window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, hight, 0);
	window->width = width;
	window->hight = hight;

	add_object_to_list(window , graphics_window_list);

	return get_new_object_index(graphics_window_list);
}
void destroy_game_window(Graphics_Window window)
{
	struct graphics_window* m_window = (struct graphics_window*)get_cell_by_index(graphics_window_list, window)->data;

	SDL_DestroyWindow(m_window->window);

	remove_object_from_list(graphics_window_list, window);
	free(m_window);
}

void set_background_color(Color color , Graphics graphics)
{
	struct graphics* m_graphics =get_graphics(graphics);
	SDL_SetRenderDrawColor(m_graphics->renderer, color.r, color.g, color.b, HIGHEST_COLOR_VALUE);
}

void draw_game(Graphics graphics)
{
	struct graphics* m_graphics = get_graphics(graphics);
	
	SDL_RenderClear(m_graphics->renderer);

	for(int curent_render_order = LOWEST_RENDER_ORDER; curent_render_order <= HIGHTST_RENDER_ORDER; curent_render_order++)
	{
		for (int sprit_index = 0; sprit_index < get_list_len(m_graphics->sprites); sprit_index++)
		{
			struct render* render = get_sprite_render(m_graphics, sprit_index);

			if (render->is_renderrd == false)
			{
				continue;
			}

			if(render->render_order != curent_render_order)
			{
				continue;
			}

			struct sprite* sprite = (struct sprite*)render->render;

			SDL_RenderCopy(m_graphics->renderer, sprite->sdl_texture, NULL, &render->rect);
		}
		for (int text_index = 0; text_index < get_list_len(m_graphics->texts); text_index++)
		{
			struct render* render = get_text_render(m_graphics, text_index);

			if (render == NULL || render->is_renderrd == false || render->render_order != curent_render_order)
			{
				continue;
			}

			struct text* text = (struct text*)render->render;

			SDL_RenderCopy(m_graphics->renderer, text->sdl_texture, &text->src_rect, &render->rect);
		}
	}


	SDL_RenderPresent(m_graphics->renderer);

}