#include "linkedlist.h"
#include <stdlib.h>
#include <string.h>

struct pact_LinkedListNode {
	void* data;
	pact_LinkedListNode* next;
	pact_LinkedListNode* prev;
};

struct pact_LinkedList {
	pact_LinkedListNode* head;
	pact_LinkedListNode* tail;
	size_t length;
};

pact_LinkedList* pact_linkedlist_new() {
	pact_LinkedList* llist = malloc(sizeof(pact_LinkedList));

	if (!llist) {
		return 0;
	}

	memset(llist, 0, sizeof(pact_LinkedList));

	return llist;
}

void pact_linkedlist_free(pact_LinkedList* llist) {
	pact_LinkedListNode* node = llist->head;
	while (node) {
		pact_LinkedListNode* next = node->next;
		free(node);
		node = next;
	}
	free(llist);
}

size_t pact_linkedlist_length(pact_LinkedList* llist) {
	return llist->length;
}

void* pact_linkedlist_popfront(pact_LinkedList* llist) {
	void* return_value;
	pact_LinkedListNode* popped_node;
	if (llist->length == 0) {
		return NULL;
	}
	popped_node = llist->head;
	return_value = popped_node->data;
	
	llist->head = popped_node->next;
	llist->head->prev = NULL;
	llist->length--;
	free(popped_node);
	return return_value;
}

void* pact_linkedlist_popback(pact_LinkedList* llist) {
	void* return_value;
	pact_LinkedListNode* popped_node;
	if (llist->length == 0) {
		return NULL;
	}

	popped_node = llist->tail;
	return_value = popped_node->data;
	llist->tail = popped_node->prev;
	llist->tail->next = NULL;
	llist->length--;

	return return_value;
}

int pact_linkedlist_pushfront(pact_LinkedList* llist, void* in) {
	pact_LinkedListNode* node = malloc(sizeof(pact_LinkedListNode));
	if (!node) {
		return 1;
	}
	memset(node, 0, sizeof(pact_LinkedListNode));

	node->data = in;
	node->next = llist->head;
	llist->head->prev = node;
	llist->head = node;
	llist->length++;
	return 0;
}

int pact_linkedlist_pushback(pact_LinkedList* llist, void* in) {
	pact_LinkedListNode* node = malloc(sizeof(pact_LinkedListNode));
	if (!node) {
		return 1;
	}
	memset(node, 0, sizeof(pact_LinkedListNode));

	node->data = in;
	node->prev = llist->tail;
	llist->tail->next = node;
	llist->tail = node;
	llist->length++;
	return 0;
}