#ifndef PACT_CONNECTION_H
#define PACT_CONNECTION_H

#include "config.h"
#include "pstring.h"

typedef enum {
	PACT_CONNECTIONPROTOCOL_IRC = 1,
	PACT_CONNECTIONPROTOCOL_XMPP = 2,
	PACT_CONNECTIONPROTOCOL_REF = 3,
} pact_ConnectionProtocol;


typedef struct {
	pact_String* host;
	pact_String* port;
	pact_String* username;
	pact_String* domain;
	pact_String* resource;
	pact_String* pass;
} pact_XMPPConnectionServerData;

typedef struct {
	pact_String* type;
	pact_String* data;
} pact_ConnectionMessage;

typedef struct pact_RefConnection pact_RefConnection;
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

PACT_EXPORT void pact_connection_q_send(pact_Connection* conn, pact_ConnectionMessage* message);
PACT_EXPORT pact_ConnectionMessage* pact_connection_q_recv(pact_Connection* conn);

#endif