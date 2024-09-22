#ifndef POMEGRANATE_ENGINE_CORE_FILE_MANAGER_H
#define POMEGRANATE_ENGINE_CORE_FILE_MANAGER_H
#include <fstream>
#include "list.h"

struct File {
private:
    std::string _path;
    List<List<char>> _lines;
    List<char> buffer;
    std::fstream _file;
public:
    File();
    File(const std::string& path);
    ~File();
    void open();
    void open(const std::string& path);
    void close();
    bool exists();
    std::string readTextLine();
    std::string readText();
    List<List<char>>& readBufferLine();
    List<char>& readBuffer();
};


#endif //POMEGRANATEENGINE_FILE_MANAGER_H
