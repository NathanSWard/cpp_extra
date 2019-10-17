// iterator.hpp

#pragma once

#include <iterator>
#include <tuple>
#include <utility>

namespace extra {

// zip - zip n number of ranges (have a begin() and end() method)
// czip - zip n number of ranges (have a cbegin() and cend() method)
// rzip - zip n number of ranges (have a rbegin() and rend() method)
// crzip - zip n number of ranges (have a crbegin() and crend() method)
// you can iterate though the zipped range, until the smallest range ends.
namespace detail {

template<class ItA, class ItB, std::size_t... Is>
inline constexpr bool not_equal(ItA const& a, ItB const& b, std::index_sequence<Is...>) 
noexcept(noexcept(((std::get<Is>(a) != std::get<Is>(b)) && ...))) {
return ((std::get<Is>(a) != std::get<Is>(b)) && ...);
}

template<class ItA, class ItB, std::size_t... Is>
inline constexpr bool equal(ItA const& a, ItB const& b, std::index_sequence<Is...>) 
noexcept(noexcept(((std::get<Is>(a) == std::get<Is>(b)) || ...))) {
return ((std::get<Is>(a) == std::get<Is>(b)) || ...);
}

template<class It, std::size_t... Is>
inline constexpr auto dereference(It&& iters, std::index_sequence<Is...>) 
noexcept(noexcept(std::forward_as_tuple(*std::get<Is>(std::forward<It>(iters))...))) {
    return std::forward_as_tuple(*std::get<Is>(std::forward<It>(iters))...);
}

template<class It, std::size_t... Is>
inline constexpr void increment(It&& iters, std::index_sequence<Is...>) 
noexcept(noexcept(((void)++std::get<Is>(std::forward<It>(iters)), ...))) {
    ((void)++std::get<Is>(std::forward<It>(iters)), ...);
}

template<class... Rs> class sentinel;
template<class... Rs> class iterator;
template<class... Rs> class const_sentinel;
template<class... Rs> class const_iterator;
template<class... Rs> class r_sentinel;
template<class... Rs> class r_iterator;
template<class... Rs> class const_r_sentinel;
template<class... Rs> class const_r_iterator;


template<class... Rs>
class sentinel {
private:
    friend class iterator<Rs...>;
    using iter_t = std::tuple<decltype(std::end(std::declval<Rs>())) const...>;
    iter_t const iters_;

public:
    constexpr explicit sentinel(iter_t iters)
    noexcept(noexcept(iter_t{std::move(iters)})) 
        : iters_{std::move(iters)} 
    {}

