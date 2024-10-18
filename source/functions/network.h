#ifndef NETWORK_H
#define NETWORK_H

#include <3ds.h>
#include <jansson.h>

void parse_pokemon_data(const char *json_data, PrintConsole *topScreen, PrintConsole *bottomScreen);
void make_request(PrintConsole *topScreen, PrintConsole *bottomScreen, const char *pokemon_name);

#endif
