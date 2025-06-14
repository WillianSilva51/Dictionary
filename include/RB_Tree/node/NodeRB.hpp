#pragma once

template <typename Key, typename Value>
struct NodeRB
{
    constexpr static bool BLACK = false;
    constexpr static bool RED = true;

    std::pair<Key, Value> key;
    bool color;
    NodeRB<Key, Value> *parent{nullptr};
    NodeRB<Key, Value> *left{nullptr};
    NodeRB<Key, Value> *right{nullptr};

    NodeRB(const std::pair<Key, Value> &key, const bool &color = RED, NodeRB<Key, Value> *parent, NodeRB<Key, Value> *left, NodeRB<Key, Value> *right)
        : key(key), color(color), left(left), right(right) {}
};