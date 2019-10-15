// iterator.hpp

#pragma once

#include <iterator>
#include <tuple>
#include <utility>

namespace extra {

// zip - zip n number of ranges (have a begin() and end() method)
// you can iterate though the zipped range, until the smallest range ends.
template <typename... Rs>
class _zip_impl {
public:
    class iterator : std::iterator<std::forward_iterator_tag,
        std::tuple<decltype(*std::begin(std::declval<Rs>()))...>> {
    private:
        using iter_t = std::tuple<decltype(std::begin(std::declval<Rs>()))...>;
        iter_t iters_;

        template<std::size_t... Is>
        constexpr auto dereference(std::index_sequence<Is...>) 
        const noexcept(noexcept(typename iterator::value_type{*std::get<Is>(iters_)...})) {
            return typename iterator::value_type{*std::get<Is>(iters_)...};
        }

        template<std::size_t... Is>
        constexpr void increment(std::index_sequence<Is...>) 
        noexcept(noexcept(((void)++std::get<Is>(iters_), ...))) {
            ((void)++std::get<Is>(iters_), ...);
        }

        template<std::size_t... Is>
        constexpr bool not_equal(iter_t const& a, iter_t const& b, std::index_sequence<Is...>) 
        const noexcept(noexcept(((std::get<Is>(a) != std::get<Is>(b)) && ...))) {
            return ((std::get<Is>(a) != std::get<Is>(b)) && ...);
        }

    public:
        constexpr explicit iterator(iter_t iters)
        noexcept(noexcept(iters_{std::move(iters)})) 
            : iters_{std::move(iters)} 
        {}

        constexpr auto operator*() const 
        noexcept(noexcept(dereference(std::index_sequence_for<Rs...>{}))) { 
            return dereference(std::index_sequence_for<Rs...>{}); 
        }

        constexpr iterator& operator++() 
        noexcept(noexcept(increment(std::index_sequence_for<Rs...>{})) && noexcept(*this)) {
            increment(std::index_sequence_for<Rs...>{});
            return *this;
        }

        constexpr iterator operator++(int) 
        noexcept(noexcept(increment(std::index_sequence_for<Rs...>{}))) {
            auto saved{*this};
            increment(std::index_sequence_for<Rs...>{});
            return saved;
        }

        constexpr bool operator!=(const iterator& other) 
        const noexcept(noexcept(not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
            return not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
        }
    };

    constexpr explicit _zip_impl(Rs&&... ranges)
    noexcept(noexcept(begin_{std::make_tuple(std::begin(ranges)...)}) 
    && noexcept(end_{std::make_tuple(std::end(ranges)...)}))
        : begin_{std::make_tuple(std::begin(ranges)...)}
        , end_{std::make_tuple(std::end(ranges)...)} 
    {}

    iterator begin() const noexcept { return begin_; }
    iterator end() const noexcept { return end_; }

private:
    iterator begin_;
    iterator end_;
};

template <typename... Rs>
constexpr auto zip(Rs&&... ranges) 
noexcept(noexcept(_zip_impl<Rs...>{std::forward<Rs>(ranges)...})) {
    return _zip_impl<Rs...>{std::forward<Rs>(ranges)...};
}

} // namespace extra