#ifndef PACT_STRING_H
#define PACT_STRING_H

#include "config.h"

typedef struct pact_String pact_String;

//allocation/destruction
pact_String* pact_string_new(const char* data);
void pact_string_free(pact_String* str);

//allocation/destruction
pact_String* pact_string_create(const char* data);
void pact_string_destroy(pact_String* str);

//operations
int pact_string_clear(pact_String* str);
int pact_string_assign(pact_String* str, const char* data);
int pact_string_assign_length(pact_String* str, const char* data, unsigned int length);
const char* pact_string_view(pact_String* str);
int pact_string_retrieve(pact_String* str, char* data, unsigned int length);
int pact_string_retrieve_substr(pact_String* str, char* data, unsigned int start, unsigned int length);
unsigned int pact_string_length(pact_String* str);
int pact_string_compare(pact_String* a, pact_String* b);
int pact_string_compare_cstr(pact_String* a, char* b);
int pact_string_clone(pact_String* src, pact_String* dst);
int pact_string_clone_substr(pact_String* src, pact_String* dst, unsigned int start, unsigned int length);
int pact_string_find(pact_String* str, pact_String* find);
int pact_string_find_after(pact_String* str, pact_String* find, unsigned int start);
int pact_string_find_cstr(pact_String* str, char* find);
int pact_string_find_cstr_after(pact_String* str, char* find, unsigned int start);
int pact_string_partition(pact_String* str, pact_String* delim, pact_String* head, pact_String* tail);
int pact_string_partition_cstr(pact_String* str, char* delim, pact_String* head, pact_String* tail);
int pact_string_append(pact_String* str, pact_String* tail);
int pact_string_append_cstr(pact_String* str, char* tail);
int pact_string_prepend(pact_String* str, pact_String* head);
int pact_string_prepend_cstr(pact_String* str, char* head);
int pact_string_chop_front(pact_String* str, unsigned int chop);
int pact_string_chop_back(pact_String* str, unsigned int chop);

#endif