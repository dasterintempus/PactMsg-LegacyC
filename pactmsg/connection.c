#include "connection.h"
#include "debug.h"
#include "connection_structs.h"
#include "data.h"
#include "data_structs.h"

void _pact_ircevent_on_connect(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count)
{
	pact_debug_write("Connected");
}

pact_connection_t* pact_connection_create(pact_connection_proto_t proto)
{
	pact_connection_t* conn = malloc(sizeof(pact_connection_t));

	if (!conn)
		return 0;

	memset(conn, 0, sizeof(pact_connection_t));

	conn->proto = proto;
	if (conn->proto == PACT_CONNECTION_TELNET)
	{
		conn->telnet = _pact_telnetconnection_create(conn);
		if (!conn->telnet)
			return 0;
	}
#ifdef PACT_SUPPORTEDCONN_IRC
	else if (conn->proto == PACT_CONNECTION_IRC)
	{
		conn->irc = _pact_ircconnection_create(conn);
		if (!conn->irc)
			return 0;
	}
#endif
#ifdef PACT_SUPPORTEDCONN_XMPP
	else if (conn->proto == PACT_CONNECTION_XMPP)
	{
		conn->xmpp = _pact_xmppclient_create(conn);
		if (!conn->xmpp)
			return 0;
	}
#endif

	conn->in_q = pact_linkedlist_create();
	conn->out_q = pact_linkedlist_create();

	return conn;
}

pact_connection_t* pact_connection_create_child(pact_connection_proto_t proto, pact_client_t* parent)
{
	pact_connection_t* conn = pact_connection_create(proto);
	if (!conn)
		return 0;
	conn->parent = parent;
	return conn;
}

void pact_connection_destroy(pact_connection_t* conn)
{
	if (conn->telnet)
		_pact_telnetconnection_destroy(conn->telnet);
#ifdef PACT_SUPPORTEDCONN_IRC
	if (conn->irc)
		_pact_ircconnection_destroy(conn->irc);
#endif
	free(conn->in_q);
	free(conn->out_q);
	free(conn);
}

int pact_connection_start(pact_connection_t* conn, void* serverdata)
{
	if (conn->proto == PACT_CONNECTION_TELNET)
	{
		return _pact_telnetconnection_start(conn->telnet, (pact_telnetserver_data_t*)serverdata);
	}
#ifdef PACT_SUPPORTEDCONN_IRC
	else if (conn->proto == PACT_CONNECTION_IRC)
	{
		return _pact_ircconnection_start(conn->irc, (pact_ircserver_data_t*)serverdata);
	}
#endif
#ifdef PACT_SUPPORTEDCONN_XMPP
	else if (conn->proto == PACT_CONNECTION_XMPP)
	{
		return _pact_xmppconnection_start(conn->xmpp, (pact_xmppserver_data_t*)serverdata);
	}
#endif
	else
	{
		return -1;
	}
}

int pact_connection_think(pact_connection_t* conn)
{
	return _pact_ircconnection_think(conn->irc);
}

void pact_connection_q_send(pact_connection_t* conn, char* message)
{
	size_t length = strlen(message);
	char* buf = malloc(length);
	strncopy(buf, message, length);
	pact_linkedlist_pushback(conn->in_q, buf);
}

char* pact_connection_q_recv(pact_connection_t* conn)
{
	if (pact_linkedlist_length(conn->out_q) == 0)
		return 0;

	char* buf;
	if (pact_linkedlist_popfront(conn->out_q, buf))
		return 0;
	size_t length = strlen(buf);
	char* message = malloc(length);
	strncopy(message, buf, length);
	free(buf);

	return message;
}

pact_telnetconnection_t* _pact_telnetconnection_create(pact_connection_t* parent)
{
	if (!parent)
		return 0;

	pact_telnetconnection_t* telnet = malloc(sizeof(pact_telnetconnection_t));
	if (!telnet)
		return 0;
	memset(telnet, 0, sizeof(pact_telnetconnection_t));

	telnet->parent = parent;

	return telnet;
}

void _pact_telnetconnection_destroy(pact_telnetconnection_t* telnet)
{
	if (telnet->serverdata)
		free(telnet->serverdata);
	free(telnet);
}

int _pact_telnetconnection_start(pact_telnetconnection_t* telnet, pact_telnetserver_data_t* serverdata)
{
	return 0;
}

int _pact_telnetconnection_think(pact_telnetconnection_t* telnet)
{
	return 0;
}

pact_ircconnection_t* _pact_ircconnection_create(pact_connection_t* parent)
{
	if (!parent)
		return 0;

	pact_ircconnection_t* irc = malloc(sizeof(pact_ircconnection_t));
	if (!irc)
		return 0; 
	memset(irc, 0, sizeof(pact_ircconnection_t));

	irc->parent = parent;

	irc->callbacks = malloc(sizeof(irc_callbacks_t));
	if (!irc->callbacks)
	{
		free(irc);
		return 0;
	}
	memset(irc->callbacks, 0, sizeof(irc_callbacks_t));

	//add event handlers
	irc->callbacks->event_connect = &_pact_ircevent_on_connect;

	irc->session = irc_create_session(irc->callbacks);

	if (!irc->session)
	{
		pact_debug_write("Couldn't create internal IRC Session");
		free(irc->callbacks);
		free(irc);
		return 0;
	}

	irc_set_ctx(irc->session, irc);

	return irc;
}

void _pact_ircconnection_destroy(pact_ircconnection_t* irc)
{
	if (irc->session)
		irc_destroy_session(irc->session);
	if (irc->serverdata)
		free(irc->serverdata);
	free(irc);
}

int _pact_ircconnection_start(pact_ircconnection_t* irc, pact_ircserver_data_t* serverdata)
{
	irc->serverdata = serverdata;

	if (irc_connect(irc->session, irc->serverdata->hostname, irc->serverdata->port, irc->serverdata->pass, irc->serverdata->nick, irc->serverdata->username, irc->serverdata->realname))
	{
		pact_debug_write("Couldn't connect to IRC");
		pact_debug_print("(%s %u %s %s %s %s)\n", irc->serverdata->hostname, irc->serverdata->port, irc->serverdata->pass, irc->serverdata->nick, irc->serverdata->username, irc->serverdata->realname);
		pact_debug_write(irc_strerror(irc_errno(irc->session)));
		return 1;
	}

	return 0;
}

int _pact_ircconnection_think(pact_ircconnection_t* irc)
{
	if (!irc_is_connected(irc->session))
	{
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

	if (select(maxfd + 1, &in_set, &out_set, 0, &tv) < 0)
	{
		pact_debug_write("Error from select()");
		return 2;
	}

	if (irc_process_select_descriptors(irc->session, &in_set, &out_set))
	{
		pact_debug_write("Error from processing select descriptors in IRC");
		pact_debug_write(irc_strerror(irc_errno(irc->session)));
		return 3;
	}

	return 0;
}
