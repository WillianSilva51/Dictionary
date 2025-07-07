#!/bin/bash

# ==============================================================================
# SCRIPT DE BENCHMARK COMPLETO PARA O PROJETO DE DICIONÁRIO
#
# Este script executa o programa de contagem de palavras várias vezes para
# coletar dados de performance e, ao final, calcula a média do tempo de
# construção e das métricas específicas de cada estrutura de dados
# (Rotações, Comparações, Colisões).
#
# Uso:
# 1. Salve este arquivo como 'benchmark.sh' na raiz do projeto.
# 2. Dê permissão de execução: chmod +x benchmark.sh
# 3. Execute: ./benchmark.sh
#
# Certifique-se de que seu computador aguente o número de execuções, pois isso pode levar algum tempo.
# ==============================================================================

# --- CONFIGURAÇÕES ---
# Quantas vezes o programa deve ser executado para o cálculo da média

if [ -z "$1" ]; then
    NUM_RUNS=5
else
    NUM_RUNS=$1
fi

# O arquivo de entrada a ser usado (deve estar na pasta 'files/')
# Sugestão: use um arquivo grande como 'bible.txt' ou 'donquijote.txt' para métricas mais significativas.
if [ -z "$2" ]; then
    INPUT_FILE="bible.txt"
else
    INPUT_FILE=$2
fi

# Caminho para o seu executável
EXECUTABLE="./build/bin/Dictionary"
# --- FIM DAS CONFIGURAÇÕES ---


# --- VALIDAÇÕES INICIAIS ---
if [ ! -f "$EXECUTABLE" ]; then
    echo "❌ Erro: Executável não encontrado em '$EXECUTABLE'"
    echo "   Por favor, compile o projeto primeiro usando 'make'."
    exit 1
fi

if [ ! -f "files/$INPUT_FILE" ]; then
    echo "❌ Erro: Arquivo de entrada não encontrado em 'files/$INPUT_FILE'"
    exit 1
fi

# --- INICIALIZAÇÃO DOS ACUMULADORES ---
# Tempo de construção (ponto flutuante)
total_time_avl=0.0; total_time_rbt=0.0; total_time_chash=0.0; total_time_ohash=0.0

# Métricas das Árvores (inteiros)
total_avl_rotations=0; total_avl_comparisons=0
total_rbt_rotations=0; total_rbt_comparisons=0

# Métricas das Tabelas Hash (inteiros)
total_chash_collisions=0; total_chash_comparisons=0
total_ohash_collisions=0; total_ohash_comparisons=0

# --- EXECUÇÃO DO BENCHMARK ---
echo "🚀 Iniciando benchmark completo (Tempo e Métricas)..."
echo "Executando '$EXECUTABLE all $INPUT_FILE' por $NUM_RUNS vezes."
echo "--------------------------------------------------------"

for i in $(seq 1 $NUM_RUNS)
do
    echo -ne "   Execução $i de $NUM_RUNS... \r"

    # Executa o programa e captura toda a saída na variável 'output'
    output=$($EXECUTABLE all "$INPUT_FILE")

    # --- Extração de TEMPO ---
    time_avl=$(echo "$output"   | awk '/structure: AVL TREE/{flag=1;next}/build time:/{if(flag){print $3;flag=0}}')
    time_rbt=$(echo "$output"   | awk '/structure: RB TREE/{flag=1;next}/build time:/{if(flag){print $3;flag=0}}')
    time_chash=$(echo "$output" | awk '/structure: CHAINING_HASH/{flag=1;next}/build time:/{if(flag){print $3;flag=0}}')
    time_ohash=$(echo "$output" | awk '/structure: OPEN_ADDRESSING_HASH/{flag=1;next}/build time:/{if(flag){print $3;flag=0}}')

    # --- Extração de MÉTRICAS ---
    # AVL
    val_avl_rotations=$(echo "$output"   | awk '/structure: AVL TREE/{flag=1;next}/Rotations:/{if(flag){print $2;flag=0}}')
    val_avl_comparisons=$(echo "$output" | awk '/structure: AVL TREE/{flag=1;next}/Comparisons:/{if(flag){print $2;flag=0}}')
    # RBT
    val_rbt_rotations=$(echo "$output"   | awk '/structure: RB TREE/{flag=1;next}/Rotations:/{if(flag){print $2;flag=0}}')
    val_rbt_comparisons=$(echo "$output" | awk '/structure: RB TREE/{flag=1;next}/Comparisons:/{if(flag){print $2;flag=0}}')
    # Chained Hash
    val_chash_collisions=$(echo "$output" | awk '/structure: CHAINING_HASH/{flag=1;next}/Collisions:/{if(flag){print $2;flag=0}}')
    val_chash_comparisons=$(echo "$output"| awk '/structure: CHAINING_HASH/{flag=1;next}/Comparisons:/{if(flag){print $2;flag=0}}')
    # Open Hash
    val_ohash_collisions=$(echo "$output" | awk '/structure: OPEN_ADDRESSING_HASH/{flag=1;next}/Collisions:/{if(flag){print $2;flag=0}}')
    val_ohash_comparisons=$(echo "$output"| awk '/structure: OPEN_ADDRESSING_HASH/{flag=1;next}/Comparisons:/{if(flag){print $2;flag=0}}')

    # --- SOMA DOS VALORES ---
    # Tempo (usando 'bc' para ponto flutuante)
    total_time_avl=$(echo "$total_time_avl + $time_avl" | bc)
    total_time_rbt=$(echo "$total_time_rbt + $time_rbt" | bc)
    total_time_chash=$(echo "$total_time_chash + $time_chash" | bc)
    total_time_ohash=$(echo "$total_time_ohash + $time_ohash" | bc)

    # Métricas (usando aritmética do bash para inteiros)
    total_avl_rotations=$((total_avl_rotations + val_avl_rotations))
    total_avl_comparisons=$((total_avl_comparisons + val_avl_comparisons))
    total_rbt_rotations=$((total_rbt_rotations + val_rbt_rotations))
    total_rbt_comparisons=$((total_rbt_comparisons + val_rbt_comparisons))
    total_chash_collisions=$((total_chash_collisions + val_chash_collisions))
    total_chash_comparisons=$((total_chash_comparisons + val_chash_comparisons))
    total_ohash_collisions=$((total_ohash_collisions + val_ohash_collisions))
    total_ohash_comparisons=$((total_ohash_comparisons + val_ohash_comparisons))
