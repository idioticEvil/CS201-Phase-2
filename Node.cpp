#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdexcept> 
#include "CircularDynamicArray.h"
using namespace std;

/**
 * @brief Node class that can be used to store any type of data in a 2-4 tree
 * 
 * @tparam K Key type
 * @tparam V Value type
 */
template <class K, class V> class Node {
    private:
        CircularDynamicArray<K> keys;
        CircularDynamicArray<V> values[3];
        CircularDynamicArray<Node*> children;
        Node* parent;
        bool isLeaf;
        int size;
        int subtreeSize;
        int rank;

    public:
        /**
         * @brief Creates a new node with no keys or values
         */
        Node() {
            keys = new CircularDynamicArray<K>;
            values = new CircularDynamicArray<V>[3];
            children = new CircularDynamicArray<Node*>;
            parent = nullptr;
            isLeaf = true;
            size = 0;
            subtreeSize = 0;
            rank = 0;
        }

        /**
         * @brief Creates a new node with one key and possibly multiple values
         * 
         * @param k Key
         * @param v Value(s)
         */
        Node(K k, V v[]) {
            keys = new CircularDynamicArray<K>;
            values = new CircularDynamicArray<V>[3];
            children = new CircularDynamicArray<Node*>;
            keys.addEnd(k);

            for (int i = 0; i < values->size; i++) {
                values[i].addEnd(v[i]);
            }
            
            parent = nullptr;
            isLeaf = true;
            size = 1;
            subtreeSize = 0;
            rank = 1;
        }

        /**
         * @brief Creates a new node with one key and a CircularDynamicArray of values
         * 
         * @param k Key
         * @param v CircularDynamicArray of values
         */
        Node(K k, CircularDynamicArray<V>& v) {
            keys = new CircularDynamicArray<K>;
            values = new CircularDynamicArray<V>[3];
            children = new CircularDynamicArray<Node*>;
            keys.addEnd(k);
            values[0] = v;
            parent = nullptr;
            isLeaf = true;
            size = 1;
            subtreeSize = 0;
            rank = 1;
        }

        /**
         * @brief Creates a new node with multiple keys and values
         * 
         * @param k Array of keys
         * @param v Array of values
         * @param size Size of the arrays
         */
        // Note: This program logic currently assumes that the values are sorted
        Node(K k[], V v[], int size) { 
            keys = new CircularDynamicArray<K>;
            values = new CircularDynamicArray<V>[3];
            children = new CircularDynamicArray<Node*>;

            for (int i = 0; i < size; i++) {
                keys.addEnd(k[i]);
                values[i].addEnd(v[i]);
            }

            parent = nullptr;
            isLeaf = true;
            this->size = size;
            subtreeSize = 0;
            rank = size;
        }

        /**
         * @brief Creates a new node with one key, value, and parent
         * 
         * @param k Key
         * @param v Value
         * @param parent Parent node
         */
        Node(K k, V v, Node* parent) {
            keys = new CircularDynamicArray<K>;
            values = new CircularDynamicArray<V>[3];
            children = new CircularDynamicArray<Node*>;
            keys.addEnd(k);
            values[0].addEnd(v);
            this->parent = parent;
            isLeaf = true;
            size = 1;
            subtreeSize = 0;
            rank = parent->rank + 1;
        }

        /**
         * @brief Creates a new node with multiple keys, values, and parent
         * 
         * @param k Array of keys
         * @param v Array of values
         * @param size Size of the arrays
         * @param parent Parent node
         */
        // Note: This program logic currently assumes that the values are sorted
        Node(K k[], V v[], int size, Node* parent) {
            keys = new CircularDynamicArray<K>;
            values = new CircularDynamicArray<V>[3];
            children = new CircularDynamicArray<Node*>;

            for (int i = 0; i < size; i++) {
                keys.addEnd(k[i]);
                values[i].addEnd(v[i]);
            }

            this->parent = parent;
            isLeaf = true;
            this->size = size;
            subtreeSize = 0;
            rank = parent->rank + size;
        }

        /**
         * @brief Destroys the node
         */
        ~Node() {
            delete keys;

            for(int i = 0; i < 3; i++) {
                delete values[i];
            }

            delete children;
        }

        /**
         * @brief Removes a key-value pair from the node
         * 
         * @param key Key to remove
         * @param value Value to remove
         */
        void removeKeyValPair(K key, V value) {
            for (int i = 0; i < size; i++) {
                if (keys[i] == key) {
                    for (int j = 0; j < values[i].size; j++) {
                        if (values[i][j] == value) {
                            values[i].removeAt(j);
                            if (values[i].size == 0) {
                                keys.removeAt(i);
                                for (int k = i; k < size - 1; k++) {
                                    keys[k] = keys[k + 1];
                                    values[k] = values[k + 1];
                                }
                                size--;
                            }
                            break;
                        }
                    }
                }
            }
        }

        /**
         * @brief Returns the child node that should be traversed to
         * 
         * @param key The key that is being checked
         * @return Node* The child node that should be traversed to
         */
        Node* traverseDirection(K key) {
            for (int i = 0; i < size; i++) {
                if (key <= keys[i]) return children[i]; // Should this be less than or equal to?
                else if (i == size - 1) return children[i + 1];
            }
        }

        /**
         * @brief Inserts a key-value pair into the node
         * 
         * @param key Key to insert
         * @param value Value to insert
         */
        void insertKeyValPair(K key, V value) {
            for (int i = 0; i < size; i++) {
                if (key < keys[i]) {
                    keys.addAt(key, i);
                    values[i].addAt(value, i);
                    size++;
                    break;
                } else if (key == keys[i]) {
                    values[i].addEnd(value);
                    break;
                } else if (i == size - 1) {
                    keys.addEnd(key);
                    values[i].addEnd(value);
                    size++;
                    break;
                }
            }
            // Do I need to do something for updating the rank? Also subtree size?
        }

        /**
         * @brief Prints the keys of the node
         */
        void printKeys() {
            for (int i = 0; i < size; i++) {
                if (i < size - 1) cout << keys[i] << " ";
                else cout << keys[i];
            }
            cout << endl;
        }
};