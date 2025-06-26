#pragma once

#include "dictionary/avl_tree/AVLTree.hpp"
#include "dictionary/rb_tree/RedBlackTree.hpp"
#include "dictionary/hash_table_c/ChainedHashTable.hpp"
#include "dictionary/hash_table_o/OpenHashTable.hpp"

#include "dictionary/Dictionary.hpp"

/**
 * @file DictionaryFactory.hpp
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
enum class DictionaryType
{
    AVL,                  ///< Árvore AVL (Adelson-Velsky e Landis)
    RBTREE,               ///< Árvore Rubro-Negra
    CHAINING_HASH,        ///< Tabela hash com encadeamento (Chained Hash Table)
    OPEN_ADDRESSING_HASH, ///< Tabela hash com endereçamento aberto (Open Addressing Hash Table)
};

/**
 * @brief Cria uma instância de um dicionário do tipo especificado.
 *
 * Esta função atua como uma fábrica (factory) para criar diferentes implementações
 * de dicionários que herdam da classe base `Dictionary`. A implementação específica
 * é escolhida com base no valor do enumerador `DictionaryType` fornecido.
 *
 * @tparam Key O tipo das chaves no dicionário.
 * @tparam Value O tipo dos valores associados às chaves.
 * @param type O tipo de dicionário a ser criado, conforme o enumerador `DictionaryType`.
 * @return Um `std::unique_ptr` para a nova instância do dicionário.
 * @throws std::invalid_argument Lançada se o `type` fornecido for desconhecido ou ainda não implementado.
 */
template <typename Key, typename Value>
std::unique_ptr<Dictionary<Key, Value>> create_dictionary(const DictionaryType &type = DictionaryType::RBTREE)
{
    switch (type)
    {
    case DictionaryType::AVL:
        return std::make_unique<AVLTree<Key, Value>>();
    case DictionaryType::RBTREE:
        return std::make_unique<RedBlackTree<Key, Value>>();
    case DictionaryType::CHAINING_HASH:
        return std::make_unique<ChainedHashTable<Key, Value>>();
    case DictionaryType::OPEN_ADDRESSING_HASH:
        return std::make_unique<OpenHashTable<Key, Value>>();
    default:
        throw std::invalid_argument("Tipo de dicionário desconhecido");
    }
}

/**
 * @brief Cria uma instância de um dicionário do tipo especificado, inicializando-o com uma lista de pares chave-valor.
 *
 * Esta função atua como uma fábrica (factory) para criar diferentes implementações
 * de dicionários que herdam da classe base `Dictionary`, inicializando-os com os valores fornecidos.
 * A implementação específica é escolhida com base no valor do enumerador `DictionaryType` fornecido.
 *
 * @tparam Key O tipo das chaves no dicionário.
 * @tparam Value O tipo dos valores associados às chaves.
 * @param type O tipo de dicionário a ser criado, conforme o enumerador `DictionaryType`.
 * @param list Uma lista de pares chave-valor para inicializar o dicionário.
 * @return Um `std::unique_ptr` para a nova instância do dicionário.
 * @throws std::invalid_argument Lançada se o `type` fornecido for desconhecido ou ainda não implementado.
 */
template <typename Key, typename Value>
std::unique_ptr<Dictionary<Key, Value>> create_dictionary(const DictionaryType &type,
                                                          const std::initializer_list<std::pair<Key, Value>> &list)
{
    switch (type)
    {
    case DictionaryType::AVL:
        return std::make_unique<AVLTree<Key, Value>>(list);
    case DictionaryType::RBTREE:
        return std::make_unique<RedBlackTree<Key, Value>>(list);
    case DictionaryType::CHAINING_HASH:
        return std::make_unique<ChainedHashTable<Key, Value>>(list);
    case DictionaryType::OPEN_ADDRESSING_HASH:
        return std::make_unique<OpenHashTable<Key, Value>>(list);
    default:
        throw std::invalid_argument("Tipo de dicionário desconhecido");
    }
}