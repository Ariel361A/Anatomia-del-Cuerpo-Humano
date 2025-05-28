#include <filesystem>
#include <string>

const char* getCurrentDirectory() {
    static std::string path = std::filesystem::current_path().string();
    return path.c_str();
}

// Uso:
const char* logl_root = getCurrentDirectory();
