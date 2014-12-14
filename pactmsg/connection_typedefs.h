#ifndef PACT_CONNECTION_TYPEDEFS_H
#define PACT_CONNECTION_TYPEDEFS_H

#include <libircclient.h>

typedef enum
{
	PACT_CONNECTION_IRC = 1,
	PACT_CONNECTION_XMPP = 2,
	PACT_CONNECTION_TELNET = 3,
} pact_connection_proto_t;

typedef struct
{
	char* name;
} pact_channel_data_t;

typedef struct
{
	char* hostname;
	unsigned short port;
	char* username;
	char* pass;
} pact_telnetserver_data_t;

typedef struct
{
	char* hostname;
	unsigned short port;
	char* username;
	char* domain;
	char* resource;
	char* pass;
} pact_xmppserver_data_t;

typedef struct
{
	char* hostname;
	unsigned short port;
	char* pass;
	char* nick;
	char* username;
	char* realname;
} pact_ircserver_data_t;

typedef struct pact_telnetconnection_s pact_telnetconnection_t;
#ifdef PACT_SUPPORTEDCONN_IRC
typedef struct pact_ircconnection_s pact_ircconnection_t;
#endif
#ifdef PACT_SUPPORTEDCONN_XMPP
typedef struct pact_xmppconnection_s pact_xmppconnection_t;
#endif
typedef struct pact_connection_s pact_connection_t;
typedef struct pact_server_data_s pact_server_data_t;
typedef struct pact_client_s pact_client_t;

#endif