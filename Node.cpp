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
        int leftSubtreeSize;

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
            leftSubtreeSize = 0;
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
            leftSubtreeSize = 0;
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
            leftSubtreeSize = 0;
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
            leftSubtreeSize = 0;
        }

        /**
         * @brief Inserts a key-value pair into the node
         * 
         * @param key Key to insert
         * @param value Value to insert
         */
        void insertKeyValPair(K key, CircularDynamicArray<V>& value) {
            for (int i = 0; i < size; i++) {
                if (key < keys[i]) {
                    keys.addAt(key, i);
                    valuesArrayInsert(i, value);
                    size++;
                    break;
                } else if (key == keys[i]) {
                    values[i].addEnd(value[0]);
                    break;
                } else if (i == size - 1) {
                    keys.addEnd(key);
                    valuesArrayInsert(size, value);
                    size++;
                    break;
                }
            }
        }

        /**
         * @brief Resorts the values array for insertion since it is a 2D array, I fucking hate this
         * 
         * @param index Index that the new value is going to be inserted at
         * @param valueToInsert New value to insert
         */
        void valuesArrayInsert(int index, CircularDynamicArray<V>& valueToInsert) {
            CircularDynamicArray<V> tempArray[size + 1];

            tempArray[index] = valueToInsert;
            for (int i = 0; i < index; i++) {
                tempArray[i] = values[i];
            }
            for (int i = index; i < size; i++) {
                tempArray[i + 1] = values[i];
            }
            for (int i = 0; i < size + 1; i++) {
                values[i] = tempArray[i];
            }
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
            int childSize = children.length();

            // Check first if the node doesn't have any children
            if (childSize == 0) {
                children.addEnd(child);
            } else {
                // Use the last key of the child node to compare
                K comparisonKey = child->keys.getEndValue();

                // Find the correct spot to insert the child node
                for (int i = 0; i < childSize; i++) {
                    if (comparisonKey <= keys[i]) {
                        children.addAt(child, i);
                        break;
                    } else if (i == childSize - 1) {
                        children.addEnd(child);
                        break;
                    }
                }
            }

            // Update the leaf status of the node
            this->isLeaf = false;
        }

        /**
         * @brief Removes a child node from the node
         * 
         * @param child Child node to remove
         */
        void removeChildNode(Node* child) {
            int childSize = children.length();

            // Find the child node to remove
            for (int i = 0; i < childSize; i++) {
                if (child == children[i]) {
                    children.removeAt(i);
                    break;
                }
            }

            // Check if the node is now a leaf
            if (childSize == 0) this->isLeaf = true;
        }

        /**
         * @brief Recalculates the left subtree size of a node
         * 
         * @param checkNode The node to recalculate the subtree size of
         */
        void calculateLeftSubtreeSize() {
            // Create a reference node to traverse up the tree
            Node<K, V>* refNode = this;

            // Traverse up the tree to recalculate the left subtree size of the node
            while (refNode != nullptr) {
                // Reset the subtree size of the node before recalculating
                refNode->leftSubtreeSize = 0;

                // Check if the node is not a leaf
                if (!refNode->isLeaf) {
                    // Recalculate the left subtree size of the node
                    for (int i = 0; i < refNode->keys.length(); i++) {
                        if (refNode->children[i]->keys[0] <= refNode->keys[i]) {
                            refNode->leftSubtreeSize += (refNode->children[i]->leftSubtreeSize + 
                            refNode->children[i]->size);
                        } 
                    }
                }
                refNode = refNode->parent;
            }
        }

        /**
         * @brief Returns the child node that should be traversed to
         * 
         * @param key The key that is being checked
         * @return Node* The child node that should be traversed to
         */
        Node* traverseDirection(K key) {
            // Traverse the tree to find the correct child node to go to
            for (int i = 0; i < size; i++) {
                if (key <= keys[i]) return children[i];
            }
            return children[size];
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

        /**
         * @brief Prints the full attributes of the node
         */
        void printFullNode() {
            cout << endl << "NODE STATUS:" << endl;
            cout << "Size: " << size << endl;
            cout << "Is Leaf: " << isLeaf << endl;
            cout << "Left Subtree Size: " << leftSubtreeSize << endl;

            cout << "Keys: ";
            for (int i = 0; i < size; i++) {
                if (i < size - 1) cout << keys[i] << ", ";
                else cout << keys[i];
            }
            cout << endl;

            cout << "Values: ";
            for (int i = 0; i < size; i++) {
                cout << "[";
                for (int j = 0; j < values[i].length(); j++) {
                    if (j < values[i].length() - 1) cout << values[i][j] << ", ";
                    else cout << values[i][j];
                }
                cout << "]";
                if (i < size - 1) cout << ", ";
            }
            cout << endl << endl;
        }
};