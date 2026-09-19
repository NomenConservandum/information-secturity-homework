#include <iostream>
#include <filesystem>

using namespace std;

// look at 3 cases: a file is added; a file is deleted; a file is modified (the hash is different)
int main(int argc, char* argv[]) {
    filesystem::path dir = argv[1];
    if (!filesystem::exists(dir)) {
        cout << "No such directory!" << endl;
        return 0;
    }
    
    return 0;
}