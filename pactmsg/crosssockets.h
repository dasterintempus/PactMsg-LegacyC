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

#endif

int pact_get_last_socket_error();
int pact_socket_create(int domain, int type, int blocking, pact_Socket* sock);
void pact_socket_close(pact_Socket* sock);
#endif