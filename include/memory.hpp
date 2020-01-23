// memory.hpp

#pragma once

#include <memory>
#include <type_traits>

#ifdef NON_NULL_PTR_DEBUG
    #include <cassert>
#endif // NON_NULL_PTR_DEBUG

namespace extra {

namespace detail {

template<class Ptr, bool = std::is_class_v<Ptr>>
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
struct is_smart_ptr<Ptr, false> : std::false_type {};

template<class Ptr>
static constexpr bool is_smart_ptr_v = is_smart_ptr<Ptr>::value;

} // namespace detail

template<class T>
class ptr {
    T* ptr_;
public:
    static_assert(!std::is_reference_v<T>, "extra::ptr of reference type is ill-formed");

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

    constexpr ptr& operator=(std::nullptr_t) noexcept { 
        ptr_ = nullptr; 
        return *this;    
    }
    
    constexpr ptr& operator=(T* const p) noexcept {
        ptr_ = p;
        return *this;
    }

    template<class U, std::enable_if_t<std::is_convertible_v<U*, T*>, int> = 0>
    constexpr ptr& operator=(U* const p) noexcept {
        ptr_ = p;
        return *this;
    }
    
    template<class P, std::enable_if_t<detail::is_smart_ptr_v<P>, int> = 0>
    constexpr ptr& operator=(P const& p) noexcept {
        ptr_ = p.get();
        return *this;
    }

    ptr& operator=(ptr const&) = default;
    ptr& operator=(ptr&&) = default;

    constexpr void reset(T* const ptr = nullptr) noexcept { ptr_ = ptr; }
    constexpr void swap(ptr& p) noexcept { std::swap(ptr_, p.ptr_); }

    [[nodiscard]] constexpr T* get() const noexcept { return ptr_; }
    [[nodiscard]] constexpr explicit operator bool() const noexcept { return ptr_ != nullptr; }

    [[nodiscard]] constexpr T& operator*() const noexcept { return *ptr_; }
    [[nodiscard]] constexpr T* operator->() const noexcept { return ptr_; }

    [[nodiscard]] constexpr explicit operator T*() const noexcept { return ptr_; }

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

#ifdef NON_NULL_PTR_DEBUG
    template<class T>
    inline void non_null_ptr_check(T* const t) {
        assert(t != nullptr);
    }
#else // NON_NULL_PTR_DEBUG
    #define non_null_ptr_check(...) static_cast<void>(0)
#endif // NON_NULL_PTR_DEBUG

template<class T>
class non_null_ptr {
    T* ptr_;
public:
    static_assert(!std::is_reference_v<T>, "extra::non_null_ptr of reference type is ill-formed");

    using pointer = T*;
    using element_type = T;

    non_null_ptr() = delete;
    non_null_ptr(std::nullptr_t) = delete;

    constexpr non_null_ptr(T* const p) noexcept 
        : ptr_(p) 
    {
        non_null_ptr_check(ptr_);
    }

    template<class U, std::enable_if_t<std::is_convertible_v<U*, T*>, int> = 0>
    constexpr non_null_ptr(U* const p) noexcept 
        : ptr_(p) 
    {
        non_null_ptr_check(ptr_);
    }
    
    template<class P, std::enable_if_t<detail::is_smart_ptr_v<P>, int> = 0>
    constexpr non_null_ptr(P const& p) noexcept 
        : ptr_(p.get()) 
    {
        non_null_ptr_check(ptr_);
    }

    non_null_ptr(non_null_ptr const&) = default;
    non_null_ptr(non_null_ptr&&) = default;

    non_null_ptr& operator=(std::nullptr_t) = delete;
    
    constexpr non_null_ptr& operator=(T* const p) noexcept {
        ptr_ = p;
        non_null_ptr_check(ptr_);
        return *this;
    }

    template<class U, std::enable_if_t<std::is_convertible_v<U*, T*>, int> = 0>
    constexpr non_null_ptr& operator=(U* const p) noexcept {
        ptr_ = p;
        non_null_ptr_check(ptr_);
        return *this;
    }
    
