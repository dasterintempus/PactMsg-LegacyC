#include "connection.h"
#include "debug.h"
#include "linkedlist.h"
#include "crosssockets.h"
#include "ircconnection.h"
#include "refconnection.h"
#include <stdlib.h>
#include <string.h>

#ifdef PACT_SUPPORTEDCONN_XMPP
struct pact_XMPPConnection {
	pact_XMPPConnectionServerData* serverdata;
	pact_Connection* parent;
};
#endif

struct pact_Connection {
	//protocol specifier
	pact_ConnectionProtocol proto;

	//protocol-specific-structure
	pact_RefConnection* ref;
#ifdef PACT_SUPPORTEDCONN_IRC
	pact_IRCConnection* irc;
#endif
#ifdef PACT_SUPPORTEDCONN_XMPP
	pact_XMPPConnection* xmpp;
#endif

	//parent link
	//pact_Client* parent;

	//message queues
	pact_LinkedList* out_q;
	pact_LinkedList* in_q;
};

pact_Connection* pact_connection_new(pact_ConnectionProtocol proto) {
	pact_Connection* conn = malloc(sizeof(pact_Connection));

	if (!conn) {
		return 0;
	}

	memset(conn, 0, sizeof(pact_Connection));

	conn->proto = proto;
	if (conn->proto == PACT_CONNECTIONPROTOCOL_REF) {
		conn->ref = _pact_refconnection_new(conn);
		if (!conn->ref) {
			return 0;
		}
	}
#ifdef PACT_SUPPORTEDCONN_IRC
	else if (conn->proto == PACT_CONNECTIONPROTOCOL_IRC) {
		conn->irc = _pact_ircconnection_new(conn);
		if (!conn->irc) {
			return 0;
		}
	}
#endif
#ifdef PACT_SUPPORTEDCONN_XMPP
	else if (conn->proto == PACT_CONNECTIONPROTOCOL_XMPP) {
		conn->xmpp = _pact_xmppconnection_new(conn);
		if (!conn->xmpp) {
			return 0;
		}
	}
#endif

	conn->in_q = pact_linkedlist_new();
	conn->out_q = pact_linkedlist_new();

	return conn;
}

/*
pact_Connection* pact_connection_create_child(pact_connection_proto_t proto, pact_client_t* parent) {
	pact_Connection* conn = pact_connection_create(proto);
	if (!conn)
		return 0;
	conn->parent = parent;
	return conn;
}
*/

void pact_connection_free(pact_Connection* conn) {
	if (conn->ref) {
		_pact_refconnection_free(conn->ref);
	}
#ifdef PACT_SUPPORTEDCONN_IRC
	if (conn->irc) {
		_pact_ircconnection_free(conn->irc);
	}
#endif
#ifdef PACT_SUPPORTEDCONN_XMPP
	if (conn->xmpp) {
		pact_xmppconnection_free(conn->xmpp);
	}
#endif
	free(conn->in_q);
	free(conn->out_q);
	free(conn);
}

int pact_connection_start(pact_Connection* conn, void* serverdata) {
	if (conn->proto == PACT_CONNECTIONPROTOCOL_REF) {
		return _pact_refconnection_start(conn->ref, (pact_RefConnectionServerData*)serverdata);
	}
#ifdef PACT_SUPPORTEDCONN_IRC
	else if (conn->proto == PACT_CONNECTIONPROTOCOL_IRC) {
		return _pact_ircconnection_start(conn->irc, (pact_IRCConnectionServerData*)serverdata);
	}
#endif
#ifdef PACT_SUPPORTEDCONN_XMPP
	else if (conn->proto == PACT_CONNECTIONPROTOCOL_XMPP) {
		return _pact_xmppconnection_start(conn->xmpp, (pact_XMPPConnectionServerData*)serverdata);
	}
#endif
	else {
		return -1;
	}
}

int pact_connection_think(pact_Connection* conn) {
	if (conn->proto == PACT_CONNECTIONPROTOCOL_REF) {
		return _pact_refconnection_think(conn->ref);
	}
#ifdef PACT_SUPPORTEDCONN_IRC
	else if (conn->proto == PACT_CONNECTIONPROTOCOL_IRC) {
		return _pact_ircconnection_think(conn->irc);
	}
#endif
#ifdef PACT_SUPPORTEDCONN_XMPP
	else if (conn->proto == PACT_CONNECTIONPROTOCOL_XMPP) {
		return _pact_xmppconnection_think(conn->xmpp);
	}
#endif
	else {
		return -1;
	}
}

void pact_connection_q_send(pact_Connection* conn, pact_ConnectionMessage* message) {
	pact_ConnectionMessage* tmp = malloc(sizeof(pact_ConnectionMessage));
	tmp->type = pact_string_new();
	tmp->data = pact_string_new();
	pact_string_clone(message->type, tmp->type);
	pact_string_clone(message->data, tmp->data);
	pact_linkedlist_pushback(conn->in_q, tmp);
}

pact_ConnectionMessage* pact_connection_q_recv(pact_Connection* conn) {
	if (pact_linkedlist_length(conn->out_q) == 0) {
		return 0;
	}

	pact_ConnectionMessage* message = (pact_ConnectionMessage*)pact_linkedlist_popfront(conn->out_q);

	return message;
}
