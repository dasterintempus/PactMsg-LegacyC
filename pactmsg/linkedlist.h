#ifndef PACT_LINKEDLIST_H
#define PACT_LINKEDLIST_H

#include "config.h"

typedef struct pact_LinkedList pact_LinkedList;
typedef struct pact_LinkedListNode pact_LinkedListNode;

pact_LinkedList* pact_linkedlist_create();
void pact_linkedlist_destroy(pact_LinkedList* llist);
unsigned int pact_linkedlist_length(pact_LinkedList* llist);
int pact_linkedlist_popfront(pact_LinkedList* llist, void* out);
int pact_linkedlist_popback(pact_LinkedList* llist, void* out);
int pact_linkedlist_pushfront(pact_LinkedList* llist, void* in);
int pact_linkedlist_pushback(pact_LinkedList* llist, void* in);

#endif