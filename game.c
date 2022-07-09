#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "my_malloc.h"

#include "game.h"
#include "input_system.h"
#include "load_file_data.h"


#define NUM_OF_LETTERS_IN_WORD 5
#define NUM_OF_WORDS 6

#define LETTER_CELL_SIZE (WIDTH / 12.0f)
#define DISTANCE_BETWEEN_LETTERS_CELLS LETTER_CELL_SIZE / 5

#define LETTER_RENDER_SCALE LETTER_CELL_SIZE / 2

#define DISTANCE_BETWEEN_CELL_AND_LETTER LETTER_CELL_SIZE / 4 

#define NUM_OF_LETTERS 26

#define LETTER_A_ASCII_VALUE 97
#define LETTER_Z_ASCII_VALUE 132

#define ENTER_KEY_ASCII_VALUE 13
#define BACKSPACE_KEY_ASCII_VALUE 8

#define MIDEL_SCREEN_X (WIDTH / 2)
#define MIDEL_SCREEN_Y (HIGHT / 2)

#define BOARD_POSE_X MIDEL_SCREEN_X 
#define BOARD_POSE_Y MIDEL_SCREEN_Y - (WIDTH / 7.0f)

#define NULL_CHAR '\0'

#define ON_SCREEN_KEY_BACKGROUND_SCALE_X (WIDTH / 18.0f)
#define ON_SCREEN_KEY_BACKGROUND_SCALE_Y (ON_SCREEN_KEY_BACKGROUND_SCALE_X * 1.75f)

#define ON_SCREEN_KEYBORD_LETTER_OR_SYMBOL_SCALE (ON_SCREEN_KEY_BACKGROUND_SCALE_X * 0.75f)

#define TEXT_DEFALUT_COLOR white
#define AN_CORECT_LETTERS_COLOR bright_grey

void init();
void start();

void init_letters();

void on_screen_keyboard_keys_callback(void** letter);

void gen_board();
void gen_background();
void gen_ui();

void gen_reset_button();
void gen_on_screen_keyboard();
void create_on_screen_keyboard_key(Vec2 position, char* letter);

void handle_add_letter_to_board(char letter);
void handle_complit_curent_line();

void handle_game_end(const char* text_to_display);

void handle_lose();
void handle_win();

void handle_destroy_letter_in_board(int index_x, int index_y);

void handle_restart_game(void** data);
void handle_clear_board();
void handel_reset_on_screan_keys_array();

void handle_backspace();
void handle_enter();

void set_correct_word(char* corect_word);

struct letter
{
	Render text_render;

	char charecter;
};

struct on_screen_key
{
	Render background;
	Render letter_render;

	Button button;

	Color color;

	char* letter;
};



struct letter* letters[NUM_OF_LETTERS] = { NULL };
struct on_screen_key* on_screen_keys[NUM_OF_LETTERS] = { NULL };

char corect_word[NUM_OF_LETTERS_IN_WORD + 1] = { 0 };



const Vec2 board_position_min_max = { .x = BOARD_POSE_X - ((NUM_OF_LETTERS_IN_WORD * (DISTANCE_BETWEEN_LETTERS_CELLS + LETTER_CELL_SIZE)) / 2) +
									0.5f * (DISTANCE_BETWEEN_LETTERS_CELLS + LETTER_CELL_SIZE),
									
									.y = BOARD_POSE_Y - ((NUM_OF_WORDS * (DISTANCE_BETWEEN_LETTERS_CELLS + LETTER_CELL_SIZE)) / 2) +
									0.5f * (DISTANCE_BETWEEN_LETTERS_CELLS + LETTER_CELL_SIZE) };

struct letter* board[NUM_OF_WORDS][NUM_OF_LETTERS_IN_WORD] = { NULL };

int curent_letter_index;
int curent_word_index;

bool game_end;

Graphics m_graphics;
Buttons  m_buttons;

Font text_defalut_font;

Button button;
Render sprite;

Render game_end_backgroud_render;
Render game_end_text_render;

