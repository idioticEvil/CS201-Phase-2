#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdexcept> 
using namespace std;

// CircularDynamicArray: A circular dynamic array that can be used to store any type of data
template <typename T> class CircularDynamicArray {
    private:
        T *data;
        int cap;
        int size;
        int front;
        bool isSorted;
        
    public:
        // Basic Constructor: Creates an array of size 2
        CircularDynamicArray() {
            srand(time(0));
            data = new T[2];
            cap = 2;
            size = 0;
            front = 0;
            isSorted = false;
        }

        // Capacity Constructor: Creates an array of size s
        CircularDynamicArray(int s) {
            srand(time(0));
            data = new T[s];
            cap = s;
            size = s;
            front = 0;
            isSorted = false;
        }

        // Copy Constructor: Uses deep copy to create a new array
        CircularDynamicArray(const CircularDynamicArray &src) {
            data = new T[src.cap];
            cap = src.cap;
            size = src.size;
            front = src.front;
            isSorted = src.isSorted;
            
            for (int i = 0; i < size; i++) {
                data[front + i] = src.data[(src.front + i) % src.cap];
            }
        }

        // Destructor: Frees all space used by the array
        ~CircularDynamicArray() {
            delete[] data;
        }

        // IncreaseSize: Increases size of array when needed by doubling capacity
        void increaseSize() {
            int newcap = cap * 2;
            T* new_data = new T[newcap];
            for (int i = 0; i < size; i++) {
                new_data[i] = data[(front + i) % cap];
            }
            delete[] data;
            data = new_data;
            cap = newcap;
            front = 0;
        }

        // DecreaseSize: Decreases size of array when needed by halving capacity
        void decreaseSize() {
            int newcap = int(cap * 0.5);
            T* new_data = new T[newcap];
            for (int i = 0; i < size; i++) {
                new_data[i] = data[(front + i) % cap];
            }
            delete[] data;
            data = new_data;
            cap = newcap;
            front = 0;
        }

        // Copy Assignment Constructor: Uses deep copy to create a new array
        CircularDynamicArray& operator=(const CircularDynamicArray &src) {
            if (this != &src) {
                delete[] data;
                data = new T[src.cap];
                cap = src.cap;
                size = src.size;
                isSorted = src.isSorted;
                front = src.front;

                for (int i = 0; i < size; i++) {
                    data[front + i] = src.data[(src.front + i) % src.cap];
                }
            }
            return *this;
        }

        // Operator==: Compares two arrays to see if they are equal
        bool operator==(const CircularDynamicArray &src) {
            if (size != src.size) {
                return false;
            }

            for (int i = 0; i < size; i++) {
                int indexVal = (front + i) % cap;
                int srcIndexVal = (src.front + i) % src.cap;

                if (data[indexVal] != src.data[srcIndexVal]) {
                    return false;
                }
            }
            return true;
        }

        // Operator[]: Returns the value at index i
        T& operator[](int i) {
            if (i < 0 || i >= size) {
                throw std::out_of_range("Index out of range");
            }
            return data[(front + i) % cap];
        }

        // AddFront: Stores v at the front of the array, and increases size by one
        void addFront(T v) {
            if (size == cap) increaseSize();
            front = (front == 0) ? cap - 1 : front - 1;
            data[front] = v;
            size++;
            isSorted = false;
        }

        // AddEnd: Stores v at the end of the array, and increases size by one
        void addEnd(T v) {
            if (size == cap) increaseSize();
            data[(front + size) % cap] = v;
            size++;
            isSorted = false;
        }

        // DelFront: Decreases size by one and removes the value at the front of the array
        void delFront() {
            if ((size) <= int(cap * 0.25)) decreaseSize();
            data[front] = T();
            front = (front + 1) % cap;
            size--;
        }

        // DelEnd: Decreases size by one and removes the value at the end of the array
        void delEnd() {
            if ((size) <= int(cap * 0.25)) decreaseSize();
            data[(front + size - 1) % cap] = T();
            size--;
        }

        // Clear: Clears the array and sets size to 0, capacity to 2, and front to 0
        void clear() {
            delete[] data;
            data = new T[2];
            cap = 2;
            size = 0;
            front = 0;
        }

        // Length: Returns the size of the array
        int length() {
            return size;
        }

        // Capacity: Returns the capacity of the array
        int capacity() {
            return cap;
        }

        // GetFront: Returns the index of the front of the array
        int getFront() {
            return front;
        }

        // PrintArray: Prints the array
        void printArray() {
            for (int i = front; i < front + size; i++) {
                cout << data[i % cap] << " ";
            }
        }

        // Linear Search: Searches for value e in the array, returns index if found, -1 if not
        int linearSearch(T e) {
            for (int i = 0; i < size; i++) {
                if (data[(front + i) % cap] == e) {
                    return i;
                }
            }
            return -1;
        }

        // Insertion Sort: Sorts the array using insertion sort when size is less than 10
        void insertionSort() {
            int i, j;
            T key;

            for (i = 1; i < size; i++) {
                key = data[(front + i) % cap];
                j = i - 1;

                while (j >= 0 && data[(front + j) % cap] > key) {
                    data[(front + j + 1) % cap] = data[(front + j) % cap];
                    j = j - 1;
                }
                data[(front + j + 1) % cap] = key;
            }
        }

        // Merge Sort: Sorts the array using merge sort
        void mergesort(int start, int end) {
            if (start != end) {
                int distance = (end - start + cap) % cap;
                int mid = (start + distance / 2) % cap;

                mergesort(start, mid);
                mergesort((mid + 1) % cap, end);
                merge(start, mid, end);
            }
        }

        // Merge: Merges two subarrays
        void merge(int start, int mid, int end) {
            int n1 = (mid - start + cap) % cap + 1;
            int n2 = (end - mid + cap) % cap;
            T* left = new T[n1];
            T* right = new T[n2];

            for (int i = 0; i < n1; i++) left[i] = data[(start + i) % cap];
            for (int j = 0; j < n2; j++) right[j] = data[(mid + 1 + j) % cap];

            int i = 0, j = 0, k = start;
            while (i < n1 && j < n2) {
                if (left[i] <= right[j]) {
                    data[(k++) % cap] = left[i++];
                } else {
                    data[(k++) % cap] = right[j++];
                }
            }

            while (i < n1) {
                data[(k++) % cap] = left[i++];
            }

            while (j < n2) {
                data[(k++) % cap] = right[j++];
            }

            delete[] left;
            delete[] right;
        }

        // Sort: Selects which sorting algorithm to use
        void Sort() {
            if (size <= 10) insertionSort();
            else mergesort(front, (front + size - 1) % cap);
            isSorted = true;
        }

        // Binary Search: Searches for value e in the array, returns index if found, -1 if not
        int binSearch(T e) {
            if (!isSorted) Sort();

            int left = front;
            int right = (front + size - 1) % cap;
            int mid, distance;

            while (left != right) {
                distance = (right - left + cap) % cap;
                mid = (left + distance / 2) % cap;

                if (data[mid] == e) return mid;
                if (data[mid] < e) left = (mid + 1) % cap;
                else right = (mid - 1) % cap;
            }
            return -1;
        }

        // QSelect: Shell function for quickselect algorithm
        T QSelect(int k) {
            return quickSelect(*this, k);
        }

        // Quickselect: Actual quickselect algorithm, returns the kth smallest value in the array
        T quickSelect(CircularDynamicArray<T>& arr, int k) {
            if (k > 0 && k <= arr.size) {
                int randomIndex = rand() % arr.size;

                CircularDynamicArray<T> left, equal, right;
                T pivot = arr[(arr.front + randomIndex) % arr.cap];
                for (int i = 0; i < arr.size; i++) {
                    int arrayIdx = (arr.front + i) % arr.cap;
                    if (arr.data[arrayIdx] < pivot) left.addEnd(arr.data[arrayIdx]);
                    else if (arr.data[arrayIdx] == pivot) equal.addEnd(arr.data[arrayIdx]);
                    else right.addEnd(arr.data[arrayIdx]);
                }

                if (k <= left.length()) return quickSelect(left, k);
                else if (k <= left.length() + equal.length()) return pivot;
                else return quickSelect(right, k - left.length() - equal.length());
            } else {
                throw std::out_of_range("k is out of range");
            }
        }
};