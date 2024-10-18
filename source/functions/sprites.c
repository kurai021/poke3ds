#include <stdio.h>
#include "sprites.h"

bool load_pokemon_sprite(PokemonSprite *sprite, int pokemon_id)
{
	int sheet_index = pokemon_id / POKEMON_PER_SHEET;
	int sprite_index = pokemon_id % POKEMON_PER_SHEET;

	if (sheet_index >= SHEET_COUNT)
	{
		printf("Error: Índice de spritesheet inválido para el ID de Pokémon %d\n", pokemon_id);
		return false;
	}

	char path[256];
	snprintf(path, sizeof(path), "romfs:/gfx/sprites%d.t3x", sheet_index);

	sprite->spriteSheet = C2D_SpriteSheetLoad(path);
	if (!sprite->spriteSheet)
	{
		printf("Error al cargar el spritesheet desde: %s\n", path);
		return false;
	}

	sprite->image = C2D_SpriteSheetGetImage(sprite->spriteSheet, sprite_index);
	if (!sprite->image.tex)
	{
		printf("Error: No se pudo obtener la imagen del sprite (ID: %d, Índice: %d).\n", pokemon_id, sprite_index);
		C2D_SpriteSheetFree(sprite->spriteSheet);
		return false;
	}

	return true;
}