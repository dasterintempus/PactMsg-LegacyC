#include "connection.h"
#include "debug.h"
#include "linkedlist.h"
#include <stdlib.h>
#include <string.h>

#ifdef PACT_SUPPORTEDCONN_IRC
struct pact_IRCConnection {
	irc_callbacks_t* callbacks;
	irc_session_t* session;
	pact_IRCConnectionServerData* serverdata;
	pact_Connection* parent;
};
#endif

#ifdef PACT_SUPPORTEDCONN_XMPP
struct pact_XMPPConnection {
	pact_XMPPConnectionServerData* serverdata;
	pact_Connection* parent;
};
#endif

struct pact_TelnetConnection {
	pact_TelnetConnectionServerData* serverdata;
	pact_Connection* parent;
};

struct pact_Connection {
	//protocol specifier
	pact_ConnectionProtocol proto;

	//protocol-specific-structure
	pact_TelnetConnection* telnet;
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

void _pact_ircevent_on_connect(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count) {
	pact_debug_write("Connected");
}

pact_Connection* pact_connection_create(pact_ConnectionProtocol proto) {
	pact_Connection* conn = malloc(sizeof(pact_Connection));

	if (!conn) {
		return 0;
	}

	memset(conn, 0, sizeof(pact_Connection));

	conn->proto = proto;
	if (conn->proto == PACT_CONNECTIONPROTOCOL_TELNET) {
		conn->telnet = _pact_telnetconnection_create(conn);
		if (!conn->telnet) {
			return 0;
		}
	}
#ifdef PACT_SUPPORTEDCONN_IRC
	else if (conn->proto == PACT_CONNECTIONPROTOCOL_IRC) {
		conn->irc = _pact_ircconnection_create(conn);
		if (!conn->irc) {
			return 0;
		}
	}
#endif
#ifdef PACT_SUPPORTEDCONN_XMPP
	else if (conn->proto == PACT_CONNECTIONPROTOCOL_XMPP) {
		conn->xmpp = _pact_xmppclient_create(conn);
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

void pact_connection_destroy(pact_Connection* conn) {
	if (conn->telnet) {
		_pact_telnetconnection_destroy(conn->telnet);
	}
#ifdef PACT_SUPPORTEDCONN_IRC
	if (conn->irc) {
		_pact_ircconnection_destroy(conn->irc);
	}
#endif
#ifdef PACT_SUPPORTEDCONN_XMPP
	if (conn->xmpp) {
		pact_xmppconnection_destory(conn->xmpp);
	}
#endif
	free(conn->in_q);
	free(conn->out_q);
	free(conn);
}

int pact_connection_start(pact_Connection* conn, void* serverdata) {
	if (conn->proto == PACT_CONNECTIONPROTOCOL_TELNET) {
		return _pact_telnetconnection_start(conn->telnet, (pact_TelnetConnectionServerData*)serverdata);
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
	return _pact_ircconnection_think(conn->irc);
}

void pact_connection_q_send(pact_Connection* conn, char* message) {
	size_t length = strlen(message);
	char* buf = malloc(length+1);
	strncpy(buf, message, length+1);
	pact_linkedlist_pushback(conn->in_q, buf);
}

char* pact_connection_q_recv(pact_Connection* conn) {
	if (pact_linkedlist_length(conn->out_q) == 0) {
		return 0;
	}

	char* buf = (char *) pact_linkedlist_popfront(conn->out_q);
	if (buf) {
		return 0;
	}
	size_t length = strlen(buf);
	char* message = malloc(length+1);
	strncpy(message, buf, length+1);
	free(buf);

	return message;
}

pact_TelnetConnection* _pact_telnetconnection_create(pact_Connection* parent) {
	if (!parent) {
		return 0;
	}

	pact_TelnetConnection* telnet = malloc(sizeof(pact_TelnetConnection));
	if (!telnet) {
		return 0;
	}
	memset(telnet, 0, sizeof(pact_TelnetConnection));

	telnet->parent = parent;

	return telnet;
}

void _pact_telnetconnection_destroy(pact_TelnetConnection* telnet) {
	if (telnet->serverdata) {
		free(telnet->serverdata);
	}
	free(telnet);
}

int _pact_telnetconnection_start(pact_TelnetConnection* telnet, pact_TelnetConnectionServerData* serverdata) {
	return 0;
}

int _pact_telnetconnection_think(pact_TelnetConnection* telnet) {
	return 0;
}

pact_IRCConnection* _pact_ircconnection_create(pact_Connection* parent) {
	if (!parent) {
		return 0;
	}

	pact_IRCConnection* irc = malloc(sizeof(pact_IRCConnection));
	if (!irc) {
		return 0;
	}
	memset(irc, 0, sizeof(pact_IRCConnection));

	irc->parent = parent;

	irc->callbacks = malloc(sizeof(irc_callbacks_t));
	if (!irc->callbacks) {
		free(irc);
		return 0;
	}
	memset(irc->callbacks, 0, sizeof(irc_callbacks_t));

	//add event handlers
	irc->callbacks->event_connect = &_pact_ircevent_on_connect;

	irc->session = irc_create_session(irc->callbacks);

	if (!irc->session) {
		pact_debug_write("Couldn't create internal IRC Session");
		free(irc->callbacks);
		free(irc);
		return 0;
	}

	irc_set_ctx(irc->session, irc);

	return irc;
}

void _pact_ircconnection_destroy(pact_IRCConnection* irc) {
	if (irc->session) {
		irc_destroy_session(irc->session);
	}
	if (irc->serverdata) {
		free(irc->serverdata);
	}
	free(irc);
}

int _pact_ircconnection_start(pact_IRCConnection* irc, pact_IRCConnectionServerData* serverdata) {
	irc->serverdata = serverdata;

	if (irc_connect(irc->session, irc->serverdata->hostname, irc->serverdata->port, irc->serverdata->pass, irc->serverdata->nick, irc->serverdata->username, irc->serverdata->realname)) {
		pact_debug_write("Couldn't connect to IRC");
		pact_debug_print("(%s %u %s %s %s %s)\n", irc->serverdata->hostname, irc->serverdata->port, irc->serverdata->pass, irc->serverdata->nick, irc->serverdata->username, irc->serverdata->realname);
		pact_debug_write(irc_strerror(irc_errno(irc->session)));
		return 1;
	}

	return 0;
}

int _pact_ircconnection_think(pact_IRCConnection* irc) {
	if (!irc_is_connected(irc->session)) {
		pact_debug_write("Lost connection to IRC");
		pact_debug_print("%s\n", irc->serverdata->hostname);
		return 1;
	}

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 250000;

	fd_set in_set, out_set;
	int maxfd = 0;
	FD_ZERO(&in_set);
	FD_ZERO(&out_set);

	irc_add_select_descriptors(irc->session, &in_set, &out_set, &maxfd);

	if (select(maxfd + 1, &in_set, &out_set, 0, &tv) < 0) {
		pact_debug_write("Error from select()");
		return 2;
	}

	if (irc_process_select_descriptors(irc->session, &in_set, &out_set)) {
		pact_debug_write("Error from processing select descriptors in IRC");
		pact_debug_write(irc_strerror(irc_errno(irc->session)));
		return 3;
	}

	return 0;
}
