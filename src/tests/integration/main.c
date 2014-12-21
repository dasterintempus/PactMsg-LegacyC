#include "../../connection.h"
#include "../../ircconnection.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	pact_Connection* pc = pact_connection_new(PACT_CONNECTIONPROTOCOL_IRC);
	pact_IRCConnectionServerData* serverdata = malloc(sizeof(pact_IRCConnectionServerData));
	memset(serverdata, 0, sizeof(pact_IRCConnectionServerData));
	serverdata->host = pact_string_new();
	serverdata->port = pact_string_new();
	serverdata->pass = pact_string_new();
	serverdata->nick = pact_string_new();
	serverdata->username = pact_string_new();
	serverdata->realname = pact_string_new();
	pact_string_assign(serverdata->host, "irc.dasterin.net");
	pact_string_assign(serverdata->port, "6667");
	pact_string_assign(serverdata->pass, "maxcom01");
	pact_string_assign(serverdata->nick, "bgak");
	pact_string_assign(serverdata->username,"bgak");
	pact_string_assign(serverdata->realname, "bgak");
	int res = pact_connection_start(pc, serverdata);
	if (res) {
		return res;
	}
	res = 0;
	while (!res) {
		res = pact_connection_think(pc);
	}
	pact_connection_free(pc);
	if (res) {
		return res;
	}
	return 0;
}
