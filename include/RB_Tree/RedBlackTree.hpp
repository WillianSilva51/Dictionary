#pragma once

#include <iostream>
#include <initializer_list>
#include <stack>

#include "RB_Tree/node/NodeRB.hpp"
#include "RB_Tree/iteratorRB/IteratorRB.hpp"

/**
 * @file RedBlackTree.hpp
 *
 * @brief Classe que implementa uma Árvore AVL.
 *
 * A Árvore AVL é uma árvore de busca binária auto-balanceável, onde a diferença
 * entre as alturas das subárvores esquerda e direita de qualquer nó é no máximo 1.
 * Isso garante que as operações de busca, inserção e remoção tenham complexidade
 * de tempo O(log n) no pior caso, onde n é o número de elementos no conjunto.
 *
 * @tparam Key, Value Tipo dos elementos armazenados no conjunto. Deve suportar operadores
 *           de comparação ( `<`, `==`, `>`).
 */
/**
 * @brief Implementação de uma Árvore Rubro-Negra (Red-Black Tree).
 *
 * A Árvore Rubro-Negra é uma árvore de busca binária auto-balanceada que
 * garante que as operações de inserção, remoção e busca tenham complexidade
 * de tempo no pior caso de O(log n), onde n é o número de nós na árvore.
 * Isso é alcançado através da manutenção de um conjunto de propriedades
 * (propriedades Rubro-Negras) que mantêm a árvore aproximadamente balanceada.
 *
 * @tparam Key O tipo da chave dos elementos.
 * @tparam Value O tipo do valor associado a cada chave.
 */
template <typename Key, typename Value>
class RedBlackTree
{
    /**
     * @brief Alias para um ponteiro para um nó da árvore.
     */
    using NodePtr = NodeRB<Key, Value> *;

    /**
     * @brief Declaração da classe IteratorRB como amiga.
     *
     * Permite que a classe IteratorRB acesse membros privados e protegidos
     * da classe RedBlackTree, facilitando a implementação de iteração sobre a árvore.
     */
    friend class IteratorRB<Key, Value>;

    /**
     * @brief Alias para o iterador da árvore Rubro-Negra.
     *
     * Este alias é utilizado para simplificar a declaração de iteradores
     * que percorrem a árvore.
     */
    using iterator = IteratorRB<Key, Value>;

private:
    /**
     * @brief Ponteiro para o nó raiz da Árvore Rubro-Negra.
     */
    NodeRB<Key, Value> *root{nullptr};

    /**
     * @brief Ponteiro para o nó nil (sentinela) da Árvore Rubro-Negra.
     *
     * O nó nil é um nó sentinela (sempre preto) que representa todas as folhas
     * externas da árvore, simplificando a lógica de inserção, remoção e
     * balanceamento ao evitar verificações de ponteiros nulos.
     */
    NodeRB<Key, Value> *nil{nullptr};

    /**
     * @brief Número de elementos atualmente no conjunto.
     */
    size_t size_m{0};

    /**
     * @brief Contador de comparações realizadas durante as operações de busca, inserção e remoção.
     *
     * Este contador é utilizado para medir a eficiência das operações na árvore.
     */
    long long comparisons{0};

    /**
     * @brief Contador de rotações realizadas durante as operações de balanceamento da árvore.
     *
     * Este contador é utilizado para medir a quantidade de rotações necessárias
     * para manter as propriedades da árvore Rubro-Negra.
     */
    long long rotations{0};

    /**
     * @brief Restaura as propriedades da Árvore Rubro-Negra após uma inserção.
     *
     * Esta função é chamada a partir do nó recém-inserido (que é sempre vermelho)
     * e sobe na árvore, corrigindo quaisquer violações das propriedades Rubro-Negras
     * (como um nó vermelho tendo um pai vermelho) através de recolorações e rotações.
     *
     * @param p Ponteiro para o nó a partir do qual a correção deve começar.
     */
    void fixup_node(NodePtr p);

