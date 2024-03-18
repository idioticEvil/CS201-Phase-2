#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdexcept> 
#include "CircularDynamicArray.cpp"
using namespace std;

/*
TODO:
- Remove unnecessary functions from CircularDynamicArray
- Add any needed functions to Node
*/

// two4tree: A 2-4 tree that can be used to store any type of data
template <typename KeyType, typename ValueType> class two4tree {
    private:
        Node* root;
        
    public:
        // Basic Constructor: Creates an empty 2-4 tree
        two4tree() {
            root = nullptr;
        }

        // Array Constructor: Creates a 2-4 tree with keys and values from arrays
        two4tree(KeyType keys[], ValueType values[], int size) {
            // IMPLEMENT THIS
        }

        // Destructor: Frees all space used by the tree
        ~two4tree() {
            // IMPLEMENT THIS
        }

        // Search: Returns the value associated with a key in the tree
        ValueType *search(KeyType key) {
            // IMPLEMENT THIS
        }

        // Insert: Inserts a key-value pair into the tree
        void insert(KeyType key, ValueType value) {
            // IMPLEMENT THIS
        }

        // Remove: Removes a key-value pair from the tree
        int remove(KeyType key) {
            // IMPLEMENT THIS
        }

        // Rank: Returns the rank of a key in the tree
        int rank(KeyType key) {
            // IMPLEMENT THIS
        }

        // Select: Returns the key at a given rank in the tree
        KeyType select(int rank) {
            // IMPLEMENT THIS
        }

        // Duplicates: Returns the number of duplicates of a key in the tree
        int duplicates(KeyType key) {
            // IMPLEMENT THIS
        }

        // Size: Returns the number of keys in the tree
        int size() {
            // IMPLEMENT THIS
        }

        // Preorder: Prints the keys of the tree in preorder
        void preorder() {
            // IMPLEMENT THIS
        }

        // Inorder: Prints the keys of the tree in inorder
        void inorder() {
            // IMPLEMENT THIS
        }

        // Postorder: Prints the keys of the tree in postorder
        void postorder() {
            // IMPLEMENT THIS
        }
};

// Node: A node that can be used to store any type of data in a 2-4 tree
template <class K, class V> class Node {
    private:
        CircularDynamicArray<K> keys;
        CircularDynamicArray<V> values;
        CircularDynamicArray<Node*> children;
        Node* parent;
        bool isLeaf;
        int size;
        int rank;

    public:
        // Basic Constructor: Creates a node with no keys, values, or children
        Node() {
            keys = CircularDynamicArray<K>(3);
            values = CircularDynamicArray<V>(3);
            children = CircularDynamicArray<Node*>(4);
            parent = nullptr;
            isLeaf = true;
            size = 0;
            rank = 0;
        }

        // Key-Value Constructor: Creates a node with one key and value
        Node(K k, V v) {
            keys = CircularDynamicArray<K>(3);
            values = CircularDynamicArray<V>(3);
            children = CircularDynamicArray<Node*>(4);
            parent = nullptr;
            isLeaf = true;
            size = 1;
            rank = 1;
            keys.addEnd(k);
            values.addEnd(v);
        }

        // Key-Value-Parent Constructor: Creates a node with one key, value, and parent
        Node(K k, V v, Node* parent) {
            keys = CircularDynamicArray<K>(3);
            values = CircularDynamicArray<V>(3);
            children = CircularDynamicArray<Node*>(4);
            this->parent = parent;
            isLeaf = true;
            size = 1;
            rank = 1;
            keys.addEnd(k);
            values.addEnd(v);
        }
};