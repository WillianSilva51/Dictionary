#include "gtest/gtest.h"
#include "dictionary/Dictionary.hpp"
#include "dictionary/avl_tree/AVLTree.hpp"
#include "dictionary/rb_tree/RedBlackTree.hpp"
#include "dictionary/hash_table_c/ChainedHashTable.hpp"
#include "dictionary/hash_table_o/OpenHashTable.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>

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

//==================================================================================
// TESTES ESPECÍFICOS PARA ÁRVORE AVL
// Foco: Forçar todos os tipos de rotações na inserção e remoção.
//==================================================================================
class AVLTreeSpecificTest : public ::testing::Test
{
protected:
    AVLTree<int, std::string> avl;
};

// Força uma rotação simples à direita (Left-Left case)
TEST_F(AVLTreeSpecificTest, InsertTriggersSingleRightRotation)
{
    avl.insert({30, "thirty"});
    avl.insert({20, "twenty"});
    avl.insert({10, "ten"}); // Causa desbalanceamento LL no nó 30

    // Após a rotação, 20 deve ser a nova raiz.
    // A verificação é feita checando o tamanho e a presença das chaves,
    // mas o principal é garantir que a inserção não quebrou a estrutura.
    EXPECT_EQ(avl.size(), static_cast<size_t>(3));
    EXPECT_TRUE(avl.contains(10));
    EXPECT_TRUE(avl.contains(20));
    EXPECT_TRUE(avl.contains(30));
    // Um teste mais profundo (caixa-branca) poderia verificar quem é a raiz.
}

// Força uma rotação simples à esquerda (Right-Right case)
TEST_F(AVLTreeSpecificTest, InsertTriggersSingleLeftRotation)
{
    avl.insert({10, "ten"});
    avl.insert({20, "twenty"});
    avl.insert({30, "thirty"}); // Causa desbalanceamento RR no nó 10

    EXPECT_EQ(avl.size(), static_cast<size_t>(3));
    EXPECT_TRUE(avl.contains(10));
    EXPECT_TRUE(avl.contains(20));
    EXPECT_TRUE(avl.contains(30));
}

// Força uma rotação dupla direita-esquerda (Right-Left case)
TEST_F(AVLTreeSpecificTest, InsertTriggersRightLeftRotation)
{
    avl.insert({10, "ten"});
    avl.insert({30, "thirty"});
    avl.insert({20, "twenty"}); // Causa desbalanceamento RL no nó 10

    EXPECT_EQ(avl.size(), static_cast<size_t>(3));
    EXPECT_TRUE(avl.contains(10));
    EXPECT_TRUE(avl.contains(20));
    EXPECT_TRUE(avl.contains(30));
}

// Força uma rotação dupla esquerda-direita (Left-Right case)
TEST_F(AVLTreeSpecificTest, InsertTriggersLeftRightRotation)
{
    avl.insert({30, "thirty"});
    avl.insert({10, "ten"});
    avl.insert({20, "twenty"}); // Causa desbalanceamento LR no nó 30

    EXPECT_EQ(avl.size(), static_cast<size_t>(3));
    EXPECT_TRUE(avl.contains(10));
    EXPECT_TRUE(avl.contains(20));
    EXPECT_TRUE(avl.contains(30));
}

// Testa a remoção que deve acionar rotações para rebalancear
TEST_F(AVLTreeSpecificTest, RemovalTriggersRebalancing)
{
    // Cria uma árvore maior e mais complexa
    int keys[] = {40, 20, 60, 10, 30, 50, 70, 5, 15, 25, 35};
    for (int key : keys)
    {
        avl.insert({key, std::to_string(key)});
    }
    ASSERT_EQ(avl.size(), static_cast<size_t>(11));

    // A remoção de uma chave distante (e.g., 70) pode forçar
    // o rebalanceamento em um nó ancestral.
    avl.remove(70);
    EXPECT_EQ(avl.size(), static_cast<size_t>(10));
    EXPECT_FALSE(avl.contains(70));

    avl.remove(50);
    EXPECT_EQ(avl.size(), static_cast<size_t>(9));
    EXPECT_FALSE(avl.contains(50));

    // Remover a raiz antiga deve funcionar
    avl.remove(40);
    EXPECT_EQ(avl.size(), static_cast<size_t>(8));
    EXPECT_FALSE(avl.contains(40));
}

//==================================================================================
// TESTES ESPECÍFICOS PARA TABELAS HASH (AMBAS IMPLEMENTAÇÕES)
// Foco: Forçar colisões e redimensionamento (rehashing).
//==================================================================================

// Usamos novamente um teste tipado para aplicar os mesmos testes
// de estresse para ChainedHashTable e OpenHashTable.
template <typename T>
class HashTableStressTest : public ::testing::Test
{
protected:
    std::unique_ptr<Dictionary<int, std::string>> hashTable;

    void SetUp() override
    {
        hashTable = std::make_unique<T>();
    }
};

TYPED_TEST_SUITE_P(HashTableStressTest);