    /**
     * @brief Função auxiliar para inserir um novo nó na árvore.
     *
     * Percorre a árvore para encontrar a posição correta de inserção.
     * Após a inserção, chama `fixup_node` para garantir que as propriedades
     * da Árvore Rubro-Negra sejam mantidas.
     *
     * @param p Ponteiro para o nó raiz da subárvore onde o nó será inserido.
     * @param key O nó a ser inserido.
     * @return NodePtr Ponteiro para a raiz da subárvore modificada.
     */
    NodeRB<Key, Value> *insert(NodePtr p, const NodePtr key);

    /**
     * @brief Atualiza o valor associado a uma chave existente na árvore.
     *
     * Localiza o nó com a chave especificada e atualiza seu valor.
     * Se a chave não existir, lança uma exceção.
     *
     * @param p Ponteiro para o nó raiz da subárvore onde a chave será atualizada.
     * @param key O par chave-valor a ser atualizado.
     */
    void update(NodePtr p, const std::pair<Key, Value> &key);

    /**
     * @brief Acessa o valor associado a uma chave na árvore.
     *
     * Localiza o nó com a chave especificada e retorna uma referência ao seu valor.
     * Se a chave não existir, lança uma exceção `std::out_of_range`.
     *
     * @param p Ponteiro para o nó raiz da subárvore onde a chave será procurada.
     * @param key A chave cujo valor será acessado.
     * @return Value& Uma referência ao valor associado à chave.
     */
    Value &at(NodePtr p, const Key &key);

    /**
     * @brief Restaura as propriedades da Árvore Rubro-Negra após uma remoção.
     *
     * A remoção de um nó pode violar as propriedades da árvore (especialmente a
     * propriedade da altura negra). Esta função corrige essas violações subindo
     * na árvore a partir do ponto da remoção, usando rotações e recolorações.
     *
     * @param p Ponteiro para o nó que substituiu o nó removido, a partir do qual
     *          a correção deve ser verificada.
     */
    void fixup_deletion(NodePtr p);

    /**
     * @brief Função auxiliar para remover um elemento da árvore.
     *
     * Localiza o nó com a chave especificada. Trata os casos de remoção de nó
     * com zero, um ou dois filhos (neste caso, substituindo pelo sucessor).
     * Após a remoção, chama `fixup_deletion` para rebalancear a árvore.
     *
     * @param key Ponteiro para o nó a ser removido.
     */
    void remove(NodePtr key);

    /**
     * @brief Encontra o nó com a menor chave na subárvore de um dado nó.
     *
     * Usado no processo de remoção para encontrar o sucessor in-order de um nó
     * (o menor nó na subárvore direita).
     *
     * @param node Ponteiro para a raiz da subárvore onde o mínimo será procurado.
     * @return NodePtr Ponteiro para o nó com a menor chave na subárvore.
     */
    NodeRB<Key, Value> *minimun(NodePtr node);

    /**
     * @brief Função auxiliar recursiva para remover todos os nós da árvore.
     *
     * Realiza um percurso em pós-ordem para deletar todos os nós e liberar a memória.
     *
     * @param root Ponteiro para o nó raiz da subárvore a ser limpa.
     * @return NodePtr Sempre retorna `nullptr` após limpar a subárvore.
     */
    NodeRB<Key, Value> *clear(NodePtr root);

    /**
     * @brief Realiza uma rotação simples à direita em torno do nó `p`.
     *
     * Usada como parte do processo de rebalanceamento para manter as
     * propriedades da Árvore Rubro-Negra.
     *
     * @param p Ponteiro para o nó que será o pivô da rotação.
     */
    void rightRotation(NodePtr p);

    /**
     * @brief Realiza uma rotação simples à esquerda em torno do nó `p`.
     *
     * Usada como parte do processo de rebalanceamento para manter as
     * propriedades da Árvore Rubro-Negra.
     *
     * @param p Ponteiro para o nó que será o pivô da rotação.
     */
    void leftRotation(NodePtr p);

