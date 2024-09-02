#ifndef POMEGRANATE_ENGINE_CORE_FILE_MANAGER_H
#define POMEGRANATE_ENGINE_CORE_FILE_MANAGER_H
#include "string.h"
#include <fstream>

class File {
private:
    String _path;
    String _name;
    String _extension;
    String _parentDirectory;
    String _content;
    List<String> _lines;
    std::fstream _file;
public:
    File();
    File(const String& path);
    ~File();
    void open(const String& path);
    String path() const;
    String name() const;
    String extension() const;
    String parentDirectory() const;
    char* content() const;
    String read();
    String readLine(int line);
    List<String> readLines();
    uint lineCount();
    void write(const String& content);
    void writeLine(int line, const String& content);
    void insertLine(const String& content, int line);
    void append(const String& content);
    void commit();
    bool exists();
    String& operator[](int index);
    void init();
    void close();
};


#endif //POMEGRANATEENGINE_FILE_MANAGER_H
