#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <stdlib.h>
#include "sprites.h"

// Simple sprite struct
typedef struct
{
	C2D_Sprite spr;
} Sprite;

static C2D_SpriteSheet spriteSheet;
static Sprite sprites[POKEMON_PER_SHEET];

int load_pokemon_sprite(int pokemon_id)
{
	int sheet_index = pokemon_id / POKEMON_PER_SHEET;
	size_t sprite_index;

	if (sheet_index == 0)
	{
		sprite_index = pokemon_id;
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

	// Comprobar si el índice está dentro del rango válido
	if (sprite_index >= POKEMON_PER_SHEET || sheet_index >= SHEET_COUNT)
	{
		printf("Error: Índice de sprite inválido para el ID de Pokémon %d\n", pokemon_id);
		return 1;
	}

	char path[256];
	snprintf(path, sizeof(path), "romfs:/gfx/sprites%d.t3x", sheet_index);

	// Liberar el spriteSheet anterior si ya está cargado
	if (spriteSheet)
	{
		free_texture();
	}

	spriteSheet = C2D_SpriteSheetLoad(path);

	if (!spriteSheet)
	{
		printf("Error al cargar el spritesheet %s\n", path);
		return 1;
	}

	// Inicializar sprites

	Sprite *sprite = &sprites[sheet_index];

	C2D_SpriteFromSheet(&sprite->spr, spriteSheet, sprite_index);
	C2D_SpriteSetPos(&sprite->spr, 170, 130);
	C2D_SpriteScale(&sprite->spr, 1.5, 1.5);

	// Create screen
	C3D_RenderTarget *top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));

	C2D_SceneBegin(top);
	C2D_DrawSprite(&sprites[sheet_index].spr);
	C3D_FrameEnd(0);

	return 0;
}

void free_texture(void)
{
	C2D_SpriteSheetFree(spriteSheet);
}