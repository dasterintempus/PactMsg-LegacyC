#include "../crosssockets.h"
#include "../pstring.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_CLIENTS 128
#define MAX_CLIENT_CHANNELS 256

struct pact_RefClient {
	pact_Socket* sock;
	pact_String* inbuf;
	pact_String* outbuf;
	pact_String* channels[MAX_CLIENT_CHANNELS];
	pact_String* id;
};

typedef struct pact_RefClient pact_RefClient;

static unsigned short int port_num = 5432;
static char* port_str = "5432";

static char hostname[256] = "";
static pact_RefClient clients[MAX_CLIENTS];
static pact_Socket* serversock = 0;

int extract_from_str_to_space(pact_String* src, pact_String* dst, char* between) {
	int find;
	int find2;
	int error;
	find = pact_string_find_cstr(src, between);
	if (find == -1) {
		return -1;
	}
	find2 = pact_string_find_cstr(src, " ");
	if (find2 == -2) {
		//No space
		error = pact_string_clone_substr(src, dst, find + 1, pact_string_length(src) - (find + 1));
	}
	else {
		error = pact_string_clone_substr(src, dst, find + 1, find2 - find);
	}
	if (error) {
		return -2;
	}
	return find;
}

int extract_id(pact_String* line, pact_String* id) {
	return extract_from_str_to_space(line, id, "@");
}

int extract_channel_name(pact_String* line, pact_String* channel) {
	return extract_from_str_to_space(line, channel, "#");
}

pact_String* get_open_channel_slot(pact_RefClient* client) {
	unsigned short int i;
	for (i = 0; i < MAX_CLIENT_CHANNELS; i++) {
		if (pact_string_length(client->channels[i]) == 0) {
			return client->channels[i];
		}
	}
	return 0;
}

pact_RefClient* get_open_client_slot() {
	unsigned short int i;
	for (i = 0; i < MAX_CLIENTS; i++) {
		if (*(clients[i].sock) == -1) {
			return clients + i;
		}
	}
	return 0;
}

pact_RefClient* get_client_by_id(pact_String* id) {
	unsigned short int i;
	for (i = 0; i < MAX_CLIENTS; i++) {
		if (pact_string_compare(clients[i].id, id) == 0) {
			return clients + i;
		}
	}
	return 0;
}

int find_client_pos(pact_RefClient* client) {
	unsigned short int i;
	for (i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].sock == client->sock) {
			return i;
		}
	}
	return -1;
}

int find_channel(pact_RefClient* client, pact_String* channel) {
	unsigned short int i;
	for (i = 0; i < MAX_CLIENT_CHANNELS; i++) {
		if (pact_string_compare(channel, client->channels[i]) == 0) {
			return i;
		}
	}
	return -1;
}

void find_clients_with_channel(unsigned short int* set, pact_String* channel) {
	unsigned short int i, j;
	for (i = 0; i < MAX_CLIENTS; i++) {
		if (*clients[i].sock != -1) {
			for (j = 0; j < MAX_CLIENT_CHANNELS; j++) {
				if (pact_string_compare(channel, clients[i].channels[j]) == 0) {
					set[i] = 1;
				}
			}
		}
		else {
			set[i] = 0;
		}
	}
}

