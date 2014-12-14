#ifndef PACT_DATA_H
#define PACT_DATA_H

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "data_typedefs.h"

pact_linkedlist_t* pact_linkedlist_create();
void pact_linkedlist_destroy(pact_linkedlist_t* llist);
unsigned int pact_linkedlist_length(pact_linkedlist_t* llist);
int pact_linkedlist_popfront(pact_linkedlist_t* llist, void* out);
int pact_linkedlist_popback(pact_linkedlist_t* llist, void* out);
int pact_linkedlist_pushfront(pact_linkedlist_t* llist, void* in);
int pact_linkedlist_pushback(pact_linkedlist_t* llist, void* in);

#endif