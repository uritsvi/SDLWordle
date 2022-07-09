#include <string.h>
#include <stdio.h>

#include "load_file_data.h"

void load_text_file_data(const char* path, int* num_of_lines, char buffer[][BUFFER_SIZE])
{
	FILE* file = fopen(path, "r");
	
	int curent_line = 0;
	char line_buffer[BUFFER_SIZE] = {0};
	
	while(fgets(line_buffer , BUFFER_SIZE , file))
	{
		
		strcpy(buffer[curent_line], line_buffer);
		curent_line++;
	}
	
	fclose(file);

	*num_of_lines = curent_line + 1;
}
