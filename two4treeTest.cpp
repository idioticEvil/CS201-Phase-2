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
    cout << "Inorder Traversal Test 2:" << endl;
    test2.inorder();
    cout << "Postorder Traversal Test 2:" << endl;
    test2.postorder();
    cout << "Rank A: " << test2.rank("A") << endl;
    cout << "Rank B: " << test2.rank("B") << endl;
    cout << "Rank C: " << test2.rank("C") << endl;
    cout << "Rank D: " << test2.rank("D") << endl;
    cout << "Rank E: " << test2.rank("E") << endl;
    cout << "Rank F: " << test2.rank("F") << endl;
    cout << "Rank G: " << test2.rank("G") << endl;
    cout << "Rank H: " << test2.rank("H") << endl;
    cout << "Rank I: " << test2.rank("I") << endl;
    cout << "Rank J: " << test2.rank("J") << endl;
    cout << "Search A: " << *(test2.search("A")) << endl;

    two4Tree<int,int> X;
	for (int i=1;i<1001000;i++) X.insert(i,i);
    for (int i=1;i<1001000;i++) {
		if(X.rank(i) != i) cout << "Rank error" << endl;
		if(X.select(i) != i) cout << "Select error" << endl;
		if(*(X.search(i)) != i) cout << "Search error" << endl;
	} 
    cout << "Done" << endl;
}