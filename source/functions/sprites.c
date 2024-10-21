#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include "sprites.h"

static C2D_SpriteSheet spriteSheet;

bool load_pokemon_sprite(int pokemon_id)
{
	int sheet_index = pokemon_id / POKEMON_PER_SHEET;
	size_t sprite_index = 0;

	if (sheet_index == 0)
	{
		sprite_index = pokemon_id + 1;
	}
	else if (sheet_index == 1)
	{
		sprite_index = pokemon_id - 200;
	}
	else if (sheet_index == 2)
	{
		sprite_index = pokemon_id - 400;
	}
	else if (sheet_index == 3)
	{
		sprite_index = pokemon_id - 600;
	}
	else if (sheet_index == 4)
	{
		sprite_index = pokemon_id - 800;
	}
	else
	{
		sprite_index = pokemon_id - 1000;
	}

	if (sheet_index >= SHEET_COUNT)
	{
		printf("Error: Índice de spritesheet inválido para el ID de Pokémon %d\n", pokemon_id);
		return false;
	}

	char *path = NULL;

	// Asignamos suficiente memoria para almacenar el resultado de la cadena formateada
	path = malloc(256 * sizeof(char)); // Asegúrate de que tienes suficiente espacio para la cadena

	if (path == NULL)
	{
		// Verificamos que la asignación de memoria haya sido exitosa
		fprintf(stderr, "Error al asignar memoria\n");
		return 1;
	}

	snprintf(path, 256, "romfs:/gfx/sprites%d.t3x", sheet_index);

	// printf("Sprite index: %lu\n", sprite_index);

	C2D_CreateScreenTarget(GFX_TOP, GFX_RIGHT);

	spriteSheet = C2D_SpriteSheetLoad(path);

	if (!spriteSheet)
	{
		svcBreak(USERBREAK_PANIC);
	}

	return C2D_DrawImageAt(C2D_SpriteSheetGetImage(spriteSheet, sprite_index), 200.0f, 50.0f, 0.0f, NULL, 1.0f, 1.0f);
}

void free_texture(void)
{
	C2D_SpriteSheetFree(spriteSheet);
}