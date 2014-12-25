#ifndef PACT_STRING_H
#define PACT_STRING_H

#include <stdlib.h>
#include "config.h"
#include "linkedlist.h"

typedef struct pact_String pact_String;

//allocation/destruction
pact_String* pact_string_new(const char* data);
pact_String* pact_string_new_length(const char* data, size_t length);
void pact_string_free(pact_String* str);

//getters
const char* pact_string_get_cstr(const pact_String* str);
size_t pact_string_get_length(const pact_String* str);

//methods
char* pact_string_copy_cstr(const pact_String* str);
char* pact_string_copy_cstr_length(const pact_String* str, size_t length);
pact_String* pact_string_substr(const pact_String* str, size_t start, size_t end);
int pact_string_compare_cstr(const pact_String* a, const char* b);
int pact_string_compare(const pact_String* a, const pact_String* b);
pact_String* pact_string_copy(const pact_String* str);
int pact_string_find_after_cstr(const pact_String* str, const char* value, size_t offset);
int pact_string_find_cstr(const pact_String* str, const char* value);
int pact_string_find_after(const pact_String* str, const pact_String* value, size_t offset);
int pact_string_find(const pact_String* str, const pact_String* value);
pact_String* pact_string_concat(const pact_String* a, const pact_String* b);
pact_String* pact_string_concat_cstr(const pact_String* a, const char* b);
pact_String* pact_string_cstr_concat(const char* a, const pact_String* b);
pact_LinkedList* pact_string_split_cstr(const pact_String* str, const char* delim);
pact_LinkedList* pact_string_split(const pact_String* str, const pact_String* delim);

//Mutators (immutable strings makes them terrible to work with)
int pact_string_prepend_cstr(pact_String* str, const char* other);
int pact_string_prepend(pact_String* str, const pact_String* other);
int pact_string_append_cstr(pact_String* str, const char* other);
int pact_string_append(pact_String* str, const pact_String* other);
int pact_string_chop_front(pact_String* str, size_t length);
int pact_string_chop_back(pact_String* str, size_t length);
#endif