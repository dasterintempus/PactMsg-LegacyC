#include "crosssockets.h"

int pact_get_last_socket_error()
{
#ifndef PACT_OS_WIN
	return errno;
#else
	return WSAGetLastError();
#endif
}

int pact_socket_create(int domain, int type, int blocking, pact_crosssocket_t* sock)
{
	*sock = socket(domain, type, 0);
	if (!blocking)
	{
#ifndef PACT_OS_WIN
		int fail = fcntl(*sock, F_SETFL, fcntl(*sock, F_GETFL, 0) | O_NONBLOCK) != 0;
#else
		unsigned long mode = 0;
		int fail = PACT_CHECK_SOCKET_ERROR(ioctlsocket(*sock, FIONBIO, &mode));
#endif
		if (fail)
		{
			pact_socket_close(sock);
			return 1;
		}
	}
	return PACK_CHECK_SOCKET_ERROR(*sock) ? 1 : 0;
}

void pact_socket_close(pact_crosssocket_t* sock)
{
#ifndef PACT_OS_WIN
	close(*sock);
#else
	closesocket(*sock);
#endif

	*sock = -1;
}