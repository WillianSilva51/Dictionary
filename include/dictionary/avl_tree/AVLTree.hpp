#pragma once

#include <iostream>
#include <initializer_list>
#include <stack>

#include "dictionary/avl_tree/Node.hpp"
#include "dictionary/avl_tree/IteratorAVL.hpp"
#include "dictionary/Dictionary.hpp"

/**
 * @file AVLTree.hpp
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
class AVLTree : public Dictionary<Key, Value>
{
    /**
     * @brief Alias para um ponteiro para um nó da árvore.
     */
    using NodePtr = Node<Key, Value> *;

    /**
     * @brief Declaração da classe IteratorAVL como amiga.
     *
     * Permite que a classe IteratorAVL acesse membros privados e protegidos
     * da classe AVLTree, facilitando a implementação de iteração sobre a árvore.
     */
    friend class IteratorAVL<Key, Value>;

    /**
     * @brief Alias para o iterador da árvore AVL.
     *
     * Este alias é utilizado para simplificar a declaração de iteradores
     * que percorrem a árvore AVL.
     */
    using iterator = IteratorAVL<Key, Value>;

private:
    /**
     * @brief Ponteiro para o nó raiz da Árvore AVL.
     */
    Node<Key, Value> *root{nullptr};

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
    Node<Key, Value> *fixup_node(NodePtr p);

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
    Node<Key, Value> *insert(NodePtr p, const std::pair<Key, Value> &key);

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
    Node<Key, Value> *update(NodePtr p, const std::pair<Key, Value> &key);

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
    Node<Key, Value> *fixup_deletion(NodePtr p);

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
    Node<Key, Value> *m_remove(NodePtr p, const Key &key);

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
    Node<Key, Value> *remove_successor(NodePtr root, NodePtr node);

    /**
     * @brief Função auxiliar recursiva para remover todos os nós da árvore.
     *
     * Realiza um percurso em pós-ordem para deletar todos os nós.
     *
     * @param root Ponteiro para o nó raiz da subárvore a ser limpa.
     * @return NodePtr Sempre retorna `nullptr` após limpar a subárvore.
     */
    Node<Key, Value> *clear(NodePtr root);

    /**
     * @brief Atualiza a altura de um nó.
     *
     * A altura de um nó é calculada como 1 + máximo(altura do filho esquerdo, altura do filho direito).
     * A altura de um nó folha é 0. A altura de `nullptr` é -1.
     *
     * @param node Ponteiro para o nó cuja altura será atualizada.
     * @return int A nova altura do nó.
     */
    int updateHeight(NodePtr node);

    /**
     * @brief Retorna a altura de um nó.
     *
     * @param node Ponteiro para o nó.
     * @return int A altura do nó. Retorna -1 se o nó for `nullptr`.
     */
    int height(NodePtr node);

    /**
     * @brief Calcula o fator de balanceamento de um nó.
     *
     * O fator de balanceamento é a diferença entre a altura da subárvore esquerda
     * e a altura da subárvore direita.
     *
     * @param node Ponteiro para o nó.
     * @return int O fator de balanceamento do nó.
     */
    int balance(NodePtr node);

    /**
     * @brief Realiza uma rotação simples à direita em torno do nó `p`.
     *
     * Usada para balancear a árvore quando ela está desbalanceada à esquerda.
     *
     * @param p Ponteiro para o nó que será a raiz da rotação (o nó desbalanceado).
     * @return NodePtr Ponteiro para a nova raiz da subárvore após a rotação.
     */
    Node<Key, Value> *rightRotation(NodePtr p);

    /**
     * @brief Realiza uma rotação simples à esquerda em torno do nó `p`.
     *
     * Usada para balancear a árvore quando ela está desbalanceada à direita.
     *
     * @param p Ponteiro para o nó que será a raiz da rotação (o nó desbalanceado).
     * @return NodePtr Ponteiro para a nova raiz da subárvore após a rotação.
     */
    Node<Key, Value> *leftRotation(NodePtr p);

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
     * @brief Clona recursivamente uma subárvore.
     *
     * Esta é uma função auxiliar que realiza uma cópia profunda de uma subárvore
     * a partir do nó fornecido. É usada pelo construtor de cópia e pelo
     * operador de atribuição de cópia para criar uma cópia idêntica, mas separada,
     * da estrutura da árvore.
     *
     * @param node_other Uma referência constante para o ponteiro do nó raiz da subárvore a ser clonada.
     * @return NodePtr Um ponteiro para a raiz da subárvore recém-criada (clonada).
     */
    NodePtr clone_recursive(const NodePtr &node_other) const;

    /**
     * @brief Função auxiliar recursiva para imprimir os elementos em ordem (in-order).
     *
     * @param node Ponteiro para o nó raiz da subárvore a ser impressa.
     */
    void printInOrder(NodePtr node) const;

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
     */
    AVLTree() = default;

    /**
     * @brief Construtor de cópia. Cria um novo conjunto como cópia de `other`.
     *
     * Realiza uma cópia profunda dos elementos.
     *
     * @param other O conjunto a ser copiado.
     */
    AVLTree(const AVLTree &other);

    /**
     * @brief Construtor a partir de uma lista inicializadora.
     *
     * Cria um conjunto e insere todos os elementos da lista.
     *
     * @param list A lista de inicialização (`std::initializer_list<std::pair<Key, Value>>`).
     */
    AVLTree(std::initializer_list<std::pair<Key, Value>> list);

    /**
     * @brief Cria uma cópia profunda da árvore AVL.
     *
     * Retorna um ponteiro inteligente para uma nova instância da árvore AVL
     * que contém os mesmos elementos que a árvore atual.
     *
     * @return std::unique_ptr<AVLTree<Key, Value>> Um ponteiro inteligente para a nova árvore AVL.
     */
    std::unique_ptr<Dictionary<Key, Value>> clone() const;

    /**
     * @brief Destrutor. Libera toda a memória alocada pelos nós da árvore.
     */
    ~AVLTree();

    /**
     * @brief Retorna um iterador para o início do conjunto.
     *
     * O iterador aponta para o menor elemento da árvore (in-order traversal).
     *
     * @return IteratorAVL<Key, Value> Um iterador para o início do conjunto.
     */
    iterator begin() noexcept { return iterator(root); }

    /**
     * @brief Retorna um iterador para o final do conjunto.
     *
     * O iterador aponta para o elemento após o maior elemento da árvore.
     *
     * @return IteratorAVL<Key, Value> Um iterador para o final do conjunto.
     */
    iterator end() noexcept { return iterator(); }

    /**
     * @brief Retorna um iterador constante para o início do conjunto.
     *
     * O iterador aponta para o menor elemento da árvore (in-order traversal).
     *
     * @return IteratorAVL<Key, Value> Um iterador constante para o início do conjunto.
     */
    iterator begin() const noexcept { return iterator(root); }

    /**
     * @brief Retorna um iterador constante para o início do conjunto.
     *
     * O iterador aponta para o menor elemento da árvore (in-order traversal).
     *
     * @return IteratorAVL<Key, Value> Um iterador constante para o início do conjunto.
     */
    iterator end() const noexcept { return iterator(); }

    /**
     * @brief Retorna um iterador constante para o início do conjunto.
     *
     * O iterador aponta para o menor elemento da árvore (in-order traversal).
     *
     * @return IteratorAVL<Key, Value> Um iterador constante para o início do conjunto.
     */
    iterator cbegin() const noexcept { return iterator(root); }

    /**
     * @brief Retorna um iterador constante para o final do conjunto.
     *
     * O iterador aponta para o elemento após o maior elemento da árvore.
     *
     * @return IteratorAVL<Key, Value> Um iterador constante para o final do conjunto.
     */
    iterator cend() const noexcept { return iterator(); }

    /**
     * @brief Operador de atribuição por cópia.
     *
     * Substitui o conteúdo do conjunto atual pelo conteúdo de `other`.
     * Garante a autotribuição segura e libera a memória antiga antes de copiar.
     *
     * @param other O conjunto a ser copiado.
     */
    void operator=(const AVLTree &other);

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
    void swap(AVLTree<Key, Value> &other) noexcept;

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
     *
     * @throw std::out_of_range Se a chave não existir.
     */
    void update(const std::pair<Key, Value> &key) { root = update(root, key); };

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
    void remove(const Key &key);

    /**
     * @brief Verifica se o conjunto contém uma determinada chave.
     *
     * @param key A chave a ser procurada.
     * @return true Se a chave estiver presente no conjunto.
     * @return false Caso contrário.
     */
    bool contains(const Key &key);

    // Funções de impressão

    /**
     * @brief Imprime os elementos do conjunto em ordem crescente (in-order traversal).
     */
    void print() const;

    /**
     * @brief Aplica uma função a cada par chave-valor no conjunto.
     *
     * Permite iterar sobre todos os elementos do conjunto e aplicar uma função
     * personalizada a cada um deles.
     *
     * @param func A função a ser aplicada a cada par chave-valor.
     */
    void forEach(const std::function<void(const std::pair<Key, Value> &)> &func) const;

    /**
     * @brief Exibe a estrutura da árvore AVL de forma visual no console.
     *
     * Útil para depuração e visualização do balanceamento da árvore.
     */
    void bshow();
};

