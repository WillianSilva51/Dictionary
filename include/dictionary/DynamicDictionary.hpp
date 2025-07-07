#pragma once

#include "dictionary/DictionaryFactory.hpp"

/**
 * @class DynamicDictionary
 * @brief Uma classe de dicionário dinâmico que atua como um wrapper (invólucro).
 *
 * Esta classe permite a criação de diferentes tipos de dicionários (como Árvore Rubro-Negra, Tabela Hash, etc.)
 * em tempo de execução, com base no `DictionaryType` fornecido. Ela delega todas as operações
 * para a implementação de dicionário subjacente que ela encapsula, fornecendo uma interface uniforme.
 *
 * @tparam Key O tipo da chave dos elementos no dicionário.
 * @tparam Value O tipo do valor associado a cada chave.
 */
template <typename Key, typename Value>
class DynamicDictionary : public Dictionary<Key, Value>
{
private:
    DictionaryType type;                                         // O tipo de dicionário a ser utilizado
    std::unique_ptr<Dictionary<Key, Value>> dictionary{nullptr}; // Ponteiro inteligente para a implementação do dicionário subjacente

    /**
     * @brief Verifica se o dicionário foi criado corretamente.
     * @throws std::runtime_error se o dicionário não foi criado corretamente.
     */
    void check_dictionary() const
    {
        if (!dictionary)
            throw std::runtime_error("Falha ao criar o dicionário. Verifique o tipo especificado.");
    }

public:
    /**
     * @brief Construtor que cria um dicionário do tipo especificado.
     * @param type O tipo de dicionário a ser criado (por exemplo, RBTREE, HASHTABLE). O padrão é RBTREE.
     * @throws std::runtime_error se o tipo de dicionário for inválido e a criação falhar.
     */
    DynamicDictionary(DictionaryType type = DictionaryType::RBTREE) : type(type), dictionary(create_dictionary<Key, Value>(type))
    {
        check_dictionary();
    }

    /**
     * @brief Construtor de cópia.
     *
     * Cria uma cópia profunda do outro dicionário, clonando sua implementação subjacente
     * através do método `clone()`.
     *
     * @param other O `DynamicDictionary` a ser copiado.
     */
    DynamicDictionary(const DynamicDictionary &other) : type(other.type), dictionary(other.dictionary->clone())
    {
        check_dictionary();
    }

    /**
     * @brief Construtor que inicializa o dicionário com uma lista de pares chave-valor.
     * @param list A `std::initializer_list` contendo os pares chave-valor para popular o dicionário.
     * @param type O tipo de dicionário a ser criado. O padrão é RBTREE.
     */
    DynamicDictionary(const std::initializer_list<std::pair<Key, Value>> list, DictionaryType type = DictionaryType::RBTREE)
        : type(type), dictionary(create_dictionary<Key, Value>(type, list))
    {
        check_dictionary();
    }

    /**
     * @brief Cria e retorna um clone (cópia profunda) do objeto DynamicDictionary atual.
     * @return Um `std::unique_ptr<Dictionary<Key, Value>>` para o novo dicionário clonado.
     */
    std::unique_ptr<Dictionary<Key, Value>> clone() const
    {
        return std::make_unique<DynamicDictionary<Key, Value>>(*this);
    }

    /**
     * @brief Insere um novo par chave-valor no dicionário.
     *
     * A operação é delegada para a implementação do dicionário subjacente.
     *
     * @param key_value O `std::pair<Key, Value>` a ser inserido.
     */
    void insert(const std::pair<Key, Value> &key_value)
    {
        dictionary->insert(key_value);
    }

    /**
     * @brief Remove um elemento do dicionário com base na chave.
     *
     * A operação é delegada para a implementação do dicionário subjacente.
     *
     * @param key A chave do elemento a ser removido.
     */
    void remove(const Key &key)
    {
        dictionary->remove(key);
    }

    /**
     * @brief Atualiza o valor de uma chave existente.
     *
     * A operação é delegada para a implementação do dicionário subjacente.
     *
     * @param key_value O `std::pair<Key, Value>` contendo a chave a ser encontrada e o novo valor.
     */
    void update(const std::pair<Key, Value> &key_value)
    {
        dictionary->update(key_value);
    }

    /**
     * @brief Verifica se o dicionário contém um elemento com a chave especificada.
     * @param key A chave a ser procurada.
     * @return `true` se a chave existir, `false` caso contrário.
     */
    bool contains(const Key &key)
    {
        return dictionary->contains(key);
    }

    /**
     * @brief Acessa o valor associado a uma chave.
     *
     * Lança uma exceção se a chave não for encontrada.
     *
     * @param key A chave do valor a ser acessado.
     * @return Uma referência ao valor associado à chave.
     * @throws std::out_of_range (ou similar, dependendo da implementação subjacente) se a chave não for encontrada.
     */
    Value &at(const Key &key)
    {
        return dictionary->at(key);
    }

    /**
     * @brief Operador de atribuição de cópia.
     *
     * Substitui o conteúdo do dicionário atual por uma cópia profunda do outro dicionário.
     *
     * @param other O outro `DynamicDictionary` a ser copiado.
     * @return Uma referência a este objeto (`*this`).
     */
    DynamicDictionary &operator=(const DynamicDictionary &other)
    {
        if (this != &other)
        {
            dictionary = other.dictionary->clone;
        }

        return *this;
    }

    /**
     * @brief Acessa o valor associado a uma chave.
     *
     * Se a chave não existir, ela é inserida com um valor padrão e uma referência a esse novo valor é retornada.
     *
     * @param key A chave do elemento a ser acessado ou inserido.
     * @return Uma referência ao valor associado à chave.
     */
    Value &operator[](const Key &key)
    {
        return dictionary->operator[](key);
    }

    /**
     * @brief Remove todos os elementos do dicionário.
     */
    void clear()
    {
        dictionary->clear();
    }

    /**
     * @brief Retorna o número de elementos no dicionário.
     * @return O número de pares chave-valor.
     */
    size_t size() const noexcept
    {
        return dictionary->size();
    }

    /**
     * @brief Verifica se o dicionário está vazio.
     * @return `true` se o dicionário estiver vazio, `false` caso contrário.
     */
    bool empty() const noexcept
    {
        return dictionary->empty();
    }

    /**
     * @brief Imprime o conteúdo do dicionário na saída padrão.
     *
     * O formato da impressão depende da implementação do dicionário subjacente.
     */
    void print() const
    {
        dictionary->print();
    }

    /**
     * @brief Aplica uma função a cada par chave-valor no dicionário.
     * @param func A função (`std::function`) a ser executada para cada elemento. A função recebe um `const std::pair<Key, Value>&`.
     */
    void forEach(const std::function<void(const std::pair<Key, Value> &)> &func) const
    {
        dictionary->forEach(func);
    }

    /**
     * @brief Obtém uma referência para a implementação do dicionário subjacente.
     * @return Uma referência ao objeto `Dictionary<Key, Value>` encapsulado.
     */
    Dictionary<Key, Value> &get_dictionary() const
    {
        return *dictionary;
    }
};