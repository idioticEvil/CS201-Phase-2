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
- Temporarily split this file into three separate files (DO NOT INCLUDE IN FINAL SUBMISSION)
    - Do I need to use namespaces / header files?
*/

// two4tree: A 2-4 tree that can be used to store any type of data
template <typename KeyType, typename ValueType> class two4tree {
    private:
        Node* root;

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