#ifndef DISPLAY_H
#define DISPLAY_H

#include <3ds.h>
#include <jansson.h>
#include "sprites.h"

#define ROWS_PER_PAGE 1

void print_pokemon_info(const char *name, int height, int weight, const PokemonSprite *sprite);
void print_moves_page(const json_t *moves, size_t start, size_t end);
void show_moves_with_pagination(const json_t *moves, const char *name, int height, int weight, int pokemon_id, PrintConsole *topScreen, PrintConsole *bottomScreen);

#endif