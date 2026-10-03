#include <iostream>
#include <filesystem>

using namespace std;

// look at 3 cases: a file is added; a file is deleted; a file is modified (the hash is different)
int main(int argc, char* argv[]) {
    filesystem::path target_dir = argv[1];

    if (filesystem::exists(target_dir) && filesystem::is_directory(target_dir)) {
        for (const auto& entry : filesystem::directory_iterator(target_dir)) {
            std::cout << (entry.is_directory() ? "[DIR] " : "[FILE] ")
                      << entry.path().filename() << '\n';
        }
    }
}