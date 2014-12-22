#ifndef PACT_IRCCONNECTION_H
#define PACT_IRCCONNECTION_H

#include "config.h"
#include "connection.h"
#include "pstring.h"

#ifdef PACT_SUPPORTEDCONN_IRC
#include <libircclient.h>
#endif

typedef struct {
	pact_String* host;
	pact_String* port;
	pact_String* pass;
	pact_String* nick;
	pact_String* username;
	pact_String* realname;
} pact_IRCConnectionServerData;

#ifdef PACT_SUPPORTEDCONN_IRC
typedef struct pact_IRCConnection pact_IRCConnection;
#endif

#ifdef PACT_SUPPORTEDCONN_IRC
void _pact_ircevent_on_connect(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count);

pact_IRCConnection* _pact_ircconnection_new(pact_Connection* parent);
void _pact_ircconnection_free(pact_IRCConnection* irc);
int _pact_ircconnection_start(pact_IRCConnection* irc, pact_IRCConnectionServerData* serverdata);
int _pact_ircconnection_think(pact_IRCConnection* irc);

#endif

#endif