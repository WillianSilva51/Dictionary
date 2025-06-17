#include <iostream>

#include <Dictionary/DynamicDictionary.hpp>

using namespace std;

int main()
{
    RedBlackTree<string, int> tree = {{"Alice", 25}, {"Bob", 30}, {"Charlie", 35}, {"David", 40}, {"Eve", 45}, {"Frank", 50}, {"Grace", 55}};

    tree.print();
    cout << endl;

    tree.remove("Charlie");

    tree.bshow();
    cout << "Size of the tree: " << tree.size() << endl;

    cout << tree["Bob"] << endl;

    tree.update({"Bob", 40}); // Update Bob's age to 40

    cout << "Updated Bob's age: " << tree["Bob"] << endl;

    tree["Bob"] = 50; // Insert Frank with age 50

    cout << "Frank's age: " << tree["Frank"] << endl;

    cout << boolalpha << tree.contains("Alice") << endl; // Check if Alice is in the tree

    cout << boolalpha << tree.contains("Zoe") << endl; // Check if Zoe is in the tree

    for (const auto &pair : tree)
    {
        cout << pair.first << ": " << pair.second << endl; // Print all elements in the tree
    }

    return 0;
}