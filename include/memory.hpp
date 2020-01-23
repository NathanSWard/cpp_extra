// memory.hpp

#pragma once

#include <memory>
#include <type_traits>

namespace extra {

namespace detail {

template<class Ptr>
struct is_smart_ptr {

    std::false_type op_star(...);
    static decltype(*std::declval<Ptr const&>(), std::true_type{}) op_star(int);

    std::false_type op_arrow(...);
    static decltype(std::declval<Ptr const&>().operator->(), std::true_type{}) op_arrow(int);

    std::false_type op_get(...);
    static decltype(std::declval<Ptr const&>().get(), std::true_type{}) op_get(int);

    static constexpr bool value = std::conjunction_v<decltype(op_star(0)), decltype(op_arrow(0)), decltype(op_get(0))>;
};

template<class Ptr>
static constexpr bool is_smart_ptr_v = is_smart_ptr<Ptr>::value;

}

template<class T>
class ptr {
    T* ptr_;
public:
    using pointer = T*;
    using element_type = T;

    constexpr ptr() noexcept : ptr_(nullptr) {}
    constexpr ptr(std::nullptr_t) noexcept : ptr_(nullptr) {}
    constexpr ptr(T* const p) noexcept : ptr_(p) {}

    template<class U, std::enable_if_t<std::is_convertible_v<U*, T*>, int> = 0>
    constexpr ptr(U* const p) noexcept : ptr_(p) {}
    
    template<class P, std::enable_if_t<detail::is_smart_ptr_v<P>, int> = 0>
    constexpr ptr(P const& p) noexcept : ptr_(p.get()) {}

    ptr(ptr const&) = default;
    ptr(ptr&&) = default;
    ptr& operator=(ptr const&) = default;
    ptr& operator=(ptr&&) = default;

    constexpr void reset(T* const ptr = nullptr) noexcept { ptr_ = ptr; }
    constexpr void swap(ptr& p) noexcept { std::swap(ptr_, p.ptr_); }

    constexpr T* get() const noexcept { return ptr_; }
    constexpr explicit operator bool() const noexcept { return ptr_ != nullptr; }

    constexpr T& operator*() const noexcept { return *ptr_; }
    constexpr T* operator->() const noexcept { return ptr_; }


    friend constexpr bool operator==(ptr const& lhs, ptr const& rhs) noexcept {
        return lhs.ptr_ == rhs.ptr_;
    }

    friend constexpr bool operator!=(ptr const& lhs, ptr const& rhs) noexcept {
        return !(lhs == rhs);
    }

    friend constexpr bool operator<(ptr const& lhs, ptr const& rhs) noexcept {
        return std::less<>{}(lhs.ptr, rhs.ptr);
    }

    friend constexpr bool operator>(ptr const& lhs, ptr const& rhs) noexcept {
        return rhs < lhs;
    }

    friend constexpr bool operator<=(ptr const& lhs, ptr const& rhs) noexcept {
        return !(rhs < lhs);
    }

    friend constexpr bool operator>=(ptr const& lhs, ptr const& rhs) noexcept {
        return rhs <= lhs;
    }
};

}