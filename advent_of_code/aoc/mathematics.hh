#ifndef ADVENT_OF_CODE_MATHEMATICS_HH
#define ADVENT_OF_CODE_MATHEMATICS_HH

#include <vector>
#include <numeric>

template<typename T>
T product(const std::vector<T> &numbers) {
    return std::accumulate(numbers.cbegin(), numbers.cend(), (T) 1, std::multiplies<T>());
}

#endif //ADVENT_OF_CODE_MATHEMATICS_HH
