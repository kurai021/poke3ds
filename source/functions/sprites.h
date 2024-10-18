#ifndef SPRITES_H
#define SPRITES_H

#include <citro2d.h>

#define MAX_POKEMON 1025
#define POKEMON_PER_SHEET 200
#define SPRITE_SIZE 64
#define SHEET_COUNT 6

typedef struct
{
	C2D_Image image;
	C2D_SpriteSheet spriteSheet;
} PokemonSprite;

bool load_pokemon_sprite(PokemonSprite *sprite, int pokemon_id);

#endif