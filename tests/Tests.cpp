#include "gtest/gtest.h"
#include "dictionary/Dictionary.hpp"
#include "dictionary/avl_tree/AVLTree.hpp"
#include "dictionary/rb_tree/RedBlackTree.hpp"
#include "dictionary/hash_table_c/ChainedHashTable.hpp"
#include "dictionary/hash_table_o/OpenHashTable.hpp"

#include <string>
#include <vector>
#include <algorithm>

// Test fixture genérico para a interface Dictionary
template <typename T>
class DictionaryTest : public ::testing::Test
{
protected:
    std::unique_ptr<Dictionary<int, std::string>> dict;

    void SetUp() override
    {
        // Cria uma nova instância da implementação de dicionário para cada teste
        dict = std::make_unique<T>();
    }

    void TearDown() override
    {
        // unique_ptr lida com a desalocação automaticamente
    }
};

// Declaração da suíte de testes tipados
TYPED_TEST_SUITE_P(DictionaryTest);

// Testa o estado inicial de um dicionário recém-criado
TYPED_TEST_P(DictionaryTest, DefaultConstructor)
{
    EXPECT_TRUE(this->dict->empty());
    EXPECT_EQ(this->dict->size(), static_cast<size_t>(0));
}

// Testa a inserção de um único elemento e o tamanho
TYPED_TEST_P(DictionaryTest, InsertAndSize)
{
    this->dict->insert({1, "one"});
    EXPECT_FALSE(this->dict->empty());
    EXPECT_EQ(this->dict->size(), static_cast<size_t>(1));
}

// Testa se a inserção de chaves duplicadas é ignorada
TYPED_TEST_P(DictionaryTest, InsertDuplicates)
{
    this->dict->insert({1, "one"});
    this->dict->insert({1, "uno"}); // Chave duplicada
    EXPECT_EQ(this->dict->size(), static_cast<size_t>(1));
    EXPECT_EQ(this->dict->at(1), "one"); // O valor original deve ser mantido
}

// Testa a funcionalidade de 'contains'
TYPED_TEST_P(DictionaryTest, Contains)
{
    this->dict->insert({10, "ten"});
    this->dict->insert({20, "twenty"});
    EXPECT_TRUE(this->dict->contains(10));
    EXPECT_TRUE(this->dict->contains(20));
    EXPECT_FALSE(this->dict->contains(30));
}

// Testa a remoção de elementos
TYPED_TEST_P(DictionaryTest, Remove)
{
    this->dict->insert({1, "one"});
    this->dict->insert({2, "two"});
    this->dict->insert({3, "three"});

    this->dict->remove(2);
    EXPECT_EQ(this->dict->size(), static_cast<size_t>(2));
    EXPECT_FALSE(this->dict->contains(2));
    EXPECT_TRUE(this->dict->contains(1));
    EXPECT_TRUE(this->dict->contains(3));

    // Tenta remover um elemento não existente
    this->dict->remove(42);
    EXPECT_EQ(this->dict->size(), static_cast<size_t>(2));
}

// Testa a remoção de um nó com dois filhos (importante para árvores)
TYPED_TEST_P(DictionaryTest, RemoveNodeWithTwoChildren)
{
    // A ordem de inserção pode influenciar a estrutura da árvore
    this->dict->insert({20, "twenty"});
    this->dict->insert({10, "ten"});
    this->dict->insert({30, "thirty"});
    this->dict->insert({5, "five"});
    this->dict->insert({15, "fifteen"});

    // Remover a raiz (20), que tem dois filhos
    this->dict->remove(20);
    EXPECT_EQ(this->dict->size(), static_cast<size_t>(4));
    EXPECT_FALSE(this->dict->contains(20));
    EXPECT_TRUE(this->dict->contains(10));
    EXPECT_TRUE(this->dict->contains(30));
    EXPECT_TRUE(this->dict->contains(5));
    EXPECT_TRUE(this->dict->contains(15));
}

// Testa a função 'at' para acesso e exceções
TYPED_TEST_P(DictionaryTest, At)
{
    this->dict->insert({1, "one"});
    EXPECT_EQ(this->dict->at(1), "one");
    EXPECT_THROW(this->dict->at(2), std::out_of_range);

    // Modificar valor através da referência
    this->dict->at(1) = "uno";
    EXPECT_EQ(this->dict->at(1), "uno");
}

