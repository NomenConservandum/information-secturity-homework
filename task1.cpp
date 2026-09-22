#include <iostream>
#include <filesystem>
#include <stack>
#include <fstream>
#include <string>

using namespace std;

// TODO: write two functions (read and write hash-file functions)

// look at 3 cases: a file is added; a file is deleted; a file is modified (the hash is different)
int main(int argc, char* argv[]) {
    filesystem::path dir = argv[1];
    
    if (!filesystem::exists(dir)) { // check the directory given
        cout << "No such directory!" << endl;
        return 1;
    }
    
    filesystem::path hashFileDir = dir / "hash-file.txt";

    if (!filesystem::exists(hashFileDir)) { // check for hash-file
        // create one
        ofstream hashFile(hashFileDir);
        hashFile.close();
    }

    fstream hashFile(hashFileDir); // I have two ways:
    // 1. put all of the hash-file work into functions;
    // 2. read it here once into a map and then write new contents into the file

    if (!hashFile.is_open()) {
        cout << "Error opening the hash-file!" << endl;
        return 1;
    }

    // DFS part
    stack<filesystem::path> DFSStack;
    DFSStack.push(dir);
    while(!DFSStack.empty()) {
        filesystem::path path = DFSStack.top();
        DFSStack.pop();

        for ( const filesystem::directory_entry& entry :
              filesystem::directory_iterator(path) ) {
            if (entry.is_directory())
                DFSStack.push(entry.path());
            if (entry.is_directory() || entry.path() == dir / "hash-file.txt")
                continue;

            // open the file and do the hash
            ifstream file(entry.path());

            if (!file.is_open()) { // Will this even work on a non-text file?
                cout << "Error reading the file!" << endl;
                return 1;
            }
            
            string content = entry.path(), temp;

            while (getline(file, temp))
                content += temp;

            file.close();

            hash<string> hash;

            // store in a map where key is path and value is hash

            // the newline character is the best delimeter I could think of
            cout << entry.path() << "\n" << hash(content) << endl;
        }
    }

    hashFile.close();

    return 0;
}