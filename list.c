#include <stdlib.h>
#include <stdio.h>

#include "list.h"
#include "my_malloc.h"

struct list* create_list()
{
	struct list* list = (struct list*)my_malloc(sizeof(struct list));

	list->header = (struct node*)my_malloc(sizeof(struct node));

	list->header->next = NULL;
	list->header->data = NULL;

	return list;
}
void destroy_list_and_data(struct list* list)
{
	struct node* curent_cell = list->header;
	struct node* next_cell = curent_cell->next;

	while (curent_cell != NULL) 
	{
		next_cell = curent_cell->next;
		
		free(curent_cell->data);
		free(curent_cell);

		curent_cell = next_cell;
	}
}

void add_object_to_list(void* data , struct list* list)
{
	struct node* curent_cell = list->header;

	while (curent_cell->next != NULL)
	{
		curent_cell = curent_cell->next;
	}

	curent_cell->next = (struct node*)my_malloc(sizeof(struct node));

	curent_cell->next->data = data; 
	curent_cell->next->next = NULL;
}
void remove_object_from_list(struct list* list , int index)
{
	struct node* curent_node = list->header;

	for (int i = 0; i < index; i++)
	{
		curent_node = curent_node->next;
	}

	struct node* affter_deleted_node = curent_node->next->next;
	free(curent_node->next);

	curent_node->next = affter_deleted_node;
}

struct node* get_cell_by_index(struct list* list, int index)
{
	struct node* curent_cell = list->header;

	for(int i = 0; i <= index; i++) // Can't by just < (its <=) because if a 0 passed in the list returns the data of the header
	{
		curent_cell = curent_cell->next;

		if(curent_cell == NULL)
		{
			printf("try to get a non existing cell in list");
			return NULL;
		}
	}

	return curent_cell;
}

int get_list_len(struct list* list)
{
	struct node* curent_cell = list->header;

	int list_len = 0;

	while (curent_cell->next != NULL)
	{
		curent_cell = curent_cell->next;
		list_len++;
	}

	return list_len;
}
