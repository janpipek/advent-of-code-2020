#ifndef ADVENT_OF_CODE_DEBUG_HH
#define ADVENT_OF_CODE_DEBUG_HH

#include <ostream>
#include <vector>

template<typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
    out << "{";
    for (int i = 0; i < v.size(); i++) {
        out << v[i];
        if (i != v.size() - 1) {
            out << ", ";
        }
    }
    out << "}";
    return out;
}

#endif //ADVENT_OF_CODE_DEBUG_HH
