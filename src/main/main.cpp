#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

#include "dictionary/DynamicDictionary.hpp"
#include "text_processor/TextProcessor.hpp"

using namespace std;

mutex mtx;

void print_usage()
{
    std::cerr << "Use: ./freq <structure> <input_file> [output_file]" << std::endl;
    std::cerr << "Available structures: avl, rbt, chash, ohash, all" << std::endl;
}

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

void write_output(const std::string &filename, const Dictionary<std::string, unsigned int> &word_count, const std::chrono::duration<double, std::milli> &buildTime, const string &structure_type)
{
    ofstream output_file(filename, ios::app);

    if (!output_file or !output_file.is_open())
        throw std::runtime_error("Failed to open output file" + filename);

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

void counter_words(const std::string &filename, Dictionary<std::string, unsigned int> &word_count, const string &structure_type)
{
    TextProcessor processor(filename);

    lock_guard<mutex> lock(mtx);

    cout << "structure: " << structure_type << endl;
    cout << "---------------------------------------" << endl;

    auto startTime = chrono::high_resolution_clock::now();

    processor.processFile([&word_count](const string &word)
                          { word_count[word]++; });

    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> buildTime = endTime - startTime;

    word_count.print();
    cout << endl;

    cout << buildTime.count() << " ms" << endl;

    cout << "---------------------------------------" << endl;

    write_output("out/" + filename, word_count, buildTime, structure_type);
}

DictionaryType get_structure_type(const std::string &structure_type)
{
    if (structure_type == "avl" or structure_type == "avltree")
        return DictionaryType::AVL;
    else if (structure_type == "rbt" or structure_type == "rbtree")
        return DictionaryType::RBTREE;
    else if (structure_type == "chash" or structure_type == "hashtable")
        return DictionaryType::CHAINING_HASH;
    else if (structure_type == "ohash" or structure_type == "openhashtable")
        return DictionaryType::OPEN_ADDRESSING_HASH;
    else
    {
        print_usage();
        throw std::invalid_argument("Unknown structure_type type: " + structure_type);
    }
}

void log(const std::exception &e)
{
    std::cerr << e.what() << '\n';
    time_t logTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    ofstream logFile("log/log.txt", ios::app);
    logFile << put_time(localtime(&logTime), "%Y-%m-%d %H:%M:%S") << " - " << e.what() << '\n';
    logFile.close();
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        print_usage();
        log(std::invalid_argument("Invalid number of arguments"));
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

            counters[0] = create_dictionary<string, unsigned int>(DictionaryType::AVL);
            counters[1] = create_dictionary<string, unsigned int>(DictionaryType::RBTREE);
            counters[2] = create_dictionary<string, unsigned int>(DictionaryType::CHAINING_HASH);
            counters[3] = create_dictionary<string, unsigned int>(DictionaryType::OPEN_ADDRESSING_HASH);

            threads[0] = thread(counter_words, input_file, ref(*counters[0]), "AVL TREE");
            threads[1] = thread(counter_words, input_file, ref(*counters[1]), "RBTREE");
            threads[2] = thread(counter_words, input_file, ref(*counters[2]), "CHAINING HASH");
            threads[3] = thread(counter_words, input_file, ref(*counters[3]), "OPEN ADDRESSING HASH");

            for (int i = 0; i < 4; ++i)
                threads[i].join();
        }
        else
        {
            unique_ptr<Dictionary<string, unsigned int>> counter(create_dictionary<string, unsigned int>(get_structure_type(structure_type)));
            counter_words(input_file, *counter, structure_type);
        }
    }
    catch (const std::exception &e)
    {
        log(e);
    }

    return 0;
}