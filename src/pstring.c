#include "pstring.h"
#include "debug.h"
#include <stdlib.h>
#include <string.h>

struct pact_String {
	char* data;
	unsigned int length;
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

pact_String* pact_string_create(const char* data) {
	pact_String* str = malloc(sizeof(pact_String));
	if (!str) {
		return 0;
	}
	memset(str, 0, sizeof(pact_String));
	if (data != NULL) {
		pact_string_assign(str, data);
	}
	return str;
}

void pact_string_destroy(pact_String* str) {
	if (str->data) {
		free(str->data);
	}
	free(str);
}

int pact_string_clear(pact_String* str) {
	return pact_string_assign(str, "");
}

int pact_string_assign(pact_String* str, const char* data) {
	unsigned int length = strlen(data);
	return pact_string_assign_length(str, data, length);
}

int pact_string_assign_length(pact_String* str, const char* data, unsigned int length) {
	if (str->data) {
		free(str->data);
	}
	str->data = malloc(length + 1);
	if (!str->data){
		return 1;
	}
	str->length = length;
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

unsigned int pact_string_length(pact_String* str) {
	return str->length;
}

int pact_string_compare(pact_String* a, pact_String* b) {
	unsigned int alen = pact_string_length(a);
	unsigned int blen = pact_string_length(b);
	if (alen < blen) {
		return -1;
	}
	else if (alen > blen) {
		return 1;
	}
	//alen == blen
	else if (alen == 0) {
		return 0;
	}
	return memcmp(a->data, b->data, alen);
}

int pact_string_compare_cstr(pact_String* a, char* b) {
	unsigned int alen = pact_string_length(a);
	unsigned int blen = strlen(b);
	if (alen < blen) {
		return -1;
	}
	else if (alen > blen) {
		return 1;
	}
	//alen == blen
	else if (alen == 0) {
		return 0;
	}
	return memcmp(a->data, b, alen);
}

int pact_string_clone(pact_String* src, pact_String* dst) {
	return pact_string_clone_substr(src, dst, 0, src->length);
}

int pact_string_clone_substr(pact_String* src, pact_String* dst, unsigned int start, unsigned int length) {
	if (start > src->length) {
		return 1;
	}
	if (length + start > src->length) {
		length = src->length - start;
	}
	if (dst->data) {
		free(dst->data);
	}
	dst->data = malloc(length + 1);
	if (!dst->data) {
		return 1;
	}
	dst->length = length;
	memcpy(dst->data, src->data + start, length);
	dst->data[length] = 0;
	return 0;
}

int pact_string_find(pact_String* str, pact_String* find) {
	unsigned int i;
	if (find->length == 0) {
		return -1;
	}
	for (i = 0; i + find->length <=str->length; i++) {
		if (memcmp(str->data + i, find->data, find->length) == 0) {
			return i;
		}
	}
	return -1;
}

int pact_string_find_after(pact_String* str, pact_String* find, unsigned int offset) {
	unsigned int i;
	if (find->length == 0) {
		return -1;
	}
	for (i = offset; i + find->length <= str->length; i++) {
		if (memcmp(str->data + i, find->data, find->length) == 0) {
			return i;
		}
	}
	return -1;
}

int pact_string_find_cstr(pact_String* str, char* find) {
	unsigned int i;
	unsigned int findlen = strlen(find);
	if (findlen == 0) {
		return -1;
	}
	for (i = 0; i + findlen <= str->length; i++)
	{
		if (memcmp(str->data + i, find, findlen) == 0) {
			return i;
		}
	}
	return -1;
}

int pact_string_find_cstr_after(pact_String* str, char* find, unsigned int offset) {
	unsigned int i;
	unsigned int findlen = strlen(find);
	if (findlen == 0) {
		return -1;
	}
	for (i = offset; i + findlen <= str->length; i++)
	{
		if (memcmp(str->data + i, find, findlen) == 0) {
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

int pact_string_partition_cstr(pact_String* str, char* delim, pact_String* head, pact_String* tail) {
	int find = pact_string_find_cstr(str, delim);
	unsigned int delimlen = strlen(delim);
	if (find == -1) {
		return -1;
	}
	pact_string_clone_substr(str, head, 0, find);
	pact_string_clone_substr(str, tail, find + delimlen, str->length - (find + delimlen));
	return find;
}

int pact_string_append(pact_String* str, pact_String* tail) {
	int totallength = str->length + tail->length;
	char* tmpbuf = malloc(str->length); //No need to null terminate this one
	pact_string_retrieve(str, tmpbuf, str->length);
	free(str->data);
	str->data = malloc(totallength + 1);
	if (!str->data) {
		return 1;
	}
	memcpy(str->data, tmpbuf, str->length);
	memcpy(str->data + str->length, tail->data, tail->length);
	str->length = totallength;
	str->data[totallength] = 0;
	return 0;
}

int pact_string_append_cstr(pact_String* str, char* tail) {
	int taillen = strlen(tail);
	int totallength = str->length + taillen;
	char* tmpbuf = malloc(str->length); //No need to null terminate this one
	pact_string_retrieve(str, tmpbuf, str->length);
	free(str->data);
	str->data = malloc(totallength + 1);
	if (!str->data) {
		return 1;
	}
	memcpy(str->data, tmpbuf, str->length);
	memcpy(str->data + str->length, tail, taillen);
	str->length = totallength;
	str->data[totallength] = 0;
	return 0;
}

int pact_string_prepend(pact_String* str, pact_String* head) {
	int totallength = str->length + head->length;
	char* tmpbuf = malloc(str->length); //No need to null terminate this one
	pact_string_retrieve(str, tmpbuf, str->length);
	free(str->data);
	str->data = malloc(totallength + 1);
	if (!str->data) {
		return 1;
	}
	memcpy(str->data + head->length, tmpbuf, str->length);
	memcpy(str->data, head->data, head->length);
	str->length = totallength;
	str->data[totallength] = 0;
	return 0;
}

int pact_string_prepend_cstr(pact_String* str, char* head) {
	int headlen = strlen(head);
	int totallength = str->length + headlen;
	char* tmpbuf = malloc(str->length); //No need to null terminate this one
	pact_string_retrieve(str, tmpbuf, str->length);
	free(str->data);
	str->data = malloc(totallength + 1);
	if (!str->data) {
		return 1;
	}
	memcpy(str->data + headlen , tmpbuf, str->length);
	memcpy(str->data, head, headlen);
	str->length = totallength;
	str->data[totallength] = 0;
	return 0;
}

int pact_string_chop_front(pact_String* str, unsigned int chop) {
	if (str->length <= chop) {
		return pact_string_clear(str);
	}
	pact_String* temp = pact_string_create(NULL);
	if (!temp) {
		return -1;
	}
	if (pact_string_clone_substr(str, temp, chop, str->length - chop)) {
		return -2;
	}
	//Magical buffer switch!
	char* buf = str->data;
	str->data = temp->data;
	temp->data = buf;
	temp->length = str->length; //correct length on decoy str
	str->length = str->length - chop; //correct length on real str
	pact_string_destroy(temp); //destroy decoy str
	return 0;
}

int pact_string_chop_back(pact_String* str, unsigned int chop) {
	if (str->length <= chop) {
		return pact_string_clear(str);
	}
	pact_String* temp = pact_string_create(NULL);
	if (!temp) {
		return -1;
	}
	if (pact_string_clone_substr(str, temp, 0, str->length - chop)) {
		return -2;
	}
	//Magical buffer switch!
	char* buf = str->data;
	str->data = temp->data;
	temp->data = buf;
	temp->length = str->length; //correct length on decoy str
	str->length = str->length - chop; //correct length on real str
	pact_string_destroy(temp); //destroy decoy str
	return 0;
}