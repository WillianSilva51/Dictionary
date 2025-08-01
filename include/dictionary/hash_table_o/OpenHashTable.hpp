#pragma once

#include <iostream>
#include <list>
#include <vector>
#include <cmath>
#include <stdexcept>

#include "dictionary/Dictionary.hpp"
#include "dictionary/hash_table_o/Slot.hpp"

/**
 * @file OpenHashTable.hpp
 *
 * @brief Implementação de uma tabela hash aberta (Open Hash Table).
 *
 * Esta classe implementa uma tabela hash aberta, onde cada slot pode conter
 * um par chave-valor. A tabela utiliza endereçamento aberto para resolver colisões,
 * e permite inserção, remoção, atualização e busca de elementos.
 * A tabela é redimensionada automaticamente quando o fator de carga
 * ultrapassa um limite máximo definido pelo usuário.
 *
 * @tparam Key Tipo da chave usada para indexação.
 * @tparam Value Tipo do valor associado a cada chave.
 * @tparam Hash Tipo da função de hash usada para calcular os índices.
 *
 * @note A tabela é projetada para ser eficiente em termos de espaço e tempo,
 *   minimizando colisões e mantendo um bom desempenho em operações de inserção,
 *   busca e remoção.
 */
template <typename Key, typename Value, typename Hash = std::hash<Key>>
class OpenHashTable : public Dictionary<Key, Value>
{
private:
    // Quantidade de pares (chave, valor)
    size_t m_number_of_elements;

    // Tamanho atual da tabela
    size_t m_table_size;

    // O maior valor que o fator de carga pode ter.
    // Seja load_factor = m_number_of_elements/m_table_size.
    // Temos que load_factor <= m_max_load_factor.
    // Quando load_factor ultrapassa o valor de m_max_load_factor,
    // eh preciso executar a operacao de rehashing.
    float m_max_load_factor;

    // tabela
    std::vector<Slot<Key, Value>> m_table;

    // referencia para a funcao de codificacao
    Hash m_hashing;

    long long comparisons{0}; // contador de comparações para análise de desempenho

    long long collisions{0}; // contador de colisões para análise de desempenho

    /**
     * @brief Calcula o menor número primo que é maior ou igual a um dado número.
     *
     * Esta função é usada para determinar o tamanho da tabela hash, garantindo que
     * seja um número primo para ajudar a distribuir melhor os elementos.
     * Para entradas menores ou iguais a 2, retorna 3.
     *
     * @param x O número a partir do qual a busca pelo próximo primo se inicia.
     * @return size_t O menor número primo >= x (e > 2).
     */
    size_t get_next_prime(size_t x);

    /**
     * @brief Calcula o índice da tabela para uma determinada chave.
     *
     * O processo envolve duas etapas:
     * 1. Computar o código hash da chave `k` usando a função de hash `m_hashing`.
     * 2. Aplicar o método de sondagem quadrática para resolver colisões,
     *    onde o índice é ajustado com base no número de tentativas (`try_count`).
     * 3. Mapear o código hash para um índice no intervalo [0, m_table_size - 1]
     *    usando o método da divisão (resto da divisão).
     *
     * @param k A chave para a qual o índice será calculado.
     * @return size_t Um índice no intervalo [0, m_table_size - 1].
     */
    size_t hash_code(const Key &k, const size_t &try_count = 0) const;

    /**
     * @brief Encontra o índice de um elemento com a chave `key`.
     *
     * Este método percorre a tabela hash, verificando cada slot
     * até encontrar um slot vazio ou um slot ativo com a chave correspondente.
     * Se a chave for encontrada, retorna o índice do slot.
     * Se a chave não for encontrada, retorna (size_t)-1.
     *
     * @param key A chave a ser procurada.
     *
     * @return size_t O índice do slot onde a chave foi encontrada, ou (size_t)-1 se não encontrada.
     */
    size_t findIndex(const Key &key);

public:
    /**
     * @brief Construtor padrão. Cria uma tabela hash vazia.
     *
     * @param tableSize O número inicial de "buckets" (slots) na tabela.
     *                  Será ajustado para o próximo número primo maior ou igual.
     * @param load_factor O fator de carga máximo permitido antes de um rehash.
     */
    OpenHashTable(const size_t &tableSize = 19, const float &load_factor = 0.5f);

