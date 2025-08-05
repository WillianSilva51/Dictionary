#!/bin/bash

if [ -z "$1" ]; then
    echo "Estrutura não especificada, usando 'all' por padrão"
    STRUCTURE="all"
else
    STRUCTURE="$1"
fi

if [ -z "$2" ]; then
    echo "Arquivo não especificado, usando 'domcasmurro.txt' por padrão"
    FILE="domcasmurro.txt"
else
    FILE="$2"
fi

EXECUTABLE="./build/bin/Dictionary"

if [ ! -f "$EXECUTABLE" ]; then
    echo "❌ Erro: Executável não encontrado em '$EXECUTABLE'"
    echo "🔧 Tentando compilar com make..."
    make MODE=release
fi

"$EXECUTABLE" "$STRUCTURE" "$FILE"
