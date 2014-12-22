#ifndef PACT_STRING_H
#define PACT_STRING_H

#include <stdlib.h>
#include "config.h"
#include "linkedlist.h"


typedef struct pact_String pact_String;

//allocation/destruction
pact_String* pact_string_new(const char* data);
void pact_string_free(pact_String* str);

//getters
const char* pact_string_get_cstr(const pact_String* str);
size_t pact_string_get_legnth(const pact_String* str);

//methods
char* pact_string_copy_cstr(const pact_String* str);
pact_String* pact_string_substr(const pact_String*str, const size_t start, const size_t end);
int pact_string_compare(const pact_String* a, const pact_String* b);
int pact_string_compare_cstr(const pact_String* a, const char* b);
inline pact_String* pact_string_copy(const pact_String* str);
int pact_string_find_after_cstr(const pact_String* str, const char* value, const size_t offset);
inline int pact_string_find_cstr(const pact_String* str, const char* value);
inline int pact_string_find_after(const pact_String* str, const pact_String* value, const size_t offset);
inline int pact_string_find(const pact_String* str, const pact_String* value);
inline pact_String* pact_string_chop_front(const pact_String* str, const size_t length);
inline pact_String* pact_string_chop_back(const pact_String* str, const size_t length);
pact_String* pact_string_concat(const pact_String* a, const pact_String* b);
pact_LinkedList* pact_string_split_cstr(const pact_String* str, const char* delim);
inline pact_LinkedList* pact_string_split(const pact_String* str, const pact_String* delim);


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
//int pact_string_chop_front(pact_String* str, unsigned int chop);
//int pact_string_chop_back(pact_String* str, unsigned int chop);

#endif