#pragma once

#include "dictionary/avl_tree/Node.hpp"
#include <iterator>
#include <stack>

template <typename Key, typename Value>
class AVLTree;

/**
 * @brief Um iterador para a árvore AVL.
 *
 * Esta classe fornece funcionalidade de iterador para percorrer
 * uma árvore AVL em ordem (in-order traversal).
 *
 * @tparam Key O tipo da chave armazenada nos nós da árvore.
 * @tparam Value O tipo do valor associado à chave nos nós da árvore.
 */
template <typename Key, typename Value>
class IteratorAVL
{
private:
    /// @brief Ponteiro para um nó da árvore AVL.
    using NodePtr = Node<Key, Value> *;
    /// @brief Pilha para armazenar o caminho durante a travessia.
    std::stack<NodePtr> path;

public:
    /// @brief Categoria do iterador, indica que é um iterador de entrada.
    using iterator_category = std::input_iterator_tag;
    /// @brief O tipo do valor apontado pelo iterador (um par chave-valor).
    using value_type = std::pair<Key, Value>;
    /// @brief Tipo para representar a diferença entre dois iteradores.
    using difference_type = std::ptrdiff_t;
    /// @brief Ponteiro para o tipo de valor.
    using pointer = value_type *;
    /// @brief Referência para o tipo de valor.
    using reference = value_type &;
    /// @brief Ponteiro constante para o tipo de valor.
    using const_pointer = const value_type *;
    /// @brief Referência constante para o tipo de valor.
    using const_reference = const value_type &;
    /// @brief Tipo do nó da árvore AVL.
    using NodeType = Node<Key, Value>;
    /// @brief Tipo do ponteiro para o nó da árvore AVL.
    using NodePtrType = NodePtr;

    /**
     * @brief Construtor padrão.
     *
     * Cria um iterador inválido (geralmente usado para representar o fim de uma coleção).
     */
    IteratorAVL() {}

    /**
     * @brief Construtor que inicializa o iterador a partir da raiz da árvore.
     *
     * O iterador é posicionado no primeiro elemento da travessia em ordem
     * (o nó mais à esquerda).
     *
     * @param root Ponteiro para o nó raiz da árvore AVL.
     */
    IteratorAVL(NodePtr root)
    {
        NodePtr current = root;
        while (current != nullptr)
        {
            path.push(current);
            current = current->left;
        }
    }

    /**
     * @brief Operador de derreferência.
     *
     * Retorna uma referência para o par chave-valor do nó atual.
     *
     * @return Referência para o par chave-valor do nó atual.
     */
    reference operator*() const
    {
        return path.top()->key;
    }

    /**
     * @brief Operador de acesso a membro.
     *
     * Retorna um ponteiro para o par chave-valor do nó atual.
     *
     * @return Ponteiro para o par chave-valor do nó atual.
     */
    pointer operator->() const
    {
        return &(path.top()->key);
    }

    /**
     * @brief Operador de incremento (pré-fixado).
     *
     * Avança o iterador para o próximo nó na travessia em ordem.
     *
     * @return Referência para o iterador atualizado.
     */
    IteratorAVL &operator++()
    {
        if (path.empty())
            return *this;

        NodePtr node = path.top();
        path.pop();

        if (node->right != nullptr)
        {
            NodePtr current = node->right;

            while (current != nullptr)
            {
                path.push(current);
                current = current->left;
            }
        }

        return *(this);
    }

    /**
     * @brief Operador de incremento (pós-fixado).
     *
     * Avança o iterador para o próximo nó na travessia em ordem e retorna
     * uma cópia do iterador antes do incremento.
     *
     * @return Uma cópia do iterador antes do incremento.
     */
    IteratorAVL operator++(int)
    {
        IteratorAVL temp = *this;
        ++(*this);
        return temp;
    }

    /**
     * @brief Operador de igualdade.
     *
     * Compara este iterador com outro iterador para verificar se apontam
     * para o mesmo nó.
     *
     * @param other O outro iterador a ser comparado.
     * @return `true` se os iteradores são iguais, `false` caso contrário.
     */
    bool operator==(const IteratorAVL &other) const
    {
        if (path.empty() && other.path.empty())
            return true;

        if (path.empty() || other.path.empty())
            return false;

        return path.top() == other.path.top();
    }

    /**
     * @brief Operador de desigualdade.
     *
     * Compara este iterador com outro iterador para verificar se apontam
     * para nós diferentes.
     *
     * @param other O outro iterador a ser comparado.
     * @return `true` se os iteradores são diferentes, `false` caso contrário.
     */
    bool operator!=(const IteratorAVL &other) const
    {
        return !(*this == other);
    }
};