    template<class P, std::enable_if_t<detail::is_smart_ptr_v<P>, int> = 0>
    constexpr non_null_ptr& operator=(P const& p) noexcept {
        ptr_ = p.get();
        non_null_ptr_check(ptr_);
        return *this;
    }

    non_null_ptr& operator=(non_null_ptr const&) = default;
    non_null_ptr& operator=(non_null_ptr&&) = default;

    void reset(std::nullptr_t) = delete;

    constexpr void reset(T* const ptr) noexcept { 
        ptr_ = ptr;
        non_null_ptr_check(ptr_);
    }
    
    constexpr void swap(non_null_ptr& p) noexcept { std::swap(ptr_, p.ptr_); }

    [[nodiscard]] constexpr T* get() const noexcept { return ptr_; }
    [[nodiscard]] constexpr operator bool() const noexcept { return true; }
    
    [[nodiscard]] constexpr T& operator*() const noexcept { return *ptr_; }
    [[nodiscard]] constexpr T* operator->() const noexcept { return ptr_; }

    [[nodiscard]] constexpr explicit operator T*() const noexcept { return ptr_; }

    friend constexpr bool operator==(non_null_ptr const& lhs, non_null_ptr const& rhs) noexcept {
        return lhs.ptr_ == rhs.ptr_;
    }

    friend constexpr bool operator!=(non_null_ptr const& lhs, non_null_ptr const& rhs) noexcept {
        return !(lhs == rhs);
    }

    friend constexpr bool operator<(non_null_ptr const& lhs, non_null_ptr const& rhs) noexcept {
        return std::less<>{}(lhs.ptr, rhs.ptr);
    }

    friend constexpr bool operator>(non_null_ptr const& lhs, non_null_ptr const& rhs) noexcept {
        return rhs < lhs;
    }

    friend constexpr bool operator<=(non_null_ptr const& lhs, non_null_ptr const& rhs) noexcept {
        return !(rhs < lhs);
    }

    friend constexpr bool operator>=(non_null_ptr const& lhs, non_null_ptr const& rhs) noexcept {
        return rhs <= lhs;
    }
};

} // namespace extra

namespace std {

template<class T>
struct hash<extra::ptr<T>> {
    [[nodiscard]] constexpr std::size_t operator()(extra::ptr<T> const& p) const noexcept {
        return hash<T*>{}(p.get());
    }
};

template<class To, class From>
[[nodiscard]] extra::ptr<To> static_pointer_cast(extra::ptr<From> p) noexcept {
    return static_cast<To*>(p.get());
}

template<class To, class From>
[[nodiscard]] extra::ptr<To> dynamic_pointer_cast(extra::ptr<From> p) noexcept {
    return dynamic_cast<To*>(p.get());
}

template<class To, class From>
[[nodiscard]] extra::ptr<To> const_pointer_cast(extra::ptr<From> p) noexcept {
    return const_cast<To*>(p.get());
}

template<class To, class From>
[[nodiscard]] extra::ptr<To> reinterpret_pointer_cast(extra::ptr<From> p) noexcept {
    return reinterpret_cast<To*>(p.get());
}

template<class T>
struct hash<extra::non_null_ptr<T>> {
    [[nodiscard]] constexpr std::size_t operator()(extra::non_null_ptr<T> const& p) const noexcept {
        return hash<T*>{}(p.get());
    }
};

template<class To, class From>
[[nodiscard]] extra::non_null_ptr<To> static_pointer_cast(extra::non_null_ptr<From> p) noexcept {
    return static_cast<To*>(p.get());
}

template<class To, class From>
[[nodiscard]] extra::non_null_ptr<To> dynamic_pointer_cast(extra::non_null_ptr<From> p) noexcept {
    return dynamic_cast<To*>(p.get());
}

template<class To, class From>
[[nodiscard]] extra::non_null_ptr<To> const_pointer_cast(extra::non_null_ptr<From> p) noexcept {
    return const_cast<To*>(p.get());
}

template<class To, class From>
[[nodiscard]] extra::non_null_ptr<To> reinterpret_pointer_cast(extra::non_null_ptr<From> p) noexcept {
    return reinterpret_cast<To*>(p.get());
}

} // namespace std