#ifndef POMEGRANATE_ENGINE_CORE_STRING_H
#define POMEGRANATE_ENGINE_CORE_STRING_H
#include "serializable.h"

//Implemented partially to avoid std::string's SSO optimization
class String : public Serializable {
public:
    char* value;
    size_t length;

    String();
    String(const char* value);
    String(int value);
    String(float value);
    String(double value);
    String(bool value);
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
    friend String& operator+(const char* lhs, String& rhs);
    friend String& operator+=(const char* lhs, String& rhs);

    [[nodiscard]] bool operator==(const String& other) const;
    [[nodiscard]] bool operator==(const char* other) const;
    [[nodiscard]] bool operator!=(const String& other) const;
    [[nodiscard]] bool operator!=(const char* other) const;

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};

template<>
struct std::hash<String> {
    size_t operator()(const String& string) const {
        return std::hash<std::string>()(string.value);
    }
};

#endif //POMEGRANATE_ENGINE_STRING_H
