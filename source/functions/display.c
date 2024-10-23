#include <stdio.h>
#include <string.h>

#include "display.h"
#include "sprites.h"
#include "network.h"

void print_pokemon_info(const char *name, int height, int weight, const char *description)
{
	float converted_height = height / 10.0f;
	float converted_weight = weight / 10.0f;

	printf("\n\nPokemon Info:\n\n");
	printf("Name: %s\n\n", name);
	printf("Height: %.1f m\n\n", converted_height);
	printf("Weight: %.1f kg\n\n", converted_weight);
	printf("Description: %s\n\n", description);
}

void print_moves_page(const json_t *moves, size_t start, size_t end)
{
	// Print header of the table
	printf("%-10s | %-2s | %-10s | %-28s\n", "Move", "Nv", "Method", "Game");
	printf("--------------------------------------------------\n");

	// Iterate and display only the rows of the current page
	for (size_t i = start; i < end && i < json_array_size(moves); i++)
	{
		json_t *move_entry = json_array_get(moves, i);
		json_t *move = json_object_get(move_entry, "move");
		json_t *move_name = json_object_get(move, "name");

		if (!json_is_string(move_name))
		{
			continue;
		}

		json_t *version_details = json_object_get(move_entry, "version_group_details");
		size_t detail_index;
		json_t *detail;
		char *version_name = NULL;

		json_array_foreach(version_details, detail_index, detail)
		{
			json_t *level = json_object_get(detail, "level_learned_at");
			int level_learned = json_integer_value(level);

			json_t *method = json_object_get(detail, "move_learn_method");
			json_t *method_name = json_object_get(method, "name");

			json_t *version = json_object_get(detail, "version_group");
			const char *version_string = json_string_value(json_object_get(version, "name"));

			// Compare and assign readable names for the game list
			if (strcmp(version_string, "lets-go-pikachu-lets-go-eevee") == 0)
			{
				version_name = "Let's Go";
			}
			else if (strcmp(version_string, "red-blue") == 0)
			{
				version_name = "Red/Blue";
			}
			else if (strcmp(version_string, "yellow") == 0)
			{
				version_name = "Yellow";
			}
			else if (strcmp(version_string, "sword-shield") == 0)
			{
				version_name = "Sword/Shield";
			}
			else if (strcmp(version_string, "gold-silver") == 0)
			{
				version_name = "Gold/Silver";
			}
			else if (strcmp(version_string, "crystal") == 0)
			{
				version_name = "Crystal";
			}
			else if (strcmp(version_string, "ruby-sapphire") == 0)
			{
				version_name = "Ruby/Sapphire";
			}
			else if (strcmp(version_string, "emerald") == 0)
			{
				version_name = "Emerald";
			}
			else if (strcmp(version_string, "platinum") == 0)
			{
				version_name = "Platinum";
			}
			else if (strcmp(version_string, "heartgold-soulsilver") == 0)
			{
				version_name = "H-Gold/S-Silver";
			}
			else if (strcmp(version_string, "black-white") == 0)
			{
				version_name = "Black/White";
			}
			else if (strcmp(version_string, "black-2-white-2") == 0)
			{
				version_name = "Black/White 2";
			}
			else if (strcmp(version_string, "omega-ruby-alpha-sapphire") == 0)
			{
				version_name = "ORAS";
			}
			else if (strcmp(version_string, "sun-moon") == 0)
			{
				version_name = "Sun/Moon";
			}
			else if (strcmp(version_string, "ultra-sun-ultra-moon") == 0)
			{
				version_name = "Ultrasun/Ultramoon";
			}
			else if (strcmp(version_string, "scarlet-violet") == 0)
			{
				version_name = "Scarlet/Violet";
			}
			else if (strcmp(version_string, "xd") == 0)
			{
				version_name = "XD";
			}
			else if (strcmp(version_string, "firered-leafgreen") == 0)
			{
				version_name = "FireRed/LeafGreen";
			}
			else if (strcmp(version_string, "colosseum") == 0)
			{
				version_name = "Colosseum";
			}
			else if (strcmp(version_string, "brilliant-diamond-and-shining-pearl") == 0)
			{
				version_name = "B-Diamond/S-Pearl";
			}
			else if (strcmp(version_string, "diamond-pearl") == 0)
			{
				version_name = "Diamond/Pearl";
			}
			else if (strcmp(version_string, "x-y") == 0)
			{
				version_name = "X/Y";
			}
			else
			{
				version_name = (char *)version_string;
			}

			// Print the information if it is valid
			if (json_is_string(method_name) && version_name != NULL)
			{
				printf("%-10s | %-2d | %-10s | %-28s\n",
					   json_string_value(move_name),
					   level_learned,
					   json_string_value(method_name),
					   version_name);
			}
		}
	}
}

void show_moves_with_pagination(const json_t *moves, const char *name, int height, int weight, int pokemon_id, const char *description, PrintConsole *topScreen, PrintConsole *bottomScreen)
{

	size_t total_moves = json_array_size(moves);
	size_t total_pages = (total_moves + ROWS_PER_PAGE - 1) / ROWS_PER_PAGE;
	size_t page = 0;

	while (true)
	{
		consoleSelect(topScreen);
		consoleClear();

		if (page == 0)
		{
			// Render the Pokémon sprite
			load_pokemon_sprite(pokemon_id);
			print_pokemon_info(name, height, weight, description);
		}
		else
		{
			size_t start = (page - 1) * ROWS_PER_PAGE;
			size_t end = start + ROWS_PER_PAGE;
			if (end > total_moves)
				end = total_moves; // Make sure not to exceed the limit
			print_moves_page(moves, start, end);
		}

		consoleSelect(bottomScreen);
		consoleClear();
		printf("Press L for previous page, R for next page, A for search a new Pokemon, START to exit.\n");
		printf("Page %zu / %zu\n", page + 1, total_pages);

		// Handling user input again
		while (aptMainLoop())
		{
			hidScanInput();
			u32 kDown = hidKeysDown();

			if (kDown & KEY_START)
			{
				free_texture();
				return;
			}

			if (kDown & KEY_A)
			{
				// Configure and display the virtual keyboard
				SwkbdState swkbd;
				char pokemon_name[MAX_POKEMON_NAME_LEN] = {0};
				swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
				swkbdSetHintText(&swkbd, "Enter the name of the Pokémon");

				// Obtener el nombre ingresado
				if (swkbdInputText(&swkbd, pokemon_name, sizeof(pokemon_name)) == SWKBD_BUTTON_RIGHT)
				{
					printf("Searching data for %s...\n", pokemon_name);
					parse_pokemon_data(&*topScreen, &*bottomScreen, "https://pokeapi.co/api/v2/pokemon", pokemon_name);
				}
				else
				{
					printf("Entry cancelled.\n");
				}
			}

			if (kDown & KEY_R)
			{
				if (page < total_pages - 1)
				{
					page++;
				}
				break;
			}

			if (kDown & KEY_L)
			{
				if (page > 0)
				{
					page--;
				}
				break;
			}

			gfxFlushBuffers();
			gfxSwapBuffers();
			gspWaitForVBlank();
		}
	}

	// Free resources
	free_texture();
}