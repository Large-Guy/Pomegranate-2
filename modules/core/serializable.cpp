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

Archive& Archive::operator<<(const Serializable& i) {

    // Serialize the object
    Archive a;
    i.serialize(a);
    // Get the bytes from the archive
    char* bytes = new char[a._data.size()];
    for(int n = 0; n < a._data.size(); n++)
    {
        bytes[n] = a.getBytes()[n];
    }

    // Push size of the object
    *this << a._data.size();

    // Add the bytes to the _data
    for (int j = 0; j < a._data.size(); j++)
    {
        _data.push_back(bytes[j]);
    }
    return *this;
}

Archive& Archive::operator<<(const Serializable* i) {
    // Serialize the object
    Archive a;
    i->serialize(a);
    // Get the bytes from the archive
    char* bytes = new char[a._data.size()];
    for(int n = 0; n < a._data.size(); n++)
    {
        bytes[n] = a.getBytes()[n];
    }

    // Push size of the object
    *this << a._data.size();

    // Add the bytes to the _data
    for (int j = 0; j < a._data.size(); j++)
    {
        _data.push_back(bytes[j]);
    }
    return *this;
}

Archive& Archive::operator>>(const long* i){
    // Convert char array to long
    char* c = new char[sizeof(long)];
    for (int j = 0; j < sizeof(long); j++)
    {
        c[j] = _data[j];
    }
    *(long*)i = *(long*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(long));
    return *this;
}

Archive& Archive::operator>>(const unsigned long* i){
    // Convert char array to ulong
    char* c = new char[sizeof(unsigned long)];
    for (int j = 0; j < sizeof(unsigned long); j++)
    {
        c[j] = _data[j];
    }
    *(unsigned long*)i = *(unsigned long*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(unsigned long));

    delete[] c;
    return *this;
}

Archive& Archive::operator>>(const int* i){
    // Convert char array to int
    char* c = new char[sizeof(int)];
    for (int j = 0; j < sizeof(int); j++)
    {
        c[j] = _data[j];
    }
    *(int*)i = *(int*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(int));
    return *this;
}

Archive& Archive::operator>>(const uint* i){
    // Convert char array to uint
    char* c = new char[sizeof(uint)];
    for (int j = 0; j < sizeof(uint); j++)
    {
        c[j] = _data[j];
    }
    *(uint*)i = *(uint*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(uint));
    return *this;
}

Archive& Archive::operator>>(const short* i){
    // Convert char array to short
    char* c = new char[sizeof(short)];
    for (int j = 0; j < sizeof(short); j++)
    {
        c[j] = _data[j];
    }
    *(short*)i = *(short*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(short));
    return *this;
}

Archive& Archive::operator>>(const ushort* i){
    // Convert char array to ushort
    char* c = new char[sizeof(ushort)];
    for (int j = 0; j < sizeof(ushort); j++)
    {
        c[j] = _data[j];
    }
    *(ushort*)i = *(ushort*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(ushort));
    return *this;
}

Archive& Archive::operator>>(const char* i){
    //Copy data to i
    *(char*)i = _data[0];
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(char));
    return *this;
}

Archive& Archive::operator>>(const char** i){
    //Copy data to i
    *(char*)i = _data[0];
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(char));
    return *this;
}

Archive& Archive::operator>>(const double* i){
    // Convert char array to double
    char* c = new char[sizeof(double)];
    for (int j = 0; j < sizeof(double); j++)
    {
        c[j] = _data[j];
    }
    *(double*)i = *(double*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(double));
    return *this;
}

Archive& Archive::operator>>(const float* i){
    // Convert char array to float
    char* c = new char[sizeof(float)];
    for (int j = 0; j < sizeof(float); j++)
    {
        c[j] = _data[j];
    }
    *(float*)i = *(float*)c;
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(float));
    return *this;
}

Archive& Archive::operator>>(const bool* i){
    // Convert char array to bool
    *(bool*)i = _data[0];
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + sizeof(bool));
    return *this;
}

Archive& Archive::operator>>(const std::string* i){
    // Get the size of the string
    size_t size;
    *this >> &size;
    // Get the string from the _data
    std::string s = "";
    for (int j = 0; j < size; j++)
    {
        s += _data[j];
    }
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + size);
    // Set the string
    *(std::string*)i = s;
    return *this;
}

Archive& Archive::operator>>(const Serializable* i) {
    // Deserialize the object
    // Get the size of the object
    size_t size;
    *this >> &size;
    //Print the size

    // Get the bytes of the object
    char* bytes = new char[size];
    for (int j = 0; j < size; j++)
    {
        bytes[j] = _data[j];
    }
    // Remove the bytes from the _data
    _data.erase(_data.begin(), _data.begin() + size);
    // Create an archive from the bytes
    Archive a;
    a._data = std::vector<char>(bytes, bytes + size);
    // Deserialize the object
    ((Serializable*)i)->deserialize(a);
    return *this;
}

size_t Archive::size(){
    return _data.size();
}

char* Archive::getBytes(){
    return _data.data();
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

void Serializable::serialize(Archive& a) const {
    // Do nothing
}

void Serializable::deserialize(Archive& a) {
    // Do nothing
}

void Serializable::toFile(const char* filename) const {
    // Serialize the object
    Archive a;
    serialize(a);
    // Write the archive to a file
    a.writeToFile(filename);
}

void Serializable::fromFile(const char* filename) {
    // Read the archive from a file
    Archive a;
    a.readFromFile(filename);
    // Deserialize the object
    deserialize(a);
}