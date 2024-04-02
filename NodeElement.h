#ifndef NODE_ELEMENT_H 
#define NODE_ELEMENT_H 

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdexcept> 
#include "CircularDynamicArray.h"
using namespace std;

/**
 * @brief NodeElement class that can be used to store keys, values, and subtree size together in 2-4 tree nodes
 * 
 * @tparam Key Key type
 * @tparam Value Value type
 */
template <class Key, class Value> class NodeElement {
    private:
        Key key;
        CircularDynamicArray<Value> values;
        int subtreeSize;

    public:
        /**
         * @brief Default constructor for the node element
         */
        NodeElement() {
            key = Key();
            values = CircularDynamicArray<Value>();
            subtreeSize = 0;
        }

        /**
         * @brief Constructs a new NodeElement object with a single value
         */
        NodeElement(Key k, Value v) {
            key = k;
            values.addEnd(v);
            subtreeSize = 0;
        }

        /**
         * @brief Constructs a new NodeElement object with multiple values
         */
        NodeElement(Key k, CircularDynamicArray<Value>& v) {
            key = k;
            values = v;
            subtreeSize = 0;
        }

        /**
         * @brief Adds a value to the node element
         * 
         * @param v Value to add
         */
        void addValue(Value v) {
            values.addEnd(v);
        }
    
        /**
         * @brief Removes a value from the node element
         * 
         * @param v Value to remove
         */
        void removeValue(Value v) {
            values.remove(v);
        }

        /**
         * @brief Returns the key stored in the node element
         * 
         * @return Key Key stored in the node element
         */
        Key getKey() {
            return key;
        }

        /**
         * @brief Returns the values stored in the node element
         * 
         * @return CircularDynamicArray<Value>& Values stored in the node element
         */
        CircularDynamicArray<Value>& getValues() {
            return values;
        }

        /**
         * @brief Returns the number of values stored in the node element
         * 
         * @return int Number of values stored in the node element
         */
        Value getValueAt(int index) {
            return values[index];
        }

        /**
         * @brief Returns the number of values stored in the node element
         * 
         * @return int Number of values stored in the node element
         */
        int getNumValues() {
            return values.length();
        }

        /**
         * @brief Returns the subtree size stored in the node element
         * 
         * @return int Subtree size stored in the node element
         */
        int getSubtreeSize() {
            return subtreeSize;
        }

        /**
         * @brief Sets the subtree size stored in the node element
         * 
         * @param size Subtree size to set
         */
        void setSubtreeSize(int size) {
            subtreeSize = size;
        }

        /**
         * @brief Increments the subtree size stored in the node element
         */
        void incrementSubtreeSize() {
            subtreeSize++;
        }

        /**
         * @brief Set the Key object
         * 
         * @param k Key object to set
         */
        void setKey(Key k) {
            key = k;
        }

        /**
         * @brief Set the Values object
         * 
         * @param v Values object to set
         */
        void setValues(CircularDynamicArray<Value>& v) {
            values = v;
        }
};

#endif
