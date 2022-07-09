#include "object.h"

Object get_new_object_index(struct list* list)
{
	return get_list_len(list) - 1;
}
