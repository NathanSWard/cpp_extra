// algorithm.hpp

#pragma once

#include <algorithm>
#include <iterator>
#include <tuple>
#include <utility>

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

// apply a binary op to each adjacent pair in a range O(n)
template<class FwIter, class BinOp>
constexpr void _adjacent_pair_impl(FwIter first, FwIter const last, BinOp bin_op, std::forward_iterator_tag) {
    if (first != last) {
        FwIter trailer = first++;
        for (; first != last; ++first, ++trailer)
            bin_op(*trailer, *first);
    }
}

template<class FwIter, class BinOp>
constexpr void _adjacent_pair_impl(FwIter first, FwIter const last, BinOp bin_op, std::input_iterator_tag) {
    if (first != last) {
        auto trailer = *first;
        for (; first != last; ++first) {
            bin_op(trailer, *first);
            trailer = *first;
        }
    }
}

template<class It, class BinOp>
constexpr void adjacent_pair(It const first, It const last, BinOp bin_op) {
    return _adjacent_pair_impl(first, last, bin_op, 
        typename std::iterator_traits<It>::iterator_category{});
}

// apply a binary op to every pair in a range O(n^2)
template<class FwIter, class BinOp>
constexpr void _for_every_pair_impl(FwIter first, FwIter const last, BinOp bin_op, std::forward_iterator_tag) {
    if (first != last) {
        FwIter trailer = first;
        ++first;
        for (; first != last, ++first, ++trailer)
            for (FwIter it = first; it != last; ++it)
                bin_op(*trailer, *it);
    }
}

template<class FwIter, class BinOp>
constexpr void _for_every_pair_impl(FwIter first, FwIter const last, BinOp bin_op, std::input_iterator_tag) {
    if (first != last) {
        auto trailer = *first;
        ++first;
        for (; first != last, ++first, ++trailer) {
            for (FwIter it = first; it != last; ++it)
                bin_op(trailer, *it);
            trailer = *first;
        }
    }
}

template<class It, class BinOp>
constexpr void for_every_pair(It const first, It const last, BinOp bin_op) {
    return _for_every_pair_impl(first, last, bin_op, 
        typename std::iterator_traits<It>::iterator_category{});
}

// copy a range until the end or the predicate return false
template<class InIter, class OutIter, class Pred>
constexpr auto copy_while(InIter first, InIter const last, OutIter result, Pred p) {
    for (; first != last && p(*first); ++first)
        *result++ = *first;
    return std::make_pair(first, result);
}

// split a range on a value T and call bin_op 
// on the range between the previous and next T
template<class InIter, class T, class BinOp>
constexpr void split(InIter first, InIter const last, T const& t, BinOp bin_op) {
    for (;;) {
        InIter found = std::find(first, last, t);
        bin_op(first, found);
        if (found == last)
            break;
        first = ++found;
    }
}

template<class ExPo, class InIter, class T, class BinOp>
constexpr void split(ExPo&& policy, InIter first, InIter const last, T const& t, BinOp bin_op) {
    for (;;) {
        InIter found = std::find(std::forward<ExPo>(policy), first, last, t);
        bin_op(first, found);
        if (found == last)
            break;
        first = ++found;
    }
}

} // namespace extra
