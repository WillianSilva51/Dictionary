#include "gtest/gtest.h"

#include "dictionary/avl_tree/AVLTree.hpp"
#include "dictionary/rb_tree/RedBlackTree.hpp"
#include "dictionary/hash_table_c/ChainedHashTable.hpp"

TEST(DictionaryTests, AVLTreeInsertAndFind)
{
    AVLTree<int, std::string> tree;
    tree.insert({1, "one"});
    tree.insert({2, "two"});
    tree.insert({3, "three"});

    EXPECT_EQ(tree.at(1), "one");
    EXPECT_EQ(tree.at(2), "two");
    EXPECT_EQ(tree.at(3), "three");
}

TEST(DictionaryTests, AVLTreeRemove)
{
    AVLTree<int, std::string> tree;
    tree.insert({1, "one"});
    tree.insert({2, "two"});
    tree.insert({3, "three"});

    tree.remove(2);
    EXPECT_THROW(tree.at(2), std::out_of_range);
}

TEST(DictionaryTests, HashTable)
{
    ChainedHashTable<int, std::string> hashTable(10, 0.75f);
    hashTable.insert({1, "one"});
    hashTable.insert({2, "two"});
    hashTable.insert({3, "three"});

    EXPECT_EQ(hashTable.at(1), "one");
    EXPECT_EQ(hashTable.at(2), "two");
    EXPECT_EQ(hashTable.at(3), "three");

    hashTable.remove(2);
    EXPECT_THROW(hashTable.at(2), std::out_of_range);
}

TEST(DictionaryTests, HashTableRehash)
{
    ChainedHashTable<int, std::string> hashTable(2, 0.75f);
    hashTable.insert({1, "one"});
    hashTable.insert({2, "two"}); // This should trigger a rehash

    EXPECT_EQ(hashTable.size(), 2);
    EXPECT_EQ(hashTable.at(1), "one");
    EXPECT_EQ(hashTable.at(2), "two");
}

TEST(DictionaryTests, RedBlackTreeInsertAndFind)
{
    RedBlackTree<int, std::string> tree;
    tree.insert({1, "one"});
    tree.insert({2, "two"});
    tree.insert({3, "three"});

    EXPECT_EQ(tree.at(1), "one");
    EXPECT_EQ(tree.at(2), "two");
    EXPECT_EQ(tree.at(3), "three");
}