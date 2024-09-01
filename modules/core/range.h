#ifndef POMEGRANATE_ENGINE_CORE_RANGE_H
#define POMEGRANATE_ENGINE_CORE_RANGE_H
#include <iterator>
#include <vector>
#include "list.h"

class range {
private:
    int _start;
    int _end;
    int _step;
public:
    range(int end);
    range(int start, int end);
    range(int start, int end, int step);
    struct iterator {
        int current;
        int step;
        iterator(int start, int step) : current(start), step(step) {}
        iterator& operator++();
        bool operator!=(const iterator& other) const;
        int operator*() const;
    };
    operator std::vector<int>() const;
    operator List<int>() const;
    iterator begin() const;
    iterator end() const;
};

#endif //POMEGRANATE_ENGINE_RANGE_H
