#pragma once

#include <vector>
#include <stdexcept>
#include <initializer_list>

/**
 * @brief Classe PriorityQueue implementa uma fila de prioridade usando uma heap binária.
 *
 * A fila de prioridade é uma estrutura de dados que permite inserir elementos com prioridades associadas,
 * onde os elementos de maior prioridade são removidos primeiro. Esta implementação utiliza uma heap binária
 * para garantir que as operações de inserção e remoção sejam eficientes.
 *
 * @tparam T Tipo dos elementos armazenados na fila de prioridade.
 * @tparam Comparator Tipo do comparador usado para definir a ordem dos elementos. O padrão é std::less<T>,
 *                   que define uma ordem decrescente. Para ordem crescente, pode-se usar std::greater<T>.
 */
template <typename T, typename Comparator = std::less<T>>
class PriorityQueue
{
private:
    std::vector<T> m_vec; // array que implementa a heap
    size_t heapSize{0};   // tamanho atual da fila de prioridade
    Comparator compare;   // comparador para definir a ordem dos elementos

    // Retorna o índice do pai do nó em i
    size_t parent(const size_t &index) { return index / 2; }

    // Retorna o índice do filho esquerdo do nó em i
    size_t left_child(const size_t &index) { return 2 * index; }

    // Retorna o índice do filho direito do nó em i
    size_t right_child(const size_t &index) { return (2 * index) + 1; }

    // Move o nó em i para cima na heap para manter a propriedade da heap
    void fixUp(size_t index);

    // Move o nó em i para baixo na heap para manter a propriedade da heap
    void fixDown(size_t index);

    // Constrói a heap máxima a partir do vetor inicial
    void buildMaxHeap();

public:
    // Desabilita a cópia do construtor
    PriorityQueue(const PriorityQueue &) = delete;

    // Desabilita a cópia do operador de atribuição
    PriorityQueue &operator=(const PriorityQueue &) = delete;

    // Construtor padrão que inicializa a fila de prioridade vazia
    PriorityQueue();

    // Construtor que inicializa a fila de prioridade com um vetor
    PriorityQueue(const std::vector<T> &v);

    // Construtor que inicializa a fila de prioridade com uma lista de inicialização
    PriorityQueue(std::initializer_list<T> il);

    // Destrutor que limpa a fila de prioridade
    ~PriorityQueue() = default;

    // Verifica se a fila de prioridade está vazia
    bool empty() const noexcept { return heapSize == 0; }

    // Retorna o tamanho da fila de prioridade
    size_t size() const noexcept { return heapSize; }

    // reserva o tamanho da fila de prioridade
    void reserve(size_t quant) { m_vec.reserve(quant + 1); }

    // Ordena os elementos de um vetor usando o algoritmo Heap Sort
    static void heapSort(std::vector<T> &vec);

    // Remove o elemento no índice especificado, lançando uma exceção se o índice estiver fora do intervalo
    void remove(const size_t &index);

    // Retorna o elemento de maior prioridade (raiz da heap), lançando uma exceção se a fila estiver vazia
    const T &top() const;

    // Insere um novo elemento na fila de prioridade
    void push(const T &value);

    // Remove o elemento de maior prioridade
    T pop();

    // Limpa a fila de prioridade, removendo todos os elementos
    void clear();

    // Troca o conteúdo desta fila de prioridade com outra fila de prioridade
    void swap(PriorityQueue &other) noexcept;
};

//---------------------------------------------------------------Implementação--------------------------------------------------------------------

template <typename T, typename Comparator>
PriorityQueue<T, Comparator>::PriorityQueue() : m_vec(), heapSize(0)
{
    m_vec.push_back(T()); // Adiciona um elemento fictício no índice 0 para facilitar o cálculo dos índices
}

