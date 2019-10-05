// algorithm.hpp

#pragma once

#include <algorithm>
#include <iterator>

namespace extra {

template<class ForwardIt, class T = typename ForwardIt::value_type>
constexpr auto min_unused(ForwardIt first, ForwardIt last, T value = {}) {
    while (last != first) {
        auto const half = (std::distance(first, last) + 1) / 2;
        auto const m = value + static_cast<T>(half);
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
constexpr auto min_unused(ExPo&& exec, ForwardIt first, ForwardIt last, T value = {}) {
    while (last != first) {
        auto const half = (std::distance(first, last) + 1) / 2;
        auto const m = value + static_cast<T>(half);
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

