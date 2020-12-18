#ifndef ADVENT_OF_CODE_CONVERSIONS_HH
#define ADVENT_OF_CODE_CONVERSIONS_HH

#include <algorithm>
#include <iterator>
#include <vector>

/**
 * Convert vector from one type to another.
 *
 * @tparam T1
 * @tparam T2
 * @param input
 * @return
 */
template<typename T1, typename T2>
std::vector<T2> asType(const std::vector<T1> &input) {
    std::vector<T2> output;
    std::transform(input.cbegin(), input.cend(), std::back_inserter(output), [](const T1 &x) { return (T2) x; });
    return output;
}

#endif //ADVENT_OF_CODE_CONVERSIONS_HH
