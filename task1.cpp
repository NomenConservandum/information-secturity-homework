#include <cstddef>
#include <iostream>
#include <filesystem>
#include <stack>
#include <fstream>
#include <string>
#include <map>

using namespace std;

map<string, size_t>* readHashFile(filesystem::path dir) {
    fstream hashFile(dir);

    if (!hashFile.is_open()) {
        cerr << "Error opening the hash-file!" << endl;
        return nullptr;
    }

    map<string, size_t>* res = new map<string, size_t>();

    // read the file
    string temp;

    while (getline(hashFile, temp)) {
        string to_string = temp.substr(1, temp.find_first_of(':') - 2),
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
    
    map<string, size_t> *oldHashes = nullptr, // read the hash-file
                        *newHashes = new map<string, size_t>();

    bool doesHashFileExist = filesystem::exists(hashFileDir);
    if (doesHashFileExist)
        oldHashes = readHashFile(hashFileDir);
    else {
        ofstream hashFile(hashFileDir);
        hashFile.close();
    }

    // DFS part
    stack<filesystem::path> DFSStack;
    DFSStack.push(dir);
    ofstream hashFile(hashFileDir);
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

            if (!file.is_open()) { // Will this even work on a non-text file? UPD: seems so
                cerr << "Error reading the file!" << endl;
                return 1;
            }
            
            string content = entry.path(), temp;

            while (getline(file, temp))
                content += temp;

            file.close();

            hash<string> hash;

            // store in a map where key is path and value is hash

            // the newline character is the best delimeter I could think of
            hashFile << entry.path() << ":" << hash(content) << endl;
            // cerr << entry.path() << ":" << hash(content) << endl;
            newHashes->insert({entry.path(), hash(content)});
        }
    }
    hashFile.close();

    if (oldHashes == nullptr)
        return 0;
    // A simple check. I'm too lazy to come up with something special.
    for (const auto& it : *oldHashes) {
        //cerr << "Looking for: " << it.first << endl;
        if (newHashes->find(it.first) == newHashes->end()) { // no such element
            cerr << "WARNING! THE FILE " << it.first << " HAS BEEN DELETED!" << endl;
        } else if (it.second != newHashes->find(it.first)->second) { // The hashes do not align
            cerr << "WARNING! THE FILE " << it.first << " HAS BEEN MODIFIED!" << endl;
        }
    }
    for (const auto& it : *newHashes) {
        //cerr << "Looking for: " << it.first << endl;
        if (oldHashes->find(it.first) == oldHashes->end()) { // this is a new file
            cerr << "WARNING! A NEW FILE HAS BEEN DETECTED: " << it.first << "!" << endl;
        }
    }
    // Give the user a choice maybe: save the new hash file, if changes present, or keep the old one

    return 0;
}