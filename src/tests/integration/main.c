#include "../../connection.h"
#include "../../ircconnection.h"
#include "../../refconnection.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	pact_ConnectionProtocol proto;
	pact_IRCConnectionServerData* ircdata = 0;
	pact_RefConnectionServerData* refdata = 0;
	pact_XMPPConnectionServerData* xmppdata = 0;
	if (argc < 2) {
		return 2;
	}
	if (strcmp(argv[1], "irc") == 0) {
		proto = PACT_CONNECTIONPROTOCOL_IRC;
		ircdata = malloc(sizeof(pact_IRCConnectionServerData));
		if (argc < 8) {
			return 2;
		}
		ircdata->host = pact_string_new();
		ircdata->port = pact_string_new();
		ircdata->pass = pact_string_new();
		ircdata->nick = pact_string_new();
		ircdata->username = pact_string_new();
		ircdata->realname = pact_string_new();
		pact_string_assign(ircdata->host, argv[2]);
		pact_string_assign(ircdata->port, argv[3]);
		pact_string_assign(ircdata->pass, argv[4]);
		pact_string_assign(ircdata->nick, argv[5]);
		pact_string_assign(ircdata->username, argv[6]);
		pact_string_assign(ircdata->realname, argv[7]);
	}
	else if (strcmp(argv[1], "xmpp") == 0) {
		proto = PACT_CONNECTIONPROTOCOL_XMPP;
		xmppdata = malloc(sizeof(pact_XMPPConnectionServerData));
		if (argc < 8) {
			return 2;
		}
		xmppdata->host = pact_string_new();
		xmppdata->port = pact_string_new();
		xmppdata->pass = pact_string_new();
		xmppdata->username = pact_string_new();
		xmppdata->domain = pact_string_new();
		xmppdata->resource = pact_string_new();
		pact_string_assign(xmppdata->host, argv[2]);
		pact_string_assign(xmppdata->port, argv[3]);
		pact_string_assign(xmppdata->pass, argv[4]);
		pact_string_assign(xmppdata->username, argv[5]);
		pact_string_assign(xmppdata->domain, argv[6]);
		pact_string_assign(xmppdata->resource, argv[7]);
	}
	else if (strcmp(argv[1], "ref") == 0) {
		proto = PACT_CONNECTIONPROTOCOL_REF;
		refdata = malloc(sizeof(pact_RefConnectionServerData));
		if (argc < 5) {
			return 2;
		}
		refdata->host = pact_string_new();
		refdata->port = pact_string_new();
		refdata->id = pact_string_new();
		pact_string_assign(refdata->host, argv[2]);
		pact_string_assign(refdata->port, argv[3]);
		pact_string_assign(refdata->id, argv[4]);
	}
	else {
		return 2;
	}
	
	pact_Connection* pc = pact_connection_new(proto);
	int res;
	if (proto == PACT_CONNECTIONPROTOCOL_IRC) {
		res = pact_connection_start(pc, ircdata);
	}
	else if (proto == PACT_CONNECTIONPROTOCOL_XMPP) {
		res = pact_connection_start(pc, xmppdata);
	}
	else if (proto == PACT_CONNECTIONPROTOCOL_REF) {
		res = pact_connection_start(pc, refdata);
	}
	else {
		return 2;
	}
	if (res) {
		return -res;
	}
	res = 0;
	while (!res) {
		res = pact_connection_think(pc);
	}
	pact_connection_free(pc);
	if (res) {
		return -res;
	}
	if (ircdata) {
		free(ircdata);
	}
	if (xmppdata) {
		free(xmppdata);
	}
	if (refdata) {
		free(refdata);
	}
	return 0;
}
