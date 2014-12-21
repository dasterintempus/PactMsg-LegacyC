#ifndef PACT_CONNECTION_H
#define PACT_CONNECTION_H

#include "config.h"
#include "pstring.h"

#ifdef PACT_SUPPORTEDCONN_IRC
#include <libircclient.h>
#endif

typedef enum {
	PACT_CONNECTIONPROTOCOL_IRC = 1,
	PACT_CONNECTIONPROTOCOL_XMPP = 2,
	PACT_CONNECTIONPROTOCOL_REF = 3,
} pact_ConnectionProtocol;

typedef struct {
	pact_String* host;
	pact_String* port;
	pact_String* id;
} pact_RefConnectionServerData;

typedef struct {
	pact_String* host;
	pact_String* port;
	pact_String* username;
	pact_String* domain;
	pact_String* resource;
	pact_String* pass;
} pact_XMPPConnectionServerData;

typedef struct {
	pact_String* host;
	pact_String* port;
	pact_String* pass;
	pact_String* nick;
	pact_String* username;
	pact_String* realname;
} pact_IRCConnectionServerData;

typedef struct pact_RefConnection pact_RefConnection;
#ifdef PACT_SUPPORTEDCONN_IRC
typedef struct pact_IRCConnection pact_IRCConnection;
#endif
#ifdef PACT_SUPPORTEDCONN_XMPP
typedef struct pact_XMPPConnection pact_XMPPConnection;
#endif
typedef struct pact_Connection pact_Connection;

//pact_Connection
PACT_EXPORT pact_Connection* pact_connection_new(pact_ConnectionProtocol proto);
//PACT_EXPORT pact_Connection* pact_connection_create_child(pact_connection_proto_t proto, pact_client_t* parent);
PACT_EXPORT void pact_connection_free(pact_Connection* conn);
PACT_EXPORT int pact_connection_start(pact_Connection* conn, void* serverdata);
PACT_EXPORT int pact_connection_think(pact_Connection* conn);

PACT_EXPORT void pact_connection_q_send(pact_Connection* conn, pact_String* message);
PACT_EXPORT pact_String* pact_connection_q_recv(pact_Connection* conn);

//ref connection
pact_RefConnection* _pact_refconnection_new(pact_Connection* parent);
void _pact_refconnection_free(pact_RefConnection* ref);
int _pact_refconnection_start(pact_RefConnection* ref, pact_RefConnectionServerData* serverdata);
int _pact_refconnection_think(pact_RefConnection* ref);

#ifdef PACT_SUPPORTEDCONN_IRC
void _pact_ircevent_on_connect(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count);

//ircconnection
pact_IRCConnection* _pact_ircconnection_new(pact_Connection* parent);
void _pact_ircconnection_free(pact_IRCConnection* irc);
int _pact_ircconnection_start(pact_IRCConnection* irc, pact_IRCConnectionServerData* serverdata);
int _pact_ircconnection_think(pact_IRCConnection* irc);

#endif

#endif