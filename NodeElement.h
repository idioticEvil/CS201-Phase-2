#ifndef NODE_ELEMENT_H 
#define NODE_ELEMENT_H 

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdexcept> 
#include "CircularDynamicArray.h"
using namespace std;

/**
 * @brief NodeElement class that can be used to store keys and values together in 2-4 tree nodes
 * 
 * @tparam Key Key type
 * @tparam Value Value type
 */
template <class Key, class Value> class NodeElement {
    private:
        Key key;
        CircularDynamicArray<Value> values;

    public:

        /**
         * @brief Constructs a new NodeElement object with a single value
         */
        NodeElement(Key k, Value v) {
            key = k;
            values.addEnd(v);
        }

        /**
         * @brief Constructs a new NodeElement object with multiple values
         */
        NodeElement(Key k, CircularDynamicArray<Value>& v) {
            key = k;
            values = v;
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
            return values.getSize();
        }
};

#endif
