#ifndef NETWORK_H
#define NETWORK_H

#include <3ds.h>
#include <jansson.h>

void parse_pokemon_data(PrintConsole *topScreen, PrintConsole *bottomScreen, const char *query, const char *request);
// void make_request(PrintConsole *topScreen, PrintConsole *bottomScreen, const char *request_url, const char *request);

#endif
