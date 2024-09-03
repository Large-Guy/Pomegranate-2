#include "file.h"

File::File() {
    this->_path = "";
    this->_lines = List<List<char>>();
}

File::File(const String& path) {
    this->_path = path;
    this->_lines = List<List<char>>();
}

File::~File() {
    this->_file.close();
}

File& File::open() {
    this->_file.open(this->_path.data());
    return *this;
}

File& File::open(const String& path) {
    this->_path = path;
    this->_file.open(this->_path.data());
    return *this;
}

File& File::close() {
    this->_file.close();
    return *this;
}

bool File::exists() {
    return this->_file.is_open();
}

String File::readTextLine() {
    String line;
    std::string s;
    std::getline(this->_file, s);
    line = s.c_str();
    return line;
}

String File::readText() {
    String text;
    std::string s;
    while (std::getline(this->_file, s)) {
        text += s.c_str();
    }
    return text;
}

List<List<char>>& File::readBufferLine() {
    this->_lines.clear();
    this->_file.seekg(0, std::ios::beg);
    while (!this->_file.eof()) {
        List<char> line;
        char c;
        while (this->_file.get(c) && c != '\n') {
            line.add(c);
        }
        this->_lines.add(line);
    }
    return this->_lines;
}

List<char>& File::readBuffer() {
    this->buffer.clear();
    this->_file.seekg(0, std::ios::beg);
    while (!this->_file.eof()) {
        char c;
        this->_file.get(c);
        this->buffer.add(c);
    }
    return this->buffer;
}