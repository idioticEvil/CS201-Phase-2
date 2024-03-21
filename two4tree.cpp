#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdexcept> 
#include "Node.cpp"
#include "CircularDynamicArray.h"
using namespace std;

/*
TODO:
- Change way that duplicate keys are handled, one key to an array of values (Side note: Are you 
  fucking serious?)
    - Change arrays for keys, values, and children in nodes to be of static size
    - Make the value arrays be an array of circular dynamic arrays for the values (which is fucking
      stupid but I digress)
    - Update necessary program logic
- Implement Split function
- Add subtree size values to node class, figure out how to update them for insertion & deletion
*/

/**
 * @brief A 2-4 tree that can store any type of data
 * 
 * @tparam KeyType The type of the keys in the tree
 * @tparam ValueType The type of the values in the tree
 */
template <typename KeyType, typename ValueType> class two4Tree {
    private:
        Node<KeyType, ValueType>* rootNode;
        int treeSize;

        /**
         * @brief Splits a node when it is overfull
         * 
         * @param nodeToSplit Node to be split
         * @param newKey Key that would make the node overfull
         * @param newVals Value(s) corresponding to the key
         */
        void split(Node<KeyType, ValueType>* nodeToSplit, KeyType newKey, CircularDynamicArray<ValueType>& newValue) {
            if (nodeToSplit == rootNode) {
                KeyType keyHold = nodeToSplit->keys[1];
                CircularDynamicArray<ValueType> valueHold = nodeToSplit->values[1];
                Node<KeyType, ValueType> newRootNode = new Node(keyHold, valueHold);
                rootNode = newRootNode;

                Node<KeyType, ValueType>* leftChild = new Node(nodeToSplit->keys[0], nodeToSplit->values[0]);
                Node<KeyType, ValueType>* rightChild = new Node(nodeToSplit->keys[2], nodeToSplit->values[2]);
                
                if (newKey <= keyHold) leftChild->insertKeyValPair(newKey, newValue);
                else rightChild->insertKeyValPair(newKey, newValue);

                rootNode->addChildNode(leftChild);
                rootNode->addChildNode(rightChild);
                leftChild->setParentNode(rootNode);
                rightChild->setParentNode(rootNode);
                delete nodeToSplit;  
            }
        }

        /**
         * @brief Construct a new 2-4 tree object from an array of keys and an array of values
         * 
         * @param keys The array of keys to insert into the tree
         * @param values The array of values to insert into the tree
         * @param size The size of the arrays
         */
        two4Tree(KeyType keys[], ValueType values[], int size) {
            // IMPLEMENT THIS
        }

        /**
         * @brief Destroy the 2-4 tree object
         */
        ~two4Tree() {
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
                Node<KeyType, ValueType>* refNode = rootNode;

                // Traverse the tree to find the correct leaf node
                while (!refNode->isLeaf) {
                    refNode = refNode->traverseDirection(key);
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
                        } else if (i == refNode->size - 1) {
                            refNode->keys.addEnd(key);
                            refNode->values.addEnd(value);
                            refNode->size++;
                        }
                    }
                } else {
                    // Split
                }
            }
            treeSize++;
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
            return treeSize;
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