#include <iostream>
#include <filesystem>
#include <stack>
#include <fstream>
#include <string>
#include <map>

using namespace std;

short makeHash(string initString) {
    short res = 0;
    int len = initString.length();
    if (len % 2 == 1) {
        initString.insert(initString.end(), 0);
        len = initString.length();
    }
    if (len == 0) {
        return 0;
    }
    
    res += (initString[1] << 8) + initString[2];
    for (int i = 2; i < len; i += 2) {
        res ^= ((initString[i] << 8) + initString[i + 1]);
    }
    return res;
}

// Check the existence of the file before you call this function
map<string, short>* readHashFile(filesystem::path dir) {
    fstream hashFile(dir);

    if (!hashFile.is_open()) {
        cerr << "Error opening the hash-file!" << endl;
        return nullptr;
    }

    map<string, short>* res = new map<string, short>();

    // read the file
    string temp;

    while (getline(hashFile, temp)) {
        string to_string = temp.substr(1, temp.find_first_of(':') - 2),
               to_short = temp.substr(temp.find_first_of(':') + 1, temp.length() - temp.find_first_of(':'));
        
        short output = short(stoi(to_short));

        res->insert({to_string,output});
    }

    hashFile.close();

    return res;
}

// look at 3 cases: a file is added; a file is deleted; a file is modified (the hash is different)
int main(int argc, char* argv[]) {
    filesystem::path dir = argv[1];
    
    if (!filesystem::exists(dir)) { // check the directory given
        cout << "No such directory!" << endl;
        return 1;
    }
    
    filesystem::path hashFileDir = dir / "hash-file.txt";
    
    map<string, short> *oldHashes = nullptr, // read the hash-file
                        *newHashes = new map<string, short>();

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

            short fileHash = makeHash(content);
            // the newline character is the best delimeter I could think of
            hashFile << entry.path() << ":" << fileHash << endl;
            // store in a map where key is path and value is hash
            newHashes->insert({entry.path(), fileHash});
        }
    }
    hashFile.close();

    if (oldHashes == nullptr)
        return 0;

    // A simple check. I'm too lazy to come up with something special.
    for (const auto& it : *oldHashes) {
        if (newHashes->find(it.first) == newHashes->end()) { // no such element
            cerr << "WARNING! THE FILE " << it.first << " HAS BEEN DELETED!" << endl;
        } else if (it.second != newHashes->find(it.first)->second) { // The hashes do not align
            cerr << "WARNING! THE FILE " << it.first << " HAS BEEN MODIFIED!" << endl;
        }
    }
    for (const auto& it : *newHashes) {
        if (oldHashes->find(it.first) == oldHashes->end()) { // this is a new file
            cerr << "WARNING! A NEW FILE HAS BEEN DETECTED: " << it.first << "!" << endl;
        }
    }
    
    return 0;
}