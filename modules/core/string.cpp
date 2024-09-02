#include "string.h"
#include <cstring>
#include <iostream>

String::String() {
    this->_data = nullptr;
    this->_length = 0;
}

String::String(const char* value) {
    this->_length = strlen(value);
    this->_data = new char[this->_length + 1]; // +1 for null terminator
    strcpy(this->_data, value);
}

String::String(const char* value, size_t length) {
    this->_length = length;
    this->_data = new char[this->_length + 1]; // +1 for null terminator
    strncpy(this->_data, value, length);
    this->_data[this->_length] = '\0';
}

String::String(int value) {
    this->_length = snprintf(nullptr, 0, "%d", value);
    this->_data = new char[this->_length + 1]; // +1 for null terminator
    sprintf(this->_data, "%d", value);
}

String::String(float value) {
    this->_length = snprintf(nullptr, 0, "%f", value);
    this->_data = new char[this->_length + 1]; // +1 for null terminator
    sprintf(this->_data, "%f", value);
}

String::String(double value) {
    this->_length = snprintf(nullptr, 0, "%f", value);
    this->_data = new char[this->_length + 1]; // +1 for null terminator
    sprintf(this->_data, "%f", value);
}

String::String(bool value) {
    this->_length = snprintf(nullptr, 0, "%d", value);
    this->_data = new char[this->_length + 1]; // +1 for null terminator
    sprintf(this->_data, "%d", value);
}

String::String(const String& other) {
    this->_length = other._length;
    this->_data = new char[this->_length + 1]; // +1 for null terminator
    strcpy(this->_data, other._data);
}

String::~String() {
    delete[] this->_data;
}

String::operator const char*() const {
    return this->_data;
}

std::ostream& operator<<(std::ostream& os, const String& string) {
    os << string._data;
    return os;
}

String& String::operator=(const String& other) {
    if (this == &other) {
        return *this;
    }
    delete[] this->_data;
    this->_length = other._length;
    this->_data = new char[this->_length + 1]; // +1 for null terminator
    strcpy(this->_data, other._data);
    return *this;
}

String& String::operator=(const char* other) {
    delete[] this->_data;
    this->_length = strlen(other);
    this->_data = new char[this->_length + 1]; // +1 for null terminator
    strcpy(this->_data, other);
    return *this;
}

String String::operator+(const String& other) {
    size_t otherLength = other._length;
    char* newValue = new char[this->_length + otherLength + 1]; // +1 for null terminator
    strcpy(newValue, this->_data);
    strcat(newValue, other._data);
    return String(newValue);
}

String String::operator+(const char* other) {
    size_t otherLength = strlen(other);
    char* newValue = new char[this->_length + otherLength + 1]; // +1 for null terminator
    strcpy(newValue, this->_data);
    strcat(newValue, other);
    return String(newValue);
}

String& String::operator+=(const String& other) {
    size_t otherLength = other._length;
    char* newValue = new char[this->_length + otherLength + 1]; // +1 for null terminator
    strcpy(newValue, this->_data);
    strcat(newValue, other._data);
    delete[] this->_data;
    this->_data = newValue;
    this->_length += otherLength;
    return *this;
}

String& String::operator+=(const char* other) {
    size_t otherLength = strlen(other);
    char* newValue = new char[this->_length + otherLength + 1]; // +1 for null terminator
    strcpy(newValue, this->_data);
    strcat(newValue, other);
    delete[] this->_data;
    this->_data = newValue;
    this->_length += otherLength;
    return *this;
}

String& String::operator+(char other) {
    char* newValue = new char[this->_length + 2]; // +1 for char and +1 for null terminator
    strcpy(newValue, this->_data);
    newValue[this->_length] = other;
    newValue[this->_length + 1] = '\0';
    delete[] this->_data;
    this->_data = newValue;
    this->_length++;
    return *this;
}

String& String::operator+=(char other) {
    return *this + other;
}

String& operator+(const char* lhs, String& rhs) {
    size_t lhsLength = strlen(lhs);
    char* newValue = new char[lhsLength + rhs._length + 1]; // +1 for null terminator
    strcpy(newValue, lhs);
    strcat(newValue, rhs._data);
    delete[] rhs._data;
    rhs._data = newValue;
    rhs._length += lhsLength;
    return rhs;
}

String& operator+=(const char* lhs, String& rhs) {
    return lhs + rhs;
}

bool String::operator==(const String& other) const {
    return strcmp(this->_data, other._data) == 0;
}

bool String::operator==(const char* other) const {
    return strcmp(this->_data, other) == 0;
}

bool String::operator!=(const String& other) const {
    return strcmp(this->_data, other._data) != 0;
}

bool String::operator!=(const char* other) const {
    return strcmp(this->_data, other) != 0;
}

char& String::operator[](size_t index) const {
    return this->_data[index];
}

List<String> String::split(char delimiter) const {
    List<String> result = List<String>();
    const char* start = _data;
    const char* current = _data;

    while (*current != '\0') {
        if (*current == delimiter) {
            result.add(String(start, current - start));
            start = current + 1;
        }
        ++current;
    }

    // Add the last segment after the last delimiter
    if (start != current) {
        result.add(String(start, current - start));
    }

    return result;
}

String String::substring(size_t start, size_t end) const {
    return String(_data + start, end - start);
}

char* String::data() const {
    return this->_data;
}

void String::reserve(size_t size) {
    char* newData = new char[size + 1];
    strcpy(newData, this->_data);
    delete[] this->_data;
    this->_data = newData;
    this->_length = size;
    this->_data[this->_length] = '\0'; // Ensure null termination
}

size_t String::length() const {
    return this->_length;
}

String String::merge(const List<String>& strings, const String& delimiter) {
    String result = "";
    for (size_t i = 0; i < strings.size(); i++) {
        result += strings[i];
        if (i < strings.size() - 1) {
            result += delimiter;
        }
    }
    return result;
}

void String::serialize(Archive &a) const {
    a << this->_length;
    for (size_t i = 0; i < this->_length; i++) {
        a << this->_data[i];
    }
}

void String::deserialize(Archive &a) {
    a >> &this->_length;
    delete[] this->_data; // Delete existing data before allocating new
    this->_data = new char[this->_length + 1]; // +1 for null terminator
    for (size_t i = 0; i < this->_length; i++) {
        a >> &this->_data[i];
    }
    this->_data[this->_length] = '\0'; // Ensure null termination
}