    /**
     * @brief Função auxiliar recursiva para verificar se uma chave está contida na árvore.
     *
     * @param root Ponteiro para o nó raiz da subárvore onde a busca será realizada.
     * @param key A chave a ser procurada.
     * @return true Se a chave for encontrada.
     * @return false Se a chave não for encontrada.
     */
    bool contains(NodePtr root, const Key &key);

    /**
     * @brief Insere todos os elementos da subárvore com raiz em `node` no conjunto `result`.
     *
     * Função auxiliar usada para a operação de União de conjuntos.
     * Percorre a subárvore `node` e insere cada elemento em `result`.
     *
     * @param result O conjunto onde os elementos serão inseridos.
     * @param node O nó raiz da subárvore cujos elementos serão inseridos.
     */
    void insertUnion(RedBlackTree<Key, Value> &result, const NodePtr &node) const;

    /**
     * @brief Função auxiliar recursiva para imprimir os elementos em ordem (in-order).
     *
     * @param node Ponteiro para o nó raiz da subárvore a ser impressa.
     */
    void printInOrder(NodePtr node);

    /**
     * @brief Função auxiliar recursiva para exibir a estrutura da árvore.
     *
     * @param node Ponteiro para o nó raiz da subárvore a ser exibida.
     * @param heranca String usada para formatar a indentação e mostrar a hierarquia.
     */
    void bshow(NodePtr node, std::string heranca);

public:
    /**
     * @brief Construtor padrão. Cria um conjunto vazio.
     *
     * Inicializa a árvore com um nó sentinela `nil` e a raiz apontando para `nil`.
     * O tamanho é inicializado como 0.
     */
    RedBlackTree();

    /**
     * @brief Construtor de cópia. Cria um novo conjunto como cópia de `other`.
     *
     * Realiza uma cópia profunda de todos os nós da árvore `other`.
     *
     * @param other O conjunto a ser copiado.
     */
    RedBlackTree(const RedBlackTree &other);

    /**
     * @brief Construtor a partir de uma lista inicializadora.
     *
     * Cria um conjunto e insere todos os elementos da lista.
     *
     * @param list A lista de inicialização (`std::initializer_list<std::pair<Key, Value>>`).
     */
    RedBlackTree(std::initializer_list<std::pair<Key, Value>> list);

    /**
     * @brief Destrutor. Libera toda a memória alocada pelos nós da árvore.
     */
    ~RedBlackTree();

    /**
     * @brief Retorna um iterador para o início do conjunto.
     *
     * O iterador aponta para o menor elemento da árvore (travessia in-order).
     *
     * @return iterator Um iterador para o início do conjunto.
     */
    iterator begin() noexcept { return iterator(root, nil); }

    /**
     * @brief Retorna um iterador para o final do conjunto.
     *
     * O iterador aponta para a posição após o último elemento (o nó sentinela `nil`).
     *
     * @return iterator Um iterador para o final do conjunto.
     */
    iterator end() noexcept { return iterator(nil, nil); }

    /**
     * @brief Retorna um iterador constante para o início do conjunto.
     *
     * O iterador aponta para o menor elemento da árvore (travessia in-order).
     *
     * @return iterator Um iterador constante para o início do conjunto.
     */
    iterator cbegin() const noexcept { return iterator(root, nil); }

    /**
     * @brief Retorna um iterador constante para o final do conjunto.
     *
     * O iterador aponta para a posição após o último elemento (o nó sentinela `nil`).
     *
     * @return iterator Um iterador constante para o final do conjunto.
     */
    iterator cend() const noexcept { return iterator(nil, nil); }

    /**
     * @brief Operador de atribuição por cópia.
     *
     * Substitui o conteúdo do conjunto atual pelo conteúdo de `other`.
     * Garante a autotribuição segura e libera a memória antiga antes de copiar.
     *
     * @param other O conjunto a ser copiado.
     */
    void operator=(const RedBlackTree &other);

    /**
     * @brief Retorna o número de elementos no conjunto.
     *
     * @return size_t O número de elementos.
     */
    size_t size() const noexcept;

