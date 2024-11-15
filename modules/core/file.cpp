#include "file.h"

File::File() {
    this->_path = "";
    this->_lines = List<List<char>>();
    this->buffer = List<char>();
}

File::File(const std::string& path) {
    this->_path = path;
    this->_lines = List<List<char>>();
    this->buffer = List<char>();
}

File::File(const File& file) {
    this->_path = file._path;
    this->_lines = file._lines;
    this->buffer = file.buffer;
}

File::~File() {
    this->_file.close();
}

void File::open() {
    this->_file.open(std::string(this->_path.data()));
}

void File::open(const std::string& path) {
    this->_path = path;
    this->_file.open(this->_path.data());
}

void File::close() {
    this->_file.close();
}

bool File::exists() {
    std::ifstream file(this->_path);
    return file.good();
}

bool File::isOpen() {
    return this->_file.is_open();
}

std::string File::readTextLine() {
    std::string line;
    std::string s;
    std::getline(this->_file, s);
    line = s.c_str();
    return line;
}

std::string File::readText() {
    std::string text;
    std::string s;
    while (std::getline(this->_file, s)) {
        text += s.c_str();
        text += '\n';
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
    //Get file length
    this->_file.seekg(0, std::ios::end);
    size_t length = this->_file.tellg();
    this->buffer.reserve(length);
    this->_file.seekg(0, std::ios::beg);
    char c;
    while (this->_file.get(c)) {
        this->buffer.add(c);
    }
    return this->buffer;
}