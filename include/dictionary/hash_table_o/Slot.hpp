#pragma once

enum class HashTableStatus
{
    EMPTY,
    ACTIVE,
    DELETED
};

template <typename Key, typename Value>
struct Slot
{
    std::pair<Key, Value> data{};
    HashTableStatus status{HashTableStatus::EMPTY};

    Slot() = default;

    Slot(const std::pair<Key, Value> &pair)
        : data(pair), status(HashTableStatus::ACTIVE) {}

    Slot(const Key &key, const Value &value)
        : data({key, value}), status(HashTableStatus::ACTIVE) {}

    bool is_empty() const noexcept
    {
        return status == HashTableStatus::EMPTY;
    }

    bool is_active() const noexcept
    {
        return status == HashTableStatus::ACTIVE;
    }

    bool is_deleted() const noexcept
    {
        return status == HashTableStatus::DELETED;
    }
};
