/*
//This code is heavily based on code from libircclient (http://www.ulduzsoft.com/libircclient/), specifically src/sockets.c
*/
#include "crosssockets.h"

int pact_get_last_socket_error() {
#ifndef PACT_OS_WIN
	return errno;
#else
	return WSAGetLastError();
#endif
}

int pact_socket_create(int domain, int type, int blocking, pact_Socket* sock) {
	*sock = socket(domain, type, 0);
	if (PACT_CHECK_SOCKET_ERROR(*sock)) {
		return *sock;
	}

	if (!blocking) {
#ifndef PACT_OS_WIN
		int fail = fcntl(*sock, F_SETFL, fcntl(*sock, F_GETFL, 0) | O_NONBLOCK) != 0;
#else
		unsigned long mode = 0;
		int fail = ioctlsocket(*sock, FIONBIO, &mode);
#endif
		if (PACT_CHECK_SOCKET_ERROR(fail)) {
			pact_socket_close(sock);
			return fail;
		}
	}
	return 0;
}

void pact_socket_close(pact_Socket* sock) {
#ifndef PACT_OS_WIN
	close(*sock);
#else
	closesocket(*sock);
#endif

	*sock = -1;
}

int pact_socket_connect(pact_Socket* sock, const struct sockaddr* saddr, socklen_t len) {
	//This just loops connect() if it gets EINTR (who needs interrupts anyway)
	//Otherwise it will return the socket error number
	int socketreturn = 0;
	while (1) {
		if (connect(*sock, saddr, len) < 0)	{
			socketreturn = pact_get_last_socket_error();
			if (socketreturn != EINTR) {
				return socketreturn;
			}
			//else keep looping
		}
		else {
			return 0;
		}
	}
	return 0;
}

int pact_socket_bind(pact_Socket* sock, const struct sockaddr* saddr, socklen_t len) {
	//This is basically a straight rename, in case I decide to wrap bind()'s functionality at all later
	return bind(*sock, saddr, len);
}

int pact_socket_accept(pact_Socket* sock, pact_Socket* newsock, struct sockaddr* saddr, socklen_t* len) {
	//See pact_socket_connect, except for accept()
	int nsocket = 0;
	int socketreturn = 0;
	while (1) {
		nsocket = accept(*sock, saddr, len);
		if (PACT_CHECK_SOCKET_ERROR(nsocket)) {
			socketreturn = pact_get_last_socket_error();
			if (socketreturn != EINTR) {
				return socketreturn;
			}
			//else keep looping
		}
		else {
			*newsock = nsocket;
			return 0;
		}
	}
	return 0;
}

int pact_socket_recv(pact_Socket* sock, void *buf, size_t len) {
	//See pact_socket_connect, except for recv()
	int length = 0;
	int socketreturn = 0;
	while (1) {
		length = recv(*sock, buf, len, 0);
		if (PACT_CHECK_SOCKET_ERROR(length)) {
			socketreturn = pact_get_last_socket_error();
			if (socketreturn != EINTR) {
				return socketreturn;
			}
			//else keep looping
		}
		else {
			return length;
		}
	}
	return 0;
}

int pact_socket_send(pact_Socket* sock, const void* buf, size_t len) {
	//See pact_socket_connect, except for send()
	int length = 0;
	int socketreturn = 0;
	while (1) {
		length = send(*sock, buf, len, 0);
		if (PACT_CHECK_SOCKET_ERROR(length)) {
			socketreturn = pact_get_last_socket_error();
			if (socketreturn != EINTR) {
				return socketreturn;
			}
			//else keep looping
		}
		else {
			return length;
		}
	}
	return 0;
}