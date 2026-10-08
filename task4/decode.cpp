#include <cstddef>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;

int encodeMessage(filesystem::path path, filesystem::path salt) {
    string initFileContent = "", temp;
    ifstream file(path);

    while (getline(file, temp))
        initFileContent += temp;

    size_t len = initFileContent.length();
    // cerr << initFileContent;
    
    // decyphering
    //for (size_t i = 0; i < len; ++i)
        // initFileContent[i] and sequence[i]
    
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
    string argumentsRequired = "1st - path to the file;\n2nd - path to the salt file";
    if (argc != 2) {
        cerr << "WRONG NUMBER OF ARGUMENTS (" << argc - 1 << ")!\n" << argumentsRequired << endl;
        return 1;
    }
    filesystem::path filePath = filesystem::path(argv[1]);
    filesystem::path saltPath = filesystem::path(argv[2]);

    if (!filesystem::exists(filePath) || !filesystem::exists(saltPath)) {
        cerr << "NO SUCH FILE(-S): " << filePath << "!";
        return 1;
    }

    decodeMessage(filePath, saltPath);
    return 0;
}