    template<class It>
    constexpr bool operator!=(It const& other) 
    const noexcept(noexcept(iter_not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
        return not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
    }

    template<class It>
    constexpr bool operator==(It const& other)
    const noexcept(noexcept(iter_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
        return equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
    }
};

template<class... Rs>
class iterator {
    friend class sentinel<Rs...>;
    using iter_t = std::tuple<decltype(std::begin(std::declval<Rs>()))...>;
    iter_t iters_;

public:
    using value_type = std::tuple<decltype(*std::begin(std::declval<Rs>()))...>;
    using iterator_category = std::forward_iterator_tag;

    constexpr explicit iterator(iter_t iters)
    noexcept(noexcept(iter_t{std::move(iters)})) 
        : iters_{std::move(iters)} 
    {}

    constexpr auto operator*() 
    noexcept(noexcept(dereference(iters_, std::index_sequence_for<Rs...>{}))) { 
        return dereference(iters_, std::index_sequence_for<Rs...>{}); 
    }

    constexpr iterator& operator++() 
    noexcept(noexcept(increment(iters_, std::index_sequence_for<Rs...>{}))) {
        increment(iters_, std::index_sequence_for<Rs...>{});
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
    const noexcept(noexcept(not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
        return not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
    }

    template<class It>
    constexpr bool operator==(It const& other)
    const noexcept(noexcept(iter_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
        return iter_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
    }
};

template<class... Rs>
class const_sentinel {
private:
    friend class const_iterator<Rs...>;
    using iter_t = std::tuple<decltype(std::cend(std::declval<Rs>())) const...>;
    iter_t const iters_;

public:
    constexpr explicit const_sentinel(iter_t iters)
    noexcept(noexcept(iter_t{std::move(iters)})) 
        : iters_{std::move(iters)} 
    {}

    template<class It>
    constexpr bool operator!=(It const& other) 
    const noexcept(noexcept(iter_not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
        return not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
    }

    template<class It>
    constexpr bool operator==(It const& other)
    const noexcept(noexcept(iter_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
        return equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
    }
};

template<class... Rs>
class const_iterator {
    friend class const_sentinel<Rs...>;
    using iter_t = std::tuple<decltype(std::cbegin(std::declval<Rs>()))...>;
    iter_t iters_;

public:
    using value_type = std::tuple<decltype(std::cbegin(std::declval<Rs>()))...>;
    using iterator_category = std::forward_iterator_tag;

    constexpr explicit const_iterator(iter_t iters)
    noexcept(noexcept(iter_t{std::move(iters)})) 
        : iters_{std::move(iters)} 
    {}

    constexpr auto operator*() 
    const noexcept(noexcept(dereference(iters_, std::index_sequence_for<Rs...>{}))) { 
        return dereference(iters_, std::index_sequence_for<Rs...>{}); 
    }

    constexpr const_iterator& operator++() 
    noexcept(noexcept(increment(iters_, std::index_sequence_for<Rs...>{}))) {
        increment(iters_, std::index_sequence_for<Rs...>{});
        return *this;
    }

    constexpr const_iterator operator++(int) 
    noexcept(noexcept(increment(iters_, std::index_sequence_for<Rs...>{}))) {
        auto saved{*this};
        increment(iters_, std::index_sequence_for<Rs...>{});
        return saved;
    }

    template<class It>
    constexpr bool operator!=(It const& other) 
    const noexcept(noexcept(not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
        return not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
    }

    template<class It>
    constexpr bool operator==(It const& other)
    const noexcept(noexcept(equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
        return equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
    }
};

template<class... Rs>
class r_sentinel {
private:
    friend class r_iterator<Rs...>;
    using iter_t = std::tuple<decltype(std::rend(std::declval<Rs>())) const...>;
    iter_t const iters_;

public:
    constexpr explicit r_sentinel(iter_t iters)
    noexcept(noexcept(iter_t{std::move(iters)})) 
        : iters_{std::move(iters)} 
    {}

    template<class It>
    constexpr bool operator!=(It const& other) 
    const noexcept(noexcept(iter_not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
        return not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
    }

    template<class It>
    constexpr bool operator==(It const& other)
    const noexcept(noexcept(iter_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
        return equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
    }
};

template<class... Rs>
class r_iterator {
    friend class r_sentinel<Rs...>;
    using iter_t = std::tuple<decltype(std::rbegin(std::declval<Rs>()))...>;
    iter_t iters_;

public:    
    using value_type = std::tuple<decltype(*std::rbegin(std::declval<Rs>()))...>;
    using iterator_category = std::forward_iterator_tag;

    constexpr explicit r_iterator(iter_t iters)
    noexcept(noexcept(iter_t{std::move(iters)})) 
        : iters_{std::move(iters)} 
    {}

    constexpr auto operator*() 
    noexcept(noexcept(dereference(iters_, std::index_sequence_for<Rs...>{}))) { 
        return dereference(iters_, std::index_sequence_for<Rs...>{}); 
    }

    constexpr r_iterator& operator++() 
    noexcept(noexcept(increment(iters_, std::index_sequence_for<Rs...>{}))) {
        increment(iters_, std::index_sequence_for<Rs...>{});
        return *this;
    }

    constexpr r_iterator operator++(int) 
    noexcept(noexcept(increment(std::index_sequence_for<Rs...>{}))) {
        auto saved{*this};
        increment(std::index_sequence_for<Rs...>{});
        return saved;
    }

    template<class It>
    constexpr bool operator!=(It const& other) 
    const noexcept(noexcept(not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
        return not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
    }

    template<class It>
    constexpr bool operator==(It const& other)
    const noexcept(noexcept(iter_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
        return iter_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
    }
};

template<class... Rs>
class const_r_sentinel {
private:
    friend class const_r_iterator<Rs...>;
    using iter_t = std::tuple<decltype(std::crend(std::declval<Rs>())) const...>;
    iter_t const iters_;

public:
    constexpr explicit const_r_sentinel(iter_t iters)
    noexcept(noexcept(iter_t{std::move(iters)})) 
        : iters_{std::move(iters)} 
    {}

    template<class It>
    constexpr bool operator!=(It const& other) 
    const noexcept(noexcept(iter_not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
        return not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
    }

    template<class It>
    constexpr bool operator==(It const& other)
    const noexcept(noexcept(iter_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
        return equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
    }
};

template<class... Rs>
class const_r_iterator {
    friend class const_r_sentinel<Rs...>;
    using iter_t = std::tuple<decltype(std::crbegin(std::declval<Rs>()))...>;
    iter_t iters_;

public:
    using value_type = std::tuple<decltype(*std::crbegin(std::declval<Rs>()))...>;
    using iterator_category = std::forward_iterator_tag;

    constexpr explicit const_r_iterator(iter_t iters)
    noexcept(noexcept(iter_t{std::move(iters)})) 
        : iters_{std::move(iters)} 
    {}

    constexpr auto operator*() 
    const noexcept(noexcept(dereference(iters_, std::index_sequence_for<Rs...>{}))) { 
        return dereference(iters_, std::index_sequence_for<Rs...>{}); 
    }

    constexpr const_r_iterator& operator++() 
    noexcept(noexcept(increment(iters_, std::index_sequence_for<Rs...>{}))) {
        increment(iters_, std::index_sequence_for<Rs...>{});
        return *this;
    }

    constexpr const_r_iterator operator++(int) 
    noexcept(noexcept(increment(std::index_sequence_for<Rs...>{}))) {
        auto saved{*this};
        increment(std::index_sequence_for<Rs...>{});
        return saved;
    }

    template<class It>
    constexpr bool operator!=(It const& other) 
    const noexcept(noexcept(not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
        return not_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
    }

    template<class It>
    constexpr bool operator==(It const& other)
    const noexcept(noexcept(iter_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{}))) {
        return iter_equal(iters_, other.iters_, std::index_sequence_for<Rs...>{});
    }
};

} // namespace detail

template <typename... Rs>
class _zip_impl {
private:
    using iter_t = detail::iterator<Rs...>;
    using sentinel_t = detail::iterator<Rs...>;

public:
    constexpr explicit _zip_impl(Rs&&... ranges)
    noexcept(noexcept(iter_t{std::make_tuple(std::begin(std::forward<Rs>(ranges))...)}) 
    && noexcept(iter_t{std::make_tuple(std::end(std::forward<Rs>(ranges))...)}))
        : begin_{std::make_tuple(std::begin(std::forward<Rs>(ranges))...)}
        , end_{std::make_tuple(std::end(std::forward<Rs>(ranges))...)} 
    {}

    constexpr iter_t begin() const noexcept { return begin_; }
    constexpr sentinel_t end() const noexcept { return end_; }

private:
    iter_t              begin_;
    sentinel_t const    end_;
};

template <typename... Rs>
class _czip_impl {
private:
    using citer_t = detail::const_iterator<Rs...>;
    using csentinel_t = detail::const_sentinel<Rs...>;

public:
    constexpr explicit _czip_impl(Rs&&... ranges)
    noexcept(noexcept(citer_t{std::make_tuple(std::cbegin(std::forward<Rs>(ranges))...)}) 
    && noexcept(csentinel_t{std::make_tuple(std::cend(std::forward<Rs>(ranges))...)}))
        : begin_{std::make_tuple(std::cbegin(std::forward<Rs>(ranges))...)}
        , end_{std::make_tuple(std::cend(std::forward<Rs>(ranges))...)} 
    {}

    constexpr citer_t begin() const noexcept { return begin_; }
    constexpr csentinel_t end() const noexcept { return end_; }

private:
    citer_t             begin_;
    csentinel_t const   end_;
};

template <typename... Rs>
class _rzip_impl {
private:
    using riter_t = detail::r_iterator<Rs...>;
    using rsentinel_t = detail::r_sentinel<Rs...>;

public:
    constexpr explicit _rzip_impl(Rs&&... ranges)
    noexcept(noexcept(riter_t{std::make_tuple(std::rbegin(std::forward<Rs>(ranges))...)}) 
    && noexcept(rsentinel_t{std::make_tuple(std::rend(std::forward<Rs>(ranges))...)}))
        : begin_{std::make_tuple(std::rbegin(std::forward<Rs>(ranges))...)}
        , end_{std::make_tuple(std::rend(std::forward<Rs>(ranges))...)} 
    {}

    constexpr riter_t begin() const noexcept { return begin_; }
    constexpr rsentinel_t end() const noexcept { return end_; }

private:
    riter_t             begin_;
    rsentinel_t const   end_;
};

template <typename... Rs>
class _crzip_impl {
private:
    using criter_t = detail::const_r_iterator<Rs...>;
    using crsentinel_t = detail::const_r_sentinel<Rs...>;

public:
    constexpr explicit _crzip_impl(Rs&&... ranges)
    noexcept(noexcept(criter_t{std::make_tuple(std::crbegin(std::forward<Rs>(ranges))...)}) 
    && noexcept(crsentinel_t{std::make_tuple(std::crend(std::forward<Rs>(ranges))...)}))
        : begin_{std::make_tuple(std::crbegin(std::forward<Rs>(ranges))...)}
        , end_{std::make_tuple(std::crend(std::forward<Rs>(ranges))...)} 
    {}

    constexpr criter_t begin() const noexcept { return begin_; }
    constexpr crsentinel_t end() const noexcept { return end_; }

private:
    criter_t            begin_;
    crsentinel_t const  end_;
};

template <typename... Rs>
constexpr auto zip(Rs&&... ranges) 
noexcept(noexcept(_zip_impl<Rs...>{std::forward<Rs>(ranges)...})) {
    return _zip_impl<Rs...>{std::forward<Rs>(ranges)...};
}

template <typename... Rs>
constexpr auto czip(Rs&&... ranges) 
noexcept(noexcept(_czip_impl<Rs...>{std::forward<Rs>(ranges)...})) {
    return _czip_impl<Rs...>{std::forward<Rs>(ranges)...};
}

template <typename... Rs>
constexpr auto rzip(Rs&&... ranges) 
noexcept(noexcept(_rzip_impl<Rs...>{std::forward<Rs>(ranges)...})) {
    return _rzip_impl<Rs...>{std::forward<Rs>(ranges)...};
}

template <typename... Rs>
constexpr auto crzip(Rs&&... ranges) 
noexcept(noexcept(_crzip_impl<Rs...>{std::forward<Rs>(ranges)...})) {
    return _crzip_impl<Rs...>{std::forward<Rs>(ranges)...};
}

} // namespace extra
