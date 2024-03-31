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
         * @brief Splits a node in the tree
         * 
         * @param nodeToSplit The node to split
         * @param newElement The new element to insert into the tree
         * @return Node<KeyType, ValueType>* Pointer to the parent node of the split nodes
         */
        Node<KeyType, ValueType>* split(Node<KeyType, ValueType>* nodeToSplit, NodeElement<KeyType, ValueType> newElement) {
            nodeToSplit->printFullNode();

            // Create a new node element to store the middle value
            NodeElement<KeyType, ValueType> middleElement = nodeToSplit->elements[1];
            cout << "Middle element key: " << middleElement.getKey() << endl;
            cout << "Middle element value: " << middleElement.getValues()[0] << endl;

            // Create new child nodes for the split
            Node<KeyType, ValueType>* leftChild = createChildNode(nodeToSplit->elements[0]);
            Node<KeyType, ValueType>* rightChild = createChildNode(nodeToSplit->elements[2]);

            // Insert the new element into the correct child node
            insertElementToChild(newElement, middleElement, leftChild, rightChild);

            // Distribute the children of the node to split to the new child nodes
            if (!nodeToSplit->isLeaf) distributeChildren(nodeToSplit, leftChild, rightChild);

            // Handle the parent node of the split nodes
            Node<KeyType, ValueType>* parentNode = handleParentNode(nodeToSplit, middleElement, leftChild, rightChild);

            return parentNode;
        }

        /**
         * @brief Creates a new child node for a split operation
         * 
         * @param element The element to create the child node from
         * @return Node<KeyType, ValueType>* The new child node
         */
        Node<KeyType, ValueType>* createChildNode(NodeElement<KeyType, ValueType> element) {
            return new Node<KeyType, ValueType>(element.getKey(), element.getValues());
        }

        /**
         * @brief Inserts a new element into the correct child node
         * 
         * @param newElement The new element to insert
         * @param middleElement The middle element of the node to split
         * @param leftChild The left child node
         * @param rightChild The right child node
         */
        void insertElementToChild(NodeElement<KeyType, ValueType> newElement, NodeElement<KeyType, ValueType> middleElement, Node<KeyType, ValueType>* leftChild, Node<KeyType, ValueType>* rightChild) {
            if (newElement.getKey() < middleElement.getKey()) 
                leftChild->insertKeyValPair(newElement);
            else 
                rightChild->insertKeyValPair(newElement);
        }

        /**
         * @brief Distributes the children of a node to the new child nodes
         * 
         * @param nodeToSplit The node to split
         * @param leftChild The left child node
         * @param rightChild The right child node
         */
        void distributeChildren(Node<KeyType, ValueType>* nodeToSplit, Node<KeyType, ValueType>* leftChild, Node<KeyType, ValueType>* rightChild) {
            for (int i = 0; i < nodeToSplit->children.length(); i++) {
                if (i < 2) { // Add the first two children to the left child
                    leftChild->addChildNode(nodeToSplit->children[i]);
                    nodeToSplit->children[i]->parent = leftChild;
                } else { // Add the last two children to the right child
                    rightChild->addChildNode(nodeToSplit->children[i]);
                    nodeToSplit->children[i]->parent = rightChild;
                }
            }
        }

        /**
         * @brief Handles the parent node of the split nodes
         * 
         * @param nodeToSplit The node to split
         * @param middleElement The middle element of the node to split
         * @param leftChild The left child node
         * @param rightChild The right child node
         * @return Node<KeyType, ValueType>* The parent node of the split nodes
         */
        Node<KeyType, ValueType>* handleParentNode(Node<KeyType, ValueType>* nodeToSplit, NodeElement<KeyType, ValueType> middleElement, Node<KeyType, ValueType>* leftChild, Node<KeyType, ValueType>* rightChild) {
            Node<KeyType, ValueType>* parentNode = nodeToSplit->parent;

            if (parentNode == nullptr) { // Splitting Root Node
                parentNode = createNewRootNode(middleElement, leftChild, rightChild);
            } else { // Splitting internal or root node
                parentNode = handleInternalNode(nodeToSplit, middleElement, parentNode, leftChild, rightChild);
            }

            return parentNode;
        }

        /**
         * @brief Creates a new root node for a split operation
         * 
         * @param middleElement The middle element of the node to split
         * @param leftChild The left child node
         * @param rightChild The right child node
         * @return Node<KeyType, ValueType>* The new root node
         */
        Node<KeyType, ValueType>* createNewRootNode(NodeElement<KeyType, ValueType> middleElement, Node<KeyType, ValueType>* leftChild, Node<KeyType, ValueType>* rightChild) {
            Node<KeyType, ValueType>* parentNode = new Node<KeyType, ValueType>(middleElement.getKey(), middleElement.getValues());
            rootNode = parentNode;
            cout << "Created new root node with middle element key: " << middleElement.getKey() << endl;
            addChildrenToParent(parentNode, leftChild, rightChild);
            return parentNode;
        }

        /**
         * @brief Adds the children to the parent node
         * 
         * @param parentNode The parent node
         * @param leftChild The left child node
         * @param rightChild The right child node
         */
        void addChildrenToParent(Node<KeyType, ValueType>* parentNode, Node<KeyType, ValueType>* leftChild, Node<KeyType, ValueType>* rightChild) {
            parentNode->addChildNode(leftChild);
            parentNode->addChildNode(rightChild);
            leftChild->parent = parentNode;
            rightChild->parent = parentNode;
        }

        /**
         * @brief Handles an internal node during a split operation
         * 
         * @param nodeToSplit The node to split
         * @param middleElement The middle element of the node to split
         * @param parentNode The parent node of the node to split
         * @param leftChild The left child node
         * @param rightChild The right child node
         * @return Node<KeyType, ValueType>* The parent node of the split nodes
         */
        Node<KeyType, ValueType>* handleInternalNode(Node<KeyType, ValueType>* nodeToSplit, NodeElement<KeyType, ValueType> middleElement, Node<KeyType, ValueType>* parentNode, Node<KeyType, ValueType>* leftChild, Node<KeyType, ValueType>* rightChild) {
            parentNode->removeChildNode(nodeToSplit);

            if (parentNode->size >= 3) { // Parent node is overfull
                cout << "Parent node with key: " << parentNode->elements[0].getKey() << " is overfull, splitting" << endl;
                parentNode = split(parentNode, middleElement);
            } else { // Parent node is not overfull
                cout << "Parent node is not overfull, inserting key-value pair" << endl;
                parentNode->insertKeyValPair(middleElement);
            }

            addChildrenToParent(parentNode, leftChild, rightChild);
            return parentNode;
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
                    refNode->calculateLeftSubtreeSize();
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