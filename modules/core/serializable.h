#ifndef POMEGRANATE_ENGINE_CORE_SERIALIZABLE_H
#define POMEGRANATE_ENGINE_CORE_SERIALIZABLE_H
#include<vector>
#include<cstdlib>
#include<cstring>
#include<string>
#include<iostream>
#include<fstream>
#include<unordered_map>

struct Archive;

struct Archive {
private:
    std::vector<char> _data = {};
    int _index = 0;
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
    Archive& operator<<(const std::string& i);
    Archive& operator<<(Archive& archive);

    Archive& operator>>(long& i);
    Archive& operator>>(unsigned long& i);
    Archive& operator>>(int& i);
    Archive& operator>>(uint& i);
    Archive& operator>>(short& i);
    Archive& operator>>(ushort& i);
    Archive& operator>>(char& i);
    Archive& operator>>(char*& i);
    Archive& operator>>(double& i);
    Archive& operator>>(float& i);
    Archive& operator>>(bool& i);
    Archive& operator>>(std::string& i);
    Archive& operator>>(Archive& archive);

    template<typename T> Archive& operator<<(T object){
        object.serialize(*this);
        return *this;
    }

    template<typename T> Archive& operator<<(std::vector<T> object){
        *this << object.size();
        for(auto& i : object){
            *this << i;
        }
        return *this;
    }

    template<typename T> Archive& operator>>(T& object){
        object.deserialize(*this);
        return *this;
    }

    template<typename T> Archive& operator>>(std::vector<T>& object){
        size_t size = 0;
        *this >> size;
        object.resize(size);
        for(auto& i : object){
            *this >> i;
        }
        return *this;
    }

    size_t size();
    const std::vector<char>& getBytes();
    bool isEnd();
    void writeToFile(const char* filename);
    void readFromFile(const char* filename);
};

template<typename T> void serialize(T& object, Archive& archive)
{
    object._serialize(archive);
}

template<typename T> void deserialize(T& object, Archive& archive)
{
    object._deserialize(archive);
}

template<typename T> T duplicate(T& object)
{
    Archive a;
    a << object;
    T result;
    a >> result;
    return result;
}

#define SERIALIZE_TO_FILE(what,filename) \
    { \
        Archive a; \
        a << (what);                       \
        a.writeToFile(filename);         \
    }

#define DESERIALIZE_FROM_FILE(what,filename) \
    { \
        Archive a; \
        a.readFromFile(filename); \
        a >> &(what); \
    }

#endif //POMEGRANATE_ENGINE_SERIALIZABLE_H
