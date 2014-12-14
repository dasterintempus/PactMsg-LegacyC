#ifndef PACT_CONNECTION_H
#define PACT_CONNECTION_H

#include "config.h"

#include <stdlib.h>
#include <string.h>

#include "connection_typedefs.h"

//connection
PACT_EXPORT pact_connection_t* pact_connection_create(pact_connection_proto_t proto);
PACT_EXPORT pact_connection_t* pact_connection_create_child(pact_connection_proto_t proto, pact_client_t* parent);
PACT_EXPORT void pact_connection_destroy(pact_connection_t* conn);
PACT_EXPORT int pact_connection_start(pact_connection_t* conn, void* serverdata);
PACT_EXPORT int pact_connection_think(pact_connection_t* conn);

PACT_EXPORT void pact_connection_q_send(pact_connection_t* conn, char* message);
PACT_EXPORT char* pact_connection_q_recv(pact_connection_t* conn);

//telnet connection
pact_telnetconnection_t* _pact_telnetconnection_create(pact_connection_t* parent);
void _pact_telnetconnection_destroy(pact_telnetconnection_t* telnet);
int _pact_telnetconnection_start(pact_telnetconnection_t* telnet, pact_telnetserver_data_t* serverdata);
int _pact_telnetconnection_think(pact_telnetconnection_t* telnet);

#ifdef PACT_SUPPORTEDCONN_IRC
void _pact_ircevent_on_connect(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count);

//ircconnection
pact_ircconnection_t* _pact_ircconnection_create(pact_connection_t* parent);
void _pact_ircconnection_destroy(pact_ircconnection_t* irc);
int _pact_ircconnection_start(pact_ircconnection_t* irc, pact_ircserver_data_t* serverdata);
int _pact_ircconnection_think(pact_ircconnection_t* irc);

#endif

#endif