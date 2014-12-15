#ifndef PACT_STRING_H
#define PACT_STRING_H

#include "config.h"

typedef struct pact_String pact_String;

//allocation/destruction
pact_String* pact_string_create();
void pact_string_destroy(pact_String* str);

//operations
int pact_string_assign(pact_String* str, const char* data);
int pact_string_assign_length(pact_String* str, const char* data, unsigned int length);
const char* pact_string_view(pact_String* str);
int pact_string_retrieve(pact_String* str, char* data, unsigned int length);
unsigned int pact_string_length(pact_String* str);

//string utility functions
int pact_string_retrieve_substr(pact_String* str, char* data, unsigned int start, unsigned int length);
int pact_string_clone_substr(pact_String* str, pact_String* out, unsigned int start, unsigned int length);
int pact_string_find(pact_String* str, pact_String* find);
int pact_string_partition(pact_String* str, pact_String* delim, pact_String* head, pact_String* tail);

#endif