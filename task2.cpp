#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;

// The third method: change Cyrillic letters to their Latin counterparts

// We're going to use the find_first_of for the Russian letters and change them recursively, if num & 1 == 1 => encode, else, skip.
int encodeMessage(filesystem::path path, string message) {
    string initFileContent = "", temp;
    ifstream file(path);

    while (getline(file, temp))
        initFileContent += temp;

    cout << initFileContent;
    
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

// the first value is the number of arguments, the second one - path to the file,
// the third one - mode (1 - encode, 2 - decode), the fourth one - message for to encode.
int main(int argc, char* argv[]) {
    string argumentsRequired = "1st - path to the file;\n2nd - mode (1 for encode and 2 for decode)\n3rd - message to encode (if mode 1 is chosen)";
    if (argc < 2 || argc > 3 || *argv[2] == '1' && argc != 4 || *argv[2] == '2' && argc != 3) {
        cerr << "WRONG NUMBER OF ARGUMENTS (" << argc - 1 << ")!\n" << argumentsRequired << endl;
        return 1;
    }
    filesystem::path filePath = filesystem::path(argv[1]);

    if (!filesystem::exists(filePath)) {
        cerr << "NO SUCH FILE: " << filePath << "!";
        return 1;
    }

    // now the branching
    switch (*argv[2]) {
        case '1': encodeMessage(filePath, argv[3]); break;
        case '2': decodeMessage(filePath); break;
        default: {
            cerr << "WRONG MODE! The arguments needed:\n" << argumentsRequired << endl;
            return 1;
        }
    }
    return 0;
}