#!/bin/bash

# Directorio de entrada de los PNG
INPUT_DIR="gfx"
# Directorio de salida de los T3S
OUTPUT_DIR="gfx/t3s"
# Archivo de salida T3S
OUTPUT_FILE="$OUTPUT_DIR/sprites.t3s"

# Asegúrate de que el directorio de salida exista
mkdir -p "$OUTPUT_DIR"

# Comienza el archivo T3S con la cabecera
echo "--atlas -f rgba8888 -z auto" > "$OUTPUT_FILE"

# Recorre todos los archivos PNG en el directorio de entrada
for sprite in "$INPUT_DIR"/*.png; do
    # Verifica si hay archivos PNG
    if [[ ! -f "$sprite" ]]; then
        echo "No se encontraron archivos PNG en $INPUT_DIR."
        exit 1
    fi
    
    # Extrae el nombre del archivo
    base_name=$(basename "$sprite")
    
    # Agrega el nombre del sprite al archivo de salida
    echo "$base_name" >> "$OUTPUT_FILE"
done

# Convierte el archivo T3S usando tex3ds
/opt/devkitpro/tools/bin/tex3ds -i "$OUTPUT_FILE" -o "$OUTPUT_DIR/sprites.t3s"

echo "All sprites have been processed into $OUTPUT_DIR/sprites.t3s."