void set_game_graphics(Graphics graphics)
{
	m_graphics = graphics;
}
void set_game_buttons(Buttons buttons)
{
	m_buttons = buttons;
}


void start_game()
{
	init();
	start();
}
void update_game()
{
	if (game_end)
	{
		return;
	}

	char curent_key_down = get_key_down();
	if (curent_key_down == NULL_CHAR)
	{
		return;
	}

	if (curent_key_down >= LETTER_A_ASCII_VALUE && curent_key_down <= LETTER_Z_ASCII_VALUE)
	{
		handle_add_letter_to_board(curent_key_down);
		return;
	}
	else if (curent_key_down == BACKSPACE_KEY_ASCII_VALUE)
	{
		handle_backspace();
	}
	else if (curent_key_down == ENTER_KEY_ASCII_VALUE)
	{
		handle_enter();
	}

}

void init()
{
	#define FONT_PATH "MontserratAlternates-Black.otf"

	text_defalut_font = create_font(FONT_PATH, m_graphics, vec2(0, 250), vec2(850, 900));

	init_letters();

	gen_background();
	gen_board();
	gen_ui();
}
void start()
{
	game_end_backgroud_render = OBJECT_NULL_VALUE;
	game_end_text_render = OBJECT_NULL_VALUE;

	game_end = false;

	set_correct_word(corect_word);

	curent_letter_index = 0;
	curent_word_index = 0;
}

void init_letters()
{
	for (int i = 0; i < NUM_OF_LETTERS; i++)
	{
		struct letter* letter = (struct letter*)my_malloc(sizeof(struct letter));

		letter->charecter = LETTER_A_ASCII_VALUE + i;

		char text[2] = { 0 };
		text[0] = letter->charecter;

		letter->text_render = create_text(text, text_defalut_font, m_graphics);

		set_text_to_not_render(letter->text_render, m_graphics);

		letters[i] = letter;
	}
}

