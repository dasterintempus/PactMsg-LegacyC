#ifndef PACT_DATA_STRUCTS_H
#define PACT_DATA_STRUCTS_H

#include "config.h"

//linked list
struct pact_linkedlist_node_s
{
	void* data;
	pact_linkedlist_node_t* next;
	pact_linkedlist_node_t* prev;
};

struct pact_linkedlist_s
{
	pact_linkedlist_node_t* head;
	pact_linkedlist_node_t* tail;
	unsigned int length;
};

#endif