done

echo -e "\n--------------------------------------------------------"
echo "✅ Benchmark finalizado. Calculando médias..."
echo ""

# --- CÁLCULO DAS MÉDIAS ---
# Usamos 'bc' com 'scale' para controlar as casas decimais
avg_time_avl=$(echo "scale=4; $total_time_avl / $NUM_RUNS" | bc)
avg_time_rbt=$(echo "scale=4; $total_time_rbt / $NUM_RUNS" | bc)
avg_time_chash=$(echo "scale=4; $total_time_chash / $NUM_RUNS" | bc)
avg_time_ohash=$(echo "scale=4; $total_time_ohash / $NUM_RUNS" | bc)

avg_avl_rotations=$(echo "scale=2; $total_avl_rotations / $NUM_RUNS" | bc)
avg_avl_comparisons=$(echo "scale=2; $total_avl_comparisons / $NUM_RUNS" | bc)
avg_rbt_rotations=$(echo "scale=2; $total_rbt_rotations / $NUM_RUNS" | bc)
avg_rbt_comparisons=$(echo "scale=2; $total_rbt_comparisons / $NUM_RUNS" | bc)
avg_chash_collisions=$(echo "scale=2; $total_chash_collisions / $NUM_RUNS" | bc)
avg_chash_comparisons=$(echo "scale=2; $total_chash_comparisons / $NUM_RUNS" | bc)
avg_ohash_collisions=$(echo "scale=2; $total_ohash_collisions / $NUM_RUNS" | bc)
avg_ohash_comparisons=$(echo "scale=2; $total_ohash_comparisons / $NUM_RUNS" | bc)

# --- IMPRESSÃO DO RELATÓRIO FINAL ---
echo "========= RELATÓRIO FINAL DO BENCHMARK (MÉDIA DE $NUM_RUNS EXECUÇÕES) ========="
echo ""
printf "%-25s | %-15s | %-15s | %-15s\n" "Estrutura" "Tempo (ms)" "Métrica 1" "Métrica 2"
printf "%-25s | %-15s | %-15s | %-15s\n" "-------------------------" "---------------" "---------------" "---------------"

# AVL
printf "%-25s | %-15s | %-15s | %-15s\n" "AVL Tree" "$avg_time_avl" "Rotations" "Comparisons"
printf "%-25s | %-15s | %-15s | %-15s\n" "" "" "$avg_avl_rotations" "$avg_avl_comparisons"
echo ""

# Red-Black Tree
printf "%-25s | %-15s | %-15s | %-15s\n" "Red-Black Tree" "$avg_time_rbt" "Rotations" "Comparisons"
printf "%-25s | %-15s | %-15s | %-15s\n" "" "" "$avg_rbt_rotations" "$avg_rbt_comparisons"
echo ""

# Chained Hash Table
printf "%-25s | %-15s | %-15s | %-15s\n" "Chained Hash Table" "$avg_time_chash" "Collisions" "Comparisons"
printf "%-25s | %-15s | %-15s | %-15s\n" "" "" "$avg_chash_collisions" "$avg_chash_comparisons"
echo ""

# Open Addressing Hash
printf "%-25s | %-15s | %-15s | %-15s\n" "Open Addressing Hash" "$avg_time_ohash" "Collisions" "Comparisons"
printf "%-25s | %-15s | %-15s | %-15s\n" "" "" "$avg_ohash_collisions" "$avg_ohash_comparisons"

echo "=========================================================================="
