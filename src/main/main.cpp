#include <iostream>

#include "dictionary/DynamicDictionary.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    std::unique_ptr<Dictionary<string, int>> tree = create_dictionary<string, int>(DictionaryType::OPEN_ADDRESSING_HASH);

    initializer_list<pair<string, int>> list{{"Alice", 25}, {"Bob", 30}, {"Charlie", 35}, {"David", 40}, {"Eve", 45}, {"Frank", 50}, {"Grace", 55}, {"Heidi", 60}, {"Ivan", 65}, {"Judy", 70}, {"Karl", 75}, {"Leo", 80}, {"Mallory", 85}, {"Nina", 90}, {"Oscar", 95}, {"Peggy", 100}};

    for (const auto &pair : list)
    {
        cout << "Inserting: " << pair.first << " with value: " << pair.second << endl;
        tree->insert(pair);
        tree->print();
    }

    OpenHashTable<string, int> *oh_table = dynamic_cast<OpenHashTable<string, int> *>(tree.get());
    if (oh_table)
    {
        cout << "Comparisons: " << oh_table->getComparisons() << endl
             << "Collisions: " << oh_table->getCollisions() << endl;
    }

    tree->print();
    cout << endl;

    tree->remove("Charlie");

    cout << "Size of the tree: " << tree->size() << endl;

    cout << (*tree)["Bob"] << endl;

    tree->operator[]("Bob");

    tree->update({"Bob", 40}); // Update Bob's age to 40

    cout << "Updated Bob's age: " << (*tree)["Bob"] << endl;

    (*tree)["Bob"] = 50; // Insert Frank with age 50

    cout << "Frank's age: " << (*tree)["Frank"] << endl;

    cout << boolalpha << tree->contains("Alice") << endl; // Check if Alice is in the tree

    cout << boolalpha << tree->contains("Zoe") << endl; // Check if Zoe is in the tree

    tree->forEach([](const pair<string, int> &pair)
                  {
                      cout << pair.first << ": " << pair.second << endl; // Print each element using forEach
                  });

    return 0;
}