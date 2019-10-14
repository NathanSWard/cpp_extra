// bit.hpp

#pragma once

#include <cstring>
#include <type_traits>

namespace extra {

// c++20 implementation of std::bit_cast
// currently not constexpr since this requies compiler intrinsics
template<class To, class From, 
    class = std::enable_if_t<
    (sizeof(To) == sizeof(From)) &&
    std::is_trivially_copyable_v<To> &&
    std::is_trivially_copyable_v<From>>>
/*constexpr*/ To bit_cast(From const& src) noexcept {
    To dst{};
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}

}