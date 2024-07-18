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
    std::vector<char> data;
    int index;
public:
    Archive();
    Archive& operator<<(long i);
    Archive& operator<<(unsigned long i);
    Archive& operator<<(int i);
    Archive& operator<<(uint i);
    Archive& operator<<(short i);
    Archive& operator<<(ushort i);
    Archive& operator<<(char i);
    Archive& operator<<(double i);
    Archive& operator<<(float i);
    Archive& operator<<(bool i);
    Archive& operator<<(const char* i);
    Archive& operator<<(const std::string& i);
    Archive& operator<<(const Serializable& i);
    Archive& operator<<(const Serializable* i);

    Archive& operator>>(long* i);
    Archive& operator>>(unsigned long* i);
    Archive& operator>>(int* i);
    Archive& operator>>(uint* i);
    Archive& operator>>(short* i);
    Archive& operator>>(ushort* i);
    Archive& operator>>(char* i);
    Archive& operator>>(double* i);
    Archive& operator>>(const float* i);
    Archive& operator>>(bool* i);
    Archive& operator>>(char** i);
    Archive& operator>>(std::string* i);
    Archive& operator>>(Serializable* i);

    size_t size();
    void get_bytes(char* buffer, size_t size);
    void write_to_file(const char* filename);
    void read_from_file(const char* filename);
};

class Serializable {
public:
    virtual void serialize(Archive&) const;
    virtual void deserialize(Archive&) const;
};


#endif //POMEGRANATEENGINE_SERIALIZABLE_H