// Testa o comportamento sob alta taxa de colisão
// Para isso, precisamos de uma função de hash que possamos controlar.
// Como não podemos, vamos simular inserindo múltiplos de um valor inicial
// que, dependendo do tamanho da tabela, provavelmente colidirão.
TYPED_TEST_P(HashTableStressTest, HighCollisionRate)
{
    // Supondo um tamanho inicial pequeno (e.g., 10),
    // inserir múltiplos de 10 causará colisões.
    const int num_elements = 20;
    const int step = 10; // Chaves: 0, 10, 20, 30...
    for (int i = 0; i < num_elements; ++i)
    {
        this->hashTable->insert({i * step, "value_" + std::to_string(i * step)});
    }

    EXPECT_EQ(this->hashTable->size(), static_cast<size_t>(num_elements));

    // Verifica se todos os elementos inseridos podem ser encontrados
    for (int i = 0; i < num_elements; ++i)
    {
        EXPECT_TRUE(this->hashTable->contains(i * step));
        EXPECT_EQ(this->hashTable->at(i * step), "value_" + std::to_string(i * step));
    }

    // Tenta remover alguns elementos e verifica a consistência
    this->hashTable->remove(0);
    this->hashTable->remove(50);
    EXPECT_FALSE(this->hashTable->contains(0));
    EXPECT_FALSE(this->hashTable->contains(50));
    EXPECT_TRUE(this->hashTable->contains(10)); // Vizinho
    EXPECT_EQ(this->hashTable->size(), static_cast<size_t>(num_elements - 2));
}

// Testa a funcionalidade de redimensionamento (rehashing)
TYPED_TEST_P(HashTableStressTest, RehashingOnHighLoadFactor)
{
    // Insere um número grande de elementos para forçar o rehash.
    // O rehash geralmente ocorre quando o fator de carga (size/capacity)
    // ultrapassa um limiar (e.g., 0.75).
    const int num_elements = 100;
    for (int i = 0; i < num_elements; ++i)
    {
        this->hashTable->insert({i, "value_" + std::to_string(i)});
    }

    EXPECT_EQ(this->hashTable->size(), static_cast<size_t>(num_elements));

    // Verifica se todos os dados permanecem consistentes após múltiplos rehashes.
    for (int i = 0; i < num_elements; ++i)
    {
        EXPECT_TRUE(this->hashTable->contains(i));
    }

    // Remove metade dos elementos
    for (int i = 0; i < num_elements; i += 2)
    {
        this->hashTable->remove(i);
    }

    EXPECT_EQ(this->hashTable->size(), static_cast<size_t>(num_elements / 2));

    // Verifica se os elementos certos foram removidos e os outros permanecem
    for (int i = 0; i < num_elements; ++i)
    {
        if (i % 2 == 0)
        {
            EXPECT_FALSE(this->hashTable->contains(i));
        }
        else
        {
            EXPECT_TRUE(this->hashTable->contains(i));
        }
    }
}

REGISTER_TYPED_TEST_SUITE_P(HashTableStressTest,
                            HighCollisionRate,
                            RehashingOnHighLoadFactor);

using HashTableImplementations = ::testing::Types<
    ChainedHashTable<int, std::string>,
    OpenHashTable<int, std::string>>;

INSTANTIATE_TYPED_TEST_SUITE_P(MyHashImplementations, HashTableStressTest, HashTableImplementations);

//==================================================================================
// TESTE DE ESTRESSE GERAL (PARA TODAS AS IMPLEMENTAÇÕES)
// Foco: Inserir e remover um grande volume de dados aleatórios.
// Isso é ótimo para pegar vazamentos de memória e problemas de estabilidade.
//==================================================================================

template <typename T>
class GeneralStressTest : public ::testing::Test
{
protected:
    std::unique_ptr<Dictionary<int, std::string>> dict;

    void SetUp() override
    {
        dict = std::make_unique<T>();
    }
};

TYPED_TEST_SUITE_P(GeneralStressTest);

TYPED_TEST_P(GeneralStressTest, LargeRandomInsertAndRemove)
{
    const int num_elements = 5000;
    std::vector<int> keys(num_elements);
    std::iota(keys.begin(), keys.end(), 0); // Preenche com 0, 1, 2, ...

    // Embaralha as chaves para garantir inserção em ordem aleatória
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(keys.begin(), keys.end(), g);

    // Insere todos os elementos
    for (int key : keys)
    {
        this->dict->insert({key, "value_" + std::to_string(key)});
    }
    ASSERT_EQ(this->dict->size(), static_cast<size_t>(num_elements));

    // Verifica se todos foram inseridos
    for (int key : keys)
    {
        ASSERT_TRUE(this->dict->contains(key));
    }

    // Embaralha novamente para remover em outra ordem aleatória
    std::shuffle(keys.begin(), keys.end(), g);

    // Remove metade dos elementos
    for (size_t i = 0; i < keys.size() / 2; ++i)
    {
        this->dict->remove(keys[i]);
    }
    ASSERT_EQ(this->dict->size(), static_cast<size_t>(num_elements - keys.size() / 2));

    // Verifica quais foram removidos e quais permaneceram
    for (size_t i = 0; i < keys.size(); ++i)
    {
        if (i < keys.size() / 2)
        {
            ASSERT_FALSE(this->dict->contains(keys[i]));
        }
        else
        {
            ASSERT_TRUE(this->dict->contains(keys[i]));
        }
    }

    // Limpa a estrutura
    this->dict->clear();
    ASSERT_EQ(this->dict->size(), static_cast<size_t>(0));
    ASSERT_TRUE(this->dict->empty());
}

REGISTER_TYPED_TEST_SUITE_P(GeneralStressTest,
                            LargeRandomInsertAndRemove);

using Implementations = ::testing::Types<
    AVLTree<int, std::string>,
    RedBlackTree<int, std::string>,
    ChainedHashTable<int, std::string>,
    OpenHashTable<int, std::string>>;

// Instancia a suíte de testes para cada uma das implementações
INSTANTIATE_TYPED_TEST_SUITE_P(MyImplementations, DictionaryTest, Implementations);

// Instancia a suíte de testes de estresse para as implementações de dicionário
INSTANTIATE_TYPED_TEST_SUITE_P(MyGeneralStress, GeneralStressTest, Implementations);