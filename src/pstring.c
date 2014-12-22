#include "pstring.h"
#include "debug.h"
#include <stdlib.h>
#include <string.h>

struct pact_String {
	char* data;
	size_t length;
};

pact_String* pact_string_new(const char* data) {
	const pact_String i_list = { .data = data, .length = strlen(data) };
	pact_String* str_obj = malloc(sizeof(pact_String));

	if (str_obj == NULL) {
		return NULL;
	}
	memcpy(str_obj, &i_list, sizeof(pact_String));
	return str_obj;
}

void pact_string_free(pact_String* str) {
	if (str->data) {
		free(str->data);
	}
	free(str);
}

const char* pact_string_get_cstr(const pact_String* str) {
	return str->data;
}

size_t pact_string_get_length(const pact_String* str) {
	return str->length;
}

char* pact_string_copy_cstr(const pact_String* str) {
	char* new_string = malloc(sizeof(pact_String) * str->length);
	if (new_string) {
		strncpy(new_string, str->data, str->length);
		return new_string;
	}
	return NULL;
}

pact_String* pact_string_substr(const pact_String* str, const size_t start, const size_t end) {
	const size_t length = end - start;
	char* substr;
	pact_String* pact_substr;
	if (end > str->length || start > str->length) {
		return NULL;
	}
	//Checking for reverse order
	if (start > end) {
		return NULL;
	}
	substr = malloc(sizeof(char) * length);
	if (substr) {
		strncpy(substr, str->data[start], length);
		pact_substr = pact_string_new(substr);
		if (pact_substr) {
			return pact_substr;
		}
		else {
			free(substr);
		}
	}
	return NULL;
}

int pact_string_compare_cstr(const pact_String* a, const char* b) {
	const size_t a_length = pact_string_get_length(a);
	const size_t b_length = strlen(b);
	if (a_length < b_length) {
		return -1;
	}
	else if (a_length > b_length) {
		return 1;
	}
	else {
		return strncmp(a->data, b, a_length);
	}
}

int pact_string_compare(const pact_String* a, pact_String* b) {
	const size_t a_length = pact_string_get_length(a);
	const size_t b_length = pact_string_get_length(b);
	if (a_length < b_length) {
		return -1;
	}
	else if (a_length > b_length) {
		return 1;
	}
	else {
		return strncmp(a->data, b->data, a_length);
	}
}

pact_String* pact_string_copy(const pact_String* str) {
	return pact_string_new(pact_string_copy_cstr(str));
}

int pact_string_find_after_cstr(const pact_String* str, const char* value, const size_t offset) {
	size_t i;
	const size_t value_length = strlen(value);
	if (str->length == 0 || value_length == 0) {
		return -1;
	}
	for (i = offset; i + value_length <= str->length; i++) {
		if (strcmp(str->data + i, value, value_length) == 0) {
			return i;
		}
	}
	return -1;
}

int pact_string_find_after(const pact_String* str, const pact_String* value, const size_t offset) {
	return pact_string_find_after_cstr(str, value->data, offset);
}

int pact_string_find(const pact_String* str, const pact_String* value) {
	return pact_string_find_after_cstr(str, value->data, 0);
}

int pact_string_find_cstr(const pact_String* str, const char* value) {
	return pact_string_find_after_cstr(str, value, 0);
}

pact_String* pact_string_chop_front(const pact_String* str, const size_t length) {
	return pact_string_substr(str, length, pact_string_get_length(str));
}

pact_String* pact_string_chop_back(const pact_String* str, const size_t length) {
	return pact_string_substr(str, 0, pact_string_get_length(str) - length);
}

pact_String* pact_string_concat(const pact_String* a, const pact_String* b) {
	const size_t length = a->length + b->length + 1;
	char* temp = malloc(sizeof(char)*length);
	memset(temp, 0, length);
	strcncpy(temp, a->data, a->length);
	strncat(temp, b->data, b->length);
	return pact_string_new(temp);
}

pact_LinkedList* pact_string_split_cstr(const pact_String* str, const char* delim) {
	pact_LinkedList* split_list = pact_linkedlist_create();
	pact_String* temp;
	size_t index = 0;
	size_t previous_index = 0;

	for (index = pact_string_find_after_cstr(str, delim, index), previous_index = 0;
		index != -1;
		previous_index = index, index = pact_string_find_after_cstr(str, delim, index)) {
		temp = pact_string_substr(str, previous_index, index);
		if (temp) {
			pact_linked_list_pushback(split_list, (void*)temp);
		}
	}
	return split_list;
}

pact_LinkedList* pact_string_split(const pact_String* str, const pact_String* delim) {
	return pact_string_split_cstr(str, delim->data);
}
