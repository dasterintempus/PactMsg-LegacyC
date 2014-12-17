#include "../../connection.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	pact_Connection* pc = pact_connection_create(PACT_CONNECTIONPROTOCOL_IRC);
	pact_IRCConnectionServerData* serverdata = malloc(sizeof(pact_IRCConnectionServerData));
	memset(serverdata, 0, sizeof(pact_IRCConnectionServerData));
	serverdata->hostname = "wisp.lab.home.dasterin.net";
	serverdata->port = 6667;
	serverdata->pass = "maxcom01";
	serverdata->nick = "bgak";
	serverdata->username = "bgak";
	serverdata->realname = "bgak";
	int res = pact_connection_start(pc, serverdata);
	if (res) {
		return res;
	}
	res = 0;
	while (!res) {
		res = pact_connection_think(pc);
	}
	pact_connection_destroy(pc);
	if (res) {
		return res;
	}
	return 0;
}
