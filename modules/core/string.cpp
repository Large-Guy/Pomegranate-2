#include "string.h"

String::String() {
    this->value = nullptr;
    this->length = 0;
}

String::String(const char* value) {
    this->length = strlen(value);
    this->value = new char[this->length];
    strcpy(this->value, value);
}

String::String(int value) {
    this->length = snprintf(nullptr, 0, "%d", value);
    this->value = new char[this->length];
    sprintf(this->value, "%d", value);
}

String::String(float value) {
    this->length = snprintf(nullptr, 0, "%f", value);
    this->value = new char[this->length];
    sprintf(this->value, "%f", value);
}

String::String(double value) {
    this->length = snprintf(nullptr, 0, "%f", value);
    this->value = new char[this->length];
    sprintf(this->value, "%f", value);
}

String::String(bool value) {
    this->length = snprintf(nullptr, 0, "%d", value);
    this->value = new char[this->length];
    sprintf(this->value, "%d", value);
}

String::String(const String& other) {
    this->length = other.length;
    this->value = new char[this->length];
    strcpy(this->value, other.value);
}

String::~String() {
    delete[] this->value;
}

String::operator const char*() const {
    return this->value;
}

std::ostream& operator<<(std::ostream& os, const String& string) {
    os << string.value;
    return os;
}

String& String::operator=(const String& other) {
    if(this == &other) {
        return *this;
    }
    delete[] this->value;
    this->length = other.length;
    this->value = new char[this->length];
    strcpy(this->value, other.value);
    return *this;
}

String& String::operator=(const char* other) {
    delete[] this->value;
    this->length = strlen(other);
    this->value = new char[this->length];
    strcpy(this->value, other);
    return *this;
}

String& String::operator+(const String& other) {
    char* newValue = new char[this->length + other.length];
    strcpy(newValue, this->value);
    strcat(newValue, other.value);
    delete[] this->value;
    this->value = newValue;
    this->length += other.length;
    return *this;
}

String& String::operator+(const char* other) {
    char* newValue = new char[this->length + strlen(other)];
    strcpy(newValue, this->value);
    strcat(newValue, other);
    delete[] this->value;
    this->value = newValue;
    this->length += strlen(other);
    return *this;
}

String& String::operator+=(const String& other) {
    return *this + other;
}

String& String::operator+=(const char* other) {
    return *this + other;
}

String& operator+(const char* lhs, String& rhs) {
    char* newValue = new char[strlen(lhs) + rhs.length];
    strcpy(newValue, lhs);
    strcat(newValue, rhs.value);
    delete[] rhs.value;
    rhs.value = newValue;
    rhs.length += strlen(lhs);
    return rhs;
}

String& operator+=(const char* lhs, String& rhs) {
    return lhs + rhs;
}

bool String::operator==(const String& other) const {
    return strcmp(this->value, other.value) == 0;
}

bool String::operator==(const char* other) const {
    return strcmp(this->value, other) == 0;
}

bool String::operator!=(const String& other) const {
    return strcmp(this->value, other.value) != 0;
}

bool String::operator!=(const char* other) const {
    return strcmp(this->value, other) != 0;
}

void String::serialize(Archive &a) const {
    a << this->length;
    for(size_t i = 0; i < this->length; i++) {
        a << this->value[i];
    }
}

void String::deserialize(Archive &a) {
    a >> &this->length;
    this->value = new char[this->length];
    for(size_t i = 0; i < this->length; i++) {
        a >> &this->value[i];
    }
}
