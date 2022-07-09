#ifndef __COLOR__
#define __COLOR__

#include <stdbool.h>

#define HIGHEST_COLOR_VALUE 255

typedef struct
{
	int r;
	int g;
	int b;
} Color;

extern Color const red;
extern Color const green;
extern Color const blue;
extern Color const grey;
extern Color const yellow;
extern Color const black;
extern Color const white;
extern Color const bright_grey;
extern Color const null_color;

bool cmp_color(Color color_1, Color color_2);

#endif // !__COLOR__
