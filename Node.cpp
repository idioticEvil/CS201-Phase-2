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
    public:
        CircularDynamicArray<K> keys;
        CircularDynamicArray<V> values[3];
        CircularDynamicArray<Node*> children;
        Node* parent;
        bool isLeaf;
        int size;
        int subtreeSize;

        /**
         * @brief Creates a new node with one key and a CircularDynamicArray of values
         * 
         * @param k Key
         * @param v CircularDynamicArray of values
         */
        Node(K k, CircularDynamicArray<V>& v) {
            keys.addEnd(k);
            values[0] = v;
            parent = nullptr;
            isLeaf = true;
            size = 1;
            subtreeSize = 0;
        }

        /**
         * @brief Creates a new node with multiple keys and values from an array
         * 
         * @param k Array of keys
         * @param v Array of values
         * @param size Size of the arrays
         */
        // Note: This program logic currently assumes that the values are sorted
        Node(K k[], V v[], int size) { 
            for (int i = 0; i < size; i++) {
                keys.addEnd(k[i]);
                values[i].addEnd(v[i]);
            }

            parent = nullptr;
            isLeaf = true;
            this->size = size;
            subtreeSize = 0;
        }

        /**
         * @brief Creates a new node with multiple keys and values from a CircularDynamicArray
         * 
         * @param k Array of keys
         * @param v Array of values
         * @param size Size of the arrays
         */
        Node(K k[], CircularDynamicArray<V> v[], int size) {
            for (int i = 0; i < size; i++) {
                keys.addEnd(k[i]);
                for (int j = 0; j < v[i].size; j++) {
                    values[i].addEnd(v[i][j]);
                }
            }

            parent = nullptr;
            isLeaf = true;
            this->size = size;
            subtreeSize = 0;
        }

        /**
         * @brief Creates a new node with one key, value, and parent
         * 
         * @param k Key
         * @param v Value
         * @param parent Parent node
         */
        Node(K k, V v, Node* parent) {
            keys.addEnd(k);
            values[0].addEnd(v);
            this->parent = parent;
            isLeaf = true;
            size = 1;
            subtreeSize = 0;
        }

        /**
         * @brief Inserts a key-value pair into the node
         * 
         * @param key Key to insert
         * @param value Value to insert
         */
        void insertKeyValPair(K key, CircularDynamicArray<V>& value) {
            for (int i = 0; i < size; i++) {
                if (key <= keys[i]) {
                    keys.addAt(key, i);
                    values[i] = value;
                    size++;
                    break;
                } else if (i == size - 1) {
                    keys.addEnd(key);
                    values[i + 1] = value;
                    size++;
                }
            }
            // Do I need to do something for updating the rank? Also subtree size?
        }

        /**
         * @brief Removes a key-value pair from the node
         * 
         * @param key Key to remove
         * @param value Value to remove
         */
        void removeKeyValPair(K key, CircularDynamicArray<V>& value) {
            for (int i = 0; i < size; i++) {
                if (key == keys[i]) {
                    keys.removeAt(i);
                    values[i].removeAt(i);
                    size--;
                    break;
                }
            }
        }

        /**
         * @brief Inserts a child node into the node at the correct spot
         * 
         * @param child Child node to insert
         */
        void addChildNode(Node* child) {
            // Check first if the node doesn't have any children
            if (children.size == 0) {
                children.addEnd(child);
            } else {
                // Use the last key of the child node to compare
                K comparisonKey = child->keys.getEndValue();

                for (int i = 0; i < size; i++) {
                    if (comparisonKey <= keys[i]) {
                        children.addAt(child, i);
                        break;
                    } else if (i == size - 1) {
                        children.addEnd(child);
                        break;
                    }
                }
            }

            this->isLeaf = false;
        }

        /**
         * @brief Removes a child node from the node
         * 
         * @param child Child node to remove
         */
        void removeChildNode(Node* child) {
            for (int i = 0; i < size; i++) {
                if (child == children[i]) {
                    children.removeAt(i);
                    break;
                }
            }

            if (children.size == 0) this->isLeaf = true;
        }

        /**
         * @brief Returns the child node that should be traversed to
         * 
         * @param key The key that is being checked
         * @return Node* The child node that should be traversed to
         */
        Node* traverseDirection(K key) {
            for (int i = 0; i < size; i++) {
                if (key <= keys[i]) return children[i];
                else if (i == size - 1) return children[i + 1];
            }
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