#include "pstring.h"
#include "debug.h"
#include <stdlib.h>
#include <string.h>

struct pact_String {
	char* data;
	size_t length;
};

pact_String* pact_string_new(const char* data) {
	size_t length;
	pact_String* str;
	str = malloc(sizeof(pact_String));
	if (!str) {
		return NULL;
	}
	if (!data) {
		str->data = malloc(sizeof(char));
		str->data[0] = 0;
		str->length = 0;
	}
	length = strlen(data);
	str->data = malloc(length + 1);
	strncpy(str->data, data, length + 1); // +1 ensures the trailing null is present
	str->length = length;
	return str;
}

pact_String* pact_string_new_length(const char* data, size_t length) {
	pact_String* str;
	if (!data) {
		return NULL;
	}
	str = malloc(sizeof(pact_String));
	if (!str) {
		return NULL;
	}
	str->data = malloc(length + 1);
	memcpy(str->data, data, length);
	str->data[length] = 0;
	str->length = length;
	return str;
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
	char* new_string = malloc(sizeof(char) * str->length);
	if (!new_string) {
		return NULL;
	}
	strncpy(new_string, str->data, str->length);
	return new_string;
}

char* pact_string_copy_cstr_length(const pact_String* str, size_t length) {
	char* new_string = malloc(sizeof(char) * length);
	if (!new_string) {
		return NULL;
	}
	strncpy(new_string, str->data, length);
	return new_string;
}

