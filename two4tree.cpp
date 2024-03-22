#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdexcept> 
#include "Node.cpp"
#include "CircularDynamicArray.h"
using namespace std;

/*
TODO:
- Implement the remove function
    - Going to fucking suck to implement
- Implement the rank function
    - Shouldn't be hard once I figure out how to update the subtree size values
- Implement the select function
    - Shouldn't be hard once I figure out how to update the subtree size values
- Implement the duplicates function
- Implement the preorder function
- Implement the inorder function
- Implement the postorder function
- Implement the destructor
- Write a function that prints the tree in a readable format for debugging
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
        void split(Node<KeyType, ValueType>* nodeToSplit, KeyType newKey, 
        CircularDynamicArray<ValueType>& newValue) {
            // Get the middle key and value(s) and get ready to push them to the parent node
            KeyType keyHold = nodeToSplit->keys[1];
            CircularDynamicArray<ValueType> valueHold = nodeToSplit->values[1];

            // Start creating the left and right children of the new root node
            Node<KeyType, ValueType>* leftChild = new Node(nodeToSplit->keys[0], 
            nodeToSplit->values[0]);
            Node<KeyType, ValueType>* rightChild = new Node(nodeToSplit->keys[2], 
            nodeToSplit->values[2]);
                
            // Insert the new key-value pair into the correct child node
            if (newKey <= keyHold) leftChild->insertKeyValPair(newKey, newValue);
            else rightChild->insertKeyValPair(newKey, newValue);

            if (nodeToSplit == rootNode) { // Splitting root node
                // Create a new root node
                Node<KeyType, ValueType>* newRootNode = new Node(keyHold, valueHold);
                rootNode = newRootNode;

                // Add the children to the new root node, set children node's parents 
                // and delete the old root node
                rootNode->addChildNode(leftChild);
                rootNode->addChildNode(rightChild);
                leftChild->parent = rootNode;
                rightChild->parent = rootNode;
                
            } else { // Splitting Leaf or Internal Node
                // If the node to split is an internal node, distribute the children 
                // to the new left and right children
                if (nodeToSplit->isLeaf == false) { 
                    for (int i = 0; i < nodeToSplit->size; i++) {
                        if (i < 2) {
                            leftChild->addChildNode(nodeToSplit->children[i]);
                            nodeToSplit->children[i]->parent = leftChild;
                        } else {
                            rightChild->addChildNode(nodeToSplit->children[i]);
                            nodeToSplit->children[i]->parent = rightChild;
                        }
                    }
                }
                
                // Set the parent node of the left and right children
                leftChild->parent = nodeToSplit->parent;
                rightChild->parent = nodeToSplit->parent;

                // Recursively split parent nodes if they are overfull
                if (nodeToSplit->parent->size == 3) split(nodeToSplit->parent, keyHold, valueHold);
                else {
                    nodeToSplit->parent->insertKeyValPair(keyHold, valueHold);
                }

                // Delete old node from the parent node's children array, and add the new children
                nodeToSplit->parent->removeChildNode(nodeToSplit);
                nodeToSplit->parent->addChildNode(leftChild);
                nodeToSplit->parent->addChildNode(rightChild);
            }

            // Calculate the subtree sizes of the left and right children
            leftChild->calculateLeftSubtreeSize();
            rightChild->calculateLeftSubtreeSize();

            delete nodeToSplit;
            return;
        }

    public:
        /**
         * @brief Construct a new 2-4 tree object
         */
        two4Tree() {
            rootNode = nullptr;
            treeSize = 0;
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
            if (rootNode == nullptr){
                CircularDynamicArray<ValueType> *valueHolder = new CircularDynamicArray<ValueType>();
                valueHolder->addEnd(value);
                rootNode = new Node<KeyType, ValueType>(key, *valueHolder);
            } 
            // If the tree is not empty, find the correct leaf node to insert the key-value pair
            else { 
                Node<KeyType, ValueType>* refNode = rootNode;

                // Traverse the tree to find the correct leaf node
                while (!refNode->isLeaf) {
                    refNode = refNode->traverseDirection(key);
                }

                // Insert the key-value pair into the leaf node
                if (refNode->size < 3) {
                    for (int i = 0; i < refNode->size; i++) {
                        if (key <= refNode->keys[i]) {
                            refNode->keys.addAt(key, i);
                            refNode->values[i].addEnd(value);
                            refNode->size++;
                            break;
                        } else if (i == refNode->size - 1) {
                            refNode->keys.addEnd(key);
                            refNode->values[i].addEnd(value);
                            refNode->size++;
                        }
                    }

                    // Update the subtree size values of the node and its ancestors
                    refNode->calculateLeftSubtreeSize();
                } else {
                    split(refNode, key, value); // Something is wrong with the 
                                                // way that values are being passed here
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