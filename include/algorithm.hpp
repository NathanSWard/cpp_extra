// algorithm.hpp

#pragma once

#include <algorithm>
#include <iterator>

namespace extra {

// returns the minimum missing value from a range [first, last)
// e.g. (min_unused(1, 4, 3) == 2);
// thanks to Bean Deane for this function implementation
template<class ForwardIt, class T = typename ForwardIt::value_type>
constexpr T min_unused(ForwardIt first, ForwardIt last, T value = {}) {
    using diff_t = decltype(value - value);
    while (last != first) {
        auto const half = (std::distance(first, last) + 1) / 2;
        auto const m = value + static_cast<diff_t>(half);
        auto const p = std::partition(first, last, [&](auto&& x) { return std::less<>{}(x, m); });
        if (p == std::next(first, half)) {
            first= p;
            value = m; 
        } 
        else
            last = p;
    }
    return value;
}

template<class ExPo, class ForwardIt, class T = typename ForwardIt::value_type>
constexpr T min_unused(ExPo&& exec, ForwardIt first, ForwardIt last, T value = {}) {
    using diff_t = decltype(value - value);
    while (last != first) {
        auto const half = (std::distance(first, last) + 1) / 2;
        auto const m = value + static_cast<diff_t>(half);
        auto const p = std::partition(std::forward<ExPo>(exec), first, last, [&](auto&& x) { return std::less<>{}(x, m); });
        if (p == std::next(first, half)) {
            first= p;
            value = m; 
        } 
        else
            last = p;
    }
    return value;
}

}

