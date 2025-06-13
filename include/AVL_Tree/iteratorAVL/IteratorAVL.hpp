#pragma once

#include "AVL_Tree/node/Node.hpp"
#include <iterator>
#include <stack>

template <typename Key, typename Value>
class AVLTree;

template <typename Key, typename Value>
class IteratorAVL
{
private:
    using NodePtr = Node<Key, Value> *;
    std::stack<NodePtr> path;

public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::pair<Key, Value>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;
    using const_pointer = const value_type *;
    using const_reference = const value_type &;
    using NodeType = Node<Key, Value>;
    using NodePtrType = NodePtr;

    IteratorAVL() {}

    IteratorAVL(NodePtr root)
    {
        NodePtr current = root;
        while (current != nullptr)
        {
            path.push(current);
            current = current->left;
        }
    }

    // Dereference operator
    reference operator*() const
    {
        return path.top()->key;
    }

    // Arrow operator
    pointer operator->() const
    {
        return &(path.top()->key);
    }

    // Pre-increment operator
    IteratorAVL &operator++()
    {
        if (path.empty())
            return *this;

        NodePtr node = path.top();
        path.pop();

        if (node->right != nullptr)
        {
            NodePtr current = node->right;

            while (current != nullptr)
            {
                path.push(current);
                current = current->left;
            }
        }

        return *(this);
    }

    // Post-increment operator
    IteratorAVL operator++(int)
    {
        IteratorAVL temp = *this;
        ++(*this);
        return temp;
    }

    // Operador de igualdade
    bool operator==(const IteratorAVL &other) const
    {
        if (path.empty() && other.path.empty())
            return true;

        if (path.empty() || other.path.empty())
            return false;

        return path.top() == other.path.top();
    }

    // Operador de desigualdade
    bool operator!=(const IteratorAVL &other) const
    {
        return !(*this == other);
    }
};