    /**
     * @brief Construtor que inicializa a tabela com uma lista de elementos.
     *
     * @param list Uma `std::initializer_list` de pares chave-valor para inserir na tabela.
     * @param tableSize O número inicial de "buckets" na tabela.
     * @param load_factor O fator de carga máximo.
     */
    OpenHashTable(const std::initializer_list<std::pair<Key, Value>> &list, const size_t &tableSize = 19, const float &load_factor = 0.5f);

    /**
     * @brief Cria e retorna uma cópia profunda (deep copy) da tabela hash.
     *
     * @return std::unique_ptr<Dictionary<Key, Value>> Um ponteiro para a nova instância clonada.
     */
    std::unique_ptr<Dictionary<Key, Value>> clone() const;

    /**
     * @brief Retorna o número de comparações realizadas durante as operações.
     *
     * Este método é útil para análise de desempenho, permitindo verificar quantas
     * comparações foram feitas ao longo das operações de inserção, busca e remoção.
     *
     * @return long long O número total de comparações realizadas.
     */
    long long getComparisons() const noexcept { return comparisons; }

    /**
     * @brief Retorna o número de colisões ocorridas durante as operações.
     *
     * Este método é útil para análise de desempenho, permitindo verificar quantas
     * colisões ocorreram ao longo das operações de inserção.
     *
     * @return long long O número total de colisões ocorridas.
     */
    long long getCollisions() const noexcept { return collisions; }

    /**
     * @brief Retorna o número de pares chave-valor na tabela.
     * @return size_t O número de elementos.
     */
    size_t size() const noexcept;

    /**
     * @brief Verifica se a tabela está vazia.
     * @return true se a tabela não contém elementos, false caso contrário.
     */
    bool empty() const noexcept;

    /**
     * @brief Retorna o número de "buckets" (slots) na tabela hash.
     * @return size_t O tamanho da tabela interna (número de listas de encadeamento).
     */
    size_t bucket_count() const noexcept;

    /**
     * @brief Retorna o índice do "bucket" onde um elemento com a chave `k` seria armazenado.
     *
     * @param k A chave a ser localizada.
     * @return size_t O índice do "bucket" correspondente.
     */
    size_t bucket(const Key &k) const;

    /**
     * @brief Remove todos os elementos da tabela, deixando-a com tamanho 0.
     */
    void clear();

    /**
     * @brief Retorna o fator de carga atual da tabela.
     * O fator de carga é a razão entre o número de elementos e o número de "buckets".
     * @return float O fator de carga atual.
     */
    float load_factor() const noexcept;

    /**
     * @brief Retorna o fator de carga máximo permitido.
     * Se `load_factor()` exceder este valor, um rehash é acionado.
     * @return float O fator de carga máximo.
     */
    float max_load_factor() const noexcept;

    /**
     * @brief Destrutor. Libera todos os recursos.
     */
    ~OpenHashTable() = default;

    /**
     * @brief Insere um novo par chave-valor na tabela.
     *
     * A inserção só ocorre se a chave ainda não existir na tabela. Se a inserção
     * fizer com que o fator de carga exceda o `max_load_factor`, um rehash é
     * executado para aumentar o tamanho da tabela.
     *
     * @param key_value O par `std::pair<Key, Value>` a ser inserido.
     */
    void insert(const std::pair<Key, Value> &key_value);

    /**
     * @brief Atualiza o valor associado a uma chave existente.
     *
     * Se a chave `key_value.first` for encontrada na tabela, seu valor
     * correspondente é atualizado para `key_value.second`. Se a chave não
     * existir, a função não realiza nenhuma operação.
     *
     * @param key_value O par `std::pair<Key, Value>` contendo a chave a ser
     *                  encontrada e o novo valor.
     */
    void update(const std::pair<Key, Value> &key_value);

    /**
     * @brief Verifica se a tabela contém um elemento com a chave especificada.
     *
     * @param k A chave a ser procurada.
     * @return true se um elemento com a chave `k` existir, false caso contrário.
     */
    bool contains(const Key &k);

