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
     * @brief Alias para o iterador da árvore AVL.
     *
     * Este alias é utilizado para simplificar a declaração de iteradores
     * que percorrem a árvore AVL.
     */
    using iterator = IteratorRB<Key, Value>;

private:
    /**
     * @brief Ponteiro para o nó raiz da Árvore AVL.
     */
    NodeRB<Key, Value> *root{nullptr};

    /**
     * @brief Ponteiro para o nó nil (sentinela) da Árvore AVL.
     *
     * O nó nil é utilizado para simplificar a lógica de inserção, remoção e
     * balanceamento, evitando verificações nulas em nós folha.
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
     * para manter a propriedade de balanceamento da árvore AVL.
     */
    long long rotations{0};

    /**
     * @brief Realiza o balanceamento da árvore AVL após uma inserção ou remoção.
     *
     * Esta função é chamada recursivamente a partir do nó inserido/removido
     * (ou seu pai) até a raiz, verificando e corrigindo desbalanceamentos
     * através de rotações simples ou duplas.
     *
     * @param p Ponteiro para o nó a partir do qual o balanceamento deve ser verificado.
     * @return NodePtr Ponteiro para a raiz da subárvore balanceada.
     */
    void fixup_node(NodePtr p);

    /**
     * @brief Função auxiliar recursiva para inserir um elemento na árvore.
     *
     * Percorre a árvore para encontrar a posição correta de inserção da chave.
     * Após a inserção, chama `fixup_node` para garantir o balanceamento da AVL.
     *
     * @param p Ponteiro para o nó raiz da subárvore onde a chave será inserida.
     * @param key A chave a ser inserida.
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
     * @param key A chave cujo valor será atualizado.
     * @return NodePtr Ponteiro para o nó atualizado.
     */
    void update(NodePtr p, const std::pair<Key, Value> &key);

    /**
     * @brief Acessa o valor associado a uma chave na árvore.
     *
     * Localiza o nó com a chave especificada e retorna seu valor.
     * Se a chave não existir, lança uma exceção.
     *
     * @param p Ponteiro para o nó raiz da subárvore onde a chave será procurada.
     * @param key A chave cujo valor será acessado.
     * @return Value O valor associado à chave.
     */
    Value &at(NodePtr p, const Key &key);

    /**
     * @brief Realiza o balanceamento da árvore AVL após uma remoção.
     *
     * Similar ao `fixup_node`, mas especificamente ajustado para o processo de
     * remoção, que pode exigir diferentes verificações de balanceamento.
     *
     * @param p Ponteiro para o nó a partir do qual o balanceamento deve ser verificado
     *          (geralmente o pai do nó removido ou o nó que o substituiu).
     * @return NodePtr Ponteiro para a raiz da subárvore balanceada.
     */
    void fixup_deletion(NodePtr p);

    /**
     * @brief Função auxiliar recursiva para remover um elemento da árvore.
     *
     * Localiza o nó com a chave especificada. Trata os casos de remoção de nó
     * folha, nó com um filho ou nó com dois filhos (neste caso, substituindo
     * pelo sucessor). Após a remoção, chama `fixup_deletion` para balancear a árvore.
     *
     * @param p Ponteiro para o nó raiz da subárvore de onde a chave será removida.
     * @param key A chave a ser removida.
     * @return NodePtr Ponteiro para a raiz da subárvore modificada.
     */
    NodeRB<Key, Value> *remove(NodePtr p, NodePtr key);

    /**
     * @brief Remove o nó sucessor de um dado nó e o retorna.
     *
     * Usado no processo de remoção de um nó com dois filhos. O sucessor é o menor
     * nó na subárvore direita.
     *
     * @param root Ponteiro para a raiz da subárvore onde o sucessor será procurado.
     * @param node O nó cujo sucessor deve ser removido. (Este parâmetro parece ser o nó a ser substituído, e `root` a sua subárvore direita)
     * @return NodePtr Ponteiro para o nó sucessor que foi removido da sua posição original.
     *         A função também modifica a árvore para remover o sucessor de sua posição original.
     */
    NodeRB<Key, Value> *remove_successor(NodePtr root, NodePtr node);

    /**
     * @brief Função auxiliar recursiva para remover todos os nós da árvore.
     *
     * Realiza um percurso em pós-ordem para deletar todos os nós.
     *
     * @param root Ponteiro para o nó raiz da subárvore a ser limpa.
     * @return NodePtr Sempre retorna `nullptr` após limpar a subárvore.
     */
    NodeRB<Key, Value> *clear(NodePtr root);

    /**
     * @brief Realiza uma rotação simples à direita em torno do nó `p`.
     *
     * Usada para balancear a árvore quando ela está desbalanceada à esquerda.
     *
     * @param p Ponteiro para o nó que será a raiz da rotação (o nó desbalanceado).
     * @return NodePtr Ponteiro para a nova raiz da subárvore após a rotação.
     */
    void rightRotation(NodePtr p);

    /**
     * @brief Realiza uma rotação simples à esquerda em torno do nó `p`.
     *
     * Usada para balancear a árvore quando ela está desbalanceada à direita.
     *
     * @param p Ponteiro para o nó que será a raiz da rotação (o nó desbalanceado).
     * @return NodePtr Ponteiro para a nova raiz da subárvore após a rotação.
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
     * @brief Insere todos os elementos da subárvore rooted em `node` no conjunto `result`.
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
     * Inicializa a árvore com um nó nil (sentinela) e define o tamanho como 0.
     */
    RedBlackTree();

    /**
     * @brief Construtor de cópia. Cria um novo conjunto como cópia de `other`.
     *
     * Realiza uma cópia profunda dos elementos.
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
     * O iterador aponta para o menor elemento da árvore (in-order traversal).
     *
     * @return IteratorRB<Key, Value> Um iterador para o início do conjunto.
     */
    iterator begin() noexcept { return iterator(root, nil); }

    /**
     * @brief Retorna um iterador para o final do conjunto.
     *
     * O iterador aponta para o elemento após o maior elemento da árvore.
     *
     * @return IteratorRB<Key, Value> Um iterador para o final do conjunto.
     */
    iterator end() noexcept { return iterator(nil, nil); }

    /**
     * @brief Retorna um iterador constante para o início do conjunto.
     *
     * O iterador aponta para o menor elemento da árvore (in-order traversal).
     *
     * @return IteratorRB<Key, Value> Um iterador constante para o início do conjunto.
     */
    iterator cbegin() const noexcept { return iterator(root, nil); }

    /**
     * @brief Retorna um iterador constante para o final do conjunto.
     *
     * O iterador aponta para o elemento após o maior elemento da árvore.
     *
     * @return IteratorRB<Key, Value> Um iterador constante para o final do conjunto.
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
     * Útil para medir a eficiência das operações na árvore.
     *
     * @return long long O número de comparações realizadas.
     */
    long long getComparisons() const noexcept { return comparisons; }

    /**
     * @brief Retorna o número de rotações realizadas durante as operações.
     *
     * Útil para medir a quantidade de balanceamento necessário na árvore.
     *
     * @return long long O número de rotações realizadas.
     */
    long long getRotations() const noexcept { return rotations; }

    /**
     * @brief Remove todos os elementos do conjunto.
     *
     * Após esta operação, `size()` retornará 0 e `empty()` retornará `true`.
     */
    void clear();

    /**
     * @brief Troca o conteúdo deste conjunto com o de `other`.
     *
     * Operação eficiente que apenas troca os ponteiros raiz e os tamanhos.
     *
     * @param other O outro conjunto com o qual trocar o conteúdo.
     */
    void swap(RedBlackTree<Key, Value> &other) noexcept;

    /**
     * @brief Insere uma chave no conjunto.
     *
     * Se a chave já existir, o conjunto não é modificado.
     * A árvore é balanceada após a inserção, se necessário.
     *
     * @param key A chave a ser inserida.
     */
    void insert(const std::pair<Key, Value> &key);

    /**
     *  @brief Retorna o nó associado a uma chave.
     *
     *  Se a chave não existir, lança uma exceção.
     *
     *  @param key A chave a ser buscada.
     *  @return NodePtr Ponteiro para o nó associado à chave.
     */
    Value &at(const Key &key) { return at(root, key); };

    /**
     * @brief Sobrecarga do operador de indexação para acessar o valor associado a uma chave.
     *
     * Permite usar a sintaxe `tree[key]` para acessar o valor associado à chave.
     *  Se a chave não existir, cria uma nova entrada com valor padrão.
     *
     * @param key A chave a ser buscada.
     * @return Value O valor associado à chave.
     */
    Value &operator[](const Key &key);

    /**
     * @brief Atualiza o valor associado a uma chave existente ou insere uma nova chave.
     *
     * Se a chave já existir, atualiza seu valor. Caso contrário, insere a nova chave.
     * A árvore é balanceada após a atualização, se necessário.
     *
     * @param key O par chave-valor a ser atualizado ou inserido.
     */
    void update(const std::pair<Key, Value> &key) { update(root, key); };

    /**
     * @brief Sobrecarga do operador de atribuição para atualizar ou inserir uma chave.
     *
     * Permite usar a sintaxe `tree = {key, value}` para atualizar ou inserir uma chave.
     *
     * @param key O par chave-valor a ser atualizado ou inserido.
     */
    void operator=(std::pair<Key, Value> &key) { root = update(root, key); };

    /**
     * @brief Remove uma chave do conjunto.
     *
     * Se a chave não existir, o conjunto não é modificado.
     * A árvore é balanceada após a remoção, se necessário.
     *
     * @param key A chave a ser removida.
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
     *
     * @param other O outro conjunto.
     * @return RedBlackTree<Key, Value> Um novo conjunto resultado da união.
     */
    RedBlackTree Union(const RedBlackTree &other) const;

    // Funções de impressão

    /**
     * @brief Imprime os elementos do conjunto em ordem crescente (in-order traversal).
     */
    void print();

    /**
     * @brief Exibe a estrutura da árvore AVL de forma visual no console.
     *
     * Útil para depuração e visualização do balanceamento da árvore.
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
        root = remove(root, aux);

    // Se a chave não for encontrada, não faz nada
}

template <typename Key, typename Value>
void RedBlackTree<Key, Value>::fixup_deletion(NodePtr p)
{
}

template <typename Key, typename Value>
NodeRB<Key, Value> *RedBlackTree<Key, Value>::remove(NodePtr p, NodePtr key)
{
    NodePtr aux{nullptr};

    if (key->left == nil or key->right == nil)
    {
        aux = key;
    }
    else
    {
        aux = remove_successor(p, key->right);
        key->key = aux->key;
        key = aux;
    }
    if (aux->left != nil)
    {
        key = aux->left;
    }

    return p;
}

template <typename Key, typename Value>
NodeRB<Key, Value> *RedBlackTree<Key, Value>::remove_successor(NodePtr root, NodePtr node)
{
    if (node->left != nil)
        node->left = remove_successor(root, node->left);
    else
    {
        root->key = node->key;
        NodePtr aux = node->right;
        delete node;
        size_m--;
        return aux;
    }

    node = fixup_deletion(node);

    return node;
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