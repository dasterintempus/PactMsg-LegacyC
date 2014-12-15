#include "pstring.h"
#include "debug.h"
#include <stdlib.h>
#include <string.h>

struct pact_String {
	char* data;
	unsigned int length;
};

pact_String* pact_string_create() {
	pact_String* str = malloc(sizeof(pact_String));
	if (!str) {
		return 0;
	}
	memset(str, 0, sizeof(pact_String));

	return str;
}

void pact_string_destroy(pact_String* str) {
	free(str);
}

int pact_string_assign(pact_String* str, const char* data) {
	unsigned int length = strlen(data);
	return pact_string_assign_length(str, data, length);
}

int pact_string_assign_length(pact_String* str, const char* data, unsigned int length) {
	str->length = length;
	if (str->data) {
		free(str->data);
	}
	str->data = malloc(length + 1);
	if (!str->data){
		return 1;
	}
	memcpy(str->data, data, length);
	str->data[length] = 0;
	return 0;
}

const char* pact_string_view(pact_String* str) {
	return str->data;
}

int pact_string_retrieve(pact_String* str, char* data, unsigned int length) {
	if (length > str->length) {
		length = str->length;
	}
	memcpy(data, str->data, length);
	data[length] = 0;
	return 0;
}

unsigned int pact_string_length(pact_String* str) {
	return str->length;
}

int pact_string_retrieve_substr(pact_String* str, char* data, unsigned int start, unsigned int length) {
	if (start > str->length) {
		return 1;
	}
	if (length + start > str->length) {
		length = str->length - start;
	}
	memcpy(data, str->data + start, length);
	data[length] = 0;
	return 0;
}

int pact_string_clone_substr(pact_String* str, pact_String* out, unsigned int start, unsigned int length) {
	if (start > str->length) {
		return 1;
	}
	if (length + start > str->length) {
		length = str->length - start;
	}
	if (out->data) {
		free(out->data);
	}
	out->data = malloc(length + 1);
	out->length = length;
	memcpy(out->data, str->data + start, length);
	out->data[length] = 0;
	return 0;
}

int pact_string_find(pact_String* str, pact_String* find) {
	unsigned int i;
	for (i = 0; i + find->length < str->length; i++)
	{
		if (memcmp(str->data+i, find->data, find->length)) {
			return i;
		}
	}
	return -1;
}

int pact_string_partition(pact_String* str, pact_String* delim, pact_String* head, pact_String* tail) {
	int find = pact_string_find(str, delim);
	if (find == -1) {
		return -1;
	}
	pact_string_clone_substr(str, head, 0, find);
	pact_string_clone_substr(str, tail, find + delim->length, str->length - (find + delim->length));
	return find;
}