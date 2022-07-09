#ifndef __GRAPHICS__
#define __GRAPHICS__

#include "color.h"
#include "object.h"

/* screen space

0,0 -> highwst point of the screen

*///////// 

#define LOWEST_RENDER_ORDER 0
#define HIGHTST_RENDER_ORDER 10
#define DEFALUT_RENDER_ORDER 0

#include "vec2.h"

typedef Object Font;
typedef Object Render;

typedef Object Graphics_Window;
typedef Object Graphics;

/// render ///////

void set_sprite_color(Color color, Graphics graphics , Render sprite);
void set_sprite_postion(Vec2 position, Graphics graphics, Render sprite);
void set_sprite_scale(Vec2 scale, Graphics graphics, Render sprite);

void set_sprite_to_not_render(Render sprite, Graphics graphics);
void set_sprite_to_render(Render sprite, Graphics graphics);

void set_sprite_render_order(int rende_order, Graphics graphics, Render sprite);

Vec2 get_sprite_position(Render sprite , Graphics graphics);
Vec2 get_sprite_sclae(Render sprite , Graphics graphics);

Render dup_sprite(Render sprite, Graphics graphics);

void set_text_color(Color color, Graphics graphics , Render text);
void set_text_postion(Vec2 position, Graphics graphics  , Render text);
void set_text_scale(Vec2 scale, Graphics graphics , Render text);

void set_text_to_not_render(Render text, Graphics graphics);
void set_text_to_render(Render text, Graphics graphics);

void set_text_render_order(int render_order, Graphics graphics, Render text);

Vec2 get_text_position(Render text , Graphics graphics);
Vec2 get_text_scale(Render text, Graphics graphics);

Render dup_text(Render text, Graphics graphics);


/// fonts /////
Font create_font(const char* path , Graphics graphics , Vec2 min_point_to_draw_letter_from , Vec2 max_point_to_draw_letter_from);
void destroy_font(Font font , Graphics graphics);


/// text //////
Render create_text(const char* text, Graphics graphics , Font font );
void destroy_text(Render text , Graphics graphics);


/// texture  ///////
Render create_sprite(const char* path , Graphics graphics);
void destroy_sprite(Render sprite, Graphics graphics);


/// render meneger
Graphics create_graphics(Graphics_Window window);
void destroy_graphics(Graphics graphics);

Graphics_Window create_game_window(int width, int hight);
void destroy_game_window(Graphics_Window window);

void set_background_color(Color color , Graphics graphics);
void draw_game(Graphics graphics);

#endif __GRAPHICS__