    /**
     * @brief Acessa o valor associado a uma chave.
     *
     * Retorna uma referência ao valor correspondente à chave `k`.
     *
     * @param k A chave do elemento a ser acessado.
     * @return Value& Uma referência ao valor.
     * @throws std::out_of_range se a chave `k` não for encontrada na tabela.
     */
    Value &at(const Key &k);

    /**
     * @brief Acessa o valor associado a uma chave (versão const).
     *
     * Retorna uma referência constante ao valor correspondente à chave `k`.
     *
     * @param k A chave do elemento a ser acessado.
     * @return const Value& Uma referência constante ao valor.
     * @throws std::out_of_range se a chave `k` não for encontrada na tabela.
     */
    const Value &at(const Key &k) const;

    /**
     * @brief Redimensiona a tabela hash para um novo tamanho.
     *
     * O tamanho da tabela é ajustado para o próximo número primo maior ou igual a `m`.
     * Se `m` for menor que o tamanho atual, a tabela não é redimensionada.
     * Se `m` for maior, a tabela é redimensionada e todos os elementos existentes
     * são re-hashados para o novo tamanho.
     *
     * @param m O novo tamanho desejado para a tabela.
     *
     * @note O tamanho da tabela deve ser um número primo para melhor distribuição dos
     *       elementos e evitar colisões.
     */
    void rehash(size_t m);

    /**
     * @brief Remove um elemento da tabela pela chave.
     *
     * Se um elemento com a chave `k` existir, ele é removido da tabela e o
     * número de elementos é decrementado. Se a chave não for encontrada,
     * a função não realiza nenhuma operação.
     *
     * @param k A chave do elemento a ser removido.
     */
    void remove(const Key &k);

    /**
     * @brief Reserva espaço para pelo menos `n` elementos.
     *
     * Se a capacidade atual não for suficiente para `n` elementos (considerando o
     * `max_load_factor`), a tabela é redimensionada (rehash) para acomodá-los.
     * A verificação é `n > bucket_count() * max_load_factor()`.
     *
     * @param n O número mínimo de elementos que a tabela deve ser capaz de conter.
     */
    void reserve(size_t n) noexcept;

    /**
     * @brief Define o fator de carga máximo.
     *
     * Altera o fator de carga máximo para `lf`. Após a alteração, a tabela pode
     * ser redimensionada se o fator de carga atual exceder o novo máximo.
     *
     * @param lf O novo valor para o fator de carga máximo (deve ser > 0).
     * @throws std::out_of_range se `lf` não for positivo.
     */
    void set_max_load_factor(float lf);

    /**
     * @brief Acessa ou insere um elemento.
     *
     * Se a chave `k` existir, retorna uma referência ao seu valor.
     * Se não existir, insere um novo elemento com a chave `k` (usando o
     * construtor padrão de `Value`) e retorna uma referência ao novo valor.
     *
     * @param k A chave do elemento a ser acessado ou inserido.
     * @return Value& Uma referência ao valor do elemento.
     */
    Value &operator[](const Key &k);

    /**
     * @brief Acessa um elemento (versão const).
     *
     * Se a chave `k` existir, retorna uma referência constante ao seu valor.
     *
     * @param k A chave do elemento a ser acessado.
     * @return const Value& Uma referência constante ao valor do elemento.
     * @throws std::out_of_range se a chave `k` não for encontrada.
     */
    const Value &operator[](const Key &k) const;

    /**
     * @brief Imprime o conteúdo da tabela no formato [chave1:valor1, chave2:valor2, ...].
     *
     * Útil para depuração. A ordem dos elementos não é garantida.
     */
    void print() const;

    /**
     * @brief Aplica uma função a cada par chave-valor na tabela.
     *
     * Itera sobre todos os elementos da tabela e executa a função `func` para cada um.
     * A ordem de iteração não é garantida.
     *
     * @param func A função a ser aplicada. Deve aceitar um `const std::pair<Key, Value>&`.
     */
    void forEach(const std::function<void(const std::pair<Key, Value> &)> &func) const;
};

//------------------------------------------------------------IMPLEMENTAÇÕES---------------------------------------------------------------------

