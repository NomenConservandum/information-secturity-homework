#include <cstddef>
#include <iostream>
#include <filesystem>
#include <stack>
#include <fstream>
#include <string>
#include <map>

using namespace std;

map<string, size_t>* readHashFile(filesystem::path dir) {
    if (!filesystem::exists(dir)) { // check for hash-file
        // create one
        ofstream hashFile(dir);
        hashFile.close();
    }

    fstream hashFile(dir);

    if (!hashFile.is_open()) {
        cerr << "Error opening the hash-file!" << endl;
        return nullptr;
    }

    map<string, size_t>* res = new map<string, size_t>();

    // read the file
    string temp;

    while (getline(hashFile, temp)) {
        string
            to_string = temp.substr(0, temp.find_first_of(':')),
            to_size_t = temp.substr(temp.find_first_of(':') + 1, temp.length() - temp.find_first_of(':'));
        
        // convert the string to size_t this weird way
        stringstream stream(to_size_t);
        size_t output;
        stream >> output;

        res->insert({to_string,output});
    }

    hashFile.close();

    return res;
}

// TODO: write the second function

// look at 3 cases: a file is added; a file is deleted; a file is modified (the hash is different)
int main(int argc, char* argv[]) {
    filesystem::path dir = argv[1];
    
    if (!filesystem::exists(dir)) { // check the directory given
        cout << "No such directory!" << endl;
        return 1;
    }
    
    filesystem::path hashFileDir = dir / "hash-file.txt";

    map<string, size_t>* hashes = readHashFile(hashFileDir); // read the hash-file

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
            cout << entry.path() << ":" << hash(content) << endl;
        }
    }

    // Give the user a choice: save the new hash file, if changes present, or keep the old one

    return 0;
}