template <typename T, typename Comparator>
PriorityQueue<T, Comparator>::PriorityQueue(const std ::vector<T> &v) : PriorityQueue()
{
    for (const T &element : v)
        m_vec.push_back(element);

    heapSize = m_vec.size() - 1; // O índice 0 é um espaço reservado, então o tamanho real é m_vec.size() - 1

    buildMaxHeap();
}

template <typename T, typename Comparator>
PriorityQueue<T, Comparator>::PriorityQueue(std::initializer_list<T> il) : PriorityQueue()
{
    for (const T &element : il)
        m_vec.push_back(element);

    heapSize = m_vec.size() - 1; // Ajusta o tamanho da heap

    buildMaxHeap();
}

template <typename T, typename Comparator>
void PriorityQueue<T, Comparator>::fixUp(size_t index)
{
    while (index != 1 and compare(m_vec[parent(index)], m_vec[index]))
    {
        std::swap(m_vec[index], m_vec[parent(index)]);
        index = parent(index);
    }
}

template <typename T, typename Comparator>
void PriorityQueue<T, Comparator>::fixDown(size_t index)
{
    while (left_child(index) <= heapSize)
    {
        size_t iLeft = left_child(index);
        size_t iRight = right_child(index);
        size_t iCompare = index;

        if (compare(m_vec[index], m_vec[iLeft]))
            iCompare = iLeft;

        if (iRight <= heapSize and compare(m_vec[iCompare], m_vec[iRight]))
            iCompare = iRight;

        if (iCompare != index)
        {
            std::swap(m_vec[index], m_vec[iCompare]);
            index = iCompare;
        }

        else
            break;
    }
}

template <typename T, typename Comparator>
void PriorityQueue<T, Comparator>::buildMaxHeap()
{
    for (size_t i = parent(m_vec.size()); i >= 1; i--)
        fixDown(i);
}

template <typename T, typename Comparator>
void PriorityQueue<T, Comparator>::heapSort(std::vector<T> &vec)
{
    PriorityQueue<T, Comparator> pq(vec);
    size_t originalSize = pq.size();

    for (size_t i = 0; i < originalSize; ++i)
        vec[i] = pq.pop(); // Remove o elemento de maior prioridade e o coloca no vetor
}

template <typename T, typename Comparator>
void PriorityQueue<T, Comparator>::remove(const size_t &index)
{
    if (index < 1 || index > heapSize)
        throw std::out_of_range("Index fora do intervalo");

    std::swap(m_vec[index], m_vec[heapSize--]); // Troca o elemento com o último e reduz o tamanho
    m_vec.pop_back();                           // Remove o último elemento, que agora é o menor

    if (compare(m_vec[parent(index)], m_vec[index]))
        fixUp(index); // Se o elemento é maior que o filho esquerdo, sobe na heap
    else
        fixDown(index); // Se o elemento é menor que os filhos, desce na heap
}

template <typename T, typename Comparator>
const T &PriorityQueue<T, Comparator>::top() const
{
    if (empty())
        throw std::underflow_error("Empty Queue");

    return m_vec[1];
}

template <typename T, typename Comparator>
void PriorityQueue<T, Comparator>::push(const T &value)
{
    m_vec.push_back(value);
    fixUp(++heapSize);
}

template <typename T, typename Comparator>
T PriorityQueue<T, Comparator>::pop()
{
    if (empty())
        throw std::underflow_error("Empty Queue");

    T value = m_vec[1]; // Guarda o valor que será removido

    std::swap(m_vec[1], m_vec[heapSize--]);
    fixDown(1);

    m_vec.pop_back(); // Remove o último elemento, que agora é o menor

    return std::move(value); // Retorna o valor removido
}

template <typename T, typename Comparator>
void PriorityQueue<T, Comparator>::clear()
{
    m_vec.clear();
    heapSize = 0;
}

template <typename T, typename Comparator>
void PriorityQueue<T, Comparator>::swap(PriorityQueue &other) noexcept
{
    std::swap(m_vec, other.m_vec);
    std::swap(heapSize, other.heapSize);
    std::swap(compare, other.compare);
}