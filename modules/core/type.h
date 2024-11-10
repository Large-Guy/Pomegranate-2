#ifndef ANYFUNCTIONS_TYPE_H
#define ANYFUNCTIONS_TYPE_H
#include <typeinfo>
//Stuff for demangling
#include <cxxabi.h>

struct TypeInfo {
    size_t size;
    size_t type;
    const char* name;
    const char* mangledName;

    TypeInfo();

    TypeInfo(const TypeInfo& other);

    template<typename T>
    TypeInfo(T& t) {
        size = sizeof(T);
        type = typeid(T).hash_code();
        name = abi::__cxa_demangle(typeid(T).name(),nullptr,nullptr,nullptr);
        mangledName = typeid(T).name();
    }

    template<typename T>
    static TypeInfo get() {
        TypeInfo t{};
        t.size = sizeof(T);
        t.type = typeid(T).hash_code();
        t.name = abi::__cxa_demangle(typeid(T).name(),nullptr,nullptr,nullptr);
        t.mangledName = typeid(T).name();
        return t;
    }

    template<>
    static TypeInfo get<void>() {
        TypeInfo t{};
        t.size = 0;
        t.type = typeid(void).hash_code();
        t.name = "void";
        t.mangledName = typeid(void).name();
        return t;
    }

    template<typename T>
    static TypeInfo get(T& t) {
        TypeInfo ti{};
        ti.size = sizeof(T);
        ti.type = typeid(T).hash_code();
        ti.name = abi::__cxa_demangle(typeid(T).name(),nullptr,nullptr,nullptr);
        ti.mangledName = typeid(T).name();
        return ti;
    }

    template<typename T>
    TypeInfo& operator=(T& t) {
        size = sizeof(T);
        type = typeid(T).hash_code();
        name = abi::__cxa_demangle(typeid(T).name(),nullptr,nullptr,nullptr);
        mangledName = typeid(T).name();
        return *this;
    }


    bool operator==(const TypeInfo& other) const;
};



#endif //ANYFUNCTIONS_TYPE_H
