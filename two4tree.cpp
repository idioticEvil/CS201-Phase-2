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

/**
 * @brief A 2-4 tree that can store any type of data
 * 
 * @tparam KeyType The type of the keys in the tree
 * @tparam ValueType The type of the values in the tree
 */
template <typename KeyType, typename ValueType> class two4tree {
    private:
        Node* rootNode;

        void split(Node* node) {
            // IMPLEMENT THIS
        }
        
    public:
        /**
         * @brief Creates an empty 2-4 tree
         */
        two4tree() {
            rootNode = nullptr;
        }

        /**
         * @brief Construct a new 2-4 tree object from an array of keys and an array of values
         * 
         * @param keys The array of keys to insert into the tree
         * @param values The array of values to insert into the tree
         * @param size The size of the arrays
         */
        two4tree(KeyType keys[], ValueType values[], int size) {
            // IMPLEMENT THIS
        }

        /**
         * @brief Destroy the 2-4 tree object
         */
        ~two4tree() {
            // IMPLEMENT THIS
        }

        /**
         * @brief Returns the value associated with a key in the tree
         * 
         * @param key The key to search for
         * @return ValueType* A pointer to the value associated with the key
         */
        ValueType *search(KeyType key) {
            // IMPLEMENT THIS
        }

        /**
         * @brief Inserts a key-value pair into the tree
         * 
         * @param key The key to insert
         * @param value The value to insert
         */
        void insert(KeyType key, ValueType value) {
            // Check if the tree is empty, if so, create a new root node
            if (rootNode == nullptr) rootNode = new Node(key, value);
            // If the tree is not empty, find the correct leaf node to insert the key-value pair
            else { 
                refNode = rootNode;

                // Traverse the tree to find the correct leaf node
                while (!refNode->isLeaf) {
                    for (int i = 0; i < refNode->size; i++) {
                        if (key < refNode->keys[i]) {
                            refNode = refNode->children[i];
                            break;
                        }
 
                        if (i == refNode->size - 1) {
                            refNode = refNode->children[i + 1];
                            break;
                        }
                    }
                }

                /*
                Insert the key-value pair into the leaf node
                NOTE: This will not split the node if it has more than 3 keys... yet
                */
                if (refNode->size < 3) {
                    for (int i = 0; i < refNode->size; i++) {
                        if (key <= refNode->keys[i]) {
                            refNode->keys.addAt(key, i);
                            refNode->values.addAt(value, i);
                            refNode->size++;
                            break;
                        }
                    }

                    if (i == refNode->size - 1) {
                        refNode->keys.addEnd(key);
                        refNode->values.addEnd(value);
                        refNode->size++;
                    }
                } else {
                    // Split
                }
            }
        }

        /**
         * @brief Removes a key from the tree
         * 
         * @param key The key to remove
         * @return int Indicates if operation was successful
         */
        int remove(KeyType key) {
            // IMPLEMENT THIS
        }

        /**
         * @brief Returns the rank of a key in the tree
         * 
         * @param key The key to find the rank of
         * @return int The rank of the key
         */
        int rank(KeyType key) {
            // IMPLEMENT THIS
        }

        /**
         * @brief Returns the key at a given position in the tree
         * 
         * @param pos The position of the key to find
         * @return KeyType The key at the given position
         */
        KeyType select(int pos) {
            // IMPLEMENT THIS
        }

        /**
         * @brief Returns the number of duplicates of a specific key in the tree
         * 
         * @param key The key to find the number of duplicates of
         * @return int The number of duplicates of the key
         */
        int duplicates(KeyType key) {
            // IMPLEMENT THIS
        }

        /**
         * @brief Returns the number of keys in the tree
         * 
         * @return int The number of keys in the tree
         */
        int size() {
            // IMPLEMENT THIS
        }

        /**
         * @brief Prints the pre-order traversal of the tree
         */
        void preorder() {
            // IMPLEMENT THIS
        }

        /**
         * @brief Prints the in-order traversal of the tree
         */
        void inorder() {
            // IMPLEMENT THIS
        }

        /**
         * @brief Prints the post-order traversal of the tree
         */
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

        // Multiple Key-Value Constructor: Creates a node with multiple keys and values
        Node(CircularDynamicArray<K> k, CircularDynamicArray<V> v) {
            keys = k;
            values = v;
            children = CircularDynamicArray<Node*>(4);
            parent = nullptr;
            isLeaf = true;
            size = k.size();
            rank = k.size();
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

        // Multiple Key-Value-Parent Constructor: Creates a node with multiple keys, values, and parent
        Node(CircularDynamicArray<K> k, CircularDynamicArray<V> v, Node* parent) {
            keys = k;
            values = v;
            children = CircularDynamicArray<Node*>(4);
            this->parent = parent;
            isLeaf = true;
            size = k.size();
            rank = k.size();
        }

        // Destructor: Frees all space used by the node
        ~Node() {
            delete keys;
            delete values;
            delete children;
        }
};