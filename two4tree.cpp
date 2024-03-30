#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdexcept> 
#include "Node.cpp"
#include "CircularDynamicArray.h"
using namespace std;

/*
TODO:
- Fix / Rewrite split function to match textbook implementation
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
         * @brief Splits a node when it is overfull
         * 
         * @param nodeToSplit Node to be split
         * @param newKey Key that would make the node overfull
         * @param newVals Value(s) corresponding to the key
         */
        void split(Node<KeyType, ValueType>* nodeToSplit, 
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
                    } else {
                        rightChild->addChildNode(nodeToSplit->children[i]);
                        nodeToSplit->children[i]->parent = rightChild;
                    }
                }
            }

            if (nodeToSplit == rootNode) {
                // Create a new root node
                Node<KeyType, ValueType>* newRootNode = new Node<KeyType, ValueType>(middleElement.getKey(), middleElement.getValues());
                rootNode = newRootNode;
                cout << "Created new root node with middle element key: " << middleElement.getKey() << endl;

                // Add the children to the new root node, set children node's parents 
                // and delete the old root node
                rootNode->addChildNode(leftChild);
                rootNode->addChildNode(rightChild);
                leftChild->parent = rootNode;
                rightChild->parent = rootNode;
                cout << "Added left and right children to new root node" << endl;

            } else { // Splitting Leaf or Internal Node    
                // Set the parent node of the left and right children
                leftChild->parent = nodeToSplit->parent;
                rightChild->parent = nodeToSplit->parent;

                // Store the parent of nodeToSplit in a temporary variable, then remove nodeToSplit
                // from the parent node's children array
                Node<KeyType, ValueType>* parentNode = nodeToSplit->parent;
                parentNode->removeChildNode(nodeToSplit);

                // Recursively split the parent node if it is overfull
                if (parentNode->size >= 3) {
                    cout << "Parent node is overfull, splitting it" << endl;
                    split(parentNode, middleElement);
                } else {
                    cout << "Parent node is not overfull, inserting key-value pair" << endl;
                    parentNode->insertKeyValPair(middleElement);
                }

                // Add the left and right children to the parent node
                parentNode->addChildNode(leftChild);
                parentNode->addChildNode(rightChild);          
            }

            // Calculate the subtree sizes of the left and right children
            leftChild->calculateLeftSubtreeSize();
            rightChild->calculateLeftSubtreeSize();

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
            // IMPLEMENT THIS
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
                    refNode->calculateLeftSubtreeSize();
                } else {
                    cout << "It's overfull! Splittin time!" << endl;
                    split(refNode, newElement);
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
            
        }

        /**
         * @brief 
         * 
         * @param refNode 
         * @param key 
         * @return int 
         */
        int rankActual(Node<KeyType, ValueType>* refNode, KeyType key, int count) {
            if (refNode == nullptr) return count;
            int tempCount = 0;

            for (int i = 0; i < refNode->size; i++) {
                
            }

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