    /**
     * @brief Verifica se o conjunto está vazio.
     *
     * @return true Se o conjunto não contiver elementos.
     * @return false Caso contrário.
     */
    bool empty() const noexcept;

    /**
     * @brief Retorna o número de comparações realizadas durante as operações.
     *
     * Útil para análise de desempenho da árvore.
     *
     * @return long long O número total de comparações.
     */
    long long getComparisons() const noexcept { return comparisons; }

    /**
     * @brief Retorna o número de rotações realizadas durante as operações.
     *
     * Útil para medir a atividade de rebalanceamento da árvore.
     *
     * @return long long O número total de rotações.
     */
    long long getRotations() const noexcept { return rotations; }

    /**
     * @brief Remove todos os elementos do conjunto.
     *
     * Após esta operação, `size()` retornará 0 e a árvore conterá apenas a raiz
     * apontando para o nó sentinela `nil`.
     */
    void clear();

    /**
     * @brief Troca o conteúdo deste conjunto com o de `other`.
     *
     * Operação eficiente que apenas troca os ponteiros internos e os contadores.
     *
     * @param other O outro conjunto com o qual trocar o conteúdo.
     */
    void swap(RedBlackTree<Key, Value> &other) noexcept;

    /**
     * @brief Insere um par chave-valor no conjunto.
     *
     * Se a chave já existir, a operação é ignorada. Caso contrário, um novo nó
     * é inserido e a árvore é rebalanceada para manter as propriedades Rubro-Negras.
     *
     * @param key O par chave-valor a ser inserido.
     */
    void insert(const std::pair<Key, Value> &key);

    /**
     * @brief Acessa o valor associado a uma chave, com verificação de limites.
     *
     * Se a chave não existir no mapa, uma exceção `std::out_of_range` é lançada.
     *
     * @param key A chave a ser buscada.
     * @return Value& Uma referência ao valor associado à chave.
     */
    Value &at(const Key &key) { return at(root, key); };

    /**
     * @brief Sobrecarga do operador de indexação para acessar ou inserir um elemento.
     *
     * Se `key` existir no mapa, retorna uma referência ao seu valor.
     * Se `key` não existir, insere um novo elemento com essa chave (usando o
     * construtor padrão de `Value`) e retorna uma referência ao novo valor.
     *
     * @param key A chave do elemento a ser acessado ou inserido.
     * @return Value& Uma referência ao valor associado à chave.
     */
    Value &operator[](const Key &key);

    /**
     * @brief Atualiza o valor de uma chave existente ou insere um novo par chave-valor.
     *
     * Se a chave já existir, seu valor é atualizado. Caso contrário, um novo
     * par chave-valor é inserido.
     *
     * @param key O par chave-valor a ser atualizado ou inserido.
     */
    void update(const std::pair<Key, Value> &key) { update(root, key); };

    /**
     * @brief Operador de atribuição para atualizar ou inserir um par chave-valor.
     *
     * Permite usar a sintaxe `tree = {key, value}` para atualizar ou inserir.
     *
     * @param key O par chave-valor a ser atualizado ou inserido.
     */
    void operator=(std::pair<Key, Value> &key) { root = update(root, key); };

    /**
     * @brief Remove um elemento do conjunto pela chave.
     *
     * Se a chave não for encontrada, o conjunto não é modificado.
     * Após a remoção, a árvore é rebalanceada para manter as propriedades Rubro-Negras.
     *
     * @param key A chave do elemento a ser removido.
     */
    void erase(const Key &key);

    /**
     * @brief Verifica se o conjunto contém uma determinada chave.
     *
     * @param key A chave a ser procurada.
     * @return true Se a chave estiver presente no conjunto.
     * @return false Caso contrário.
     */
    bool contains(const Key &key);

