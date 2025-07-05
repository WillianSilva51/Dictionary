#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <filesystem>

#include "dictionary/DynamicDictionary.hpp"
#include "text_processor/TextProcessor.hpp"

using namespace std;

const string INPUT_DIR = "files/"; // Diretório de entrada para os arquivos de texto
const string OUTPUT_DIR = "out/";  // Diretório de saída para os arquivos de contagem
const string LOG_DIR = "log/";     // Diretório de log para erros e exceções

mutex mtx; // Mutex para proteger o acesso ao dicionário

/**
 * @brief Cria um diretório se ele não existir.
 *
 * Esta função verifica se o diretório especificado existe e, se não existir,
 * tenta criá-lo.
 *
 * @param DIR O caminho do diretório a ser criado.
 *
 * @throw std::runtime_error Se ocorrer um erro ao criar o diretório.
 */
void create_directory(const string &DIR)
{
    try
    {
        if (!filesystem::exists(DIR))
            filesystem::create_directory(DIR); // Cria o diretório se não existir
    }
    catch (const std::exception &e)
    {
        cerr << "Error creating directory '" << DIR << "': " << e.what() << endl;
        throw std::runtime_error("Failed to create directory: " + DIR);
    }
}

/**
 * @brief Registra uma exceção em um arquivo de log.
 *
 * Esta função captura uma exceção e registra sua mensagem em um arquivo de log,
 * juntamente com a data e hora atuais. O log é armazenado no diretório LOG_DIR
 * e o arquivo é nomeado "log.txt".
 *
 * @details
 * A função utiliza a biblioteca `<chrono>` para obter o tempo atual e a biblioteca
 * `<iomanip>` para formatar a data e hora. A mensagem da exceção é obtida através
 * do método `what()` da classe `std::exception`. O log é escrito em um arquivo
 * no formato "YYYY-MM-DD HH:MM:SS - mensagem_da_exceção".
 *
 * @param e A exceção a ser registrada.
 */
void logException(const std::exception &e)
{
    create_directory(LOG_DIR); // Cria o diretório de log se não existir

    time_t logTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    ofstream logFile(LOG_DIR + "log.txt", ios::app);
    logFile << put_time(localtime(&logTime), "%Y-%m-%d %H:%M:%S") << " - " << e.what() << '\n';
    logFile.close();
}

/**
 * @brief Imprime a mensagem de uso do programa.
 *
 * Esta função exibe uma mensagem de erro informando como usar o programa corretamente,
 * incluindo os parâmetros esperados e as estruturas de dados disponíveis.
 */
void print_usage()
{
    std::cerr << "Use: ./Dictionary <structure> <input_file> [output_file]" << std::endl;
    std::cerr << "Available structures: avl, rbt, chash, ohash, all" << std::endl;
}

/**
 * @brief Imprime as métricas de desempenho de uma estrutura de dicionário.
 *
 * Esta função recebe um dicionário contendo a contagem de palavras e imprime
 * as métricas de desempenho, como colisões, comparações, rotações, etc., dependendo
 * do tipo da estrutura de dados utilizada.
 *
 * @param word_count O dicionário contendo a contagem de palavras.
 * @param output_file O arquivo onde as métricas serão escritas.
 */
void metrics(const Dictionary<std::string, unsigned int> &word_count, std::ofstream &output_file)
{
    if (auto *chained_hash = dynamic_cast<const ChainedHashTable<std::string, unsigned int> *>(&word_count))
    {
        output_file << "Collisions: " << chained_hash->getCollisions() << endl;
        output_file << "Comparisons: " << chained_hash->getComparisons() << endl;
    }
    else if (auto *open_hash = dynamic_cast<const OpenHashTable<std::string, unsigned int> *>(&word_count))
    {
        output_file << "Collisions: " << open_hash->getCollisions() << endl;
        output_file << "Comparisons: " << open_hash->getComparisons() << endl;
    }
    else if (auto *avl_tree = dynamic_cast<const AVLTree<std::string, unsigned int> *>(&word_count))
    {
        output_file << "Rotations: " << avl_tree->getRotations() << endl;
        output_file << "Comparisons: " << avl_tree->getComparisons() << endl;
    }
    else if (auto *rbtree = dynamic_cast<const RedBlackTree<std::string, unsigned int> *>(&word_count))
    {
        output_file << "Rotations: " << rbtree->getRotations() << endl;
        output_file << "Comparisons: " << rbtree->getComparisons() << endl;
    }
}

/**
 * @brief Escreve a contagem de palavras e métricas em um arquivo de saída.
 *
 * Esta função recebe um dicionário contendo a contagem de palavras, o tempo de construção
 * e o tipo de estrutura utilizada, e escreve essas informações em um arquivo especificado.
 *
 * @param filename O nome do arquivo onde as informações serão escritas.
 * @param word_count O dicionário contendo as palavras e suas respectivas contagens.
 * @param buildTime O tempo gasto para construir a estrutura de dados.
 * @param structure_type O tipo da estrutura de dados utilizada (ex: "AVL", "RBTREE", etc.).
 */