// Testa a função 'update'
TYPED_TEST_P(DictionaryTest, Update)
{
    this->dict->insert({1, "one"});
    this->dict->update({1, "uno"});
    EXPECT_EQ(this->dict->at(1), "uno");

    // 'update' em uma chave não existente deve lançar uma exceção
    EXPECT_THROW(this->dict->update({2, "dos"}), std::out_of_range);
}

// Testa o operador de colchetes []
TYPED_TEST_P(DictionaryTest, BracketOperator)
{
    (*this->dict)[1] = "one"; // Inserir
    EXPECT_EQ(this->dict->size(), static_cast<size_t>(1));
    EXPECT_EQ((*this->dict)[1], "one");

    (*this->dict)[1] = "uno"; // Atualizar
    EXPECT_EQ(this->dict->size(), static_cast<size_t>(1));
    EXPECT_EQ((*this->dict)[1], "uno");

    (*this->dict)[2] = "two"; // Inserir
    EXPECT_EQ(this->dict->size(), static_cast<size_t>(2));
    EXPECT_EQ((*this->dict)[2], "two");
}

// Testa a limpeza do dicionário
TYPED_TEST_P(DictionaryTest, Clear)
{
    this->dict->insert({1, "one"});
    this->dict->insert({2, "two"});
    this->dict->clear();
    EXPECT_TRUE(this->dict->empty());
    EXPECT_EQ(this->dict->size(), static_cast<size_t>(0));
    EXPECT_FALSE(this->dict->contains(1));
}

// Testa a clonagem do dicionário
TYPED_TEST_P(DictionaryTest, Clone)
{
    this->dict->insert({1, "one"});
    this->dict->insert({2, "two"});

    auto clone = this->dict->clone();
    ASSERT_NE(clone, nullptr);

    // Verifica se não são o mesmo objeto
    EXPECT_NE(clone.get(), this->dict.get());

    // Verifica se o conteúdo é o mesmo
    EXPECT_EQ(clone->size(), this->dict->size());
    EXPECT_TRUE(clone->contains(1));
    EXPECT_EQ(clone->at(1), "one");
    EXPECT_TRUE(clone->contains(2));
    EXPECT_EQ(clone->at(2), "two");

    // Modifica o original e verifica se o clone não foi afetado (deep copy)
    this->dict->remove(1);
    EXPECT_FALSE(this->dict->contains(1));
    EXPECT_TRUE(clone->contains(1)); // O clone ainda deve ter o elemento
    EXPECT_EQ(clone->size(), static_cast<size_t>(2));
}

// Testa a iteração com forEach
TYPED_TEST_P(DictionaryTest, ForEach)
{
    this->dict->insert({3, "three"});
    this->dict->insert({1, "one"});
    this->dict->insert({2, "two"});

    std::vector<int> keys;
    this->dict->forEach([&keys](const std::pair<int, std::string> &pair)
                        { keys.push_back(pair.first); });

    // Para árvores, a ordem deve ser mantida. Para a tabela hash, não.
    // Então só verificamos se os elementos estão presentes.
    EXPECT_EQ(keys.size(), static_cast<size_t>(3));
    std::sort(keys.begin(), keys.end());
    EXPECT_EQ(keys[0], 1);
    EXPECT_EQ(keys[1], 2);
    EXPECT_EQ(keys[2], 3);
}

// Registra todos os testes definidos acima para a suíte
REGISTER_TYPED_TEST_SUITE_P(DictionaryTest,
                            DefaultConstructor,
                            InsertAndSize,
                            InsertDuplicates,
                            Contains,
                            Remove,
                            RemoveNodeWithTwoChildren,
                            At,
                            Update,
                            BracketOperator,
                            Clear,
                            Clone,
                            ForEach);

// #############################################################################
// ## INSTRUÇÕES DE DEPURAÇÃO:
// ## Para isolar um teste, comente as outras implementações na linha abaixo.
// ## Deixe apenas uma ativa para descobrir qual delas está com vazamento.
// #############################################################################
using Implementations = ::testing::Types<
    AVLTree<int, std::string>,
    RedBlackTree<int, std::string>,
    ChainedHashTable<int, std::string>,
    OpenHashTable<int, std::string>>;

// Instancia a suíte de testes para cada uma das implementações
INSTANTIATE_TYPED_TEST_SUITE_P(MyImplementations, DictionaryTest, Implementations);