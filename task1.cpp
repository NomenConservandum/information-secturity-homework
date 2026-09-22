#include <iostream>
#include <filesystem>
#include <stack>
#include <fstream>
#include <string>

using namespace std;

// for DFS you might need a stack of strings (simply paths)

// look at 3 cases: a file is added; a file is deleted; a file is modified (the hash is different)
int main(int argc, char* argv[]) {
    filesystem::path dir = argv[1];
    
    if (!filesystem::exists(dir)) { // and check for hash-file
        cout << "No such directory!" << endl;
        return 1;
    }

    // DFS part
    stack<filesystem::path> DFSStack;
    DFSStack.push(dir);
    while(!DFSStack.empty()) {
        filesystem::path path = DFSStack.top();
        DFSStack.pop();

        for (const filesystem::directory_entry& entry : filesystem::directory_iterator(path)) {
            if (entry.is_regular_file()) { // do the hash
                cout << "[FILE: HASH AND PATH]: ";
                ifstream file(entry.path());

                if (!file.is_open()) { // Will this even work on a non-text file?
                    cout << "Error reading the file!" << endl;
                    return 1;
                }
                
                string content, temp;

                while (getline(file, temp))
                    content += temp;

                file.close();

                hash<string> hash;

                cout << hash(content) << " | ";
            } else
                cout << "[DIR]: ";
            
            cout << entry.path() << endl;

            if (entry.is_directory())
                DFSStack.push(entry.path());
        }
    }

    return 0;
}