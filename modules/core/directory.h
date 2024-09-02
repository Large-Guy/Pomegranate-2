#ifndef POMEGRANATE_ENGINE_CORE_DIRECTORY_H
#define POMEGRANATE_ENGINE_CORE_DIRECTORY_H
#include "string.h"
#include "list.h"
#include "file.h"
#include <filesystem>

class Directory {
private:
    String _path;
public:
    Directory();
    Directory(const String& path);
    void open(const String& path);
    String path() const;
    List<String> files();
    List<String> directories();
    void init();
    void remove();
    bool exists();
    Directory copy(const String& destination);
};


#endif //POMEGRANATEENGINE_DIRECTORY_H