void handle_add_letter_to_board(char letter)
{

	if (curent_letter_index == NUM_OF_LETTERS_IN_WORD)
	{
		return;
	}

	Render letter_render = dup_text(letters[letter - LETTER_A_ASCII_VALUE]->text_render, m_graphics);
	set_text_to_render(letter_render, m_graphics);

	set_text_postion(vec2(board_position_min_max.x + curent_letter_index * (LETTER_CELL_SIZE + DISTANCE_BETWEEN_LETTERS_CELLS),
		board_position_min_max.y + curent_word_index * (LETTER_CELL_SIZE + DISTANCE_BETWEEN_LETTERS_CELLS)), m_graphics, letter_render);
	set_text_scale(vec2(LETTER_RENDER_SCALE, LETTER_RENDER_SCALE), m_graphics, letter_render);
	set_text_color(TEXT_DEFALUT_COLOR, m_graphics, letter_render);

	struct letter* letter_struct = (struct letter*)my_malloc(sizeof(struct letter));

	letter_struct->charecter = letter;
	letter_struct->text_render = letter_render;

	board[curent_word_index][curent_letter_index] = letter_struct;

	curent_letter_index++;
}
void handle_destroy_letter_in_board(int index_x, int index_y)
{
	destroy_text(board[index_y][index_x]->text_render, m_graphics);
	free(board[index_y][index_x]);

	board[index_y][index_x] = NULL;
}
void handle_complit_curent_line()
{
	char curent_line_word[NUM_OF_LETTERS_IN_WORD + 1] = { 0 };
	for (int i = 0; i < NUM_OF_LETTERS_IN_WORD; i++)
	{
		curent_line_word[i] = board[curent_word_index][i]->charecter;
	}
	curent_line_word[NUM_OF_LETTERS_IN_WORD] = NULL_CHAR;

	if (strcmpi(curent_line_word, corect_word) == 0)
	{
		for (int i = 0; i < NUM_OF_LETTERS_IN_WORD; i++)
		{
			set_text_color(green, m_graphics, board[curent_word_index][i]->text_render);
		}

		handle_win();
		return;
	}

	char m_corect_word[NUM_OF_LETTERS_IN_WORD + 1];
	strcpy(m_corect_word, corect_word);

	for (int i = 0; i < NUM_OF_LETTERS_IN_WORD; i++)
	{
		if (tolower(curent_line_word[i]) == tolower(m_corect_word[i]))
		{
			set_text_color(green, m_graphics, on_screen_keys[curent_line_word[i] - LETTER_A_ASCII_VALUE]->letter_render);
			on_screen_keys[curent_line_word[i] - LETTER_A_ASCII_VALUE]->color = green;

			m_corect_word[i] = NULL_CHAR;
			curent_line_word[i] = NULL_CHAR;

			set_text_color(green, m_graphics, board[curent_word_index][i]->text_render);
		}
	}

	for (int i = 0; i < NUM_OF_LETTERS_IN_WORD; i++)
	{
		for (int j = 0; j < NUM_OF_LETTERS_IN_WORD; j++)
		{
			if (curent_line_word[j] == NULL_CHAR || m_corect_word[i] == NULL_CHAR)
			{
				continue;
			}

			if (tolower(curent_line_word[j]) != tolower(m_corect_word[i]))
			{
				continue;
			}


			set_text_color(yellow, m_graphics, board[curent_word_index][j]->text_render);

	
			if(cmp_color(on_screen_keys[curent_line_word[j] - LETTER_A_ASCII_VALUE]->color , green) == false)
			{
				set_text_color(yellow, m_graphics, on_screen_keys[curent_line_word[j] - LETTER_A_ASCII_VALUE]->letter_render);
				on_screen_keys[curent_line_word[j] - LETTER_A_ASCII_VALUE]->color = yellow;
			}


			m_corect_word[i] = NULL_CHAR;
			curent_line_word[j] = NULL_CHAR;


		}
	}

	for (int i = 0; i < NUM_OF_LETTERS_IN_WORD; i++)
	{
		if (curent_line_word[i] == NULL_CHAR)
		{
			continue;
		}

		if(cmp_color(on_screen_keys[curent_line_word[i] - LETTER_A_ASCII_VALUE]->color, null_color) == true)
		{
			set_text_color(AN_CORECT_LETTERS_COLOR, m_graphics, on_screen_keys[curent_line_word[i] - LETTER_A_ASCII_VALUE]->letter_render);
			on_screen_keys[curent_line_word[i] - LETTER_A_ASCII_VALUE]->color = yellow;
		}

		set_text_color(AN_CORECT_LETTERS_COLOR, m_graphics, board[curent_word_index][i]->text_render);
	}

}

void gen_board()
{
	Render board_cell = create_sprite("board_letter_cell.png", m_graphics);;
	set_sprite_to_not_render(board_cell, m_graphics);

	for (int x = 0; x < NUM_OF_LETTERS_IN_WORD; x++)
	{
		for (int y = 0; y < NUM_OF_WORDS; y++)
		{
			Render cell = dup_sprite(board_cell, m_graphics);

			set_sprite_to_render(cell, m_graphics);
			set_sprite_color(bright_grey, m_graphics, cell);
			set_sprite_scale(vec2(LETTER_CELL_SIZE, LETTER_CELL_SIZE), m_graphics, cell);
			set_sprite_postion(vec2(x * (LETTER_CELL_SIZE + DISTANCE_BETWEEN_LETTERS_CELLS) + board_position_min_max.x,
				y * (LETTER_CELL_SIZE + DISTANCE_BETWEEN_LETTERS_CELLS) + board_position_min_max.y), m_graphics, cell);
		}
	}
}
void gen_background()
{
	set_background_color(grey, m_graphics);
}
void gen_ui()
{
	gen_reset_button();
	gen_on_screen_keyboard();
}

