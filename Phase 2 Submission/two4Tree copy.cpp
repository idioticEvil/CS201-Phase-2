#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdexcept> 
#include "Node.cpp"
#include "CircularDynamicArray.h"
using namespace std;

/*
TODO:
- Implement remove function
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
         * @brief Splits a node into two nodes and returns the parent node
         * 
         * @param nodeToSplit The overfull node to split
         * @param newElement The new key-value pair to insert
         * @return Node<KeyType, ValueType>* A pointer to the parent node of the split nodes
         */
        Node<KeyType, ValueType>* split(Node<KeyType, ValueType>* nodeToSplit, 
            NodeElement<KeyType, ValueType> newElement) {

            // Get the middle key and value(s) and get ready to push them to the parent node
            NodeElement<KeyType, ValueType> middleElement = nodeToSplit->elements[1];

            // Start creating the left and right children of the new root node
            Node<KeyType, ValueType>* leftChild = new Node<KeyType, ValueType>
            (nodeToSplit->elements[0].getKey(), nodeToSplit->elements[0].getValues());
            Node<KeyType, ValueType>* rightChild = new Node<KeyType, ValueType>
            (nodeToSplit->elements[2].getKey(), nodeToSplit->elements[2].getValues());

            // Pointer to hold what node to return for proper parent child relationship assignment
            Node<KeyType, ValueType>* returnNode = nullptr;
                
            // Insert the new key-value pair into the correct child node
            if (newElement.getKey() < middleElement.getKey()) 
            leftChild->insertKeyValPair(newElement);
            else rightChild->insertKeyValPair(newElement);

            // If the node to split is an internal node, distribute the children 
            // to the new left and right children
            if (nodeToSplit->isLeaf == false) { 
                for (int i = 0; i < nodeToSplit->children.length(); i++) {
                    if (i < 2) {
                        leftChild->addChildNode(nodeToSplit->children[i]);
                        nodeToSplit->children[i]->parent = leftChild;
                        returnNode = leftChild;
                    } else {
                        rightChild->addChildNode(nodeToSplit->children[i]);
                        nodeToSplit->children[i]->parent = rightChild;
                        returnNode = rightChild;
                    }
                }
            }

            // Get the parent node of the node to split
            Node<KeyType, ValueType>* parentNode = nodeToSplit->parent;

            if (parentNode == nullptr) { // Splitting Root Node
                parentNode = new Node<KeyType, ValueType>(middleElement.getKey(), middleElement.getValues());
                rootNode = parentNode;
                
                // Add the left and right children to the parent node
                parentNode->addChildNode(leftChild);
                parentNode->addChildNode(rightChild);
                leftChild->parent = parentNode;
                rightChild->parent = parentNode;
            } else { // Splitting internal or root node
                // If the parent node is overfull, split it, otherwise insert the middle element
                if (parentNode->size >= 3) {
                    //cout << "Parent node with key: " << parentNode->elements[0].getKey() << " is overfull, splitting" << endl;
                    parentNode = split(parentNode, middleElement);
                } else {
                    //cout << "Parent node is not overfull, inserting key-value pair" << endl;
                    parentNode->insertKeyValPair(middleElement);
                }

                // Remove the node to split from the parent node's children array
                parentNode->removeChildNode(nodeToSplit);
                // Add the left and right children to the parent node
                leftChild->parent = parentNode;
                rightChild->parent = parentNode;
                parentNode->addChildNode(leftChild);
                parentNode->addChildNode(rightChild);
            }

            // Calculate the subtree sizes of the left and right children
            leftChild->updateSubtreeSizes();
            rightChild->updateSubtreeSizes();

            // Return the correct node to assign the parent child relationship
            if (returnNode != nullptr) return returnNode;
            else return parentNode;
        }

        /**
         * @brief Returns the rank of a key in the tree, but actually does the traversal
         * 
         * @param refNode The node currently being traversed
         * @param key The key to find the rank of
         * @return int The rank of the key
         */
        int rankActual(Node<KeyType, ValueType>* refNode, KeyType key) {
            if (refNode == nullptr) return 0; // Base case

            int rank = 0;
            // Traverse the tree to find the correct rank of the key
            for (int i = 0; i < refNode->size; i++) {
                if (i == 0 && key < refNode->elements[0].getKey()) { 
                    // If the key is less than the first key
                    return rank + rankActual(refNode->children[0], key);
                } else if (key > refNode->elements[i].getKey()) { 
                    // If the key is greater than the current key
                    rank += refNode->elements[i].getSubtreeSize();
                } else if (key < refNode->elements[i].getKey()) { 
                    // If the key is less than the current key
                    return rank + rankActual(refNode->children[i], key);
                } else if (key == refNode->elements[i].getKey()){ 
                    // If the key is equal to the current key
                    return rank + refNode->elements[i].getSubtreeSize();
                }
            }
            return rank + rankActual(refNode->children[refNode->size], key);
        }

        /**
         * @brief Returns the key at a given position in the tree, but actually does the traversal
         * 
         * @param refNode The node currently being traversed
         * @param pos The position of the key to find
         * @return KeyType The key at the given position
         */
        KeyType selectActual(Node<KeyType, ValueType>* refNode, int pos) {
            if (refNode == nullptr) return 0; // Base case

            int rank = 0;
            for (int i = 0; i < refNode->size; i++) {
                if (rank + refNode->elements[i].getSubtreeSize() >= pos) {
                    if (refNode->isLeaf || rank + refNode->elements[i].getSubtreeSize() == pos) {
                        return refNode->elements[i].getKey();
                    } else {
                        return selectActual(refNode->children[i], pos - rank);
                    }
                } else {
                    rank += refNode->elements[i].getSubtreeSize();
                }
            }

            return selectActual(refNode->children[refNode->size], pos - rank);
        }

        /**
         * @brief Returns the number of duplicates of a specific key in the tree, but actually does the traversal
         * 
         * @param refNode The node currently being traversed
         * @param key The key to find the number of duplicates of
         * @return int The number of duplicates of the key
         */
        int duplicatesActual(Node<KeyType, ValueType>* refNode, KeyType key, int& count) {
            if (refNode == nullptr) return count; // Base case

            // Traverse the children of the node
            for (int i = 0; i < refNode->size; i++) {
                // Visit the child before the key
                if (!refNode->isLeaf) duplicatesActual(refNode->children[i], key, count);

                // Check if the key is a duplicate
                if (key == refNode->elements[i].getKey() && refNode->elements[i].getNumValues() > 0) {
                    for (int j = 0; j < refNode->elements[i].getNumValues(); j++) {
                        count++;
                    }
                }
            }

            // Visit the last child after the last key, if the node is not a leaf
            if (!refNode->isLeaf) duplicatesActual(refNode->children[refNode->size], key, count);

            return count;
        }

        /**
         * @brief Prints the pre-order traversal of the tree, but actually does the traversal
         * 
         * @param refNode The node currently being traversed
         */
        void preorderActual(Node<KeyType, ValueType>* refNode) {
            // Check if the node is null
            if (refNode == nullptr) return;

            // Print the keys of the node
            refNode->printKeys();

            // Recursively traverse the children of the node
            if (!refNode->isLeaf) {
                for (int i = 0; i < refNode->children.length(); i++) {
                    preorderActual(refNode->children[i]);
                }
            }
        }

        /**
         * @brief Actually traverses the tree in-order recursively
         * 
         * @param refNode The node currently being traversed
         */
        void inorderActual(Node<KeyType, ValueType>* refNode) {
            // Check if the node is null
            if (refNode == nullptr) return;

            // Traverse the children of the node
            for (int i = 0; i < refNode->size; i++) {
                // Visit the child before the key
                if (!refNode->isLeaf) inorderActual(refNode->children[i]);

                // Print all the duplicates of the key
                for (int j = 0; j < refNode->elements[i].getNumValues(); j++) {
                    cout << refNode->elements[i].getKey() << " ";
                }
            }

            // Visit the last child after the last key, if the node is not a leaf
            if (!refNode->isLeaf) inorderActual(refNode->children[refNode->size]);
        }

        /**
         * @brief Actually traverses the tree post-order recursively
         * 
         * @param refNode The node currently being traversed
         */
        void postorderActual(Node<KeyType, ValueType>* refNode) {
            // Check if the node is null
            if (refNode == nullptr) return;

            // Recursively traverse the children of the node
            if (!refNode->isLeaf) {
                for (int i = 0; i < refNode->children.length(); i++) {
                    postorderActual(refNode->children[i]);
                }
            }

            // Print the keys of the node
            refNode->printKeys();
        }

        /**
         * @brief Swaps a key with its inorder successor or predecessor, used in the remove function
         * 
         * @param refNode Node that contains the key to swap
         * @param key The key to swap
         */
        void inorderRemovalSwap(Node<KeyType, ValueType>* refNode, KeyType key) {
            // Check if the node is null
            if (refNode == nullptr) return;

            // Find the key in the node
            int index = refNode->findKeyIndex(key);

            // Check if the key is found in the node
            if (index != -1) {
                // Check if the node has a left child
                if (!refNode->isLeaf && index > 0) {
                    // Find the inorder predecessor
                    Node<KeyType, ValueType>* predecessor = refNode->children[index - 1];
                    while (!predecessor->isLeaf) {
                        predecessor = predecessor->children[predecessor->size];
                    }

                    // Swap the key with the predecessor
                    NodeElement<KeyType, ValueType> predecessorElement = predecessor->elements[predecessor->size - 1];
                    refNode->elements[index].setKey(predecessorElement.getKey());
                    refNode->elements[index].setValues(predecessorElement.getValues());
                    key = predecessorElement.getKey();
                    refNode = predecessor;
                }
                // Check if the node has a right child
                else if (!refNode->isLeaf && index < refNode->size) {
                    // Find the inorder successor
                    Node<KeyType, ValueType>* successor = refNode->children[index + 1];
                    while (!successor->isLeaf) {
                        successor = successor->children[0];
                    }

                    // Swap the key with the successor
                    NodeElement<KeyType, ValueType> successorElement = successor->elements[0];
                    refNode->elements[index].setKey(successorElement.getKey());
                    refNode->elements[index].setValues(successorElement.getValues());
                    key = successorElement.getKey();
                    refNode = successor;
                }
            }

            // Delete the key from the appropriate child node
            refNode->removeKey(key);
        }
        
        /**
         * @brief Restructures the tree after a key is removed
         * 
         * @param refNode The node to restructure after removal
         */
        void restructureAfterRemoval(Node<KeyType, ValueType>* refNode) {
            // Check if the node is null
            if (refNode == rootNode) {
                if (refNode->size == 0) {
                    rootNode = refNode->children[0];
                    delete refNode;
                }
                return;
            }

            int minKeys = refNode->isLeaf ? 1 : 2;

            if (refNode->size >= minKeys) {
                return;
            }

            // Find the parent node of the node to restructure
            Node<KeyType, ValueType>* parentNode = refNode->parent;
            int nodeIndex = parentNode->findKeyIndex(refNode->elements[0].getKey());

            // Find the left and right siblings of the node
            Node<KeyType, ValueType>* leftSibling = (nodeIndex > 0) ? parentNode->children[nodeIndex - 1] : nullptr;
            Node<KeyType, ValueType>* rightSibling = (nodeIndex < parentNode->size) ? parentNode->children[nodeIndex + 1] : nullptr;

            // Check if the left sibling has more than the minimum number of keys
            if (leftSibling && leftSibling->size > minKeys) {
                NodeElement<KeyType, ValueType> borrowedKey = leftSibling->elements[leftSibling->size - 1];
                refNode->insertKeyValPair(borrowedKey);

                // Check if the left sibling has children
                if (!refNode->isLeaf) {
                    Node<KeyType, ValueType>* borrowedChild = leftSibling->children[leftSibling->size];
                    refNode->addChildNode(borrowedChild);
                    leftSibling->removeChildNode(leftSibling->children[leftSibling->size]);
                }

                // Check if the left sibling has more than one key
                parentNode->elements[nodeIndex - 1].setKey(leftSibling->elements[leftSibling->size - 2].getKey());
                leftSibling->removeKey(leftSibling->elements[leftSibling->size - 2].getKey());
                return;
            }

            // Check if the right sibling has more than the minimum number of keys
            if (rightSibling && rightSibling->size > minKeys) {
                NodeElement<KeyType, ValueType> borrowedKey = rightSibling->elements[0];
                refNode->insertKeyValPair(borrowedKey);

                // Check if the right sibling has children
                if (!refNode->isLeaf) {
                    Node<KeyType, ValueType>* borrowedChild = rightSibling->children[0];
                    refNode->addChildNode(borrowedChild);
                    rightSibling->removeChildNode(rightSibling->children[0]);
                }

                // Check if the right sibling has more than one key
                if (rightSibling->size > 1) {
                    parentNode->elements[nodeIndex].setKey(rightSibling->elements[1].getKey());
                    rightSibling->removeKey(rightSibling->elements[1].getKey());
                } else {
                    parentNode->elements[nodeIndex].setKey(borrowedKey.getKey());
                    rightSibling->removeKey(borrowedKey.getKey());
                }
                return;
            }

            // Merge with a sibling or move children up to parent
            if (leftSibling) {
                leftSibling->insertKeyValPair(parentNode->elements[nodeIndex - 1]);
                leftSibling->merge(refNode);
                parentNode->removeChildNode(parentNode->children[nodeIndex]);
            } else if (rightSibling) {
                refNode->insertKeyValPair(parentNode->elements[nodeIndex]);
                refNode->merge(rightSibling);
                parentNode->removeChildNode(parentNode->children[nodeIndex + 1]);
            } else {
                for (int i = 0; i < refNode->size; i++) {
                    parentNode->insertKeyValPair(refNode->elements[i]);
                }
                parentNode->removeChildNode(refNode);
            }

            restructureAfterRemoval(parentNode);
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
            rootNode = nullptr;
            treeSize = 0;

            // Insert the keys and values into the tree
            for (int i = 0; i < size; i++) {
                insert(keys[i], values[i]);
            }
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
            // Use a pointer to reference the current node being traversed
            Node<KeyType, ValueType>* currentNode = rootNode;

            // Traverse the tree to find the correct leaf node
            while (currentNode != nullptr) {
                for (int i = 0; i < currentNode->size; i++) {
                    if (key == currentNode->elements[i].getKey()) { 
                        // If the key is found, return the value
                        return &(currentNode->elements[i].getValues()[0]);
                    } else if (key < currentNode->elements[i].getKey()) { 
                        // If the key is less than the current key, traverse left
                        currentNode = currentNode->children[i];
                        break;
                    } else if (i == currentNode->size - 1) { 
                        // If the key is greater than the last key, traverse right
                        currentNode = currentNode->children[currentNode->size];
                        break;
                    }
                }
            }
            // Return nullptr if the key is not found
            return nullptr; 
        }

        /**
         * @brief Inserts a key-value pair into the tree
         * 
         * @param key The key to insert
         * @param value The value to insert
         */
        void insert(KeyType key, ValueType value) {
            // Create a new NodeElement object to store the key-value pair
            NodeElement<KeyType, ValueType> newElement = NodeElement<KeyType, ValueType>(key, value);

            // Check if the tree is empty, if so, create a new root node
            if (rootNode == nullptr) rootNode = new Node<KeyType, ValueType>(key, value); 
            // If the tree is not empty, find the correct leaf node to insert the key-value pair
            else { 
                Node<KeyType, ValueType>* refNode = rootNode;

                // Traverse the tree to find the correct leaf node
                while (!refNode->isLeaf) refNode = refNode->traverseDirection(key);

                // Insert the key-value pair into the leaf node
                if (refNode->size < 3) {
                    refNode->insertKeyValPair(newElement);
                    //refNode->printFullNode();

                    // Update the subtree size values of the node and its ancestors
                    refNode->updateSubtreeSizes();
                } else {
                    Node<KeyType, ValueType>* newParentNode = split(refNode, newElement);
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
            // Use a pointer to reference the current node being traversed
            Node<KeyType, ValueType>* currentNode = rootNode;

            // Traverse the tree to find the correct leaf node
            while (currentNode != nullptr) {
                for (int i = 0; i < currentNode->size; i++) {
                    if (key == currentNode->elements[i].getKey()) { 
                        break; 
                    } else if (key < currentNode->elements[i].getKey()) { 
                        // If the key is less than the current key, traverse left
                        currentNode = currentNode->children[i];
                        break;
                    } else if (i == currentNode->size - 1) { 
                        // If the key is greater than the last key, traverse right
                        currentNode = currentNode->children[currentNode->size];
                        break;
                    }
                }
            }

            // If the key is not found, return 0
            if (currentNode == nullptr) return 0;
            else if (!currentNode->isLeaf) { // Node is an internal node
                // Swap the key with its inorder successor or predecessor
                //inorderRemovalSwap(currentNode, key);
                return 1;
            } else { // Node is a leaf node
                // remove key from node, restructure if necessary
                switch (currentNode->removeKey(key)) {
                    case 0: // Key not found
                        return 0;
                    case 1: // Key removed, restructuring needed
                        //restructureAfterRemoval(currentNode);
                        return 1;
                    case 2: // Key removed, restructuring not needed
                        return 1;
                }
            }
            return 0;
        }

        /**
         * @brief Returns the rank of a key in the tree
         * 
         * @param key The key to find the rank of
         * @return int The rank of the key
         */
        int rank(KeyType key) {
            return rankActual(rootNode, key);
        }

        /**
         * @brief Returns the key at a given position in the tree
         * 
         * @param pos The position of the key to find
         * @return KeyType The key at the given position
         */
        KeyType select(int pos) {
            if (pos < 1 || pos > treeSize) {
                throw std::out_of_range("Index out of range");
            }
            return selectActual(rootNode, pos);
        }

        /**
         * @brief Returns the number of duplicates of a specific key in the tree
         * 
         * @param key The key to find the number of duplicates of
         * @return int The number of duplicates of the key
         */
        int duplicates(KeyType key) {
            int count = 0;
            return duplicatesActual(rootNode, key, count);
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
         * @brief Prints the pre-order traversal of the tree, more of a shell functino
         */
        void preorder() {
            preorderActual(rootNode);
        }

        /**
         * @brief Prints the pre-order traversal of the tree, but prints the attributes of the nodes
         * 
         * @param refNode The node currently being traversed
         */
        void preorderPrintAttributes(Node<KeyType, ValueType>* refNode) {
            // Check if the node is null
            if (refNode == nullptr) return;

            // Print the keys of the node
            refNode->printKeys();

            // Recursively traverse the children of the node
            if (!refNode->isLeaf) {
                for (int i = 0; i < refNode->children.length(); i++) {
                    preorderPrintAttributes(refNode->children[i]);
                }
            }
        }

        /**
         * @brief Returns the root node of the tree
         * 
         * @return Node<KeyType, ValueType>* The root node of the tree
         */
        Node<KeyType, ValueType>* getRootNode() {
            return rootNode;
        }

        /**
         * @brief Prints the in-order traversal of the tree
         */
        void inorder() {
            inorderActual(rootNode);
            cout << endl;
        }

        /**
         * @brief Prints the post-order traversal of the tree
         */
        void postorder() {
            postorderActual(rootNode);
        }
};