#include "ircconnection.h"
#include "debug.h"
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

void _pact_ircevent_on_connect(irc_session_t* session, const char* event, const char* origin, const char** params, unsigned int count) {
	pact_debug_write("Connected");
}

pact_IRCConnection* _pact_ircconnection_new(pact_Connection* parent) {
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

void _pact_ircconnection_free(pact_IRCConnection* irc) {
	if (irc->session) {
		irc_destroy_session(irc->session);
	}
	if (irc->serverdata) {
		if (irc->serverdata->host) {
			pact_string_free(irc->serverdata->host);
		}
		if (irc->serverdata->port) {
			pact_string_free(irc->serverdata->port);
		}
		if (irc->serverdata->pass) {
			pact_string_free(irc->serverdata->pass);
		}
		if (irc->serverdata->nick) {
			pact_string_free(irc->serverdata->nick);
		}
		if (irc->serverdata->username) {
			pact_string_free(irc->serverdata->username);
		}
		if (irc->serverdata->realname) {
			pact_string_free(irc->serverdata->realname);
		}
		free(irc->serverdata);
	}
	free(irc);
}

int _pact_ircconnection_start(pact_IRCConnection* irc, pact_IRCConnectionServerData* serverdata) {
	irc->serverdata = serverdata;
	unsigned int portnum = strtol(pact_string_get_cstr(irc->serverdata->port), 0, 10);

	if (irc_connect(irc->session, pact_string_get_cstr(irc->serverdata->host), portnum, pact_string_get_cstr(irc->serverdata->pass), pact_string_get_cstr(irc->serverdata->nick), pact_string_get_cstr(irc->serverdata->username), pact_string_get_cstr(irc->serverdata->realname))) {
		pact_debug_write("Couldn't connect to IRC");
		pact_debug_print("(%s %u %s %s %s %s)\n", pact_string_get_cstr(irc->serverdata->host), pact_string_get_cstr(irc->serverdata->port), pact_string_get_cstr(irc->serverdata->pass), pact_string_get_cstr(irc->serverdata->nick), pact_string_get_cstr(irc->serverdata->username), pact_string_get_cstr(irc->serverdata->realname));
		pact_debug_write(irc_strerror(irc_errno(irc->session)));
		return 1;
	}

	return 0;
}

int _pact_ircconnection_think(pact_IRCConnection* irc) {
	if (!irc_is_connected(irc->session)) {
		pact_debug_write("Lost connection to IRC");
		pact_debug_print("%s\n", pact_string_get_cstr(irc->serverdata->host));
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