void gen_reset_button()
{
#define UI_RENDER_ORDER 3

#define DISTANCE_BETWEEN_AND_OF_SCREEN_RESET_BUTTON 50

#define RESET_BUTTON_POS_X (WIDTH - DISTANCE_BETWEEN_AND_OF_SCREEN_RESET_BUTTON)
#define RESET_BUTTON_POS_Y DISTANCE_BETWEEN_AND_OF_SCREEN_RESET_BUTTON

#define RESET_BUTTON_SCALE_X WIDTH / 16
#define RESET_BUTTON_SCALE_Y RESET_BUTTON_SCALE_X

	Render reset_game_button = create_sprite("restart_button.png", m_graphics);

	set_sprite_postion(vec2(RESET_BUTTON_POS_X, RESET_BUTTON_POS_Y), m_graphics, reset_game_button);
	set_sprite_scale(vec2(RESET_BUTTON_SCALE_X, RESET_BUTTON_SCALE_Y), m_graphics, reset_game_button);
	set_sprite_render_order(LOWEST_RENDER_ORDER, m_graphics, reset_game_button);

	Button button = create_button_from_sprite(reset_game_button, m_graphics, m_buttons);
	set_button_callback(button, m_buttons, handle_restart_game, NULL);
}

void on_screen_keyboard_keys_callback(void** letter)
{
	char* m_letter = (char*)letter;
	handle_add_letter_to_board(m_letter[0]);
}

void create_on_screen_keyboard_key(Vec2 position, char* letter)
{
	struct on_screen_key* on_screen_key = (struct on_screen_key*)my_malloc(sizeof(struct on_screen_key));

	on_screen_key->background = create_sprite("on_screen_key_background.png", m_graphics);
	set_sprite_scale(vec2(ON_SCREEN_KEY_BACKGROUND_SCALE_X, ON_SCREEN_KEY_BACKGROUND_SCALE_Y), m_graphics, on_screen_key->background);
	set_sprite_postion(position, m_graphics, on_screen_key->background);

	on_screen_key->letter_render = create_text(letter, m_graphics, text_defalut_font);
	set_text_color(TEXT_DEFALUT_COLOR, m_graphics, on_screen_key->letter_render);
	set_text_scale(vec2(ON_SCREEN_KEYBORD_LETTER_OR_SYMBOL_SCALE, ON_SCREEN_KEYBORD_LETTER_OR_SYMBOL_SCALE), m_graphics, on_screen_key->letter_render);
	set_text_postion(position, m_graphics, on_screen_key->letter_render);

	on_screen_key->button = create_button_from_sprite(on_screen_key->background, m_graphics, m_buttons);

	on_screen_key->letter = strdup(letter);
	set_button_callback(on_screen_key->button, m_buttons, on_screen_keyboard_keys_callback, on_screen_key->letter);

	on_screen_key->color = null_color;

	on_screen_keys[letter[0] - LETTER_A_ASCII_VALUE] = on_screen_key;
}

