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

int decodeMessage(filesystem::path path, string salt) {
    // reading the file
    ifstream file(path, ios::binary);
    ostringstream ss;
    ss << file.rdbuf();
    string initFileContent = ss.str();
    file.close();
    
    size_t len = initFileContent.length();
    char* sequence = generateSequence(salt, len);
    // cerr << initFileContent;
    
    // decyphering
    for (size_t i = 0; i < len; ++i)
        initFileContent[i] ^= sequence[i];

    // write into the file
    ofstream outputFile(path.parent_path() / "decoded.txt");
    outputFile << initFileContent;
    outputFile.close();
    
    delete[] sequence;
    return 0;
}

// ./bin/decode /home/nomen/Documents/folder1/encoded.txt /home/nomen/Documents/folder1/salt.txt
int main(int argc, char* argv[]) {
    string argumentsRequired = "1st - path to the file;\n2nd - path to the salt file";
    if (argc != 3) {
        cerr << "WRONG NUMBER OF ARGUMENTS (" << argc - 1 << ")!\n" << argumentsRequired << endl;
        return 1;
    }
    filesystem::path filePath = filesystem::path(argv[1]);
    filesystem::path saltPath = filesystem::path(argv[2]);

    if (!filesystem::exists(filePath) || !filesystem::exists(saltPath)) {
        cerr << "NO SUCH FILE(-S): " << filePath << "!";
        return 1;
    }

    // Read the salt from the salt file
    ifstream saltFile(saltPath);
    string saltString;
    saltFile >> saltString;
    saltFile.close();

    return decodeMessage(filePath, saltString);
}