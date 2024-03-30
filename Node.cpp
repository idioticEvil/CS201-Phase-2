#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdexcept> 
#include "CircularDynamicArray.h"
#include "NodeElement.h"
using namespace std;

/**
 * @brief Node class that can be used to store any type of data in a 2-4 tree
 * 
 * @tparam K Key type
 * @tparam V Value type
 */
template <class K, class V> class Node {
    public:
        CircularDynamicArray<NodeElement<K, V> > elements;
        CircularDynamicArray<Node*> children;
        Node* parent;
        bool isLeaf;
        int size;
        int leftSubtreeSize;

        /**
         * @brief Default constructor for the node
         */
        Node() {
            parent = nullptr;
            isLeaf = true;
            size = 0;
            leftSubtreeSize = 0;
        }

        /**
         * @brief Creates a new node with one key and value
         * 
         * @param k Key
         * @param v Value
         */
        Node(K k, V v) {
            elements.addEnd(NodeElement<K, V>(k, v));
            parent = nullptr;
            isLeaf = true;
            size = 1;
            leftSubtreeSize = 0;
        }

        /**
         * @brief Creates a new node with one key and a CircularDynamicArray of values
         * 
         * @param k Key
         * @param v CircularDynamicArray of values
         */
        Node(K k, CircularDynamicArray<V>& v) {
            
            elements.addEnd(NodeElement<K, V>(k, v));
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
                elements.addEnd(NodeElement<K, V>(k[i], v[i]));
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
                elements.addEnd(NodeElement<K, V>(k[i], v[i]));
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
            elements.addEnd(NodeElement<K, V>(k, v));
            this->parent = parent;
            isLeaf = true;
            size = 1;
            leftSubtreeSize = 0;
        }

        /**
         * @brief Inserts a key-value pair into the node at the correct spot
         * 
         * @param element Element to insert
         */
        void insertKeyValPair(NodeElement<K, V> element) {
            // Check if the node is empty
            if (size == 0) {
                elements.addEnd(element);
                size++;
            } else {
                // Find the correct spot to insert the key-value pair
                for (int i = 0; i < size; i++) {
                    if (element.getKey() <= elements[i].getKey()) {
                        elements.addAt(element, i);
                        size++;
                        break;
                    } else if (i == size - 1) {
                        elements.addEnd(element);
                        size++;
                        break;
                    }
                }
            }
        }

        /**
         * @brief Removes a key-value pair from the node
         * 
         * @param key Key to remove
         * @param value Value to remove
         */
        void removeKeyValPair(K key, CircularDynamicArray<V>& value) {
            // Find the key-value pair to remove
            for (int i = 0; i < size; i++) {
                if (key == elements[i].getKey() && value == elements[i].getValues()) {
                    elements.removeAt(i);
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
                K childKey = child->elements.getEndValue().getKey();

                // Find the correct spot to insert the child node
                for (int i = 0; i < childSize; i++) {
                    if (childKey <= elements[i].getKey()) {
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
                    for (int i = 0; i < refNode->elements.length(); i++) {
                        // Check if the child node is not a leaf and the first key of the child node is less than the key of the node
                        if (refNode->children[i]->elements.length() > 0 && 
                            refNode->children[i]->elements[0].getKey() <= 
                            refNode->elements[i].getKey()) {
                            // Check if there are at least i+1 children
                            if (refNode->children.length() > i) {
                                refNode->leftSubtreeSize += (refNode->children[i]->leftSubtreeSize + 
                                refNode->children[i]->size);
                            }
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
                if (key <= elements[i].getKey()) return children[i];
            }
            return children[size];
        }

        /**
         * @brief Prints the keys of the node
         */
        void printKeys() {
            for (int i = 0; i < size; i++) {
                if (i < size - 1) cout << elements[i].getKey() << " ";
                else cout << elements[i].getKey();
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
            printKeys();

            cout << "Values: ";
            for (int i = 0; i < size; i++) {
                cout << "[";
                for (int j = 0; j < elements[i].getNumValues(); j++) {
                    cout << elements[i].getValueAt(j);
                    if (j < elements[i].getNumValues() - 1) cout << ", ";
                }
                cout << "]";
                if (i < size - 1) cout << ", ";
            }
            cout << endl << endl;
        }
};