void gen_on_screen_keyboard()
{
#define KEYBOARD_START_Y (HIGHT * 0.7f)
#define KEYBORD_START_X MIDEL_SCREEN_X - (((NUM_OF_KEYS_IN_FIRST_LINE / 2.0f) + 1.0f) * ON_SCREEN_KEY_BACKGROUND_SCALE_X)

#define NUM_OF_KEYS_IN_FIRST_LINE 10
#define NUM_OF_KEY_SECEOND_LINE 9
#define NUM_OF_KEYS_THIRD_LINE 7

#define DISTANCE_BETWEEN_ON_SCREEN_KEYBOARD_KEYS 10

	const Vec2 first_line_keyboard_start_position = vec2(KEYBORD_START_X , KEYBOARD_START_Y);

	const Vec2 seceond_line_keyboard_start_position = vec2(first_line_keyboard_start_position.x + (0.5f *  (DISTANCE_BETWEEN_ON_SCREEN_KEYBOARD_KEYS + ON_SCREEN_KEY_BACKGROUND_SCALE_X)),
		first_line_keyboard_start_position.y + DISTANCE_BETWEEN_ON_SCREEN_KEYBOARD_KEYS + ON_SCREEN_KEY_BACKGROUND_SCALE_Y);

	const Vec2 third_line_keyboard_start_position = vec2(first_line_keyboard_start_position.x + 1.5f * (DISTANCE_BETWEEN_ON_SCREEN_KEYBOARD_KEYS + ON_SCREEN_KEY_BACKGROUND_SCALE_X),
		first_line_keyboard_start_position.y + (2 * (DISTANCE_BETWEEN_ON_SCREEN_KEYBOARD_KEYS + ON_SCREEN_KEY_BACKGROUND_SCALE_Y)));


	char keyboard_keys_order[NUM_OF_LETTERS] = "qwertyuiopasdfghjklzxcvbnm";

	int curent_letter_index = 0;

	for (int i = 0; i < NUM_OF_KEYS_IN_FIRST_LINE; i++)
	{
		char letter[2] = { NULL_CHAR };
		letter[0] = keyboard_keys_order[curent_letter_index];

		create_on_screen_keyboard_key(vec2(first_line_keyboard_start_position.x + (i * (DISTANCE_BETWEEN_ON_SCREEN_KEYBOARD_KEYS + ON_SCREEN_KEY_BACKGROUND_SCALE_X)),
			first_line_keyboard_start_position.y), letter);

		curent_letter_index++;
	}
	for (int i = 0; i < NUM_OF_KEY_SECEOND_LINE; i++)
	{
		char letter[2] = { NULL_CHAR };
		letter[0] = keyboard_keys_order[curent_letter_index];

		create_on_screen_keyboard_key(vec2(seceond_line_keyboard_start_position.x + (i * (DISTANCE_BETWEEN_ON_SCREEN_KEYBOARD_KEYS + ON_SCREEN_KEY_BACKGROUND_SCALE_X)),
			seceond_line_keyboard_start_position.y), letter);

		curent_letter_index++;
	}
	for (int i = 0; i < NUM_OF_KEYS_THIRD_LINE; i++)
	{
		char letter[2] = { NULL_CHAR };
		letter[0] = keyboard_keys_order[curent_letter_index];

		create_on_screen_keyboard_key(vec2(third_line_keyboard_start_position.x + (i * (DISTANCE_BETWEEN_ON_SCREEN_KEYBOARD_KEYS + ON_SCREEN_KEY_BACKGROUND_SCALE_X)),
			third_line_keyboard_start_position.y), letter);

		curent_letter_index++;
	}

	Vec2 simbols_scale = vec2(ON_SCREEN_KEYBORD_LETTER_OR_SYMBOL_SCALE, ON_SCREEN_KEYBORD_LETTER_OR_SYMBOL_SCALE);

	Vec2 enter_key_psoition = vec2(third_line_keyboard_start_position.x - (DISTANCE_BETWEEN_ON_SCREEN_KEYBOARD_KEYS + ON_SCREEN_KEY_BACKGROUND_SCALE_X),
		third_line_keyboard_start_position.y);

	Vec2 backspace_key_position = vec2(third_line_keyboard_start_position.x + ((NUM_OF_KEYS_THIRD_LINE) * (DISTANCE_BETWEEN_ON_SCREEN_KEYBOARD_KEYS + ON_SCREEN_KEY_BACKGROUND_SCALE_X)),
		third_line_keyboard_start_position.y);

	Vec2 ket_background_scale = vec2(ON_SCREEN_KEY_BACKGROUND_SCALE_X, ON_SCREEN_KEY_BACKGROUND_SCALE_Y);


	Render enter_key_background = create_sprite("on_screen_key_background.png", m_graphics);
	set_sprite_scale(ket_background_scale, m_graphics, enter_key_background);
	set_sprite_postion(enter_key_psoition, m_graphics, enter_key_background);

	Render enter_symbol = create_sprite("enter_symbol.png", m_graphics);
	set_sprite_scale(simbols_scale, m_graphics, enter_symbol);
	set_sprite_postion(enter_key_psoition, m_graphics, enter_symbol);

	Button enter_key = create_button_from_sprite(enter_key_background, m_graphics, m_buttons);
	set_button_callback(enter_key, m_buttons, handle_enter, NULL);



	Render backspace_key_background = dup_sprite(enter_key_background, m_graphics);
	set_sprite_postion(backspace_key_position, m_graphics, backspace_key_background);
	set_sprite_scale(ket_background_scale, m_graphics, backspace_key_background);

	Render backspace_symbol = create_sprite("delete_symbol.png", m_graphics);
	set_sprite_scale(simbols_scale, m_graphics, backspace_symbol);
	set_sprite_postion(backspace_key_position, m_graphics, backspace_symbol);

	Button delete_key = create_button_from_sprite(backspace_key_background, m_graphics, m_buttons);
	set_button_callback(delete_key, m_buttons, handle_backspace, NULL);


}

