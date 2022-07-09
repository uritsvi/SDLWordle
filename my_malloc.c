#include <stdlib.h>
#include <stdio.h>

#include "my_malloc.h"

void* my_malloc(int size)
{
	void* memory = malloc(size);

	if(memory == NULL)
	{
		printf("Can't mallocate memory to program");
		exit(-1);
	}

	return memory;
}