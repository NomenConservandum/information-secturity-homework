#include <iostream>
#include <filesystem>

using namespace std;

// The third method: change Cyrillic letters to their Latin counterparts

// We're going to use the find_first_of for the Russian letters and change them recursively, if num & 1 == 1 => encode, else, skip.

// the first value is the number of arguments, the second one - path to the file,
// the third one - mode (1 - incode, 2 - decode), the fourth one - message for to incode.
int main(int argc, char* argv[]) {
    string argumentsRequired = "1st - path to the file;\n2nd - mode (1 for incode and 2 for decode)\n3rd - message to decode (if mode 1 is chosen)";
    if (argc < 2 || argc > 3 || *argv[2] == '1' && argc != 3 || *argv[2] == '2' && argc != 2) {
        cerr << "WRONG NUMBER OF ARGUMENTS!" << argumentsRequired << endl;
        return 1;
    }
    filesystem::path filePath = filesystem::path(argv[1]);

    if (!filesystem::exists(filePath)) {
        cerr << "NO SUCH FILE: " << filePath << "!";
        return 1;
    }

    
    return 0;
}