    /**
     * @brief Retorna um novo conjunto que é a união deste conjunto com `other`.
     *
     * A união contém todos os elementos que estão em `this` ou em `other` (ou em ambos).
     * Se uma chave existir em ambos, o valor de `this` é preservado.
     *
     * @param other O outro conjunto.
     * @return RedBlackTree<Key, Value> Um novo conjunto resultado da união.
     */
    RedBlackTree Union(const RedBlackTree &other) const;

    // Funções de impressão

    /**
     * @brief Imprime os elementos do conjunto em ordem crescente (travessia in-order).
     */
    void print();

    /**
     * @brief Exibe a estrutura da Árvore Rubro-Negra de forma visual no console.
     *
     * Útil para depuração e visualização da estrutura e cores dos nós.
     */
    void bshow();
};

// -------------------------------------------Implementação da classe RedBlackTree.------------------------------------------------------------------

template <typename Key, typename Value>
RedBlackTree<Key, Value>::RedBlackTree()
{
    nil = new NodeRB<Key, Value>({Key(), Value()}, NodeRB<Key, Value>::BLACK, nullptr, nullptr, nullptr);
    nil->left = nil->right = nil->parent = nil;
    root = nil;
    root->parent = nil;
}

template <typename Key, typename Value>
RedBlackTree<Key, Value>::RedBlackTree(std::initializer_list<std::pair<Key, Value>> list) : RedBlackTree()
{
    for (const auto &key : list)
        insert(key);
}

template <typename Key, typename Value>
RedBlackTree<Key, Value>::RedBlackTree(const RedBlackTree &other) : RedBlackTree()
{
    insertUnion(*this, other.root);
}

