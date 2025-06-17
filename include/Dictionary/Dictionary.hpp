#pragma once

/**
 * @brief Define a interface para uma estrutura de dados de dicionário (ou mapa).
 *
 * @details Um dicionário é uma coleção de pares chave-valor, onde cada chave é única.
 * Esta classe de interface pura (abstrata) estabelece o contrato que todas as
 * implementações de dicionário devem seguir.
 *
 * @tparam Key O tipo da chave. Deve ser único para cada elemento no dicionário.
 * @tparam Value O tipo do valor associado a uma chave.
 */
template <typename Key, typename Value>
class Dictionary
{
public:
    virtual ~Dictionary() = default;

    virtual void insert(const std::pair<Key, Value> &key_value) = 0;

    virtual void remove(const Key &key) = 0;

    virtual void update(const std::pair<Key, Value> &key_value) = 0;

    virtual bool contains(const Key &key) = 0;

    virtual Value &at(const Key &key) = 0;

    virtual Value &operator[](const Key &key) = 0;

    virtual void clear() = 0;

    virtual size_t size() const noexcept = 0;

    virtual bool empty() const noexcept = 0;

    virtual void print() const = 0;
};