void handle_client_line(pact_RefClient* client, pact_String* line) {
	pact_String* channelslot;
	pact_String* buffer = 0;
	pact_String* message;
	pact_RefClient* target;
	int find;
	int res;
	unsigned short int* set;
	unsigned short int i;

	if (pact_string_find_cstr(line, "SUB") == 0) {
		//SUB commmand at the start of the string
		if (pact_string_find_cstr(line, "SUB #") == 0) {
			//We found a SUB command, with the channel identifier prefix, at the start of the string!
			buffer = pact_string_create();
			//copy the relevant portions of the line into buffer
			res = extract_channel_name(line, buffer);
			if (res < 0) {
				pact_string_append_cstr(client->outbuf, "-\r\n"); //Send error reply
				pact_string_destroy(buffer);
				return;
			}
			channelslot = get_open_channel_slot(client);
			pact_string_clone(buffer, channelslot);

			pact_string_append_cstr(client->outbuf, "+\r\n"); //Send ok reply
		}
		else {
			//Badly formatted SUB command (only works on channels)
			pact_string_append_cstr(client->outbuf, "-\r\n"); //Send error reply
		}
	}
	else if (pact_string_find_cstr(line, "NSUB") == 0) {
		//NSUB commmand at the start of the string
		if (pact_string_find_cstr(line, "NSUB #") == 0) {
			//We found an NSUB command, with the channel identifier prefix, at the start of the string!
			buffer = pact_string_create();
			//copy the relevant portions of the line into buffer
			res = extract_channel_name(line, buffer);
			if (res < 0) {
				pact_string_append_cstr(client->outbuf, "-\r\n"); //Send error reply
				pact_string_destroy(buffer);
				return;
			}
			find = find_channel(client, buffer);
			if (find != -1) {
				channelslot = client->channels[find];
				pact_string_clear(channelslot);
				pact_string_append_cstr(client->outbuf, "+\r\n"); //Send ok reply
			}
			else {
				pact_string_append_cstr(client->outbuf, "-\r\n"); //Send error reply
			}
		}
		else {
			//Badly formatted NSUB command (only works on channels)
			pact_string_append_cstr(client->outbuf, "-\r\n"); //Send error reply
		}
	}
	else if (pact_string_find_cstr(line, "MSG") == 0) {
		//MSG commmand at the start of the string
		if (pact_string_find_cstr(line, "MSG #") == 0) {
			//We found a MSG command, with the channel identifier prefix, at the start of the string!
			buffer = pact_string_create();
			//copy the relevant portions of the line into buffer
			res = extract_channel_name(line, buffer);
			if (res < 0) {
				pact_string_append_cstr(client->outbuf, "-\r\n"); //Send error reply
				pact_string_destroy(buffer);
				return;
			}
			message = pact_string_create();
			pact_string_clone_substr(line, message, 5 + pact_string_length(buffer), pact_string_length(line) - (5 + pact_string_length(buffer)));
			pact_string_append_cstr(message, "\r\n");
			set = malloc(sizeof(unsigned short int) * MAX_CLIENTS);
			find_clients_with_channel(set, buffer);
			for (i = 0; i < MAX_CLIENTS; i++) {
				if (set[i]) {
					pact_string_append(clients[i].outbuf, message);
				}
			}
			pact_string_append_cstr(client->outbuf, "+\r\n"); //Send ok reply
			pact_string_destroy(message);
		}
		else if (pact_string_find_cstr(line, "MSG @") == 0) {
			//We found a MSG command, with the user identifier prefix, at the start of the string!
			buffer = pact_string_create();
			res = extract_id(line, buffer);
			if (res < 0) {
				pact_string_append_cstr(client->outbuf, "-\r\n"); //Send error reply
				pact_string_destroy(buffer);
				return;
			}
			message = pact_string_create();
			pact_string_clone_substr(line, message, 5 + pact_string_length(buffer), pact_string_length(line) - (5 + pact_string_length(buffer)));
			pact_string_append_cstr(message, "\r\n");
			target = get_client_by_id(buffer);
			if (!target) {
				pact_string_append_cstr(client->outbuf, "-\r\n"); //Send error reply
				pact_string_destroy(buffer);
				pact_string_destroy(message);
				return;
			}
			pact_string_append(target->outbuf, message);
			pact_string_append_cstr(client->outbuf, "+\r\n"); //Send ok reply
			pact_string_destroy(message);
		}
		else {
			//Badly formatted MSG command (only works on channels or users)
			pact_string_append_cstr(client->outbuf, "-\r\n"); //Send error reply
		}
	}
	else if (pact_string_find_cstr(line, "ID") == 0) {
		//ID commmand at the start of the string
		if (pact_string_find_cstr(line, "ID @") == 0) {
			//We found an ID command, with the user identifier prefix, at the start of the string!
			buffer = pact_string_create();
			res = extract_id(line, buffer);
			if (res < 0) {
				pact_string_append_cstr(client->outbuf, "-\r\n"); //Send error reply
				pact_string_destroy(buffer);
				return;
			}
			target = get_client_by_id(buffer);
			if (target) {
				//ID in use
				pact_string_append_cstr(client->outbuf, "-\r\n"); //Send error reply
				pact_string_destroy(buffer);
				return;
			}
			pact_string_clone(buffer, client->id);
		}
		else {
			//Badly formatted ID command (only works on users)
			pact_string_append_cstr(client->outbuf, "-\r\n"); //Send error reply
		}
	}
	if (buffer) {
		pact_string_destroy(buffer);
	}
}