void set_correct_word(char* corect_word)
{
	int num_of_lines;
	char buffer[ARRAY_BUFFER_SIZE][BUFFER_SIZE] = { 0 };
	load_text_file_data("common five letters words.txt", &num_of_lines, buffer);

	srand(time(NULL));

	int corect_word_index = rand() % num_of_lines - 2;
	strncpy(corect_word, buffer[corect_word_index], NUM_OF_LETTERS_IN_WORD);
	corect_word[NUM_OF_LETTERS_IN_WORD] = NULL_CHAR;

	printf("%s\n", corect_word);
}

void handle_game_end(const char* text_to_display)
{
#define END_GAME_TEXT_SCALE (WIDTH - 200)

#define END_GAME_SCREEN_BACKGROUND_RENDER_ORDER 1
#define END_GAME_SCREEN_TEXT_RENDER_ORDER 2

	game_end = true;

	game_end_backgroud_render = create_sprite("transparent_background.png", m_graphics);
	set_sprite_render_order(END_GAME_SCREEN_BACKGROUND_RENDER_ORDER, m_graphics, game_end_backgroud_render);

	set_sprite_postion(vec2(MIDEL_SCREEN_X, MIDEL_SCREEN_Y), m_graphics, game_end_backgroud_render);
	set_sprite_scale(vec2(WIDTH - 200, HIGHT - 200), m_graphics, game_end_backgroud_render);

	game_end_text_render = create_text(text_to_display, text_defalut_font, m_graphics);
	set_text_postion(vec2(MIDEL_SCREEN_X, MIDEL_SCREEN_Y), m_graphics, game_end_text_render);
	set_text_scale(vec2(END_GAME_TEXT_SCALE, END_GAME_TEXT_SCALE / 4), m_graphics, game_end_text_render);

	set_text_render_order(END_GAME_SCREEN_TEXT_RENDER_ORDER, m_graphics, game_end_text_render);
}

void handle_lose()
{
	handle_game_end("You lost!");
}
void handle_win()
{
	handle_game_end("You won!");
}

void handle_backspace()
{
	if (curent_letter_index == 0)
	{
		return;
	}


	handle_destroy_letter_in_board(--curent_letter_index, curent_word_index);
}
void handle_enter()
{
	if (curent_letter_index != NUM_OF_LETTERS_IN_WORD)
	{
		return;
	}

	handle_complit_curent_line();
	if (game_end)
	{
		return;
	}

	if (curent_word_index == NUM_OF_WORDS - 1)
	{
		handle_lose();
		return;
	}

	curent_letter_index = 0;
	curent_letter_index = 0;
	curent_word_index++;
}

void handle_restart_game(void** data)
{
	if (game_end_backgroud_render != OBJECT_NULL_VALUE && game_end_text_render != OBJECT_NULL_VALUE)
	{
		destroy_sprite(game_end_backgroud_render, m_graphics);
		destroy_text(game_end_text_render, m_graphics);
	}

	handle_clear_board();
	handel_reset_on_screan_keys_array();

	start();
}
void handle_clear_board()
{
	for (int y = 0; y < NUM_OF_WORDS; y++)
	{
		for (int x = 0; x < NUM_OF_LETTERS_IN_WORD; x++)
		{
			if (board[y][x] == NULL)
			{
				return;
			}

			handle_destroy_letter_in_board(x, y);
		}
	}
}

void handel_reset_on_screan_keys_array()
{
	for(int i = 0; i < NUM_OF_LETTERS; i++)
	{
		on_screen_keys[i]->color = null_color;
		set_text_color(TEXT_DEFALUT_COLOR, m_graphics, on_screen_keys[i]->letter_render);
	}
}
