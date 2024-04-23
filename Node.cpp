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

        /**
         * @brief Default constructor for the node
         */
        Node() {
            parent = nullptr;
            isLeaf = true;
            size = 0;
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
            for (int i = 0; i < size; i++) elements.addEnd(NodeElement<K, V>(k[i], v[i]));

            parent = nullptr;
            isLeaf = true;
            this->size = size;
        }

        /**
         * @brief Creates a new node with multiple keys and values from a CircularDynamicArray
         * 
         * @param k Array of keys
         * @param v Array of values
         * @param size Size of the arrays
         */
        Node(K k[], CircularDynamicArray<V> v[], int size) {
            for (int i = 0; i < size; i++) elements.addEnd(NodeElement<K, V>(k[i], v[i]));

            parent = nullptr;
            isLeaf = true;
            this->size = size;
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
                    if (element.getKey() < elements[i].getKey()) {
                        elements.addAt(element, i);
                        size++;
                        break;
                    } else if (element.getKey() == elements[i].getKey()) {
                        elements[i].addValue(element.getValueAt(0));
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
            if (childSize == 0) children.addEnd(child);
            else {
                // Use the last key of the child node to compare
                K childKey = child->elements.getEndValue().getKey();

                // Find the correct spot to insert the child node
                for (int i = 0; i < this->size; i++) {
                    if (childKey <= elements[i].getKey()) {
                        children.addAt(child, i);
                        break;
                    } else if (i == this->size - 1) {
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
         * @brief Updates the subtree sizes for each NodeElement in the node, and their parents
         */
        void updateSubtreeSizes() {
            if (isLeaf) {
                for (int i = 0; i < size; i++) elements[i].setSubtreeSize(1);
            } else {
                for (int i = 0; i < size; i++) {
                    int subtreeSize = 1;

                    if (i < children.length()) subtreeSize += children[i]->getTotalSubtreeSize();
                    elements[i].setSubtreeSize(subtreeSize);
                }
            }

            if (parent != nullptr) parent->updateSubtreeSizes();
        }

        /**
         * @brief Returns the total size of the left subtree for the node
         * 
         * @return int Total size of the left subtree
         */
        int getTotalSubtreeSize() {
            int totalSize = 0;
            for (int i = 0; i < size; i++) totalSize += elements[i].getSubtreeSize();

            // Include the rightmost child's total subtree size if this is not a leaf
            if (!isLeaf && children.length() > size) {
                totalSize += children[size]->getTotalSubtreeSize();
            }

            return totalSize;
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
         * @brief Returns the index of the key in the node, or -1 if not found
         * 
         * @param key Key to f
         * @return int Index of the key in the node
         */
        int findKeyIndex(K key) {
            for (int i = 0; i < size; i++) {
                if (elements[i].getKey() == key) return i;
            }
            return -1;
        }

        /**
         * @brief Merges the node with a sibling node
         * 
         * @param sibling Sibling node to merge with
         */
        void merge(Node* sibling) {
            // Merge the sibling node into this node
            for (int i = 0; i < sibling->size; i++) {
                elements.addEnd(sibling->elements[i]);
            }

            // Update the size of the node
            size += sibling->size;

            // Update the children of the node
            for (int i = 0; i < sibling->children.length(); i++) {
                children.addEnd(sibling->children[i]);
            }

            // Update the parent of the children
            for (int i = 0; i < sibling->children.length(); i++) {
                sibling->children[i]->parent = this;
            }

            // Remove the sibling node from the parent
            if (parent != nullptr) {
                parent->removeChildNode(sibling);
            }

            // Delete the sibling node
            delete sibling;
        }

        /**
         * @brief Remove a key from the node
         * 
         * @param key The key to remove
         */
        int removeKey(K key) {
            // Find and remove the key from the node
            for (int i = 0; i < size; i++) {
                if (elements[i].getKey() == key) {
                    elements.removeAt(i);
                    size--;
                    break;
                } else if (elements[i].getKey() > key) {
                    return 0; // Key not found
                }
            }

            // If the node is empty, delete it
            if (size == 0) {
                if (parent != nullptr) {
                    parent->removeChildNode(this);
                }
                delete this;
                return 1; // Key found, removed, node deleted, restructuring needed
            }
            return 2; // Key found and removed, no restructuring needed
        }

        /**
         * @brief Prints the full attributes of the node
         */
        void printFullNode() {
            cout << endl << "NODE STATUS:" << endl;
            cout << "Size: " << size << endl;
            cout << "Is Leaf: " << isLeaf << endl;

            cout << "Keys: ";
            printKeys();

            cout << "Left Subtree Sizes: ";
            for (int i = 0; i < size; i++) {
                cout << elements[i].getSubtreeSize();
                if (i < size - 1) cout << ", ";
            }

            cout << endl << "Values: ";
            for (int i = 0; i < size; i++) {
                cout << "[";
                for (int j = 0; j < elements[i].getNumValues(); j++) {
                    cout << elements[i].getValueAt(j);
                    if (j < elements[i].getNumValues() - 1) cout << ", ";
                }
                cout << "]";
                if (i < size - 1) cout << ", ";
            }

            cout << endl << "Children: ";
            for (int i = 0; i < children.length(); i++) {
                cout << "[";
                for (int j = 0; j < children[i]->size; j++) {
                    cout << children[i]->elements[j].getKey();
                    if (j < children[i]->size - 1) cout << ", ";
                }
                cout << "]";
                if (i < children.length() - 1) cout << ", ";
            }

            cout << endl << "Parent: ";
            if (parent != nullptr) cout << parent->elements[0].getKey();
            else cout << "None";
            cout << endl;
        }
};