#ifndef PACT_REFCONNECTION_H
#define PACT_REFCONNECTION_H

#include "config.h"
#include "connection.h"
#include "pstring.h"

typedef struct {
	pact_String* host;
	pact_String* port;
	pact_String* id;
} pact_RefConnectionServerData;

pact_RefConnection* _pact_refconnection_new(pact_Connection* parent);
void _pact_refconnection_free(pact_RefConnection* ref);
int _pact_refconnection_start(pact_RefConnection* ref, pact_RefConnectionServerData* serverdata);
int _pact_refconnection_think(pact_RefConnection* ref);


#endif