#include "range.h"

range::range(int end) {
    _start = 0;
    _end = end;
    _step = 1;
}

range::range(int start, int end) {
    _start = start;
    _end = end;
    _step = 1;
}

range::range(int start, int end, int step) {
    _start = start;
    _end = end;
    _step = step;
}

range::iterator& range::iterator::operator++() {
    current += step;
    return *this;
}

bool range::iterator::operator!=(const range::iterator& other) const {
    return current != other.current;
}

int range::iterator::operator*() const {
    return current;
}

range::operator std::vector<int>() const {
    int size = (_end - _start + _step - 1) / _step; // Adjust size calculation for rounding up
    std::vector<int> result(size);
    int index = 0;
    for (int i = _start; i < _end; i += _step) {
        result[index] = i;
        index++;
    }
    return result;
}

range::operator List<int>() const {
    List<int> result = List<int>();
    result.resize((_end - _start + _step - 1) / _step); // Adjust size calculation for rounding up
    int index = 0;
    for (int i = _start; i < _end; i += _step) {
        result[index] = i;
        index++;
    }
    return result;
}


range::iterator range::begin() const {
    return iterator(_start, _step);
}

range::iterator range::end() const {
    return iterator(_end, _step);
}