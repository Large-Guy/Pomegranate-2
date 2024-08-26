#include "table.h"



Table::Table() {
    _data = {};
}

Table::~Table() {
    for (auto& [key, value] : _data) {
        delete value;
    }
}
