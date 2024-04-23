using namespace std;
#include <iostream>
#include <cstdlib>
#include "two4Tree.cpp"

void printErrors(string errors, int numOfErrors) {
	if(numOfErrors < 5){
		cout << errors << " passed (Key:1s612w)" << endl;
	} else if (numOfErrors < 100){
		cout << errors << " caused " << numOfErrors << " of errors ADD+1 (Key:b8d2a0)" << endl;
		cout << errors << " caused " << numOfErrors << " of errors ADD+1 (Key:b8d2a0)" << endl;
		cout << errors << " caused " << numOfErrors << " of errors ADD+1 (Key:b8d2a0)" << endl;
	} else if (numOfErrors < 1000){
		cout << errors << " caused " << numOfErrors << " of errors ADD+1 (Key:b8d2a0)" << endl;
		cout << errors << " caused " << numOfErrors << " of errors ADD+1 (Key:b8d2a0)" << endl;
	} else if (numOfErrors < 10000){
		cout << errors << " caused " << numOfErrors << " of errors ADD+1 (Key:b8d2a0)" << endl;
	} else {
		cout << errors << " caused " << numOfErrors << " of errors" << endl;
	}
}

void test1() { // ISSUE WITH PREORDER TRAVERSAL
	string K[13] = {"AA","BB","CC","DD","EE","FF","GG","HH","II","JJ","KK","LL","MM"};
    int V[13] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
	
	two4Tree<string,int> T(K,V,13);
	cout << "T size should be 13=" << T.size() << endl;
	cout << "T inorder should be AA BB CC DD EE FF GG HH II JJ KK LL MM" << endl;
    cout << "Inorder is:         "; T.inorder(); cout<<endl;
	
	cout << "T select(7)should be GG = " << T.select(7) << endl; 
	cout << "T select(2)should be BB = " << T.select(2) << endl;
	
	cout << "T rank(CC) should be 3 = "<< T.rank("CC") << endl;
	cout << "T rank(II) should be 9 = "<< T.rank("II") << endl;	
	
	cout << "T search(JJ) should be 10 = " << *(T.search("JJ")) << endl;	
	cout << "T search(LL) should be 12 = " << *(T.search("LL")) << endl;
	cout << "T search(NN) should be NULL = ";
	if (T.search("NN") == NULL) cout << "NULL" << endl;
	else cout << "NOT NULL" << endl;

	cout << "T preorder should be \nDD HH\nBB\nAA\nCC\nFF\nEE\nGG\nJJ\nII\nKK LL MM" << endl;
	
    // Preorder is not matching the expected output
    cout << "Preorder Traversal:" << endl; T.preorder(); 
	cout << endl;
}

void test2(){ // ALL TESTS PASSED
    int K[10] = {10, 5, 50, 25, 2, 4, 1, 9, 7, 8};
    int V[10] = {10, 5, 50, 25, 2, 4, 1, 9, 7, 8};
	
	two4Tree<int,int> T;
	for(int i=0; i<10; i++) T.insert(K[i],V[i]);

	cout << "T inorder should be 1 2 4 5 7 8 9 10 25 50" << endl;
	cout << "T inorder is:       "; T.inorder(); cout << endl;

	cout << "T select(7)should be 9 = " << T.select(7) << endl; 
	cout << "T rank(5) should be 4 = "<< T.rank(5) << endl;
	cout << "T search(25) should be 25 = "<< *(T.search(25)) << endl;	
	cout << "T search(7) should be 7 = "<< *(T.search(7)) << endl;
	cout << "T search(6) should be NULL = ";
	if (T.search(6) == NULL) cout << "NULL"<<endl;
	else cout << "NOT NULL" << endl;

	cout << "T preorder should be:\n4 7 10\n1 2\n5\n8 9\n25 50" << endl;
    cout << "T preorder is:" << endl; T.preorder(); cout << endl;	
}

