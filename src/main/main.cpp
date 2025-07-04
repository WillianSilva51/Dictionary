#include <iostream>
#include <chrono>

#include "dictionary/DynamicDictionary.hpp"
#include "text_processor/TextProcessor.hpp"

using namespace std;

void print_usage()
{
    std::cerr << "Use: ./freq <structure> <input_file> [output_file]" << std::endl;
    std::cerr << "Available structures: avl, rbt, chash, ohash" << std::endl;
}

void counter_words(const std::string &filename, Dictionary<std::string, unsigned int> &word_count)
{
    TextProcessor processor(filename);

    auto startTime = chrono::high_resolution_clock::now();

    processor.processFile([&word_count](const string &word)
                          { word_count[word]++; });

    auto endTime = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> buildTime = endTime - startTime;

    word_count.print();
    cout << endl;

    cout << buildTime.count() << endl;
}

int main(int argc, char *argv[])
{
    if (argc < 3 or argc > 4)
    {
        print_usage();
        return -1;
    }

    setlocale(LC_ALL, "Pt_BR.UTF-8");

    string structure_type = argv[1];
    string input_file = argv[2];
    string output_file = (argc == 4) ? argv[3] : "log/" + input_file + ".txt";

    TextProcessor::toLowerCase(structure_type);

    unique_ptr<Dictionary<string, unsigned int>> counter;

    if (structure_type == "avl" or structure_type == "avltree")
    {
        counter = create_dictionary<string, unsigned int>(DictionaryType::AVL);
    }
    else if (structure_type == "rbt" or structure_type == "rbtree")
    {
        counter = create_dictionary<string, unsigned int>(DictionaryType::RBTREE);
    }
    else if (structure_type == "chash" or structure_type == "hashtable")
    {
        counter = create_dictionary<string, unsigned int>(DictionaryType::CHAINING_HASH);
    }
    else if (structure_type == "ohash" or structure_type == "openhashtable")
    {
        counter = create_dictionary<string, unsigned int>(DictionaryType::OPEN_ADDRESSING_HASH);
    }
    else
    {
        throw invalid_argument("Unknown structure_type type: " + structure_type + " (valid types are: avl, rbt, chash, ohash)");
    }

    counter_words(input_file, *counter);

    return 0;
}