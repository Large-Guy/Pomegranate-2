#include "serializable.h"

Archive::Archive(){
    data = std::vector<char>();
}

Archive& Archive::operator<<(long i){
    // Convert long to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(long); j++)
    {
        data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(ulong i){
    // Convert ulong to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(ulong); j++)
    {
        data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(int i){
    // Convert int to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(int); j++)
    {
        data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(uint i){
    // Convert uint to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(uint); j++)
    {
        data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(short i){
    // Convert short to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(short); j++)
    {
        data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(ushort i){
    // Convert ushort to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(ushort); j++)
    {
        data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(char i){
    // Convert char to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(char); j++)
    {
        data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(double i){
    // Convert double to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(double); j++)
    {
        data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(float i){
    // Convert float to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(float); j++)
    {
        data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(bool i){
    // Convert bool to char array
    char* c = (char*)&i;
    for (int j = 0; j < sizeof(bool); j++)
    {
        data.push_back(c[j]);
    }
    return *this;
}

Archive& Archive::operator<<(const std::string& i){
    //Add the string size
    *this << i.size();

    // Add the string to the data
    for (char j : i)
    {
        data.push_back(j);
    }
    return *this;
}

Archive& Archive::operator<<(const Serializable& i) {

    // Serialize the object
    Archive a;
    i.serialize(a);
    // Get the bytes from the archive
    char* bytes = new char[a.data.size()];
    a.get_bytes(bytes, a.data.size());

    // Push size of the object
    *this << a.data.size();

    // Add the bytes to the data
    for (int j = 0; j < a.data.size(); j++)
    {
        data.push_back(bytes[j]);
    }
    return *this;
}

Archive& Archive::operator<<(const Serializable* i) {
    // Serialize the object
    Archive a;
    i->serialize(a);
    // Get the bytes from the archive
    char* bytes = new char[a.data.size()];
    a.get_bytes(bytes, a.data.size());

    // Push size of the object
    *this << a.data.size();

    // Add the bytes to the data
    for (int j = 0; j < a.data.size(); j++)
    {
        data.push_back(bytes[j]);
    }
    return *this;
}

Archive& Archive::operator>>(const long* i){
    // Convert char array to long
    char* c = new char[sizeof(long)];
    for (int j = 0; j < sizeof(long); j++)
    {
        c[j] = data[j];
    }
    *(long*)i = *(long*)c;
    // Remove the bytes from the data
    data.erase(data.begin(), data.begin() + sizeof(long));
    return *this;
}

Archive& Archive::operator>>(const ulong* i){
    // Convert char array to ulong
    char* c = new char[sizeof(ulong)];
    for (int j = 0; j < sizeof(ulong); j++)
    {
        c[j] = data[j];
    }
    *(ulong*)i = *(ulong*)c;
    // Remove the bytes from the data
    data.erase(data.begin(), data.begin() + sizeof(ulong));
    return *this;
}

Archive& Archive::operator>>(const int* i){
    // Convert char array to int
    char* c = new char[sizeof(int)];
    for (int j = 0; j < sizeof(int); j++)
    {
        c[j] = data[j];
    }
    *(int*)i = *(int*)c;
    // Remove the bytes from the data
    data.erase(data.begin(), data.begin() + sizeof(int));
    return *this;
}

Archive& Archive::operator>>(const uint* i){
    // Convert char array to uint
    char* c = new char[sizeof(uint)];
    for (int j = 0; j < sizeof(uint); j++)
    {
        c[j] = data[j];
    }
    *(uint*)i = *(uint*)c;
    // Remove the bytes from the data
    data.erase(data.begin(), data.begin() + sizeof(uint));
    return *this;
}

Archive& Archive::operator>>(const short* i){
    // Convert char array to short
    char* c = new char[sizeof(short)];
    for (int j = 0; j < sizeof(short); j++)
    {
        c[j] = data[j];
    }
    *(short*)i = *(short*)c;
    // Remove the bytes from the data
    data.erase(data.begin(), data.begin() + sizeof(short));
    return *this;
}

Archive& Archive::operator>>(const ushort* i){
    // Convert char array to ushort
    char* c = new char[sizeof(ushort)];
    for (int j = 0; j < sizeof(ushort); j++)
    {
        c[j] = data[j];
    }
    *(ushort*)i = *(ushort*)c;
    // Remove the bytes from the data
    data.erase(data.begin(), data.begin() + sizeof(ushort));
    return *this;
}

Archive& Archive::operator>>(const char* i){
    // Convert char array to char
    *(char*)i = data[0];
    // Remove the bytes from the data
    data.erase(data.begin(), data.begin() + sizeof(char));
    return *this;
}

Archive& Archive::operator>>(const double* i){
    // Convert char array to double
    char* c = new char[sizeof(double)];
    for (int j = 0; j < sizeof(double); j++)
    {
        c[j] = data[j];
    }
    *(double*)i = *(double*)c;
    // Remove the bytes from the data
    data.erase(data.begin(), data.begin() + sizeof(double));
    return *this;
}

Archive& Archive::operator>>(const float* i){
    // Convert char array to float
    char* c = new char[sizeof(float)];
    for (int j = 0; j < sizeof(float); j++)
    {
        c[j] = data[j];
    }
    *(float*)i = *(float*)c;
    // Remove the bytes from the data
    data.erase(data.begin(), data.begin() + sizeof(float));
    return *this;
}

Archive& Archive::operator>>(const bool* i){
    // Convert char array to bool
    *(bool*)i = data[0];
    // Remove the bytes from the data
    data.erase(data.begin(), data.begin() + sizeof(bool));
    return *this;
}

Archive& Archive::operator>>(const std::string* i){
    // Get the size of the string
    size_t size;
    *this >> &size;
    // Get the string from the data
    std::string s = "";
    for (int j = 0; j < size; j++)
    {
        s += data[j];
    }
    // Remove the bytes from the data
    data.erase(data.begin(), data.begin() + size);
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
    std::cout << size << std::endl;

    // Get the bytes of the object
    char* bytes = new char[size];
    for (int j = 0; j < size; j++)
    {
        bytes[j] = data[j];
    }
    // Remove the bytes from the data
    data.erase(data.begin(), data.begin() + size);
    // Create an archive from the bytes
    Archive a;
    a.data = std::vector<char>(bytes, bytes + size);
    // Deserialize the object
    i->deserialize(a);
    return *this;
}

size_t Archive::size(){
    return data.size();
}

void Archive::get_bytes(char* buffer, size_t size){
    // Copy the data to the buffer
    for (int i = 0; i < size; i++)
    {
        buffer[i] = data[i];
    }
}

void Archive::write_to_file(const char* filename) {
    // Open the file
    std::ofstream file;
    file.open(filename, std::ios::out | std::ios::binary);
    // Write the data to the file
    for (int i = 0; i < data.size(); i++)
    {
        file << data[i];
    }
    // Close the file
    file.close();
}

void Archive::read_from_file(const char* filename) {
    // Open the file
    std::ifstream file;
    file.open(filename, std::ios::in | std::ios::binary);
    // Read the data from the file
    char c;
    while (file >> c)
    {
        data.push_back(c);
    }
    // Close the file
    file.close();
}

void Serializable::serialize(Archive& a) const {
    // Do nothing
}

void Serializable::deserialize(Archive& a) const {
    // Do nothing
}

