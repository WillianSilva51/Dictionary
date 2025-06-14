#pragma once

/**
 * @brief Estrutura que representa um nó em uma Árvore Rubro-Negra (Red-Black Tree).
 *
 * Esta estrutura armazena um par chave-valor, a cor do nó (VERMELHO ou PRETO),
 * e ponteiros para o nó pai, filho esquerdo e filho direito.
 *
 *
 * @tparam Key Tipo da chave armazenada no nó.
 * @tparam Value Tipo do valor associado à chave.
 *
 *  @note A cor do nó é representada por um booleano, onde `true` indica VERMELHO
 *  e `false` indica PRETO. As constantes estáticas `RED` e `BLACK` são usadas para
 *  melhorar a legibilidade do código.
 *
 *  @details Esta estrutura é utilizada internamente na implementação de uma Árvore Rubro-Negra,
 *  que é uma estrutura de dados de árvore balanceada. A cor dos nós é usada para garantir
 *  que a árvore mantenha suas propriedades de balanceamento, o que permite operações de inserção,
 *  remoção e busca eficientes.
 */
template <typename Key, typename Value>
struct NodeRB
{
    /**
     * @brief Constante estática que representa a cor PRETA para um nó.
     * O valor é `false`.
     */
    constexpr static bool BLACK = false;

    /**
     * @brief Constante estática que representa a cor VERMELHA para um nó.
     * O valor é `true`.
     */
    constexpr static bool RED = true;

    /**
     * @brief Par chave-valor armazenado no nó.
     */
    std::pair<Key, Value> key;

    /**
     * @brief Cor do nó (VERMELHO ou PRETO).
     * Utiliza as constantes `RED` ou `BLACK`.
     */
    bool color;

    /**
     * @brief Ponteiro para o nó pai.
     * Inicializado como `nullptr` por padrão.
     */
    NodeRB<Key, Value> *parent{nullptr};

    /**
     * @brief Ponteiro para o filho esquerdo.
     * Inicializado como `nullptr` por padrão.
     */
    NodeRB<Key, Value> *left{nullptr};

    /**
     * @brief Ponteiro para o filho direito.
     * Inicializado como `nullptr` por padrão.
     */
    NodeRB<Key, Value> *right{nullptr};

    /**
     * @brief Construtor para um nó da Árvore Rubro-Negra.
     *
     * @param key O par chave-valor a ser armazenado no nó.
     * @param color A cor do nó. O padrão é `RED`.
     * @param parent Ponteiro para o nó pai.
     * @param left Ponteiro para o filho esquerdo.
     * @param right Ponteiro para o filho direito.
     */
    NodeRB(const std::pair<Key, Value> &key, const bool &color = RED, NodeRB<Key, Value> *parent = nullptr, NodeRB<Key, Value> *left = nullptr, NodeRB<Key, Value> *right = nullptr)
        : key(key), color(color), parent(parent), left(left), right(right) {}
};