void test3() {
    int K[10] = {10, 5, 50, 25, 2, 4, 1, 9, 7, 8};
    int V[10] = {10, 5, 50, 25, 2, 4, 1, 9, 7, 8};

    two4Tree<int, int> T;
    for (int i = 0; i < 10; i++) {
        T.insert(K[i], V[i]); // same tree as test 2
    }

    cout << "testing" << endl;

    T.remove(2); // 3 node leaf

    cout << "T search(3) should be NULL = ";
    if (T.search(3) == NULL) {
        cout << "NULL" << endl;
    } else {
        cout << "NOT NULL" << endl;
    }

    cout << "T rank(5) should be 3 = " << T.rank(5) << endl;

    T.remove(1); // 2 node leaf with 2 node right sibling
    cout << "T select(7) should be 25 = " << T.select(7) << endl;
    cout << "T rank(9) should be 5 = " << T.rank(9) << endl;
    cout << "T search(4) should be 4 = " << *(T.search(4)) << endl;
    cout << "T search(50) should be 50 = " << *(T.search(50)) << endl;

    cout << "T preorder should be:\n7 10\n4 5\n8 9\n25 50" << endl;
    cout << "T preorder is:" << endl; T.preorder();
    cout << endl;

    T.remove(5); // 3 node leaf
    T.remove(4); // 2 node leaf with 3 node right sibling
    cout << "T select(6) should be 50 = " << T.select(6) << endl;
    cout << "T rank(9) should be 3 = " << T.rank(9) << endl;

    cout << "T preorder should be:\n8 10\n7\n9\n25 50" << endl;
    cout << "T preorder is:" << endl; T.preorder();
    cout << endl;

    T.remove(8); // root
    cout << "T select(3) should be 10 = " << T.select(3) << endl;
    cout << "T rank(9) should be 2 = " << T.rank(9) << endl;

    cout << "T preorder should be:\n10\n7 9\n25 50" << endl;
    cout << "T preorder is:" << endl; T.preorder();
    cout << endl;
}

void test4() { // ALL TESTS PASSED
	two4Tree<long int,long int> Y1;
	two4Tree<wchar_t,wchar_t> Y2;
	two4Tree<float,float> Y3;
	cout << "Passed Generics Test" << endl;
}

void test5() {
    int K[10] = {10, 5, 50, 25, 2, 4, 1, 9, 7, 8};
    float V[10] = {10, 5, 50, 25, 2, 4, 1, 9, 7, 8};

    two4Tree<int, float> T;
    for (int i = 0; i < 10; i++) {
        T.insert(K[i], V[i]); // same tree as test 2
    }

    T.insert(5, 5.1);
    T.insert(5, 5.2);
    T.insert(5, 5.3);
    T.insert(5, 5.4);

    cout << "T inorder should be 1 2 4 5 5 5 5 5 7 8 9 10 25 50" << endl;
    T.inorder();
    cout << endl;

    cout << "T select(7) should be 5 = " << T.select(7) << endl;
    cout << "T rank(7) should be 9 = " << T.rank(7) << endl;
    cout << "T search(5) should be 5 = " << *(T.search(5)) << endl;

    T.remove(5);
    T.remove(5);

    cout << "T duplicates(5) should be 3 = " << T.duplicates(5) << endl;
    cout << "T duplicates(2) should be 1 = " << T.duplicates(2) << endl;
    cout << "T duplicates(30) should be 0 = " << T.duplicates(30) << endl;

    cout << "T select(7) should be 7 = " << T.select(7) << endl;
    cout << "T rank(9) should be 9 = " << T.rank(9) << endl;
    cout << "T search(5) should be 5.2 = " << *(T.search(5)) << endl;

    cout << "T preorder should be 4 8 10\n 1 2\n 5 7\n 9\n 25 50" << endl;
    T.preorder();
    cout << endl;
}

void test6() {
    int rankError = 0;
    int selectError = 0;
    int searchError = 0;
    two4Tree<int, int> X;
    for (int i = 1111111; i > 0; i--) {
        X.insert(i, i);
    }
    for (int i = 1; i < 1111111; i++) {
        if (X.rank(i) != i) {
            rankError++;
        }
        if (X.select(i) != i) {
            selectError++;
        }
        if (*(X.search(i)) != i) {
            searchError++;
        }
    }
    printErrors("Rank errors ", rankError);
    printErrors("Select errors ", selectError);
    printErrors("Search errors ", searchError);
}

void test7() {
    int rank = 0;
    int rankError = 0;
    int selectError = 0;
    int searchError = 0;
    two4Tree<int, int> X;
    for (int i = 1111111; i > 0; i--) {
        X.insert(i, i);
    }
    for (int i = 1; i <= 1111111; i += 10) {
        X.remove(i);
    }
    for (int i = 1; i < 1111111; i++) {
        if (i % 10 != 1) {
            if (X.rank(i) != ++rank) {
                rankError++;
            }
            if (X.select(rank) != i) {
                selectError++;
            }
            if (*(X.search(i)) != i) {
                searchError++;
            }
        }
    }
    printErrors("Rank errors ", rankError);
    printErrors("Select errors ", selectError);
    printErrors("Search errors ", searchError);
}

int main(int argc, char **argv) {
    int testToRun = atoi(argv[1]);
    switch (testToRun) {
        case 1:
            test1();
            break;
        case 2:
            test2();
            break;
        case 3:
            test3();
            break;
        case 4:
            test4();
            break;
        case 5:
            test5();
            break;
        case 6:
            test6();
            break;
        case 7:
            test7();
            break;
    }
    return 0;
}

