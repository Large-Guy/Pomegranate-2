#include "range.h"

Range::Range(int end) {
    _start = 0;
    _end = end;
    _step = 1;
}

Range::Range(int start, int end) {
    _start = start;
    _end = end;
    _step = 1;
}

Range::Range(int start, int end, int step) {
    _start = start;
    _end = end;
    _step = step;
}

Range::iterator& Range::iterator::operator++() {
    current += step;
    return *this;
}

bool Range::iterator::operator!=(const Range::iterator& other) const {
    return current != other.current;
}

int Range::iterator::operator*() const {
    return current;
}

Range::operator std::vector<int>() const {
    int size = (_end - _start + _step - 1) / _step; // Adjust size calculation for rounding up
    std::vector<int> result(size);
    int index = 0;
    for (int i = _start; i < _end; i += _step) {
        result[index] = i;
        index++;
    }
    return result;
}


Range::iterator Range::begin() const {
    return iterator(_start, _step);
}

Range::iterator Range::end() const {
    return iterator(_end, _step);
}