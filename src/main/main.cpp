#include <iostream>

#include "dictionary/DynamicDictionary.hpp"

using namespace std;

int main()
{
    auto tree = create_dictionary<string, int>(DictionaryType::RBTREE, {{"Alice", 25}, {"Bob", 30}, {"Charlie", 35}, {"David", 40}, {"Eve", 45}, {"Frank", 50}, {"Grace", 55}});

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