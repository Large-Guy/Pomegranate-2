#ifndef POMEGRANATE_ENGINE_CORE_STRING_H
#define POMEGRANATE_ENGINE_CORE_STRING_H
#include "serializable.h"
#include "list.h"

//Implemented partially to avoid std::string's SSO optimization
class String : public Serializable {
private:
    char* _data;
    size_t _length;
public:
    String();
    String(const char* value);
    String(const char* value, size_t length);
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
    String operator+(const String& other);
    String operator+(const char* other);
    String& operator+=(const String& other);
    String& operator+=(const char* other);
    String& operator+(char other);
    String& operator+=(char other);
    friend String& operator+(const char* lhs, String& rhs);
    friend String& operator+=(const char* lhs, String& rhs);

    [[nodiscard]] bool operator==(const String& other) const;
    [[nodiscard]] bool operator==(const char* other) const;
    [[nodiscard]] bool operator!=(const String& other) const;
    [[nodiscard]] bool operator!=(const char* other) const;

    char& operator[](size_t index) const;

    List<String> split(char delimiter) const;
    String substring(size_t start, size_t end) const;
    char* data() const;
    void reserve(size_t size);
    size_t length() const;

    static String merge(const List<String>& strings, const String& delimiter = "");

    void serialize(Archive& a) const override;
    void deserialize(Archive& a) override;
};

template<>
struct std::hash<String> {
    size_t operator()(const String& string) const {
        return std::hash<std::string>()(string.data());
    }
};

#endif //POMEGRANATE_ENGINE_STRING_H
