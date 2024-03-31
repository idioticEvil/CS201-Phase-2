#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdexcept> 
#include "Node.cpp"
#include "CircularDynamicArray.h"
using namespace std;

/*
TODO:
- Implement search function
- Implement remove function
- Implement rank function
- Implement select function
- Implement duplicates function
- Update tree traversal functions to print out duplicate keys
- Update logic to treat duplicates as multiple keys
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
            nodeToSplit->printFullNode();

            // Get the middle key and value(s) and get ready to push them to the parent node
            NodeElement<KeyType, ValueType> middleElement = nodeToSplit->elements[1];
            cout << "Middle element key: " << middleElement.getKey() << endl;
            cout << "Middle element value: " << middleElement.getValues()[0] << endl;

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
                cout << "Created new root node with middle element key: " << middleElement.getKey() << endl;
                
                // Add the left and right children to the parent node
                parentNode->addChildNode(leftChild);
                parentNode->addChildNode(rightChild);
                leftChild->parent = parentNode;
                rightChild->parent = parentNode;
            } else { // Splitting internal or root node
                // If the parent node is overfull, split it, otherwise insert the middle element
                if (parentNode->size >= 3) {
                    cout << "Parent node with key: " << parentNode->elements[0].getKey() << " is overfull, splitting" << endl;
                    parentNode = split(parentNode, middleElement);
                } else {
                    cout << "Parent node is not overfull, inserting key-value pair" << endl;
                    parentNode->insertKeyValPair(middleElement);
                }

                // Remove the node to split from the parent node's children array
                parentNode->removeChildNode(nodeToSplit);
                // Add the left and right children to the parent node
                cout << "Adding left and right children to parent node with key: " << parentNode->elements[0].getKey() << endl;
                cout << "Left child key: " << leftChild->elements[0].getKey() << " Right child key: " << rightChild->elements[0].getKey() << endl;
                leftChild->parent = parentNode;
                rightChild->parent = parentNode;
                parentNode->addChildNode(leftChild);
                cout << "Added left child with key: " << leftChild->elements[0].getKey() << " to parent node with key: " << parentNode->elements[0].getKey() << endl;
                parentNode->addChildNode(rightChild);
                cout << "Added right child with key: " << rightChild->elements[0].getKey() << " to parent node with key: " << parentNode->elements[0].getKey() << endl;
            }

            // Calculate the subtree sizes of the left and right children
            leftChild->updateLeftSubtreeSizes();
            rightChild->updateLeftSubtreeSizes();

            if (returnNode != nullptr) return returnNode;
            else return parentNode;
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

            cout << "Started empty tree constructor" << endl;

            // Insert the keys and values into the tree
            for (int i = 0; i < size; i++) {
                cout << "About to insert key: " << keys[i] << " and value: " << values[i] << endl;
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
            while (rootNode != nullptr) {
                // Traverse the tree to find the correct leaf node
                for (int i = 0; i < rootNode->size; i++) {
                    if (key == rootNode->elements[i].getKey()) { 
                        // If the key is found, return the value
                        return &(rootNode->elements[i].getValues()[0]);
                    } else if (key < rootNode->elements[i].getKey()) { 
                        // If the key is less than the current key, traverse left
                        rootNode = rootNode->children[i];
                        break;
                    } else if (i == rootNode->size - 1) { 
                        // If the key is greater than the last key, traverse right
                        rootNode = rootNode->children[rootNode->size];
                        break;
                    }
                }
            }
            return nullptr; // Return nullptr if the key is not found
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
            cout << "Created new element with key: " << key << " and value: " << value << endl;

            // Check if the tree is empty, if so, create a new root node
            if (rootNode == nullptr){
                rootNode = new Node<KeyType, ValueType>(key, value);
                cout << "Inserted key: " << key << " and value: " << value << endl;
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
                    refNode->insertKeyValPair(newElement);
                    cout << "Inserted key: " << key << " and value: " << value << endl;
                    refNode->printFullNode();

                    // Update the subtree size values of the node and its ancestors
                    refNode->updateLeftSubtreeSizes();
                } else {
                    cout << "It's overfull! Splittin time!" << endl;
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
            // IMPLEMENT THIS
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
         * @brief Returns the rank of a key in the tree, but actually does the traversal
         * 
         * @param refNode The node currently being traversed
         * @param key The key to find the rank of
         * @return int The rank of the key
         */
        int rankActual(Node<KeyType, ValueType>* refNode, KeyType key) {
            if (refNode == nullptr) return 0; // Base case

            int rank = 0;
            for (int i = 0; i < refNode->size; i++) {
                if (i == 0 && key < refNode->elements[0].getKey()) {
                    return rank + rankActual(refNode->children[0], key);
                } else if (key > refNode->elements[i].getKey()) {
                    rank += refNode->elements[i].getSubtreeSize();
                } else if (key < refNode->elements[i].getKey()) {
                    return rank + rankActual(refNode->children[i], key);
                } else if (key == refNode->elements[i].getKey()){
                    return rank + refNode->elements[i].getSubtreeSize();
                }
            }

            return rank + rankActual(refNode->children[refNode->size], key);
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
         * @brief Prints the pre-order traversal of the tree, more of a shell functino
         */
        void preorder() {
            preorderActual(rootNode);
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
         * @brief Prints the pre-order traversal of the tree, but prints the attributes of the nodes
         * 
         * @param refNode The node currently being traversed
         */
        void preorderPrintAttributes(Node<KeyType, ValueType>* refNode) {
            // Check if the node is null
            if (refNode == nullptr) return;

            // Print the keys of the node
            refNode->printFullNode();

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

                // Print the key
                cout << refNode->elements[i].getKey() << " ";
            }

            // Visit the last child after the last key, if the node is not a leaf
            if (!refNode->isLeaf) inorderActual(refNode->children[refNode->size]);
        }

        /**
         * @brief Prints the post-order traversal of the tree
         */
        void postorder() {
            postorderActual(rootNode);
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
};