#ifndef POMEGRANATEENGINE_SERIALIZABLE_H
#define POMEGRANATEENGINE_SERIALIZABLE_H
#include<vector>
#include<cstdlib>
#include<cstring>
#include<string>
#include<iostream>
#include<fstream>

class Serializable;
struct Archive;

struct Archive {
private:
    std::vector<char> _data;
    int _index;
public:
    Archive();
    Archive& operator<<(long i);
    Archive& operator<<(ulong i);
    Archive& operator<<(int i);
    Archive& operator<<(uint i);
    Archive& operator<<(short i);
    Archive& operator<<(ushort i);
    Archive& operator<<(char i);
    Archive& operator<<(double i);
    Archive& operator<<(float i);
    Archive& operator<<(bool i);
    Archive& operator<<(const std::string& i);
    Archive& operator<<(const Serializable& i);
    Archive& operator<<(const Serializable* i);

    Archive& operator>>(const long* i);
    Archive& operator>>(const ulong* i);
    Archive& operator>>(const int* i);
    Archive& operator>>(const uint* i);
    Archive& operator>>(const short* i);
    Archive& operator>>(const ushort* i);
    Archive& operator>>(const char* i);
    Archive& operator>>(const double* i);
    Archive& operator>>(const float* i);
    Archive& operator>>(const bool* i);
    Archive& operator>>(const std::string* i);
    Archive& operator>>(const Serializable* i);

    size_t size();
    void getBytes(char* buffer, size_t size);
    void writeToFile(const char* filename);
    void readFromFile(const char* filename);
};

class Serializable {
public:
    virtual void serialize(Archive&) const;
    virtual void deserialize(Archive&);
};


#endif //POMEGRANATEENGINE_SERIALIZABLE_H