template <typename Key, typename Value, typename Hash>
size_t OpenHashTable<Key, Value, Hash>::get_next_prime(size_t x)
{
    if (x <= 2)
        return 3;

    x = (x % 2 == 0) ? x + 1 : x;
    bool not_prime = true;

    while (not_prime)
    {
        not_prime = false;
        for (int i = 3; i <= sqrt(x); i += 2)
        {
            if (x % i == 0)
            {
                not_prime = true;
                break;
            }
        }
        x += 2;
    }

    return x - 2;
}

template <typename Key, typename Value, typename Hash>
size_t OpenHashTable<Key, Value, Hash>::hash_code(const Key &k, const size_t &try_count) const
{
    return (m_hashing(k) + (try_count * try_count)) % m_table_size;
}

template <typename Key, typename Value, typename Hash>
OpenHashTable<Key, Value, Hash>::OpenHashTable(const size_t &tableSize, const float &load_factor) : m_number_of_elements(0), m_table_size(tableSize)
{
    m_table.resize(m_table_size);

    if (load_factor <= 0)
        m_max_load_factor = 0.5f;
    else
        m_max_load_factor = load_factor;
}

template <typename Key, typename Value, typename Hash>
OpenHashTable<Key, Value, Hash>::OpenHashTable(const std::initializer_list<std::pair<Key, Value>> &list, const size_t &tableSize, const float &load_factor) : OpenHashTable(tableSize, load_factor)
{
    for (const auto &pair : list)
        insert(pair);
}

template <typename Key, typename Value, typename Hash>
std::unique_ptr<Dictionary<Key, Value>> OpenHashTable<Key, Value, Hash>::clone() const
{
    return std::make_unique<OpenHashTable<Key, Value, Hash>>(*this);
}

template <typename Key, typename Value, typename Hash>
size_t OpenHashTable<Key, Value, Hash>::size() const noexcept
{
    return m_number_of_elements;
}

template <typename Key, typename Value, typename Hash>
bool OpenHashTable<Key, Value, Hash>::empty() const noexcept
{
    return m_number_of_elements == 0;
}

template <typename Key, typename Value, typename Hash>
size_t OpenHashTable<Key, Value, Hash>::bucket_count() const noexcept
{
    return m_table_size;
}

template <typename Key, typename Value, typename Hash>
size_t OpenHashTable<Key, Value, Hash>::bucket(const Key &k) const
{
    return hash_code(k);
}

template <typename Key, typename Value, typename Hash>
float OpenHashTable<Key, Value, Hash>::load_factor() const noexcept
{
    return static_cast<float>(m_number_of_elements) / m_table_size;
}

template <typename Key, typename Value, typename Hash>
float OpenHashTable<Key, Value, Hash>::max_load_factor() const noexcept
{
    return m_max_load_factor;
}

template <typename Key, typename Value, typename Hash>
void OpenHashTable<Key, Value, Hash>::clear()
{
    m_table.clear();
    m_table.resize(m_table_size);

    m_number_of_elements = 0;
}

template <typename Key, typename Value, typename Hash>
void OpenHashTable<Key, Value, Hash>::insert(const std::pair<Key, Value> &key_value)
{
    if (load_factor() >= m_max_load_factor)
        rehash(m_table_size * 2);

    size_t hash_index{(size_t)-1};
    size_t first_deleted_index{(size_t)-1};

    for (size_t i = 0; i < m_table_size; i++)
    {
        size_t current_index = hash_code(key_value.first, i);

        if (m_table[current_index].is_empty())
        {
            hash_index = current_index;
            break;
        }
        else if (m_table[current_index].is_active())
        {
            comparisons++;
            if (m_table[current_index].data.first == key_value.first)
                return;
            collisions++;
        }
        else
        {
            if (first_deleted_index == (size_t)-1)
                first_deleted_index = current_index;
        }
    }

    if (first_deleted_index != (size_t)-1)
        hash_index = first_deleted_index;

    if (hash_index == (size_t)-1)
        throw std::out_of_range("Hash table is full, cannot insert new element");

    m_table[hash_index] = Slot<Key, Value>(key_value);
    m_number_of_elements++;
}

template <typename Key, typename Value, typename Hash>
void OpenHashTable<Key, Value, Hash>::update(const std::pair<Key, Value> &key_value)
{
    size_t hash_index = findIndex(key_value.first);

    if (hash_index != (size_t)-1)
        m_table[hash_index].data.second = key_value.second;
    else
        throw std::out_of_range("Key not found in the hash table");
}

