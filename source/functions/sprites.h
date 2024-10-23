#ifndef SPRITES_H
#define SPRITES_H

#include <citro2d.h>

#define MAX_POKEMON 1026
#define POKEMON_PER_SHEET 200
#define SPRITE_SIZE 64
#define SHEET_COUNT 6

int load_pokemon_sprite(int pokemon_id);
void free_texture(void);

#endif