#ifndef PACT_CONNECTION_STRUCTS_H
#define PACT_CONNECTION_STRUCTS_H

#include "config.h"

#include "data_typedefs.h"
#include "data_structs.h"

//connection.h
#ifdef PACT_SUPPORTEDCONN_IRC
struct pact_ircconnection_s
{
	irc_callbacks_t* callbacks;
	irc_session_t* session;
	pact_ircserver_data_t* serverdata;
	pact_connection_t* parent;
};
#endif

struct pact_telnetconnection_s
{
	pact_telnetserver_data_t* serverdata;
	pact_connection_t* parent;
};

struct pact_connection_s
{
	//protocol specifier
	pact_connection_proto_t proto;

	//protocol-specific-structure
	pact_telnetconnection_t* telnet;
#ifdef PACT_SUPPORTEDCONN_IRC
	pact_ircconnection_t* irc;
#endif
#ifdef PACT_SUPPORTEDCONN_XMPP
	pact_xmppconnection_t* xmpp;
#endif
	
	//parent link
	pact_client_t* parent;

	//message queues
	pact_linkedlist_t* out_q;
	pact_linkedlist_t* in_q;
};

//client.h
/*
struct pact_server_data_s
{
	char* hostname;
	unsigned short int port;
	char* pass;
	pact_channel_data_t* channels;
	unsigned short int channelcount;
};
*/
/*
struct pact_client_s
{
	pact_server_data_t* serverdata;
	pact_connection_t* conn;
	void* userdata;
};
*/
#endif