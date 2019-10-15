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

// zip
template <typename... T>
class _zip_impl {
public:
    class iterator : std::iterator<std::forward_iterator_tag,
        std::tuple<decltype(*std::begin(std::declval<T>()))...>> {
    private:
        std::tuple<decltype(std::begin(std::declval<T>()))...> iters_;

        template<std::size_t... I>
        constexpr auto deref(std::index_sequence<I...>) const {
            return typename iterator::value_type{*std::get<I>(iters_)...};
        }

        template<std::size_t... I>
        constexpr void increment(std::index_sequence<I...>) {
            ((void)++std::get<I>(iters_), ...);
        }

    public:
        constexpr explicit iterator(decltype(iters_) iters) 
            : iters_{std::move(iters)} 
        {}

        constexpr iterator& operator++() {
            increment(std::index_sequence_for<T...>{});
            return *this;
        }

        constexpr iterator operator++(int) {
            auto saved{*this};
            increment(std::index_sequence_for<T...>{});
            return saved;
        }

        constexpr bool operator!=(const iterator& other) const {
            return iters_ != other.iters_;
        }

        constexpr auto operator*() const { 
            return deref(std::index_sequence_for<T...>{}); 
        }
    };

    constexpr explicit _zip_impl(T&... seqs)
        : begin_{std::make_tuple(std::begin(seqs)...)}
        , end_{std::make_tuple(std::end(seqs)...)} 
    {}

    iterator begin() const { return begin_; }
    iterator end() const { return end_; }

private:
    iterator begin_;
    iterator end_;
};

template <typename... T>
constexpr auto zip(T&&... seqs) {
    return _zip_impl<T...>{std::forward<T>(seqs)...};
}

}