template <typename Key, typename Value, typename Hash>
size_t OpenHashTable<Key, Value, Hash>::findIndex(const Key &key)
{
    size_t hash_index{(size_t)-1};

    for (size_t i = 0; i < m_table_size; i++)
    {
        size_t current_index = hash_code(key, i);

        if (m_table[current_index].is_empty())
            break;

        comparisons++;
        if (m_table[current_index].is_active() and m_table[current_index].data.first == key)
        {
            hash_index = current_index;
            break;
        }
    }

    return hash_index;
}

template <typename Key, typename Value, typename Hash>
bool OpenHashTable<Key, Value, Hash>::contains(const Key &k)
{
    return findIndex(k) != (size_t)-1;
}

template <typename Key, typename Value, typename Hash>
Value &OpenHashTable<Key, Value, Hash>::at(const Key &k)
{
    size_t hash_index = findIndex(k);

    if (hash_index != (size_t)-1)
        return m_table[hash_index].data.second;
    else
        throw std::out_of_range("Key not found in the hash table");
}

template <typename Key, typename Value, typename Hash>
const Value &OpenHashTable<Key, Value, Hash>::at(const Key &k) const
{
    size_t hash_index = findIndex(k);

    if (hash_index != (size_t)-1)
        return m_table[hash_index].data.second;
    else
        throw std::out_of_range("Key not found in the hash table");
}

template <typename Key, typename Value, typename Hash>
void OpenHashTable<Key, Value, Hash>::rehash(size_t m)
{
    size_t new_table_size = get_next_prime(m);

    if (new_table_size > m_table_size)
    {
        std::vector<Slot<Key, Value>> aux;
        m_table.swap(aux);
        m_table.resize(new_table_size);

        m_table_size = new_table_size;
        m_number_of_elements = 0;

        for (auto &slot : aux)
            if (slot.is_active())
                insert({slot.data.first, slot.data.second});
    }
}

template <typename Key, typename Value, typename Hash>
void OpenHashTable<Key, Value, Hash>::remove(const Key &k)
{
    size_t slot = findIndex(k); // calcula o slot em que estaria a chave

    if (slot != (size_t)-1)
    {
        m_number_of_elements--;
        m_table[slot].status = HashTableStatus::DELETED;
    }
}

template <typename Key, typename Value, typename Hash>
void OpenHashTable<Key, Value, Hash>::reserve(size_t n) noexcept
{
    if (n > m_table_size * m_max_load_factor)
        rehash(n / m_max_load_factor);
}

template <typename Key, typename Value, typename Hash>
void OpenHashTable<Key, Value, Hash>::set_max_load_factor(float lf)
{
    if (lf <= 0)
        throw std::out_of_range("max load factor must be greater than 0");

    m_max_load_factor = lf;

    // Se o novo fator de carga for menor que o atual,
    // podemos precisar redimensionar a tabela.
    if (load_factor() > m_max_load_factor)
        reserve(m_number_of_elements);
}

template <typename Key, typename Value, typename Hash>
Value &OpenHashTable<Key, Value, Hash>::operator[](const Key &k)
{
    size_t hash_index = findIndex(k);

    if (hash_index != (size_t)-1)
        return m_table[hash_index].data.second;
    else
        insert({k, Value{}}); // insere um novo elemento com valor padrão

    return m_table[findIndex(k)].data.second; // retorna o valor associado a chave
}

template <typename Key, typename Value, typename Hash>
const Value &OpenHashTable<Key, Value, Hash>::operator[](const Key &k) const
{
    return at(k); // chama a funcao at para obter o valor associado a chave
}

template <typename Key, typename Value, typename Hash>
void OpenHashTable<Key, Value, Hash>::print() const
{
    forEach([](const std::pair<Key, Value> &par)
            { std::cout << "[" << par.first << ", " << par.second << "]" << std::endl; });
}

template <typename Key, typename Value, typename Hash>
void OpenHashTable<Key, Value, Hash>::forEach(const std::function<void(const std::pair<Key, Value> &)> &func) const
{
    for (const auto &slot : m_table)
        if (slot.is_active()) // verifica se o slot esta ativo
            func(slot.data);  // aplica a funcao a cada par chave-valor
}