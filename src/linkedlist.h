#ifndef PACT_LINKEDLIST_H
#define PACT_LINKEDLIST_H

#include "config.h"
#include <stdlib.h>

typedef struct pact_LinkedList pact_LinkedList;
typedef struct pact_LinkedListNode pact_LinkedListNode;
typedef pact_LinkedListNode pact_LinkedListIter;

pact_LinkedList* pact_linkedlist_new();
void pact_linkedlist_free(pact_LinkedList* llist);
size_t pact_linkedlist_get_length(pact_LinkedList* llist);
void* pact_linkedlist_popfront(pact_LinkedList* llist);
void* pact_linkedlist_popback(pact_LinkedList* llist);
int pact_linkedlist_pushfront(pact_LinkedList* llist, void* in);
int pact_linkedlist_pushback(pact_LinkedList* llist, void* in);
pact_LinkedListIter* pact_linkedlist_get_iter(pact_LinkedList* llist);
pact_LinkedListIter* pact_linkedlist_iter(pact_LinkedListIter* iter);
void* pact_linkedlist_iter_get_data(pact_LinkedListIter* iter);

#endif