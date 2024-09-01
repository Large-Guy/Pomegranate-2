#ifndef POMEGRANATEENGINE_STRING_H
#define POMEGRANATEENGINE_STRING_H
#include "serializable.h"

//Implemented partially to avoid std::string's SSO optimization
class String : public Serializable {
public:
    char* value;
    size_t length;

    String();
    String(const char* value);
    String(const String& other);
    ~String();

    explicit operator const char*() const;
    friend std::ostream& operator<<(std::ostream& os, const String& string);
    String& operator=(const String& other);
    String& operator=(const char* other);
    String& operator+(const String& other);
    String& operator+(const char* other);
    String& operator+=(const String& other);
    String& operator+=(const char* other);

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};

template<>
struct std::hash<String> {
    size_t operator()(const String& string) const {
        return std::hash<std::string>()(string.value);
    }
};

#endif //POMEGRANATEENGINE_STRING_H
