#include "directory.h"

Directory::Directory()
{
    this->_path = "";
}

Directory::Directory(const std::string& path)
{
    this->_path = path;
}

void Directory::open(const std::string& path)
{
    this->_path = path;
}

std::string Directory::path() const
{
    return this->_path;
}

List<std::string> Directory::files()
{
    List<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(this->_path.data())) {
        files.add(entry.path().string().c_str());
    }
    return files;
}

List<std::string> Directory::directories()
{
    List<std::string> directories;
    for (const auto& entry : std::filesystem::directory_iterator(this->_path.data())) {
        if (entry.is_directory()) {
            directories.add(entry.path().string().c_str());
        }
    }
    return directories;
}

void Directory::init()
{
    if(!exists())
        std::filesystem::create_directories(this->_path.data());
}

void Directory::remove()
{
    //Remove just the top level directory
    std::filesystem::remove(this->_path.data());
}

bool Directory::exists()
{
    return std::filesystem::exists(this->_path.data());
}

Directory Directory::copy(const std::string& destination)
{
    std::filesystem::copy(this->_path.data(), destination.data());
    return Directory(destination);
}