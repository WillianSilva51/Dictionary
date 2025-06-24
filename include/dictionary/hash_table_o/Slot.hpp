#pragma once

#include <utility>

/**
 * @enum HashTableStatus
 * @brief Enum que representa o estado de um slot na tabela hash.
 *
 * Utilizado para controlar o estado de cada posição na tabela em esquemas de
 * endereçamento aberto, permitindo diferenciar entre slots vazios, ocupados e
 * removidos (lápides).
 */
enum class HashTableStatus : char
{
    EMPTY,  ///< O slot está vazio e nunca foi usado.
    ACTIVE, ///< O slot contém um par chave-valor ativo.
    DELETED ///< O slot continha um par chave-valor que foi removido (lápide).
};

/**
 * @class Slot
 * @brief Representa um único slot em uma tabela hash de endereçamento aberto.
 *
 * Cada slot armazena um par chave-valor e um status que indica se o slot
 * está vazio, ativo (contendo dados válidos) ou se foi deletado (marcado como
 * uma lápide para não interromper as sequências de sondagem).
 *
 * @tparam Key O tipo da chave.
 * @tparam Value O tipo do valor.
 */
template <typename Key, typename Value>
struct Slot
{
    /**
     * @var data
     * @brief O par chave-valor armazenado no slot.
     */
    std::pair<Key, Value> data{};

    /**
     * @var status
     * @brief O estado atual do slot (EMPTY, ACTIVE, ou DELETED).
     */
    HashTableStatus status{HashTableStatus::EMPTY};

    /**
     * @brief Construtor padrão.
     *
     * Inicializa um slot com o estado `EMPTY`.
     */
    Slot() = default;

    /**
     * @brief Construtor que inicializa o slot com um par chave-valor.
     *
     * Cria um slot e o marca como `ACTIVE`.
     * @param pair O `std::pair<Key, Value>` a ser armazenado.
     */
    explicit Slot(const std::pair<Key, Value> &pair)
        : data(pair), status(HashTableStatus::ACTIVE) {}

    /**
     * @brief Construtor que inicializa o slot com uma chave e um valor.
     *
     * Cria um slot a partir de uma chave e um valor e o marca como `ACTIVE`.
     * @param key A chave a ser armazenada.
     * @param value O valor a ser armazenado.
     */
    Slot(const Key &key, const Value &value)
        : data({key, value}), status(HashTableStatus::ACTIVE) {}

    /**
     * @brief Verifica se o slot está vazio.
     * @return `true` se o status do slot for `EMPTY`, `false` caso contrário.
     */
    bool is_empty() const noexcept
    {
        return status == HashTableStatus::EMPTY;
    }

    /**
     * @brief Verifica se o slot está ativo.
     * @return `true` se o status do slot for `ACTIVE`, `false` caso contrário.
     */
    bool is_active() const noexcept
    {
        return status == HashTableStatus::ACTIVE;
    }

    /**
     * @brief Verifica se o slot foi marcado como deletado.
     * @return `true` se o status do slot for `DELETED`, `false` caso contrário.
     */
    bool is_deleted() const noexcept
    {
        return status == HashTableStatus::DELETED;
    }
};
