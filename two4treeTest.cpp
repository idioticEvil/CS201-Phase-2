#include <iostream>
#include "two4Tree.cpp"
using namespace std;

int main() {
    //int valInsert1[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    //string keyInsert1[9] = {"A", "B", "C", "D", "E", "F", "G", "H", "I"};
    int valInsert2[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    string keyInsert2[10] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};

    //two4Tree<string, int> test1(keyInsert1, valInsert1, 9);
    two4Tree<string, int> test2;

    for (int i = 0; i < 10; i++) {
        test2.insert(keyInsert2[i], valInsert2[i]);
    }

    /*cout << "Preorder Traversal Test 1:" << endl;
    test1.preorder();
    cout << "Inorder Traversal Test 1:" << endl;
    test1.inorder();
    cout << "Postorder Traversal Test 1:" << endl;
    test1.postorder();*/

    cout << "Preorder Traversal Test 2:" << endl;
    test2.preorder();
    test2.preorderPrintAttributes(test2.getRootNode());
    cout << "Inorder Traversal Test 2:" << endl;
    test2.inorder();
    cout << "Postorder Traversal Test 2:" << endl;
    test2.postorder();
}