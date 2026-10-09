#include <cstddef>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <random>

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

// Give the salt and the length of the data
char* generateSequence(string salt, size_t length) {
    char* sequence = new char[length];
    short seed = makeHash(salt);
    mt19937 generator(seed);

    for (size_t i = 0; i < length; ++i) 
        sequence[i] = (char)(generator() & 255);

    return sequence;
}

// ./bin/encode /home/nomen/Documents/folder1/test_message.txt "mySalt"
int encodeMessage(filesystem::path path, string salt) {
    // reading the file
    ifstream file(path, ios::binary);
    ostringstream ss;
    ss << file.rdbuf();
    string initFileContent = ss.str();
    file.close();
    
    size_t len = initFileContent.length();
    char* sequence = generateSequence(salt, len);
    // cerr << initFileContent;
    
    // cyphering
    for (size_t i = 0; i < len; ++i)
        initFileContent[i] ^= sequence[i];
    
    // write into the salt file
    ofstream saltFile(path.parent_path() / "salt.txt");
    saltFile << salt;
    saltFile.close();

    // write into the file
    ofstream outputFile(path.parent_path() / "encoded.txt");
    outputFile << initFileContent;
    outputFile.close();
    
    delete[] sequence;
    return 0;
}

int main(int argc, char* argv[]) {
    string argumentsRequired = "1st - path to the file;\n2nd - salt";
    if (argc != 3) {
        cerr << "WRONG NUMBER OF ARGUMENTS (" << argc - 1 << ")!\n" << argumentsRequired << endl;
        return 1;
    }
    filesystem::path filePath = filesystem::path(argv[1]);

    if (!filesystem::exists(filePath)) {
        cerr << "NO SUCH FILE: " << filePath << "!";
        return 1;
    }

    return encodeMessage(filePath, argv[2]);
}