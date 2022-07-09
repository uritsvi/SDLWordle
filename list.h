#ifndef __LIST__
#define __LIST__

struct  node
{
	void** data;
	struct node* next;
};

struct list
{
	struct node* header;
};

struct list* create_list();
void destroy_list_and_data(struct list* list);

void add_object_to_list(void* data, struct list* list);
void remove_object_from_list(struct list* list, int index);

struct node* get_cell_by_index(struct list* list, int index);

int get_list_len(struct list* list);

#endif // !__LIST__
