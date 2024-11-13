#include "serializable.h"

Archive::Archive(){
    _data = std::vector<char>();
}

Archive& Archive::operator<<(long i){
    // Convert long to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(long); j++)
    {
        _data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(unsigned long i){
    // Convert ulong to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(unsigned long); j++)
    {
        _data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(int i){
    // Convert int to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(int); j++)
    {
        _data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(uint i){
    // Convert uint to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(uint); j++)
    {
        _data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(short i){
    // Convert short to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(short); j++)
    {
        _data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(ushort i){
    // Convert ushort to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(ushort); j++)
    {
        _data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(char i){
    // Convert char to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(char); j++)
    {
        _data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(double i){
    // Convert double to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(double); j++)
    {
        _data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(float i){
    // Convert float to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(float); j++)
    {
        _data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(bool i){
    // Convert bool to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(bool); j++)
    {
        _data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(const std::string& i){
    //Add the string size
    *this << (unsigned long)i.size();

    // Add the string to the _data
    for (int j = 0; j < i.size(); j++)
    {
        _data.push_back(i[j]);
    }
    return *this;
}

Archive& Archive::operator<<(Archive& archive){
    // Add the size of the archive
    *this << (unsigned long)archive.size();
    // Add the bytes from the archive to the _data
    for (int i = 0; i < archive.size(); i++)
    {
        _data.push_back(archive.getBytes()[i]);
    }
    return *this;
}

Archive& Archive::operator>>(long& i){
    // Convert char array to long
    char* c = new char[sizeof(long)];
    for (int j = 0; j < sizeof(long); j++)
    {
        c[j] = _data[j];
    }
    i = *(long*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(long));
    return *this;
}

Archive& Archive::operator>>(unsigned long& i){
    // Convert char array to ulong
    char* c = new char[sizeof(unsigned long)];
    for (int j = 0; j < sizeof(unsigned long); j++)
    {
        c[j] = _data[j];
    }
    i = *(unsigned long*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(unsigned long));

    delete[] c;
    return *this;
}

Archive& Archive::operator>>(int& i){
    // Convert char array to int
    char* c = new char[sizeof(int)];
    for (int j = 0; j < sizeof(int); j++)
    {
        c[j] = _data[j];
    }
    i = *(int*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(int));
    return *this;
}

Archive& Archive::operator>>(uint& i){
    // Convert char array to uint
    char* c = new char[sizeof(uint)];
    for (int j = 0; j < sizeof(uint); j++)
    {
        c[j] = _data[j];
    }
    i = *(uint*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(uint));
    return *this;
}

Archive& Archive::operator>>(short& i){
    // Convert char array to short
    char* c = new char[sizeof(short)];
    for (int j = 0; j < sizeof(short); j++)
    {
        c[j] = _data[j];
    }
    i = *(short*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(short));
    return *this;
}

Archive& Archive::operator>>(ushort& i){
    // Convert char array to ushort
    char* c = new char[sizeof(ushort)];
    for (int j = 0; j < sizeof(ushort); j++)
    {
        c[j] = _data[j];
    }
    i = *(ushort*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(ushort));
    return *this;
}

Archive& Archive::operator>>(char& i){
    // Convert char array to char
    char* c = new char[sizeof(char)];
    for (int j = 0; j < sizeof(char); j++)
    {
        c[j] = _data[j];
    }
    i = *(char*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(char));
    return *this;
}

Archive& Archive::operator>>(char*& i){
    //Copy _data to i
    size_t len = strlen(_data.data());
    memccpy(i, _data.data(), len, sizeof(char));
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(char) * len);
    return *this;
}

Archive& Archive::operator>>(double& i){
    // Convert char array to double
    char* c = new char[sizeof(double)];
    for (int j = 0; j < sizeof(double); j++)
    {
        c[j] = _data[j];
    }
    i = *(double*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(double));
    return *this;
}

Archive& Archive::operator>>(float& i){
    // Convert char array to float
    char* c = new char[sizeof(float)];
    for (int j = 0; j < sizeof(float); j++)
    {
        c[j] = _data[j];
    }
    i = *(float*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(float));
    return *this;
}

Archive& Archive::operator>>(bool& i){
    // Convert char array to bool
    char* c = new char[sizeof(bool)];
    for (int j = 0; j < sizeof(bool); j++)
    {
        c[j] = _data[j];
    }
    i = *(bool*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(bool));
    return *this;
}

Archive& Archive::operator>>(std::string& i){
    // Get the size of the string
    size_t size;
    *this >> size;
    // Get the string from the _data
    std::string s = "";
    for (int j = 0; j < size; j++)
    {
        s += _data[j];
    }
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + size);
    // Set the string
    i = s;
    return *this;
}

Archive& Archive::operator>>(Archive& archive){
    // Get the size of the archive
    size_t size;
    *this >> size;
    // Get the bytes from the _data
    std::vector<char> v;
    for (int i = 0; i < size; i++)
    {
        v.push_back(_data[i]);
    }
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + size);
    // Set the archive
    archive = Archive();
    archive._data = v;
    return *this;
}

size_t Archive::size(){
    return _data.size();
}

const std::vector<char>& Archive::getBytes(){
    return _data;
}

bool Archive::isEnd() {
    return _index >= _data.size();
}

void Archive::writeToFile(const char* filename) {
    // Open the file in binary mode
    std::ofstream file(filename, std::ios::out | std::ios::binary);

    // Check if file opened successfully
    if (!file.is_open()) {
        // Handle error - couldn't open file
        return;
    }

    // Write _data using write function
    file.write(_data.data(), _data.size() * sizeof(_data[0]));

    // Close the file
    file.close();
}

void Archive::readFromFile(const char* filename) {
    // Open the file in binary mode
    std::ifstream file (filename, std::ios::in | std::ios::binary);

    // Check if file opened successfully
    if (!file.is_open()) {
        // Handle error - couldn't open file
        return;
    }

    // Get the size of the file
    file.seekg(0, file.end);
    int size = file.tellg();
    file.seekg(0, file.beg);

    // Read the file into _data
    _data.resize(size);
    file.read(_data.data(), size);

    // Close the file
    file.close();
}