template <typename Key, typename Value>
RedBlackTree<Key, Value>::~RedBlackTree()
{
    clear();
    delete nil; // Libera o nó nil (sentinela) após limpar a árvore
    nil = nullptr;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::operator=(const RedBlackTree &other)
{
    if (this != &other)
    {
        clear();
        insertUnion(*this, other.root);
    }
}

template <typename Key, typename Value>
size_t RedBlackTree<Key, Value>::size() const noexcept
{
    return size_m;
}

template <typename Key, typename Value>
bool RedBlackTree<Key, Value>::empty() const noexcept
{
    return root == nil;
}

template <typename Key, typename Value>
NodeRB<Key, Value> *RedBlackTree<Key, Value>::clear(NodePtr root)
{
    if (root != nil)
    {
        root->left = clear(root->left);
        root->right = clear(root->right);

        delete root;
        return nil; // Retorna o nó nil após limpar a subárvore
    }

    return root;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::clear()
{
    root = clear(root);
    size_m = 0;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::swap(RedBlackTree<Key, Value> &other) noexcept
{
    std::swap(root, other.root);
    std::swap(size_m, other.size_m);
    std::swap(nil, other.nil);
    std::swap(comparisons, other.comparisons);
    std::swap(rotations, other.rotations);
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::fixup_node(NodePtr p)
{
    while (p != root && p->parent->color == NodeRB<Key, Value>::RED)
    {
        if (p->parent == p->parent->parent->left)
        {
            NodePtr uncle = p->parent->parent->right;

            if (uncle->color == NodeRB<Key, Value>::RED)
            {
                p->parent->color = NodeRB<Key, Value>::BLACK;
                uncle->color = NodeRB<Key, Value>::BLACK;
                p->parent->parent->color = NodeRB<Key, Value>::RED;
                p = p->parent->parent;
            }
            else
            {
                if (p == p->parent->right)
                {
                    p = p->parent;
                    rotations++;
                    leftRotation(p);
                }

                p->parent->color = NodeRB<Key, Value>::BLACK;
                p->parent->parent->color = NodeRB<Key, Value>::RED;
                rotations++;
                rightRotation(p->parent->parent);
            }
        }
        else
        {
            NodePtr uncle = p->parent->parent->left;

            if (uncle->color == NodeRB<Key, Value>::RED)
            {
                p->parent->color = NodeRB<Key, Value>::BLACK;
                uncle->color = NodeRB<Key, Value>::BLACK;
                p->parent->parent->color = NodeRB<Key, Value>::RED;
                p = p->parent->parent;
            }
            else
            {
                if (p == p->parent->left)
                {
                    p = p->parent;
                    rotations++;
                    rightRotation(p);
                }

                p->parent->color = NodeRB<Key, Value>::BLACK;
                p->parent->parent->color = NodeRB<Key, Value>::RED;
                rotations++;
                leftRotation(p->parent->parent);
            }
        }
    }

    root->color = NodeRB<Key, Value>::BLACK; // A raiz sempre deve ser preta
}

template <typename Key, typename Value>
NodeRB<Key, Value> *RedBlackTree<Key, Value>::insert(NodePtr p, const NodePtr key)
{
    NodePtr aux = p;
    NodePtr parent = nil;

    while (aux != nil)
    {
        comparisons++;
        parent = aux;
        if (key->key.first < aux->key.first)
            aux = aux->left;
        else if (key->key.first > aux->key.first)
        {
            comparisons++;
            aux = aux->right;
        }
        else
            return p; // A chave já existe, não insere novamente
    }
    size_m++;

    key->parent = parent;

    comparisons++;
    if (parent == nil)
    {
        root = key; // Se a árvore estava vazia, o novo nó se torna a raiz
    }
    else if (key->key.first < parent->key.first)
    {
        comparisons++;
        parent->left = key;
    }
    else
    {
        comparisons += 2;
        parent->right = key;
    }

    key->left = nil;
    key->right = nil;

    fixup_node(key);

    return root; // Retorna a nova raiz da árvore
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::insert(const std::pair<Key, Value> &key)
{
    root = insert(root, new NodeRB<Key, Value>(key, NodeRB<Key, Value>::RED, nil, nil, nil));
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::erase(const Key &key)
{
    NodePtr aux = root;

    while (aux != nil and aux->key.first != key)
    {
        comparisons++;
        if (key < aux->key.first)
            aux = aux->left;
        else
        {
            comparisons++;
            aux = aux->right;
        }
    }

    if (aux != nil) // Realiza a remoção se a chave for encontrada
        remove(aux);

    // Se a chave não for encontrada, não faz nada
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::fixup_deletion(NodePtr x)
{
    while (x != root and x->color == NodeRB<Key, Value>::BLACK)
    {
        if (x == x->parent->left) // Se x for filho esquerdo
        {
            NodePtr w = x->parent->right;            // Irmão de x
            if (w->color == NodeRB<Key, Value>::RED) // Caso 1
            {
                w->color = NodeRB<Key, Value>::BLACK;       // Muda a cor do irmão para preto
                x->parent->color = NodeRB<Key, Value>::RED; // Muda a cor do pai para vermelho
                leftRotation(x->parent);                    // Rotação à esquerda
                w = x->parent->right;                       // Atualiza w
            }

            if (w->left->color == NodeRB<Key, Value>::BLACK and
                w->right->color == NodeRB<Key, Value>::BLACK) // Caso 2
            {
                w->color = NodeRB<Key, Value>::RED; // Muda a cor do irmão para vermelho
                x = x->parent;                      // Move x para o pai
            }
            else
            {
                if (w->right->color == NodeRB<Key, Value>::BLACK) // Caso 3
                {
                    w->left->color = NodeRB<Key, Value>::BLACK; // Muda a cor do filho esquerdo do irmão para preto
                    w->color = NodeRB<Key, Value>::RED;         // Muda a cor do irmão para vermelho
                    rightRotation(w);                           // Rotação à direita
                    w = x->parent->right;                       // Atualiza w
                }

                w->color = x->parent->color; // Caso 4
                x->parent->color = NodeRB<Key, Value>::BLACK;
                w->right->color = NodeRB<Key, Value>::BLACK;
                leftRotation(x->parent);
                x = root; // Termina o loop
            }
        }
        else
        {
            NodePtr w = x->parent->left;             // Irmão de x
            if (w->color == NodeRB<Key, Value>::RED) // Caso 1
            {
                w->color = NodeRB<Key, Value>::BLACK;       // Muda a cor do irmão para preto
                x->parent->color = NodeRB<Key, Value>::RED; // Muda a cor do pai para vermelho
                rightRotation(x->parent);                   // Rotação à direita
                w = x->parent->left;                        // Atualiza w
            }

            if (w->right->color == NodeRB<Key, Value>::BLACK and
                w->left->color == NodeRB<Key, Value>::BLACK) // Caso 2
            {
                w->color = NodeRB<Key, Value>::RED; // Muda a cor do irmão para vermelho
                x = x->parent;                      // Move x para o pai
            }
            else
            {
                if (w->left->color == NodeRB<Key, Value>::BLACK) // Caso 3
                {
                    w->right->color = NodeRB<Key, Value>::BLACK; // Muda a cor do filho direito do irmão para preto
                    w->color = NodeRB<Key, Value>::RED;          // Muda a cor do irmão para vermelho
                    leftRotation(w);                             // Rotação à esquerda
                    w = x->parent->left;                         // Atualiza w
                }

                w->color = x->parent->color; // Caso 4
                x->parent->color = NodeRB<Key, Value>::BLACK;
                w->left->color = NodeRB<Key, Value>::BLACK;
                rightRotation(x->parent);
                x = root; // Termina o loop
            }
        }
    }
    x->color = NodeRB<Key, Value>::BLACK; // Garante que a raiz seja preta
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::remove(NodePtr key)
{
    NodePtr aux{nullptr};
    NodePtr aux2{nullptr};

    if (key->left == nil or key->right == nil)
    {
        aux = key;
    }
    else
    {
        aux = minimun(key->right); // Encontra o sucessor (menor na subárvore direita)
    }
    if (aux->left != nil)
    {
        aux2 = aux->left;
    }
    else
    {
        aux2 = aux->right;
    }
    aux2->parent = aux->parent;

    if (aux->parent == nil)
    {
        root = aux2; // Se o nó removido era a raiz, atualiza a raiz
    }
    else if (aux == aux->parent->left)
    {
        aux->parent->left = aux2;
    }
    else
    {
        aux->parent->right = aux2;
    }

    if (aux != key)
    {
        key->key = aux->key; // Copia a chave do nó removido para o nó substituto
    }

    if (aux->color == NodeRB<Key, Value>::BLACK)
    {
        // Se o nó removido era preto, precisamos corrigir o balanceamento
        fixup_deletion(aux2);
    }

    delete aux; // Libera o nó removido
    size_m--;
}

template <typename Key, typename Value>
NodeRB<Key, Value> *RedBlackTree<Key, Value>::minimun(NodePtr node)
{
    NodePtr aux = node;

    while (aux->left != nil)
        aux = aux->left;

    if (aux == nil)
        throw std::out_of_range("No minimum node found in the subtree");

    return aux; // Retorna o nó com a menor chave na subárvore
}

template <typename Key, typename Value>
Value &RedBlackTree<Key, Value>::at(NodePtr p, const Key &key)
{
    if (p == nil)
        throw std::out_of_range("Key not found in the Red-Black Tree");

    comparisons++;
    if (key == p->key.first)
        return p->key.second;

    comparisons++;
    if (key < p->key.first)
    {
        return at(p->left, key);
    }
    else
        return at(p->right, key);
}

template <typename Key, typename Value>
Value &RedBlackTree<Key, Value>::operator[](const Key &key)
{
    NodePtr aux = root;

    while (aux != nil)
    {
        comparisons++;
        if (key == aux->key.first)
            return aux->key.second;

        comparisons++;
        if (key < aux->key.first)
            aux = aux->left;
        else
            aux = aux->right;
    }
    // Se a chave não for encontrada, insere um novo nó com valor padrão
    root = insert(root, new NodeRB<Key, Value>(std::pair<Key, Value>(key, Value()), NodeRB<Key, Value>::RED, nil, nil, nil));

    return at(root, key); // Retorna o valor associado à nova chave
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::update(NodePtr p, const std::pair<Key, Value> &key)
{
    if (p == nil)
    {
        insert(key); // Se o nó não existir, insere a chave
        return;
    }

    comparisons++;
    if (key.first == p->key.first)
    {
        p->key.second = key.second; // Atualiza o valor
    }
    else if (key.first < p->key.first)
    {
        comparisons++;
        update(p->left, key);
    }
    else
        update(p->right, key);

    fixup_node(p);
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::rightRotation(NodePtr p)
{
    rotations++; // Incrementa o contador de rotações

    NodePtr aux = p->left;
    p->left = aux->right;

    if (aux->right != nil)
        aux->right->parent = p;
    aux->parent = p->parent;

    if (p->parent == nil)
    {
        root = aux; // Atualiza a raiz se necessário
    }
    else if (p == p->parent->right)
    {
        p->parent->right = aux;
    }
    else
    {
        p->parent->left = aux;
    }
    aux->right = p;
    p->parent = aux;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::leftRotation(NodePtr p)
{
    rotations++; // Incrementa o contador de rotações

    NodePtr aux = p->right;
    p->right = aux->left;

    if (aux->left != nil)
        aux->left->parent = p;
    aux->parent = p->parent;

    if (p->parent == nil)
    {
        root = aux; // Atualiza a raiz se necessário
    }
    else if (p == p->parent->left)
    {
        p->parent->left = aux;
    }
    else
    {
        p->parent->right = aux;
    }
    aux->left = p;
    p->parent = aux;
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::insertUnion(RedBlackTree<Key, Value> &result, const NodePtr &node) const
{
    if (node == nil)
        return;

    std::stack<NodePtr> nodes;
    nodes.push(node);

    while (!nodes.empty())
    {
        NodePtr atual = nodes.top();
        nodes.pop();

        result.insert(atual->key);

        if (atual->left != nil)
            nodes.push(atual->left);

        if (atual->right != nil)
            nodes.push(atual->right);
    }
}

template <typename Key, typename Value>
RedBlackTree<Key, Value> RedBlackTree<Key, Value>::Union(const RedBlackTree<Key, Value> &other) const
{
    RedBlackTree<Key, Value> result;

    insertUnion(result, root);
    insertUnion(result, other.root);

    return result;
}

template <typename Key, typename Value>
bool RedBlackTree<Key, Value>::contains(NodePtr node, const Key &key)
{
    if (node == nil)
        return false;

    comparisons++;
    if (key == node->key.first)
        return true;

    comparisons++;
    if (key < node->key.first)
    {
        return contains(node->left, key);
    }
    else
        return contains(node->right, key);
}

template <typename Key, typename Value>
bool RedBlackTree<Key, Value>::contains(const Key &key)
{
    return contains(root, key);
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::print()
{
    printInOrder(root);
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::printInOrder(NodePtr node)
{
    if (node == nil)
        return;

    else
    {
        printInOrder(node->left);
        std::cout << "(" << node->key.first << ", " << node->key.second << ")" << std::endl;
        printInOrder(node->right);
    }
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::bshow()
{
    bshow(root, "");
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::bshow(NodePtr node, std::string heranca)
{
    if (node != nil and (node->left != nil or node->right != nil))
        bshow(node->right, heranca + "r");

    for (int i = 0; i < (int)heranca.size() - 1; i++)
        std::cout << (heranca[i] != heranca[i + 1] ? "│   " : "    ");

    if (heranca != "")
        std::cout << (heranca.back() == 'r' ? "┌───" : "└───");

    if (node == nil)
    {
        std::cout << "#" << std::endl;
        return;
    }

    std::cout << (node->color == NodeRB<Key, Value>::RED ? "\x1b[31m" : "\x1b[30m") << "(" << node->key.first << ", " << node->key.second << ")"
              << "\x1b[0m" << std::endl;

    if (node != nil and (node->left != nil or node->right != nil))
        bshow(node->left, heranca + "l");
}