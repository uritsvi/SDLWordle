#include "color.h"

const Color red = { .r = HIGHEST_COLOR_VALUE , .g = 0 , .b = 0 };
const Color green = { .r = 0 , .g = HIGHEST_COLOR_VALUE , .b = 0 };
const Color blue = { .r = 0 , .g = 0 , .b = HIGHEST_COLOR_VALUE };
const Color yellow = { .r = HIGHEST_COLOR_VALUE,.g = HIGHEST_COLOR_VALUE , .b = 0 };
const Color grey = { .r = 25 , .g = 25 , .b = 25 };
const Color black = { .r = 0 , .g = 0 , .b = 0 };
const Color white = { .r = HIGHEST_COLOR_VALUE , .g = HIGHEST_COLOR_VALUE , .b = HIGHEST_COLOR_VALUE };
const Color bright_grey = { .r = 100 , .g = 100 , .b = 100 };
const Color null_color = { .r = -1000 ,.g = -1000 , .b = -1000 };

bool cmp_color(Color color_1, Color color_2)
{
	if(	(color_1.r == color_2.r) &&
		(color_1.g == color_2.g) && 
		(color_1.b == color_2.b))
	{
		return true;
	}
	return false;
}






