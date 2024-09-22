#ifndef POMEGRANATE_ENGINE_CORE_DIRECTORY_H
#define POMEGRANATE_ENGINE_CORE_DIRECTORY_H
#include "list.h"
#include "file.h"
#include <filesystem>

class Directory {
private:
    std::string _path;
public:
    Directory();
    Directory(const std::string& path);
    void open(const std::string& path);
    std::string path() const;
    List<std::string> files();
    List<std::string> directories();
    void init();
    void remove();
    bool exists();
    Directory copy(const std::string& destination);
};


#endif //POMEGRANATEENGINE_DIRECTORY_H
