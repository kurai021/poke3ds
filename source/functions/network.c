#include <stdio.h>
#include <string.h>
#include <jansson.h>

#include "network.h"
#include "display.h"

void parse_pokemon_data(const char *json_data, PrintConsole *topScreen, PrintConsole *bottomScreen)
{
	json_error_t error;
	json_t *root = json_loads(json_data, 0, &error);

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

	show_moves_with_pagination(moves, name, height, weight, pokemon_id, topScreen, bottomScreen);
	json_decref(root);
}

void make_request(PrintConsole *topScreen, PrintConsole *bottomScreen, const char *pokemon_name)
{
	// Inicializar el servicio HTTP
	httpcInit(0);

	// Crear la URL dinámica con el nombre del Pokémon
	char url[128];
	snprintf(url, sizeof(url), "https://pokeapi.co/api/v2/pokemon/%s/", pokemon_name);

	// Inicializar variables para la solicitud
	httpcContext context;
	Result res = httpcOpenContext(&context, HTTPC_METHOD_GET, url, 1);

	if (res != 0)
	{
		printf("Error al abrir el contexto: 0x%x\n", res);
		return;
	}

	// Hacer la solicitud
	res = httpcBeginRequest(&context);
	if (res != 0)
	{
		printf("Error al iniciar la solicitud: 0x%x\n", res);
		httpcCloseContext(&context);
		return;
	}

	// Recibir la respuesta
	u32 responseCode;
	res = httpcGetResponseStatusCode(&context, &responseCode);

	if (res != 0)
	{
		printf("Error al obtener el estado de la respuesta: 0x%x\n", res);
		httpcCloseContext(&context);
		return;
	}

	if (responseCode != 200)
	{
		printf("Código de respuesta HTTP: %d\n", responseCode);
		httpcCloseContext(&context);
		return;
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
	parse_pokemon_data(json_data, topScreen, bottomScreen);

	// Liberar memoria
	free(json_data);

	// Cerrar el contexto
	httpcCloseContext(&context);
	httpcExit();
}