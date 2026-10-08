#include <cstddef>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;

// Give the salt and the length of the data
char* generateSequence(string salt, size_t length) {
    return nullptr;
}

int encodeMessage(filesystem::path path, string salt) {
    string initFileContent = "", temp;
    ifstream file(path);

    while (getline(file, temp))
        initFileContent += temp;

    file.close();
    
    size_t len = initFileContent.length();
    char* sequence = generateSequence(salt, len);
    // cerr << initFileContent;
    
    // cyphering
    for (size_t i = 0; i < len; ++i)
        initFileContent[i] ^= sequence[i];
    
    // write into the file
    // ...

    return 0;
}

int decodeMessage(filesystem::path path) {
    string initFileContent = "", temp;
    ifstream file(path);

    while (getline(file, temp))
        initFileContent += temp;

    cout << initFileContent;
    
    return 0;
}

int main(int argc, char* argv[]) {
    string argumentsRequired = "1st - path to the file;\n2nd - salt";
    if (argc != 2) {
        cerr << "WRONG NUMBER OF ARGUMENTS (" << argc - 1 << ")!\n" << argumentsRequired << endl;
        return 1;
    }
    filesystem::path filePath = filesystem::path(argv[1]);

    if (!filesystem::exists(filePath)) {
        cerr << "NO SUCH FILE: " << filePath << "!";
        return 1;
    }

    encodeMessage(filePath, argv[3]);
    return 0;
}