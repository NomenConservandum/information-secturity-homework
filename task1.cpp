#include <iostream>

using namespace std;

// look at 3 cases: a file is added; a file is deleted; a file is modified (the hash is different)
int main(int argc, char* argv[]) {
    string temp = argv[1];
    cout << "The string: " << temp << endl;
    return 0;
}