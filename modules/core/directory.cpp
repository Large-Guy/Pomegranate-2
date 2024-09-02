#include "directory.h"

Directory::Directory()
{
    this->_path = "";
}

Directory::Directory(const String& path)
{
    this->_path = path;
}

void Directory::open(const String& path)
{
    this->_path = path;
}

String Directory::path() const
{
    return this->_path;
}

List<String> Directory::files()
{
    List<String> files;
    for (const auto& entry : std::filesystem::directory_iterator(this->_path.data())) {
        files.add(entry.path().string().c_str());
    }
    return files;
}

List<String> Directory::directories()
{
    List<String> directories;
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

Directory Directory::copy(const String& destination)
{
    std::filesystem::copy(this->_path.data(), destination.data());
    return Directory(destination);
}