void get_client_input(pact_RefClient* client, pact_String* input) {
	pact_String* line;
	pact_String* buf;
	int find;

	pact_string_append(client->inbuf, input);

	//Check for CRLFs
	while (1) {
		line = pact_string_create();
		buf = pact_string_create();
		find = pact_string_partition_cstr(client->inbuf, "\r\n", line, buf);
		if (find != -1) {
			//We found one!
			//Nuke the old buffer
			pact_string_destroy(client->inbuf);
			//Replace it with the remainder after the partition call
			client->inbuf = buf;

			handle_client_line(client, line);
			pact_string_destroy(line); //Clean up the line after we're done with it
		}
		else {
			pact_string_destroy(line);
			pact_string_destroy(buf);
			break;
		}
	}
}

void init_client(pact_RefClient* client)
{
	unsigned short int i;

	client->sock = malloc(sizeof(pact_Socket));
	*client->sock = -1;
	client->inbuf = pact_string_create();
	client->outbuf = pact_string_create();
	client->id = pact_string_create();
	for (i = 0; i < MAX_CLIENT_CHANNELS; i++)
	{
		client->channels[i] = pact_string_create();
	}
}

void clear_client(pact_RefClient* client)
{
	unsigned short int i;

	free(client->sock);
	pact_string_destroy(client->inbuf);
	pact_string_destroy(client->outbuf);
	pact_string_destroy(client->id);
	for (i = 0; i < MAX_CLIENT_CHANNELS; i++)
	{
		pact_string_destroy(client->channels[i]);
	}
}

int add_clients_to_fdsets(fd_set* fdset, fd_set* fdset2, fd_set* fdset3) {
	unsigned short int i;
	int max = -1;
	for (i = 0; i < MAX_CLIENTS; i++) {
		if (*(clients[i].sock) != -1) {
			FD_SET(*(clients[i].sock), fdset);
			FD_SET(*(clients[i].sock), fdset2);
			FD_SET(*(clients[i].sock), fdset3);
			if ((int)*(clients[i].sock) > max) {
				max = *(clients[i].sock);
			}
		}
	}
	return max;
}

