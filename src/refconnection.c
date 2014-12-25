#include "refconnection.h"
#include "crosssockets.h"
#include "debug.h"
#include <stdlib.h>
#include <string.h>

struct pact_RefConnection {
	pact_Socket* socket;
	pact_RefConnectionServerData* serverdata;
	pact_Connection* parent;
};

pact_RefConnection* _pact_refconnection_new(pact_Connection* parent) {
	if (!parent) {
		return 0;
	}

	pact_RefConnection* ref = malloc(sizeof(pact_RefConnection));
	if (!ref) {
		return 0;
	}
	memset(ref, 0, sizeof(pact_RefConnection));

	ref->parent = parent;
	ref->socket = malloc(sizeof(pact_Socket));
	*(ref->socket) = -1;

	return ref;
}

void _pact_refconnection_free(pact_RefConnection* ref) {
	if (ref->serverdata) {
		if (ref->serverdata->host) {
			pact_string_free(ref->serverdata->host);
		}
		if (ref->serverdata->port) {
			pact_string_free(ref->serverdata->port);
		}
		if (ref->serverdata->id) {
			pact_string_free(ref->serverdata->id);
		}
		free(ref->serverdata);
	}
	free(ref);
}

int _pact_refconnection_start(pact_RefConnection* ref, pact_RefConnectionServerData* serverdata) {
	ref->serverdata = serverdata;

	int result = 0;
	struct sockaddr_in addr;
	struct addrinfo hints;
	struct addrinfo* addrresult;
	struct addrinfo* rp;

	memset(&addr, 0, sizeof(struct sockaddr_in));
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = 0;
	hints.ai_canonname = 0;
	hints.ai_addr = 0;
	hints.ai_next = 0;

	result = getaddrinfo(pact_string_get_cstr(ref->serverdata->host), pact_string_get_cstr(ref->serverdata->port), &hints, &addrresult);
	if (PACT_CHECK_SOCKET_ERROR(result)) {
		//barf
		pact_debug_print("getaddrinfo: %ls\n", gai_strerror(pact_get_last_socket_error()));
		return 1;
	}

	for (rp = addrresult; rp != NULL; rp = rp->ai_next) {
		*(ref->socket) = -1;
		result = pact_socket_create(rp->ai_family, rp->ai_socktype, 0, ref->socket);
		if (result) {
			//keep trying
			continue;
		}
		result = pact_socket_connect(ref->socket, rp->ai_addr, rp->ai_addrlen);
		if (result) {
			//keep trying
			pact_socket_close(ref->socket);
			continue;
		}
	}

	freeaddrinfo(addrresult);
	if (*(ref->socket) == -1) {
		//NOW fail
		pact_debug_write("no serversock\n");
		return 1;
	}

	return 0;
}

int _pact_refconnection_think(pact_RefConnection* ref) {
	return 0;
}