// -------------------------------------------Implementação da classe AVLTree.------------------------------------------------------------------

template <typename Key, typename Value>
AVLTree<Key, Value>::AVLTree(std::initializer_list<std::pair<Key, Value>> list) : AVLTree()
{
    for (const auto &key : list)
        insert(key);
}

template <typename Key, typename Value>
AVLTree<Key, Value>::AVLTree(const AVLTree &other) : AVLTree()
{
    if (other.root != nullptr)
    {
        root = clone_recursive(other.root);
        size_m = other.size_m;
        comparisons = other.comparisons;
        rotations = other.rotations;
    }
}

template <typename Key, typename Value>
std::unique_ptr<Dictionary<Key, Value>> AVLTree<Key, Value>::clone() const
{
    return std::make_unique<AVLTree<Key, Value>>(*this);
}

template <typename Key, typename Value>
AVLTree<Key, Value>::~AVLTree()
{
    clear();
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::operator=(const AVLTree &other)
{
    if (this != &other)
    {
        clear(); // Limpa a árvore atual
        if (other.root != nullptr)
        {
            root = clone_recursive(other.root);
            size_m = other.size_m;
            comparisons = other.comparisons;
            rotations = other.rotations;
        }
    }
}

template <typename Key, typename Value>
size_t AVLTree<Key, Value>::size() const noexcept
{
    return size_m;
}

template <typename Key, typename Value>
bool AVLTree<Key, Value>::empty() const noexcept
{
    return root == nullptr;
}

template <typename Key, typename Value>
Node<Key, Value> *AVLTree<Key, Value>::clear(NodePtr root)
{
    if (root != nullptr)
    {
        root->left = clear(root->left);
        root->right = clear(root->right);

        delete root;
        return nullptr;
    }

    return root;
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::clear()
{
    root = clear(root);
    size_m = 0;
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::swap(AVLTree<Key, Value> &other) noexcept
{
    std::swap(root, other.root);
    std::swap(size_m, other.size_m);
    std::swap(comparisons, other.comparisons);
    std::swap(rotations, other.rotations);
}

template <typename Key, typename Value>
Node<Key, Value> *AVLTree<Key, Value>::fixup_node(NodePtr p)
{
    p->height = updateHeight(p);

    int bal = balance(p);

    if (bal == -2 and height(p->left->left) > height(p->left->right))
    {
        return rightRotation(p);
    }
    else if (bal == -2 and height(p->left->left) < height(p->left->right))
    {
        p->left = leftRotation(p->left);
        return rightRotation(p);
    }
    else if (bal == 2 and height(p->right->right) > height(p->right->left))
    {
        return leftRotation(p);
    }
    else if (bal == 2 and height(p->right->right) < height(p->right->left))
    {
        p->right = rightRotation(p->right);
        return leftRotation(p);
    }

    return p;
}

template <typename Key, typename Value>
Node<Key, Value> *AVLTree<Key, Value>::insert(NodePtr p, const std::pair<Key, Value> &key)
{
    if (p == nullptr)
    {
        size_m++;
        return new Node<Key, Value>(key);
    }

    comparisons++;
    if (key.first == p->key.first)
        return p;

    comparisons++;
    if (key < p->key)
        p->left = insert(p->left, key);
    else
        p->right = insert(p->right, key);

    p = fixup_node(p);

    return p;
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::insert(const std::pair<Key, Value> &key)
{
    root = insert(root, key);
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::remove(const Key &key)
{
    root = m_remove(root, key);
}

template <typename Key, typename Value>
Node<Key, Value> *AVLTree<Key, Value>::fixup_deletion(NodePtr p)
{
    int bal = balance(p);

    if (bal == 2 and balance(p->right) >= 0)
    {
        return leftRotation(p);
    }
    if (bal == 2 and balance(p->right) < 0)
    {
        p->right = rightRotation(p->right);
        return leftRotation(p);
    }
    if (bal == -2 and balance(p->left) <= 0)
    {
        return rightRotation(p);
    }
    if (bal == -2 and balance(p->left) > 0)
    {
        p->left = leftRotation(p->left);
        return rightRotation(p);
    }

    p->height = updateHeight(p);

    return p;
}

template <typename Key, typename Value>
Node<Key, Value> *AVLTree<Key, Value>::m_remove(NodePtr p, const Key &key)
{
    if (p == nullptr)
        return p;

    comparisons++;
    if (key < p->key.first)
        p->left = m_remove(p->left, key);
    else if (key > p->key.first)
    {
        comparisons++;
        p->right = m_remove(p->right, key);
    }
    else if (p->right == nullptr)
    {
        comparisons += 2;

        NodePtr child = p->left;
        delete p;
        size_m--;
        return child;
    }
    else
        p->right = remove_successor(p, p->right);

    p = fixup_deletion(p);

    return p;
}

template <typename Key, typename Value>
Node<Key, Value> *AVLTree<Key, Value>::remove_successor(NodePtr root, NodePtr node)
{
    if (node->left != nullptr)
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
Value &AVLTree<Key, Value>::at(NodePtr p, const Key &key)
{
    if (p == nullptr)
        throw std::out_of_range("Key not found in AVL Tree");

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
Value &AVLTree<Key, Value>::operator[](const Key &key)
{
    NodePtr aux = root;

    while (aux != nullptr)
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
    root = insert(root, {key, Value()});

    return at(root, key); // Retorna o valor associado à nova chave
}

template <typename Key, typename Value>
Node<Key, Value> *AVLTree<Key, Value>::update(NodePtr p, const std::pair<Key, Value> &key)
{
    if (p == nullptr)
    {
        throw std::out_of_range("Key not found in AVL Tree");
    }

    comparisons++;
    if (key.first == p->key.first)
    {
        p->key.second = key.second; // Atualiza o valor
        return p;
    }
    else if (key.first < p->key.first)
    {
        comparisons++;
        p->left = update(p->left, key);
    }
    else
    {
        comparisons += 2;
        p->right = update(p->right, key);
    }
    p = fixup_node(p);

    return p;
}

template <typename Key, typename Value>
int AVLTree<Key, Value>::updateHeight(NodePtr node)
{
    return 1 + std::max(height(node->left), height(node->right));
}

template <typename Key, typename Value>
int AVLTree<Key, Value>::height(NodePtr node)
{
    return (!node) ? 0 : node->height;
}

template <typename Key, typename Value>
int AVLTree<Key, Value>::balance(NodePtr node)
{
    return height(node->right) - height(node->left);
}

template <typename Key, typename Value>
Node<Key, Value> *AVLTree<Key, Value>::rightRotation(NodePtr p)
{
    rotations++;

    NodePtr aux = p->left;
    p->left = aux->right;
    aux->right = p;

    p->height = updateHeight(p);
    aux->height = updateHeight(aux);

    return aux;
}

template <typename Key, typename Value>
Node<Key, Value> *AVLTree<Key, Value>::leftRotation(NodePtr p)
{
    rotations++;

    NodePtr aux = p->right;
    p->right = aux->left;
    aux->left = p;

    p->height = updateHeight(p);
    aux->height = updateHeight(aux);

    return aux;
}

template <typename Key, typename Value>
Node<Key, Value> *AVLTree<Key, Value>::clone_recursive(const NodePtr &node_other) const
{
    if (node_other == nullptr)
        return nullptr;

    // Cria o novo nó com os mesmos dados e altura
    NodePtr new_node = new Node<Key, Value>(node_other->key);
    new_node->height = node_other->height;

    // Define recursivamente os filhos esquerdo e direito
    new_node->left = clone_recursive(node_other->left);
    new_node->right = clone_recursive(node_other->right);

    return new_node;
}

template <typename Key, typename Value>
bool AVLTree<Key, Value>::contains(NodePtr node, const Key &key)
{
    if (node == nullptr)
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
bool AVLTree<Key, Value>::contains(const Key &key)
{
    return contains(root, key);
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::print() const
{
    printInOrder(root);
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::printInOrder(NodePtr node) const
{
    if (node == nullptr)
        return;

    else
    {
        printInOrder(node->left);
        std::cout << "[" << node->key.first << ", " << node->key.second << "]" << std::endl;
        printInOrder(node->right);
    }
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::forEach(const std::function<void(const std::pair<Key, Value> &)> &func) const
{
    for (const auto &key : *this)
        func(key);
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::bshow()
{
    bshow(root, "");
}

template <typename Key, typename Value>
void AVLTree<Key, Value>::bshow(NodePtr node, std::string heranca)
{
    if (node != nullptr and (node->left != nullptr or node->right != nullptr))
        bshow(node->right, heranca + "r");

    for (int i = 0; i < (int)heranca.size() - 1; i++)
        std::cout << (heranca[i] != heranca[i + 1] ? "│   " : "    ");

    if (heranca != "")
        std::cout << (heranca.back() == 'r' ? "┌───" : "└───");

    if (node == nullptr)
    {
        std::cout << "#" << std::endl;
        return;
    }

    std::cout << "[" << node->key.first << ", " << node->key.second << "]" << std::endl;

    if (node != nullptr and (node->left != nullptr or node->right != nullptr))
        bshow(node->left, heranca + "l");
}