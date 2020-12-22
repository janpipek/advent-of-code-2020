#ifndef ADVENT_OF_CODE_DEBUG_HH
#define ADVENT_OF_CODE_DEBUG_HH

#include <ostream>
#include <deque>
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

template<typename T>
std::ostream &operator<<(std::ostream &out, const std::deque<T> &v) {
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

#ifdef AOC_DEBUG
    std::ostream& debug = std::cout;
#else
    class NullBuffer : public std::streambuf
    {
    public:
        int overflow(int c) { return c; }
    };

    NullBuffer null_buffer;

    std::ostream debug(&null_buffer);
#endif

#endif //ADVENT_OF_CODE_DEBUG_HH
