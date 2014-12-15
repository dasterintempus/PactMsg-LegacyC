/*
//This code is heavily based on code from libircclient (http://www.ulduzsoft.com/libircclient/), specifically src/sockets.c
*/

#ifndef PACT_CROSSSOCKETS_H
#define PACT_CROSSSOCKETS_H

#include "config.h"

#ifndef PACT_OS_WIN

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>	
#include <netinet/in.h>
#include <fcntl.h>

typedef int pact_Socket;

#define PACT_CHECK_SOCKET_ERROR(e) ((e)<0)

#else

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

typedef SOCKET pact_Socket;

#define PACT_CHECK_SOCKET_ERROR(e)	((e)==SOCKET_ERROR)

#if !defined(EWOULDBLOCK)
#define EWOULDBLOCK		WSAEWOULDBLOCK
#endif
#if !defined(EINPROGRESS)
#define EINPROGRESS		WSAEINPROGRESS
#endif
#if !defined(EINTR)
#define EINTR			WSAEINTR
#endif
#if !defined(EAGAIN)
#define EAGAIN			EWOULDBLOCK
#endif

#endif

int pact_get_last_socket_error();
int pact_socket_create(int domain, int type, int blocking, pact_Socket* sock);
void pact_socket_close(pact_Socket* sock);
int pact_socket_connect(pact_Socket* sock, const struct sockaddr* saddr, socklen_t len);
int pact_socket_bind(pact_Socket* sock, const struct sockaddr* saddr, socklen_t len);
int pact_socket_accept(pact_Socket* sock, pact_Socket* newsock, struct sockaddr* saddr, socklen_t* len);
int pact_socket_recv(pact_Socket* sock, void* buf, size_t len);
int pact_socket_send(pact_Socket* sock, const void* buf, size_t len);
#endif