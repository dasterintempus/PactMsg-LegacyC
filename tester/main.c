#include "connection.h"

int main(int argc, char** argv)
{
	pact_connection_t* pc = pact_connection_create(PACT_CONNECTION_IRC);
	pact_ircserver_data_t* serverdata = malloc(sizeof(pact_ircserver_data_t));
	memset(serverdata, 0, sizeof(pact_ircserver_data_t));
	serverdata->hostname = "wisp.lab.home.dasterin.net";
	serverdata->port = 6667;
	serverdata->pass = "maxcom01";
	serverdata->nick = "bgak";
	serverdata->username = "bgak";
	serverdata->realname = "bgak";
	int res = pact_connection_start(pc, serverdata);
	if (res)
	{
		return res;
	}
	res = 0;
	while (!res)
	{
		res = pact_connection_think(pc);
	}
	pact_connection_destroy(pc);
	if (res)
	{
		return res;
	}
	return 0;
}
