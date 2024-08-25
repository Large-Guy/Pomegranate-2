#ifndef POMEGRANATEENGINE_LIST_H
#define POMEGRANATEENGINE_LIST_H
#include "serializable.h"

template <typename T>
struct List {
    T* elements;
    size_t count;
    size_t capacity;

    List() : elements(nullptr), count(0), capacity(0) {}
};


#endif //POMEGRANATEENGINE_LIST_H
