#pragma once

#include <memory>

#include <Dictionary/DictionaryType.hpp>

template <typename Key, typename Value>
class DynamicDictionary
{
private:
    std::unique_ptr<IDictionary<Key, Value>> dictionary{nullptr};
    DictionaryType type{DictionaryType::AVL}; // O tipo de dicionário a ser utilizado

public:
    DynamicDictionary(DictionaryType type = DictionaryType::AVL) : type(type), dictionary(create_dictionary<Key, Value>(type))
    {
        if (!dictionary)
            throw std::runtime_error("Falha ao criar o Dicionário do tipo específico");
    }

    void insert(const std::pair<Key, Value> &key_value)
    {
        dictionary->insert(key_value);
    }

    void remove(const Key &key)
    {
        dictionary->remove(key);
    }

    void update(const std::pair<Key, Value> &key_value)
    {
        dictionary->update(key_value);
    }

    bool contains(const Key &key)
    {
        return dictionary->contains(key);
    }

    Value &at(const Key &key)
    {
        return dictionary->at(key);
    }

    Value &operator[](const Key &key)
    {
        return dictionary->operator[](key);
    }

    void clear()
    {
        dictionary->clear();
    }

    size_t size() const noexcept
    {
        return dictionary->size();
    }

    bool empty() const noexcept
    {
        return dictionary->empty();
    }

    void print() const
    {
        dictionary->print();
    }

    IDictionary<Key, Value> &get_dictionary() const
    {
        return *dictionary;
    }
};