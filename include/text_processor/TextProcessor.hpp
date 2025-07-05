#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <regex>
#include <functional>

/**
 * @file TextProcessor.hpp
 *
 * @class TextProcessor
 *
 * @brief Responsável por processar ficheiros de texto, extraindo e normalizando palavras.
 *
 * Esta classe abre um ficheiro de texto, lê o seu conteúdo palavra por palavra,
 * normaliza cada palavra (converte para minúsculas e remove caracteres inválidos)
 * e, em seguida, passa cada palavra válida para uma função de callback fornecida pelo utilizador.
 *
 * @details
 * O processador de texto utiliza uma expressão regular para identificar palavras válidas.
 * As palavras são consideradas válidas se consistirem apenas de letras (incluindo acentuadas),
 * hífens e apóstrofos, desde que não estejam no início ou no fim da palavra.
 */
class TextProcessor
{
private:
    /**
     * @brief Fluxo de entrada para o ficheiro de texto a ser processado.
     */
    std::ifstream file_stream;

    /**
     * @brief Normaliza uma palavra.
     *
     * Converte a palavra para minúsculas e verifica se corresponde a um formato de palavra válido
     * usando uma expressão regular. A expressão regular permite letras (incluindo acentuadas),
     * hífens e apóstrofos, desde que não estejam no início ou no fim da palavra.
     *
     * @param word A palavra a ser normalizada.
     * @return A palavra normalizada em minúsculas se for válida; caso contrário, uma string vazia.
     */
    std::string normalize(const std::string &word) const;

public:
    /**
     * @brief Construtor da classe TextProcessor.
     *
     * Inicializa o processador de texto abrindo o ficheiro de entrada especificado.
     * Se o ficheiro não puder ser aberto, uma mensagem de erro crítico é exibida
     * e o programa é encerrado.
     *
     * @param input_file O nome do ficheiro de texto a ser processado.
     * @throws std::runtime_error Se o ficheiro não puder ser aberto, uma exceção é lançada.
     *                            A mensagem de erro é exibida no fluxo de erro padrão.
     *                            O programa é encerrado com um código de erro implícito.
     *
     * @note O ficheiro deve existir e ser legível. Caso contrário, o programa
     *       exibirá uma mensagem de erro e encerrará a execução.
     */
    explicit TextProcessor(const std::string &input_file);

    /**
     * @brief Destrutor padrão da classe TextProcessor.
     */
    ~TextProcessor() = default;

    /**
     * @brief Converte uma string para minúsculas.
     *
     * Esta é uma função de utilidade estática que modifica a string fornecida,
     * convertendo todos os seus caracteres para a forma minúscula.
     *
     * @param text A string a ser convertida. A conversão é feita no local (in-place).
     */
    static void toLowerCase(std::string &text);

    /**
     * @brief Processa o ficheiro de texto palavra por palavra.
     *
     * Lê o ficheiro associado a esta instância, extraindo palavras. Cada palavra
     * é então normalizada através da função `normalize`. Se a palavra normalizada
     * não for vazia, ela é passada para a função de callback `wordHandler`.
     *
     * @param wordHandler Uma função (ou objeto de função, como um lambda) que aceita
     *                    uma `const std::string&` e será chamada para cada palavra
     *                    válida e normalizada encontrada no ficheiro.
     */
    void processFile(const std::function<void(const std::string &)> &wordHandler);
};