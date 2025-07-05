#pragma once

#include <cstdint>
#include <stdexcept>

/**
 * @file DictionaryType.hpp
 *
 * @brief Enumera as estruturas de dados subjacentes disponíveis para uma implementação de dicionário.
 *
 * Isso permite a seleção de um tipo de dicionário específico em tempo de execução ou de compilação,
 * cada um com diferentes características de desempenho em relação às operações de inserção, exclusão e busca.
 *
 * - `AVL`: Uma árvore de busca binária auto-balanceada (árvore Adelson-Velsky e Landis).
 * - `RBTREE`: Uma árvore de busca binária auto-balanceada (Árvore Rubro-Negra).
 * - `CHAINING_HASH`: Uma tabela hash que resolve colisões por encadeamento.
 * - `OPEN_ADDRESSING_HASH`: Uma tabela hash que resolve colisões usando endereçamento aberto.
 */
enum class DictionaryType : uint8_t
{
    AVL,                  ///< Árvore AVL (Adelson-Velsky e Landis)
    RBTREE,               ///< Árvore Rubro-Negra
    CHAINING_HASH,        ///< Tabela hash com encadeamento (Chained Hash Table)
    OPEN_ADDRESSING_HASH, ///< Tabela hash com endereçamento aberto (Open Addressing Hash Table)
};

/**
 * @brief Obtém o nome da estrutura de dados a partir do tipo de dicionário.
 *
 * Esta função converte um valor do enumerador `DictionaryType` em uma string representando
 * o nome da estrutura de dados correspondente.
 *
 * @param type O tipo de dicionário, conforme o enumerador `DictionaryType`.
 * @return std::string O nome da estrutura de dados correspondente.
 */
std::string get_structure_name(DictionaryType type)
{
    switch (type)
    {
    case DictionaryType::AVL:
        return "AVL";
    case DictionaryType::RBTREE:
        return "RBTREE";
    case DictionaryType::CHAINING_HASH:
        return "CHAINING_HASH";
    case DictionaryType::OPEN_ADDRESSING_HASH:
        return "OPEN_ADDRESSING_HASH";
    default:
        throw std::invalid_argument("Tipo de estrutura desconhecido");
    }
}

/**
 * @brief Obtém o tipo de estrutura de dados a partir de uma string.
 *
 * Esta função converte uma string representando o tipo de estrutura de dados
 * em um valor do enum DictionaryType correspondente.
 *
 * @param structure_type A string representando o tipo da estrutura de dados.
 * @return DictionaryType O tipo da estrutura de dados correspondente.
 */
DictionaryType get_structure_type(const std::string &structure_type)
{
    if (structure_type == "avl" or structure_type == "avltree")
        return DictionaryType::AVL;
    else if (structure_type == "rbt" or structure_type == "rbtree")
        return DictionaryType::RBTREE;
    else if (structure_type == "chash" or structure_type == "hashtable")
        return DictionaryType::CHAINING_HASH;
    else if (structure_type == "ohash" or structure_type == "openhashtable")
        return DictionaryType::OPEN_ADDRESSING_HASH;
    else
        throw std::invalid_argument("Unknown structure_type type: " + structure_type);
}