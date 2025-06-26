#pragma once

#include <memory>
#include <functional>

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
    /**
     * @brief Destrutor virtual para permitir a destruição correta de classes derivadas.
     */
    virtual ~Dictionary() = default;

    /**
     * @brief Insere um par chave-valor no dicionário.
     *
     * @param key_value O par chave-valor a ser inserido.
     */
    virtual void insert(const std::pair<Key, Value> &key_value) = 0;

    /**
     * @brief Remove um elemento do dicionário pela chave.
     *
     * @param key A chave do elemento a ser removido.
     */
    virtual void remove(const Key &key) = 0;

    /**
     * @brief Atualiza o valor associado a uma chave existente no dicionário.
     *
     * @param key_value O par chave-valor a ser atualizado.
     */
    virtual void update(const std::pair<Key, Value> &key_value) = 0;

    /**
     * @brief Verifica se uma chave está presente no dicionário.
     *
     * @param key A chave a ser verificada.
     * @return true Se a chave estiver presente.
     * @return false Se a chave não estiver presente.
     */
    virtual bool contains(const Key &key) = 0;

    /**
     * @brief Obtém o valor associado a uma chave.
     *
     * @param key A chave cujo valor será obtido.
     * @return O valor associado à chave.
     */
    virtual Value &at(const Key &key) = 0;

    /**
     * @brief Sobrecarga do operador de indexação para acessar o valor associado a uma chave.
     *
     * @param key A chave cujo valor será acessado.
     * @return O valor associado à chave.
     */
    virtual Value &operator[](const Key &key) = 0;

    /**
     * @brief Limpa todos os elementos do dicionário.
     */
    virtual void clear() = 0;

    /**
     * @brief Obtém o número de elementos no dicionário.
     *
     * @return O número de elementos no dicionário.
     */
    virtual size_t size() const noexcept = 0;

    /**
     * @brief Verifica se o dicionário está vazio.
     *
     * @return true Se o dicionário não contiver elementos.
     * @return false Se o dicionário contiver pelo menos um elemento.
     */
    virtual bool empty() const noexcept = 0;

    /**
     * @brief Imprime o conteúdo do dicionário.
     *
     * @details Esta função deve ser implementada para exibir os pares chave-valor
     * de forma legível, dependendo da implementação específica do dicionário.
     */
    virtual void print() const = 0;

    /**
     * @brief Itera sobre todos os pares chave-valor no dicionário e aplica uma função a cada um.
     *
     * @param func A função a ser aplicada a cada par chave-valor.
     */
    virtual void forEach(const std::function<void(const std::pair<Key, Value> &)> &func) const = 0;

    /**
     * @brief Clona o dicionário atual.
     *
     * @return Um ponteiro único para uma nova instância do dicionário com os mesmos elementos.
     */
    virtual std::unique_ptr<Dictionary<Key, Value>>
    clone() const = 0;
};
