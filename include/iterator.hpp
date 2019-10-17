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
private:
    template<class ItA, class ItB, std::size_t... Is>
    static constexpr bool iter_not_equal(ItA const& a, ItB const& b, std::index_sequence<Is...>) 
    noexcept(noexcept(((std::get<Is>(a) != std::get<Is>(b)) && ...))) {
        return ((std::get<Is>(a) != std::get<Is>(b)) && ...);
    }

    template<class ItA, class ItB, std::size_t... Is>
    static constexpr bool iter_equal(ItA const& a, ItB const& b, std::index_sequence<Is...>) 
    noexcept(noexcept(((std::get<Is>(a) == std::get<Is>(b)) || ...))) {
        return ((std::get<Is>(a) == std::get<Is>(b)) || ...);
    }
public:
    class sentinel;
    class iterator {
    public:
        using value_type = std::tuple<decltype(*std::begin(std::declval<Rs>()))...>;
        using iterator_category = std::forward_iterator_tag;
    private:
        friend class sentinel;
        using iter_t = std::tuple<decltype(std::begin(std::declval<Rs>()))...>;
        iter_t iters_;

        template<std::size_t... Is>
        constexpr auto dereference(std::index_sequence<Is...>) 
        const noexcept(noexcept(value_type{*std::get<Is>(iters_)...})) {
            return value_type{*std::get<Is>(iters_)...};
        }

        template<std::size_t... Is>
        constexpr void increment(std::index_sequence<Is...>) 
        noexcept(noexcept(((void)++std::get<Is>(iters_), ...))) {
            ((void)++std::get<Is>(iters_), ...);
        }

    public:
        constexpr explicit iterator(iter_t iters)
        noexcept(noexcept(iter_t{std::move(iters)})) 
            : iters_{std::move(iters)} 
        {}

        constexpr auto operator*() 
        const noexcept(noexcept(dereference(std::index_sequence_for<Rs...>{}))) { 
            return dereference(std::index_sequence_for<Rs...>{}); 
        }

        constexpr iterator& operator++() 
        noexcept(noexcept(increment(std::index_sequence_for<Rs...>{}))) {
            increment(std::index_sequence_for<Rs...>{});
            return *this;
        }

        constexpr iterator operator++(int) 
        noexcept(noexcept(increment(std::index_sequence_for<Rs...>{}))) {
            auto saved{*this};
            increment(std::index_sequence_for<Rs...>{});
            return saved;
        }

        template<class It>
        constexpr bool operator!=(It const& other) 
        const noexcept(noexcept(iter_not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
            return iter_not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
        }

        template<class It>
        constexpr bool operator==(It const& other)
        const noexcept(noexcept(iter_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
            return iter_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
        }
    };

    class sentinel {
    private:
        friend class iterator;
        using iter_t = std::tuple<decltype(std::end(std::declval<Rs>()))...>;
        iter_t const iters_;

    public:
        constexpr explicit sentinel(iter_t iters)
        noexcept(noexcept(iter_t{std::move(iters)})) 
            : iters_{std::move(iters)} 
        {}

        template<class It>
        constexpr bool operator!=(It const& other) 
        const noexcept(noexcept(iter_not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
            return iter_not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
        }

        template<class It>
        constexpr bool operator==(It const& other)
        const noexcept(noexcept(iter_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
            return iter_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
        }
    };

    constexpr explicit _zip_impl(Rs&&... ranges)
    noexcept(noexcept(iterator{std::make_tuple(std::begin(std::forward<Rs>(ranges))...)}) 
    && noexcept(iterator{std::make_tuple(std::end(std::forward<Rs>(ranges))...)}))
        : begin_{std::make_tuple(std::begin(std::forward<Rs>(ranges))...)}
        , end_{std::make_tuple(std::end(std::forward<Rs>(ranges))...)} 
    {}

    constexpr iterator begin() const noexcept { return begin_; }
    constexpr sentinel end() const noexcept { return end_; }

private:
    iterator        begin_;
    sentinel const  end_;
};

template <typename... Rs>
constexpr auto zip(Rs&&... ranges) 
noexcept(noexcept(_zip_impl<Rs...>{std::forward<Rs>(ranges)...})) {
    return _zip_impl<Rs...>{std::forward<Rs>(ranges)...};
}

} // namespace extra
