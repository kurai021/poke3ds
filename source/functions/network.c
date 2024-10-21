#include <stdio.h>
#include <string.h>
#include <jansson.h>

#include "network.h"
#include "display.h"

char *make_request(PrintConsole *topScreen, PrintConsole *bottomScreen, const char *request_url, const char *request)
{
	// Inicializar el servicio HTTP
	httpcInit(0);

	// Crear la URL dinámica con el nombre del Pokémon
	char url[128];
	snprintf(url, sizeof(url), "%s/%s/", request_url, request);

	// Inicializar variables para la solicitud
	httpcContext context;
	Result res = httpcOpenContext(&context, HTTPC_METHOD_GET, url, 1);

	if (res != 0)
	{
		printf("Error al abrir el contexto: 0x%x\n", res);
		return 0;
	}

	// Hacer la solicitud
	res = httpcBeginRequest(&context);
	if (res != 0)
	{
		printf("Error al iniciar la solicitud: 0x%x\n", res);
		httpcCloseContext(&context);
		return 0;
	}

	// Recibir la respuesta
	u32 responseCode;
	res = httpcGetResponseStatusCode(&context, &responseCode);

	if (res != 0)
	{
		printf("Error al obtener el estado de la respuesta: 0x%x\n", res);
		httpcCloseContext(&context);
		return 0;
	}

	if (responseCode != 200)
	{
		printf("Código de respuesta HTTP: %d\n", responseCode);
		httpcCloseContext(&context);
		return 0;
	}

	// Manejar la respuesta de manera incremental
	char buffer[4096];
	u32 bytesRead = 0;
	size_t totalSize = 0;
	char *json_data = malloc(1);
	json_data[0] = '\0';

	printf("Descargando los datos...\n");

	while (true)
	{
		res = httpcDownloadData(&context, buffer, sizeof(buffer), &bytesRead);

		if (res == HTTPC_RESULTCODE_DOWNLOADPENDING || (res == 0 && bytesRead > 0))
		{
			buffer[bytesRead] = '\0';
			json_data = realloc(json_data, totalSize + bytesRead + 1);
			if (!json_data)
			{
				printf("Error al reallocar memoria.\n");
				break;
			}
			strcpy(json_data + totalSize, buffer);
			totalSize += bytesRead;

			printf("\nBytes descargados: %zu\n", totalSize);
		}
		else if (res == 0 && bytesRead == 0)
		{
			printf("\nDescarga completa. Total de bytes descargados: %zu\n", totalSize);
			break;
		}
		else
		{
			printf("\nError al descargar los datos: 0x%x\n", res);
			break;
		}
	}

	// Analizar el JSON completo
	// parse_pokemon_data(json_data, topScreen, bottomScreen);
	// Liberar memoria
	// free(json_data);

	// Cerrar el contexto
	httpcCloseContext(&context);
	httpcExit();

	return json_data;
}

void parse_pokemon_data(PrintConsole *topScreen, PrintConsole *bottomScreen, const char *query, const char *request)
{
	const char *pokemon_info = make_request(topScreen, bottomScreen, query, request);
	json_error_t error;
	json_t *root = json_loads(pokemon_info, 0, &error);

	if (!root)
	{
		printf("Error al analizar JSON: %s\n", error.text);
		return;
	}

	json_t *id_json = json_object_get(root, "id");
	int pokemon_id = json_integer_value(id_json);

	json_t *name_json = json_object_get(root, "name");
	const char *name = json_string_value(name_json);

	json_t *height_json = json_object_get(root, "height");
	int height = json_integer_value(height_json);

	json_t *weight_json = json_object_get(root, "weight");
	int weight = json_integer_value(weight_json);

	json_t *moves = json_object_get(root, "moves");
	if (!json_is_array(moves))
	{
		printf("No se encontró una lista de movimientos.\n");
		json_decref(root);
		return;
	}

	char *str_pokemon_id = (char *)malloc(12 * sizeof(char));
	sprintf(str_pokemon_id, "%d", pokemon_id);

	const char *pokemon_description = make_request(topScreen, bottomScreen, "https://pokeapi.co/api/v2/pokemon-species", str_pokemon_id);
	json_t *entries = json_loads(pokemon_description, 0, &error);
	if (!entries)
	{
		printf("Error al analizar JSON: %s\n", error.text);
		return;
	}

	json_t *description_entries = json_object_get(entries, "flavor_text_entries");
	const char *description_text = NULL;

	if (json_is_array(description_entries))
	{
		size_t index;
		json_t *entry;
		json_array_foreach(description_entries, index, entry)
		{
			json_t *language = json_object_get(entry, "language");
			json_t *lang_name = json_object_get(language, "name");

			if (json_is_string(lang_name) && strcmp(json_string_value(lang_name), "es") == 0)
			{
				json_t *text = json_object_get(entry, "flavor_text");
				if (json_is_string(text))
				{
					description_text = json_string_value(text);
					show_moves_with_pagination(moves, name, height, weight, pokemon_id, description_text, topScreen, bottomScreen);
				}
			}
		}
	}

	json_decref(root);
	json_decref(entries);
	free(pokemon_info);
	free(pokemon_description);
}