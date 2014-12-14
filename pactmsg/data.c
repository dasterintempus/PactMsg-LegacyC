#include "data.h"
#include "data_structs.h"

pact_linkedlist_t* pact_linkedlist_create()
{
	pact_linkedlist_t* llist = malloc(sizeof(pact_linkedlist_t));

	if (!llist)
		return 0;

	memset(llist, 0, sizeof(pact_linkedlist_t));

	return llist;
}

void pact_linkedlist_destroy(pact_linkedlist_t* llist)
{
	pact_linkedlist_node_t* node = llist->head;
	while (node)
	{
		pact_linkedlist_node_t* next = node->next;
		free(node);
		node = next;
	}
	free(llist);
}

unsigned int pact_linkedlist_length(pact_linkedlist_t* llist)
{
	return llist->length;
}

int pact_linkedlist_popfront(pact_linkedlist_t* llist, void* out)
{
	if (llist->length == 0)
		return 1;

	out = llist->head->data;
	llist->head = llist->head->next;
	llist->length--;

	return 0;
}

int pact_linkedlist_popback(pact_linkedlist_t* llist, void* out)
{
	if (llist->length == 0)
		return 1;

	out = llist->tail->data;
	llist->tail = llist->tail->prev;
	llist->length--;

	return 0;
}

int pact_linkedlist_pushfront(pact_linkedlist_t* llist, void* in)
{
	pact_linkedlist_node_t* node = malloc(sizeof(pact_linkedlist_node_t));
	if (!node)
		return 1;
	memset(node, 0, sizeof(pact_linkedlist_node_t));

	node->data = in;
	node->next = llist->head;
	llist->head->prev = node;
	llist->head = node;
	llist->length++;
	return 0;
}

int pact_linkedlist_pushback(pact_linkedlist_t* llist, void* in)
{
	pact_linkedlist_node_t* node = malloc(sizeof(pact_linkedlist_node_t));
	if (!node)
		return 1;
	memset(node, 0, sizeof(pact_linkedlist_node_t));

	node->data = in;
	node->prev = llist->tail;
	llist->tail->next = node;
	llist->tail = node;
	llist->length++;
	return 0;
}