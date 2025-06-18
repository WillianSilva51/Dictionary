#pragma once

#include "dictionary/rb_tree/NodeRB.hpp"
#include <iterator>
#include <stack>

template <typename Key, typename Value>
class RBTree;

/**
 * @brief Classe de iterador para a Red-Black Tree.
 *
 * Este iterador permite percorrer os nós da árvore Red-Black Tree em ordem crescente
 * (in-order traversal), retornando pares chave-valor.
 *
 * @tparam Key Tipo da chave dos nós da árvore.
 * @tparam Value Tipo do valor associado às chaves.
 */
template <typename Key, typename Value>
class IteratorRB
{
private:
    /// @brief Ponteiro para um nó da árvore Red-Black Tree.
    using NodePtr = NodeRB<Key, Value> *;

    /// @brief Pilha para armazenar o caminho durante a travessia.
    std::stack<NodePtr> path;

    /// @brief Ponteiro para o nó nil (sentinela) da árvore.
    NodePtr nil{nullptr};

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
    /// @brief Tipo do nó da árvore Red-Black Tree.
    using NodeType = NodeRB<Key, Value>;
    /// @brief Tipo do ponteiro para o nó da árvore Red-Black Tree.
    using NodePtrType = NodePtr;

    /**
     * @brief Construtor padrão.
     *
     * Cria um iterador inválido (geralmente usado para representar o fim de uma coleção).
     */
    IteratorRB() = default;

    /**
     * @brief Construtor que inicializa o iterador a partir da raiz da árvore.
     *
     * O iterador é posicionado no primeiro elemento da travessia em ordem
     * (o nó mais à esquerda).
     *
     * @param root Ponteiro para o nó raiz da árvore Red-Black Tree.
     */
    IteratorRB(NodePtr root, NodePtr nil) : nil(nil)
    {
        NodePtr current = root;
        while (current != nil)
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
    IteratorRB &operator++()
    {
        if (path.empty())
            return *this;

        NodePtr node = path.top();
        path.pop();

        if (node->right != nil)
        {
            NodePtr current = node->right;

            while (current != nil)
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
    IteratorRB operator++(int)
    {
        IteratorRB temp = *this;
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
    bool operator==(const IteratorRB &other) const
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
    bool operator!=(const IteratorRB &other) const
    {
        return !(*this == other);
    }
};