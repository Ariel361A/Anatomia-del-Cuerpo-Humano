#include "FileSystem.h"
#include "root_directory.h" // Este lo necesitas porque usas logl_root

std::string FileSystem::getPathRelativeRoot(const std::string& path)
{
    return getRoot() + std::string("/") + path;
}

std::string FileSystem::getPathRelativeBinary(const std::string& path)
{
    return "../../../" + path;
}

FileSystem::Builder FileSystem::getPathBuilder()
{
    if (getRoot() != "")
        return &FileSystem::getPathRelativeRoot;
    else
        return &FileSystem::getPathRelativeBinary;
}

const std::string& FileSystem::getRoot()
{
    static std::string root = []() {
        char* envRoot = nullptr;
        size_t len = 0;

        if (_dupenv_s(&envRoot, &len, "LOGL_ROOT_PATH") == 0 && envRoot != nullptr)
        {
            std::string result(envRoot);
            free(envRoot);
            return result;
        }
        return std::string(logl_root != nullptr ? logl_root : "");
        }();

    return root;
}