void write_output(const std::string &filename, const Dictionary<std::string, unsigned int> &word_count, const std::chrono::duration<double, std::milli> &buildTime, const string &structure_type)
{
    create_directory(OUTPUT_DIR); // Cria o diretório de saída se não existir

    ofstream output_file(OUTPUT_DIR + filename, ios::app);

    if (!output_file or !output_file.is_open())
        throw std::runtime_error("Failed to open output file " + filename);

    word_count.forEach([&output_file](const std::pair<std::string, unsigned int> &pair)
                       { output_file << "[" << pair.first << ", " << pair.second << "]" << endl; });

    output_file << endl;
    output_file << "---------------------------------------" << endl;

    output_file << "Metrics:" << endl;
    output_file << "Structure: " << structure_type << endl;
    output_file << "Build time: " << buildTime.count() << " ms" << endl;
    output_file << "Size: " << word_count.size() << endl;

    metrics(word_count, output_file);

    output_file << "---------------------------------------" << endl
                << endl;

    output_file.close();
}

/**
 * @brief Conta as palavras em um arquivo de texto e armazena a contagem em um dicionário.
 *
 * Esta função processa um arquivo de texto, conta a ocorrência de cada palavra
 * e armazena a contagem em um dicionário. A função é executada em uma thread separada
 * para permitir a contagem concorrente de palavras em diferentes estruturas de dados.
 *
 * @details
 * A função utiliza a classe `TextProcessor` para ler o arquivo e processar cada palavra.
 * Cada palavra é normalizada e passada para um lambda que incrementa a contagem no dicionário.
 * A contagem de palavras é feita de forma concorrente, utilizando um mutex para proteger o acesso ao dicionário.
 *
 * @param filename O nome do arquivo de texto a ser processado.
 * @param word_count O dicionário onde as palavras e suas contagens serão armazenadas.
 * @param structure_type O tipo da estrutura de dados utilizada para armazenar as contagens de palavras.
 *
 */
void counter_words(const std::string &filename, Dictionary<std::string, unsigned int> &word_count, const string &structure_type)
{
    TextProcessor processor(INPUT_DIR + filename);

    auto startTime = chrono::high_resolution_clock::now();

    processor.processFile([&word_count](const string &word)
                          { word_count[word]++; });

    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> buildTime = endTime - startTime;

    {
        lock_guard<mutex> lock(mtx);

        cout << "structure: " << structure_type << endl;
        cout << "---------------------------------------" << endl;

        word_count.print();
        cout << endl;

        cout << buildTime.count() << " ms" << endl;

        cout << "---------------------------------------" << endl;
    }
    {
        lock_guard<mutex> lock(mtx);

        write_output(filename, word_count, buildTime, structure_type);
    }
}

/**
 * @brief Função principal do programa.
 *
 * Esta função serve como ponto de entrada para a aplicação de contagem de palavras.
 * Ela processa os argumentos da linha de comando para determinar qual(is) estrutura(s) de dados
 * utilizar e qual arquivo de texto processar.
 *
 * A função pode operar em dois modos:
 *
 * 1.  **Modo específico**: Se um tipo de estrutura de dados (ex: "avl", "rbt") é fornecido,
 *     a contagem de palavras é realizada usando apenas essa estrutura.
 *
 * 2.  **Modo "all"**: Se o argumento for "all", a função cria quatro threads, cada uma
 *     executando a contagem de palavras em paralelo com uma estrutura de dados diferente
 *     (AVL, Red-Black Tree, Chaining Hash, Open Addressing Hash) para fins de comparação de desempenho.
 *
 * A função também lida com a validação do número de argumentos e captura exceções que possam
 * ocorrer durante a execução, registrando-as em um log.
 *
 * @param argc O número de argumentos fornecidos na linha de comando. O programa espera pelo menos 3 (nome do programa, tipo da estrutura, arquivo de entrada).
 * @param argv Um vetor de strings contendo os argumentos da linha de comando.
 *
 *             - `argv[0]`: O nome do programa (geralmente "Dictionary").
 *
 *             - `argv[1]`: O tipo da estrutura de dados a ser usada ("avl", "rbt", "chash", "ohash") ou "all" para usar todas.
 *
 *             - `argv[2]`: O caminho para o arquivo de texto de entrada.
 *
 * @return int Retorna 0 em caso de execução bem-sucedida. Retorna um código de erro implícito em caso de falha (geralmente gerenciado pelo sistema operacional).
 */
int main(int argc, char *argv[])
{
    create_directory(INPUT_DIR); // Cria o diretório de entrada se não existir

    if (argc < 3)
    {
        print_usage();
        logException(std::invalid_argument("Invalid number of arguments"));
        throw std::invalid_argument("Invalid number of arguments. Expected at least 3 arguments.");
    }

    setlocale(LC_ALL, "Pt_BR.UTF-8");

    string structure_type = argv[1];
    string input_file = argv[2];

    TextProcessor::toLowerCase(structure_type);

    try
    {
        if (structure_type == "all")
        {
            thread threads[4];
            unique_ptr<Dictionary<string, unsigned int>> counters[4];

            for (size_t i = 0; i < 4; i++)
            {
                counters[i] = create_dictionary<string, unsigned int>(DictionaryType(i));
                threads[i] = thread(counter_words, input_file, ref(*counters[i]), get_structure_name(DictionaryType(i)));
            }
            

            for (int i = 0; i < 4; ++i)
                threads[i].join();
        }
        else
        {
            DictionaryType type = get_structure_type(structure_type);
            unique_ptr<Dictionary<string, unsigned int>> counter(create_dictionary<string, unsigned int>(type));
            counter_words(input_file, *counter, get_structure_name(type));
        }
    }
    catch (const std::exception &e)
    {
        logException(e);
    }

    return 0;
}