#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>

#include "dictionary/DynamicDictionary.hpp"

using namespace std;

mutex mtx;

void counter_words(const std::string &filename, Dictionary<std::string, unsigned int> &word_count)
{

    word_count.print();
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Pt_BR.UTF-8");

    if (argc < 3)
        throw invalid_argument("Usage: " + string(argv[0]) + " <dictionary_type>" + " <dictionary_file>");


    string dictionary = string(argv[1]);

    for (auto &i : dictionary)
        i = tolower(i);

    unique_ptr<Dictionary<string, unsigned int>> counter;

    if (dictionary == "avl" or dictionary == "avltree")
    {
        counter = create_dictionary<string, unsigned int>(DictionaryType::AVL);
    }
    else if (dictionary == "rb" or dictionary == "rbtree")
    {
        counter = create_dictionary<string, unsigned int>(DictionaryType::RBTREE);
    }
    else if (dictionary == "chash" or dictionary == "hashtable")
    {
        counter = create_dictionary<string, unsigned int>(DictionaryType::CHAINING_HASH);
    }
    else if (dictionary == "ohash" or dictionary == "openhashtable")
    {
        counter = create_dictionary<string, unsigned int>(DictionaryType::OPEN_ADDRESSING_HASH);
    }
    else if (dictionary == "all")
    {
    }
    else
    {
        throw invalid_argument("Unknown dictionary type: " + dictionary);
    }

    return 0;
}