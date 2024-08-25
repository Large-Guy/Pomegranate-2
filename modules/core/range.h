#ifndef POMEGRANATEENGINE_RANGE_H
#define POMEGRANATEENGINE_RANGE_H
#include <iterator>
#include <vector>

class Range {
private:
    int _start;
    int _end;
    int _step;
public:
    Range(int end);
    Range(int start, int end);
    Range(int start, int end, int step);
    struct iterator {
        int current;
        int step;
        iterator(int start, int step) : current(start), step(step) {}
        iterator& operator++();
        bool operator!=(const iterator& other) const;
        int operator*() const;
    };
    explicit operator std::vector<int>() const;
    iterator begin() const;
    iterator end() const;
};

#endif //POMEGRANATEENGINE_RANGE_H
