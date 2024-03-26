#include <iostream>
#include "two4Tree.cpp"
using namespace std;

int main() {
    int valInsert[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    string keyInsert[10] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};

    two4Tree<string, int> test1(keyInsert, valInsert, 10);
}