#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdexcept> 
#include "CircularDynamicArray.cpp"
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
        Node* rootNode;
        int treeSize;

        void split(Node* node) {
            // IMPLEMENT THIS
        }
        
    public:
        /**
         * @brief Creates an empty 2-4 tree
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
            if (rootNode == nullptr) rootNode = new Node(key, value);
            // If the tree is not empty, find the correct leaf node to insert the key-value pair
            else { 
                refNode = rootNode;

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
                        }
                    }

                    if (i == refNode->size - 1) {
                        refNode->keys.addEnd(key);
                        refNode->values.addEnd(value);
                        refNode->size++;
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

/**
 * @brief Node class that can be used to store any type of data in a 2-4 tree
 * 
 * @tparam K Key type
 * @tparam V Value type
 */
template <class K, class V> class Node {
    private:
        CircularDynamicArray<K> keys;
        CircularDynamicArray<V> values[3];
        CircularDynamicArray<Node*> children;
        Node* parent;
        bool isLeaf;
        int size;
        int subtreeSize;
        int rank;

    public:
        /**
         * @brief Creates a new node with no keys or values
         */
        Node() {
            keys = new CircularDynamicArray<K>;
            values = new CircularDynamicArray<V>[3];
            children = new CircularDynamicArray<Node*>;
            parent = nullptr;
            isLeaf = true;
            size = 0;
            subtreeSize = 0;
            rank = 0;
        }

        /**
         * @brief Creates a new node with one key and value
         * 
         * @param k Key
         * @param v Value
         */
        Node(K k, V v) {
            keys = new CircularDynamicArray<K>;
            values = new CircularDynamicArray<V>[3];
            children = new CircularDynamicArray<Node*>;
            keys[0] = k;
            values[0].addEnd(v);
            parent = nullptr;
            isLeaf = true;
            size = 1;
            subtreeSize = 0;
            rank = 1;
        }

        /**
         * @brief Creates a new node with multiple keys and values
         * 
         * @param k Array of keys
         * @param v Array of values
         * @param size Size of the arrays
         */
        // Note: This program logic currently assumes that the values are sorted
        Node(K k[], V v[], int size) { 
            keys = new CircularDynamicArray<K>;
            values = new CircularDynamicArray<V>[3];
            children = new CircularDynamicArray<Node*>;

            for (int i = 0; i < size; i++) {
                keys.addEnd(k[i]);
                values[i].addEnd(v[i]);
            }

            parent = nullptr;
            isLeaf = true;
            this->size = size;
            subtreeSize = 0;
            rank = size;
        }

        /**
         * @brief Creates a new node with one key, value, and parent
         * 
         * @param k Key
         * @param v Value
         * @param parent Parent node
         */
        Node(K k, V v, Node* parent) {
            keys = new CircularDynamicArray<K>;
            values = new CircularDynamicArray<V>[3];
            children = new CircularDynamicArray<Node*>;
            keys.addEnd(k);
            values[0].addEnd(v);
            this->parent = parent;
            isLeaf = true;
            size = 1;
            subtreeSize = 0;
            rank = parent.rank + 1;
        }

        /**
         * @brief Creates a new node with multiple keys, values, and parent
         * 
         * @param k Array of keys
         * @param v Array of values
         * @param size Size of the arrays
         * @param parent Parent node
         */
        // Note: This program logic currently assumes that the values are sorted
        Node(K k[], V v[], int size, Node* parent) {
            keys = new CircularDynamicArray<K>;
            values = new CircularDynamicArray<V>[3];
            children = new CircularDynamicArray<Node*>;

            for (int i = 0; i < size; i++) {
                keys.addEnd(k[i]);
                values[i].addEnd(v[i]);
            }

            this->parent = parent;
            isLeaf = true;
            this->size = size;
            subtreeSize = 0;
            rank = parent.rank + size;
        }

        /**
         * @brief Destroys the node
         */
        ~Node() {
            delete keys;

            for(int i = 0; i < 3; i++) {
                delete values[i];
            }

            delete children;
        }

        /**
         * @brief Returns the child node that should be traversed to
         * 
         * @param key The key that is being checked
         * @return Node* The child node that should be traversed to
         */
        Node* traverseDirection(K key) {
            for (int i = 0; i < size; i++) {
                if (key <= keys[i]) return children[i]; // Should this be less than or equal to?
                else if (i == size - 1) return children[i + 1];
            }
        }

        /**
         * @brief Inserts a key-value pair into the node
         * 
         * @param key Key to insert
         * @param value Value to insert
         */
        void insertKeyValPair(K key, V value) {
            for (int i = 0; i < size; i++) {
                if (key < keys[i]) {
                    keys.addAt(key, i);
                    values[i].addAt(value, i);
                    size++;
                    break;
                } else if (key == keys[i]) {
                    values[i].addEnd(value);
                    break;
                } else if (i == size - 1) {
                    keys.addEnd(key);
                    values[i].addEnd(value);
                    size++;
                    break;
                }
            }
            // Do I need to do something for updating the rank? Also subtree size?
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