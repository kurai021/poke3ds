#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include <stdlib.h>
#include "functions/network.h"

#define MAX_POKEMON_NAME_LEN 50

int main()
{
	// Inicializar servicios de la 3DS
	romfsInit();
	gfxInitDefault();
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	PrintConsole topScreen, bottomScreen;

	consoleInit(GFX_TOP, &topScreen);
	consoleInit(GFX_BOTTOM, &bottomScreen);

	printf("Hola, Mundo desde la Nintendo 3DS!\n");
	printf("Presiona A para buscar un Pokémon.\n");
	printf("Presiona START para salir.\n");

	// Bucle principal
	while (aptMainLoop())
	{
		hidScanInput();
		u32 kDown = hidKeysDown();

		if (kDown & KEY_START)
			break; // Salir al presionar START

		if (kDown & KEY_A)
		{
			// Configurar y mostrar el teclado virtual
			SwkbdState swkbd;
			char pokemon_name[MAX_POKEMON_NAME_LEN] = {0};
			swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
			swkbdSetHintText(&swkbd, "Ingresa el nombre del Pokémon");

			// Obtener el nombre ingresado
			if (swkbdInputText(&swkbd, pokemon_name, sizeof(pokemon_name)) == SWKBD_BUTTON_RIGHT)
			{
				printf("Buscando datos de %s...\n", pokemon_name);
				parse_pokemon_data(&topScreen, &bottomScreen, "https://pokeapi.co/api/v2/pokemon", pokemon_name);
			}
			else
			{
				printf("Entrada cancelada.\n");
			}
		}

		gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}

	// Deinit libs
	C2D_Fini();
	gfxExit();
	romfsExit();
	return 0;
}