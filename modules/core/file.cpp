#include "file.h"

File::File() {
    this->_path = "";
    this->_name = "";
    this->_extension = "";
    this->_parentDirectory = "";
    this->_content = "";
    this->_lines = List<String>();
    this->_file.open(this->_path.data());
}

File::File(const String& path) {
    this->_path = path;
    List<String> parts = path.split('/');
    this->_name = parts.last();
    parts.remove(parts.size() - 1);
    this->_parentDirectory = String::merge(parts,"/");
    this->_content = "";
    this->_lines = List<String>();
}

File::~File() {
    this->close();
}

File& File::open() {
    List<String> parts = this->_path.split('/');
    this->_name = parts.last();
    parts.remove(parts.size() - 1);
    this->_parentDirectory = String::merge(parts,"/");
    this->_file.open(this->_path.data());

    if (exists()) {
        std::ifstream file(this->_path.data());
        std::string line;
        while (std::getline(file, line)) {
            this->_content += String(line.c_str(),line.size()) + '\n';
            this->_lines.add(line.c_str());
        }
        file.close();
    }
    return *this;
}

File& File::open(const String &path) {
    this->_path = path;
    List<String> parts = path.split('/');
    this->_name = parts.last();
    parts.remove(parts.size() - 1);
    this->_parentDirectory = String::merge(parts,"/");
    this->_file.open(this->_path.data());

    if (exists()) {
        std::ifstream file(this->_path.data());
        std::string line;
        while (std::getline(file, line)) {
            this->_content += String(line.c_str()) + '\n';
            this->_lines.add(line.c_str());
        }
        file.close();
    }
    return *this;
}

String File::path() const {
    return this->_path;
}

String File::name() const {
    return this->_name;
}

String File::extension() const {
    return this->_extension;
}

String File::parentDirectory() const {
    return this->_parentDirectory;
}

char* File::content() const {
    return this->_content.data();
}

String File::read() {
    return this->_content;
}

String File::readLine(int line) {
    return this->_lines[line];
}

List<String> File::readLines() {
    return this->_lines;
}

uint File::lineCount() {
    return this->_lines.size();
}

void File::write(const String& content) {
    this->_content = content;
    this->_lines = this->_content.split('\n');
}

void File::writeLine(int line, const String &content) {
    this->_lines[line] = content;
}

void File::insertLine(const String &content, int line) {
    this->_lines.insert(line, content);
}

void File::append(const String& content) {
    this->_content += content;
    this->_lines = this->_content.split('\n');
}

void File::commit() {
    this->_file << this->_content.data();
}

bool File::exists() {
    std::ifstream file(_path.data());
    return file.good();
}

String& File::operator[](int index) {
    return this->_lines[index];
}

void File::init() {
    // Used to create a new file
    if (!exists()) {
        this->_file.open(this->_path.data(), std::ios::out);
        this->_file.close();
    }
}

void File::remove() {
    this->_file.close();
    //Delete the file
    std::remove(this->_path.data());
}

void File::close() {
    this->_file.close();
    this->_content = "";
    this->_lines.clear();
}