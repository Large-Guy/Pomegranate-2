#ifndef POMEGRANATE_ENGINE_CORE_FILE_MANAGER_H
#define POMEGRANATE_ENGINE_CORE_FILE_MANAGER_H
#include "string.h"
#include <fstream>

struct File : public Serializable {
private:
    String _path;
    List<List<char>> _lines;
    List<char> buffer;
    std::fstream _file;
public:
    File();
    File(const String& path);
    ~File();
    void open();
    void open(const String& path);
    void close();
    bool exists();
    String readTextLine();
    String readText();
    List<List<char>>& readBufferLine();
    List<char>& readBuffer();
};


#endif //POMEGRANATEENGINE_FILE_MANAGER_H
