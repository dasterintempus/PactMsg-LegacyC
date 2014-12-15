#include "crosssockets.h"
#include <stdlib.h>
#include <string.h>

#define MAX_CLIENTS 128
#define MAX_CLIENT_CHANNELS 256

struct pact_RefClient {
	pact_Socket sock;
	char* inbuf;
	char* outbuf;
	char* channels[MAX_CLIENT_CHANNELS];
};

typedef struct pact_RefClient pact_RefClient;

static unsigned short int port_num = 5432;

static char hostname[256] = "";
static pact_RefClient clients[MAX_CLIENTS];
static pact_Socket* serversock = 0;

void handle_client_line(pact_RefClient* client, char* line) {

}

void get_client_input(pact_RefClient* client, char* input) {
	int length1, length2;
	char* nbuf;
	char* line;
	int span;

	//build bigger buffer, copy new data into it.
	length1 = strlen(client->inbuf);
	length2 = strlen(input);
	nbuf = malloc(length1 + length2 + 1);
	strncpy(nbuf, client->inbuf, length1);
	strncpy(nbuf + length1, input, length2 + 1);
	free(client->inbuf);
	client->inbuf = nbuf;

	//Check for CRLFs
	while (1) {
		length1 = strlen(client->inbuf);
		span = strcspn(client->inbuf, "\r\n");
		if (span != length1) {
			//We found one!
			//Crazy string slicing magic time
			nbuf = malloc(length1 - (span + 2) + 1); //This will be client->inbuf eventually
			line = malloc(span + 1); //The line at the start
			strncpy(line, client->inbuf, span + 1);
			strncpy(nbuf, client->inbuf + span + 2, length1 - span + 1);
			free(client->inbuf);
			client->inbuf = nbuf;

			handle_client_line(client, line);
			free(line);
		}
		else {
			break;
		}
	}
}

void init_client(pact_RefClient* client)
{
	unsigned short int i;
	client->sock = -1;
	for (i = 0; i < MAX_CLIENT_CHANNELS; i++)
	{
		client->channels[i] = malloc(1);
		client->channels[i][0] = 0; //null-byte!
	}
}

int add_clients_to_fdsets(fd_set* fdset, fd_set* fdset2) {
	unsigned short int i;
	int max = 0;
	for (i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].sock != -1) {
			FD_SET(clients[i].sock, fdset);
			FD_SET(clients[i].sock, fdset2);
			if (clients[i].sock > (unsigned int)max) {
				max = clients[i].sock;
			}
		}
	}
	return max;
}

pact_RefClient* get_open_client_slot() {
	unsigned short int i;
	for (i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].sock == -1) {
			return clients + i;
		}
	}
	return 0;
}

int find_client(pact_RefClient client) {
	int i;
	for (i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i].sock == client.sock) {
			return i;
		}
	}
	return -1;
}

int main(int argc, char** argv) {
	int result = 0;
	struct sockaddr_in addr;
	struct hostent* hostdata;
	pact_RefClient* next_client;
	unsigned short int i;
	struct timeval tv;
	fd_set in_set, out_set;
	int maxfd = 0;

	for (i = 0; i < 128; i++) {
		init_client(clients + i);
	}

	memset(&addr, 0, sizeof(struct sockaddr_in));
	gethostname(hostname, 255);
	hostdata = gethostbyname(hostname);
	if (!hostdata) {
		//barf
		return 1;
	}

	addr.sin_family = hostdata->h_addrtype;
	addr.sin_port = htons(port_num);
	result = pact_socket_create(AF_INET, SOCK_STREAM, 0, serversock);
	if (result) {
		//barf
		return 2;
	}
	result = pact_socket_bind(serversock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
	if (result) {
		//barf
		return 3;
	}
	result = pact_socket_listen(serversock, 3);
	if (result) {
		//barf
		return 4;
	}

	tv.tv_sec = 0;
	tv.tv_usec = 250000;

	while (1) {
		next_client = get_open_client_slot();
		if (!next_client) {
			return 5;
		}
		result = pact_socket_accept(serversock, &(next_client->sock), 0, 0);
		if (result) {
			if (result != EAGAIN && result != EWOULDBLOCK && result != EINTR) {
				//barf
				return 6;
			}
		}
		else {
			init_client(next_client); //Clear the client we attempted to set
		}


		FD_ZERO(&in_set);
		FD_ZERO(&out_set);
		maxfd = add_clients_to_fdsets(&in_set, &out_set);

		result = select(maxfd, &in_set, &out_set, 0, &tv);
		if (result)	{
			if (result != EINTR) {
				//barf
				return 7;
			}
		}
		else {
			for (i = 0; i < MAX_CLIENTS; i++) {
				if (FD_ISSET(clients[i].sock, &in_set)) {
					//Can read!

				}
			}
		}
	}
}