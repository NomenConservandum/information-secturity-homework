#include <iostream>
#include <filesystem>
#include <stack>


using namespace std;

// for DFS you might need a stack of strings (simply paths)

// look at 3 cases: a file is added; a file is deleted; a file is modified (the hash is different)
int main(int argc, char* argv[]) {
    filesystem::path dir = argv[1];
    
    if (!filesystem::exists(dir)) { // and check for hash-file
        cout << "No such directory!" << endl;
        return 0;
    }

    // DFS part
    stack<filesystem::path> DFSStack;
    DFSStack.push(dir);
    while(!DFSStack.empty()) {
        filesystem::path path = DFSStack.top();
        DFSStack.pop();

        for (const filesystem::directory_entry& entry : filesystem::directory_iterator(path)) {
            if (entry.is_regular_file())
                cout << "[FILE]: ";
            else
                cout << "[DIR]: ";
            cout << entry.path() << endl;
            if (entry.is_directory())
                DFSStack.push(entry.path());
        }
    }

    return 0;
}