int main(int argc, char** argv) {
	int result = 0;
	struct sockaddr_in addr;
	struct addrinfo hints;
	struct addrinfo* addrresult;
	struct addrinfo* rp;
	pact_RefClient* next_client;
	unsigned short int i;
	struct timeval tv;
	fd_set in_set, out_set, err_set;
	int maxfd = 0;
	char* buf;
	pact_String* bufstr;

	for (i = 0; i < MAX_CLIENTS; i++) {
		init_client(clients + i);
	}

	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return 1;
	}

	memset(&addr, 0, sizeof(struct sockaddr_in));
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
	hints.ai_canonname = 0;
	hints.ai_addr = 0;
	hints.ai_next = 0;
	
	result = getaddrinfo(0, port_str, &hints, &addrresult);
	if (PACT_CHECK_SOCKET_ERROR(result)) {
		//barf
		fprintf(stderr, "getaddrinfo: %ls\n", gai_strerror(pact_get_last_socket_error()));
		return 2;
	}
	serversock = malloc(sizeof(pact_Socket));
	for (rp = addrresult; rp != NULL; rp = rp->ai_next) {
		*serversock = -1;
		result = pact_socket_create(rp->ai_family, rp->ai_socktype, 0, serversock);
		if (result) {
			//hang on
			continue;
		}
		result = pact_socket_bind(serversock, rp->ai_addr, rp->ai_addrlen);
		if (result) {
			//hang on
			pact_socket_close(serversock);
			continue;
		}
	}
	freeaddrinfo(addrresult);
	if (*serversock == -1) {
		//NOW barf
		fprintf(stderr, "no serversock: %ls\n");
		return 3;
	}
	result = pact_socket_listen(serversock, 3);
	if (result) {
		//barf
		fprintf(stderr, "accept: %i\n", result);
		return 4;
	}

	while (1) {
		next_client = get_open_client_slot();
		if (!next_client) {
			fprintf(stderr, "out of client slots\n");
			return 5;
		}
		result = pact_socket_accept(serversock, next_client->sock, 0, 0);
		if (result) {
			*(next_client->sock) = -1; //Reset the client we tried to set
			if (result != EAGAIN && result != EWOULDBLOCK && result != EINTR) {
				//barf
				fprintf(stderr, "accept: %i\n", result);
				return 6;
			}
		}
		else {
			//Greeting
			pact_string_append_cstr(next_client->outbuf, "...\r\n");
		}

		FD_ZERO(&in_set);
		FD_ZERO(&out_set);
		FD_ZERO(&err_set);
		maxfd = add_clients_to_fdsets(&in_set, &out_set, &err_set);
		if (maxfd == -1) {
			//No sockets yet
			continue;
		}
		tv.tv_sec = 0;
		tv.tv_usec = 250000;
		result = select(maxfd+1, &in_set, &out_set, &err_set, &tv);
		if (PACT_CHECK_SOCKET_ERROR(result)) {
			result = pact_get_last_socket_error();
			if (result != EINTR) {
				//barf
				fprintf(stderr, "select: %i\n", result);
				return 7;
			}
		}
		else {
			for (i = 0; i < MAX_CLIENTS; i++) {
				if (*clients[i].sock == -1) {
					continue;
				}
				if (FD_ISSET(*(clients[i].sock), &in_set)) {
					//Can read!
					buf = malloc(1024);
					result = pact_socket_recv(clients[i].sock, buf, 1024);
					if (result < 0) {
						if (result != EINTR) {
							//Assume dead client
							pact_socket_close(clients[i].sock);
							clear_client(clients + i);
							init_client(clients + i);
						}
					}
					else if (result > 0) {
						//We have data
						bufstr = pact_string_create();
						pact_string_assign_length(bufstr, buf, result);
						get_client_input(clients + i, bufstr);
						pact_string_destroy(bufstr);
					}
					free(buf);
				} //end if for reading
				if (FD_ISSET(*(clients[i].sock), &out_set)) {
					//Can write!
					if (pact_string_length(clients[i].outbuf) > 0) {
						//Have stuff to write
						buf = malloc(1024);
						pact_string_retrieve(clients[i].outbuf, buf, 1024);
						result = pact_socket_send(clients[i].sock, buf, pact_string_length(clients[i].outbuf));
						if (result < 0) {
							if (result != EINTR) {
								//Assume dead client
								pact_socket_close(clients[i].sock);
								clear_client(clients + i);
								init_client(clients + i);
							}
						}
						else {
							//We sent data
							pact_string_chop_front(clients[i].outbuf, result); //Remove data that was transmitted from outbuffer
						}
						free(buf);
					}
				} //end if for writing
				if (FD_ISSET(*(clients[i].sock), &err_set)) {
					//Error!
					//Assume dead client
					pact_socket_close(clients[i].sock);
					clear_client(clients + i);
					init_client(clients + i);
				} //end if for errors
			} //endfor
		}
	}
}