pact_String* pact_string_substr(const pact_String* str, size_t start, size_t end) {
	size_t length;
	pact_String* pact_substr;
	//Checking for reverse order
	if (start > end) {
		return NULL;
	}
	//Bounds check
	if (start > str->length) {
		return NULL;
	}
	//If the end is past the string, only go to the end of the string
	if (end > str->length) {
		end = str->length;
	}
	length = end - start;
	pact_substr = malloc(sizeof(pact_String));
	if (!pact_substr) {
		return NULL;
	}
	pact_substr->data = malloc(sizeof(char) * length + 1);
	memcpy(pact_substr->data, str->data + start, length);
	pact_substr->data[length] = 0;
	pact_substr->length = length;
	return pact_substr;
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

int pact_string_compare(const pact_String* a, const pact_String* b) {
	const size_t a_length = pact_string_get_length(a);
	const size_t b_length = pact_string_get_length(b);
	if (a_length < b_length) {
		return -1;
	}
	else if (a_length > b_length) {
		return 1;
	}
	else {
		return memcmp(a->data, b->data, a_length);
	}
}

pact_String* pact_string_copy(const pact_String* str) {
	return pact_string_new(pact_string_copy_cstr(str));
}

int pact_string_find_after_cstr(const pact_String* str, const char* value, size_t offset) {
	size_t i;
	const size_t value_length = strlen(value);
	if (str->length == 0 || value_length == 0) {
		return -1;
	}
	for (i = offset; i + value_length <= str->length; i++) {
		if (strncmp(str->data + i, value, value_length) == 0) {
			return i;
		}
	}
	return -1;
}

int pact_string_find_cstr(const pact_String* str, const char* value) {
	return pact_string_find_after_cstr(str, value, 0);
}

int pact_string_find_after(const pact_String* str, const pact_String* value, size_t offset) {
	size_t i;
	if (str->length == 0 || value->length == 0) {
		return -1;
	}
	for (i = offset; i + value->length <= str->length; i++) {
		if (memcmp(str->data + i, value, value->length) == 0) {
			return i;
		}
	}
	return -1;
}

int pact_string_find(const pact_String* str, const pact_String* value) {
	return pact_string_find_after(str, value, 0);
}

pact_String* pact_string_concat(const pact_String* a, const pact_String* b) {
	pact_String* pact_string;
	const size_t length = a->length + b->length;
	char* temp = malloc(sizeof(char)*(length+1));
	memset(temp, 0, length+1);
	memcpy(temp, a->data, a->length);
	memcpy(temp, (b->data) + a->length, b->length);
	b->data[length] = 0;
	pact_string = pact_string_new(temp);
	free(temp);
	return pact_string;
}

pact_String* pact_string_concat_cstr(const pact_String* a, const char* b) {
	pact_String* pact_string;
	const size_t length = a->length + strlen(b);
	char* temp = malloc(sizeof(char)*(length + 1));
	memset(temp, 0, length + 1);
	strncpy(temp, a->data, a->length);
	strcat(temp, b);
	pact_string = pact_string_new(temp);
	free(temp);
	return pact_string;
}

pact_String* pact_string_cstr_concat(const char* a, const pact_String* b) {
	pact_String* pact_string;
	const size_t length = strlen(a) + b->length;
	char* temp = malloc(sizeof(char)*(length + 1));
	memset(temp, 0, length + 1);
	strncpy(temp, a, strlen(a));
	strcat(temp, b->data);
	pact_string = pact_string_new(temp);
	free(temp);
	return pact_string;
}

pact_LinkedList* pact_string_split_cstr(const pact_String* str, const char* delim) {
	pact_LinkedList* split_list = pact_linkedlist_new();
	pact_String* temp;
	size_t delimlength = strlen(delim);
	size_t index = 0;
	size_t previous_index = 0;

	for (index = pact_string_find_after_cstr(str, delim, index), previous_index = 0;
		index != -1;
		previous_index = index + delimlength, index = pact_string_find_after_cstr(str, delim, index)) {
		temp = pact_string_substr(str, previous_index, index);
		if (temp) {
			pact_linkedlist_pushback(split_list, (void*)temp);
		}
	}
	return split_list;
}

pact_LinkedList* pact_string_split(const pact_String* str, const pact_String* delim) {
	pact_LinkedList* split_list = pact_linkedlist_new();
	pact_String* temp;
	size_t index = 0;
	size_t previous_index = 0;

	for (index = pact_string_find_after(str, delim, index), previous_index = 0;
		index != -1;
		previous_index = index + delim->length, index = pact_string_find_after(str, delim, index)) {
		temp = pact_string_substr(str, previous_index, index);
		if (temp) {
			pact_linkedlist_pushback(split_list, (void*)temp);
		}
	}
	return split_list;
}

int pact_string_prepend_cstr(pact_String* str, const char* other) {
	size_t newlen = str->length + strlen(other);
	char* buf = malloc(newlen + 1);
	if (!buf) {
		return -1;
	}
	strncpy(buf, other, strlen(other));
	if (str->data) {
		memcpy(buf + strlen(other), str->data, str->length);
		free(str->data);
	}
	buf[newlen] = 0;
	str->data = buf;
	str->length = newlen;
	return 0;
}

int pact_string_prepend(pact_String* str, const pact_String* other) {
	size_t newlen = str->length + other->length;
	char* buf = malloc(newlen + 1);
	if (!buf) {
		return -1;
	}
	memcpy(buf, other->data, other->length);
	if (str->data) {
		memcpy(buf + other->length, str->data, str->length);
		free(str->data);
	}
	buf[newlen] = 0;
	str->data = buf;
	str->length = newlen;
	return 0;
}

int pact_string_append_cstr(pact_String* str, const char* other) {
	size_t newlen = str->length + strlen(other);
	char* buf = malloc(newlen + 1);
	if (!buf) {
		return -1;
	}
	if (str->data) {
		memcpy(buf, str->data, str->length);
		free(str->data);
	}
	strcat(buf, other);
	buf[newlen] = 0;
	str->data = buf;
	str->length = newlen;
	return 0;
}

int pact_string_append(pact_String* str, const pact_String* other) {
	size_t newlen = str->length + other->length;
	char* buf = malloc(newlen + 1);
	if (!buf) {
		return -1;
	}
	if (str->data) {
		memcpy(buf, str->data, str->length);
		free(str->data);
	}
	memcpy(buf + str->length, other->data, other->length);
	buf[newlen] = 0;
	str->data = buf;
	str->length = newlen;
	return 0;
}

int pact_string_chop_front(pact_String* str, size_t length) {
	if (length > str->length) {
		return -1;
	}
	if (length == 0) {
		return 0;
	}
	size_t newlen = str->length - length;
	char* buf = malloc(newlen + 1);
	if (!buf) {
		return -1;
	}
	if (str->data) {
		memcpy(buf, str->data + length, newlen);
		free(str->data);
	}
	buf[newlen] = 0;
	str->data = buf;
	str->length = newlen;
	return 0;
}

int pact_string_chop_back(pact_String* str, size_t length) {
	if (length > str->length) {
		return -1;
	}
	if (length == 0) {
		return 0;
	}
	size_t newlen = str->length - length;
	char* buf = malloc(newlen + 1);
	if (!buf) {
		return -1;
	}
	if (str->data) {
		memcpy(buf, str->data, newlen);
		free(str->data);
	}
	buf[newlen] = 0;
	str->data = buf;
	str->length = newlen;
	return 0;
}
