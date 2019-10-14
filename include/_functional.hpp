// _functional.hpp

#pragma once

#include <cstddef>
#include <tuple>
#include "type_traits.hpp"

namespace extra {
namespace detail {

// T - checked type
// I - recursive iteration
// Fn - function/callable
template<class T, std::size_t I, class Fn>
struct is_next_arg;

// callable
template<class T, std::size_t I, class R, class... Args>
struct is_next_arg<T, I, R(Args...)> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

// wrapper around a callable
template<class T, std::size_t I, template<class...> class F, class R, class... Args, class... After>
struct is_next_arg<T, I, F<R(Args...), After...>> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

// function pointers
template<class T, std::size_t I, class R, class... Args>
struct is_next_arg<T, I, R(*)(Args...)> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

// wrapper around function pointer (used for extra::bind_front)
template<class T, std::size_t I, template<class...> class F, class R, class... Args, class... After>
struct is_next_arg<T, I, F<R(*)(Args...), After...>> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

// member function pointer
// base case - requres an object of type Class
template<class T, class Ret, class Class, class... Args>
struct is_next_arg<T, 0, Ret(Class::*)(Args...)> {
    static constexpr bool value = !std::is_const_v<std::remove_reference_t<remove_ref_wrap_t<std::remove_reference_t<T>>>> 
        && (std::is_convertible_v<T, Class> || std::is_convertible_v<T, Class*>);
};

template<class T, class Ret, class Class, class... Args>
struct is_next_arg<T, 0, Ret(Class::*)(Args...) const> {
    static constexpr bool value = std::is_convertible_v<T, Class const> || std::is_convertible_v<T, Class const*>; 
};

template<class T, class Ret, class Class, class... Args>
struct is_next_arg<T, 0, Ret(Class::*)(Args...) volatile> {
    static constexpr bool value = std::is_convertible_v<T, Class volatile> || std::is_convertible_v<T, Class volatile*>; 
};

template<class T, class Ret, class Class, class... Args>
struct is_next_arg<T, 0, Ret(Class::*)(Args...) const volatile> {
    static constexpr bool value = !std::is_const_v<std::remove_reference_t<remove_ref_wrap_t<T>>> 
        && (std::is_convertible_v<T, Class const volatile> || std::is_convertible_v<T, Class const volatile*>);
};

// &
template<class T, class Ret, class Class, class... Args>
struct is_next_arg<T, 0, Ret(Class::*)(Args...) &> {
    static constexpr bool value = !std::is_const_v<std::remove_reference_t<remove_ref_wrap_t<std::remove_reference_t<T>>>> 
        && (std::is_convertible_v<T, Class> || std::is_convertible_v<T, Class*>);
};

template<class T, class Ret, class Class, class... Args>
struct is_next_arg<T, 0, Ret(Class::*)(Args...) const&> {
    static constexpr bool value = std::is_convertible_v<T, Class const> || std::is_convertible_v<T, Class const*>; 
};

template<class T, class Ret, class Class, class... Args>
struct is_next_arg<T, 0, Ret(Class::*)(Args...) volatile&> {
    static constexpr bool value = std::is_convertible_v<T, Class volatile> || std::is_convertible_v<T, Class volatile*>; 
};

template<class T, class Ret, class Class, class... Args>
struct is_next_arg<T, 0, Ret(Class::*)(Args...) const volatile&> {
    static constexpr bool value = !std::is_const_v<std::remove_reference_t<remove_ref_wrap_t<T>>> 
        && (std::is_convertible_v<T, Class const volatile> || std::is_convertible_v<T, Class const volatile*>);
};

// &&
template<class T, class Ret, class Class, class... Args>
struct is_next_arg<T, 0, Ret(Class::*)(Args...) &&> {
    static constexpr bool value = !std::is_const_v<std::remove_reference_t<remove_ref_wrap_t<std::remove_reference_t<T>>>> 
        && (std::is_convertible_v<T, Class> || std::is_convertible_v<T, Class*>);
};

template<class T, class Ret, class Class, class... Args>
struct is_next_arg<T, 0, Ret(Class::*)(Args...) const&&> {
    static constexpr bool value = std::is_convertible_v<T, Class const> || std::is_convertible_v<T, Class const*>; 
};

template<class T, class Ret, class Class, class... Args>
struct is_next_arg<T, 0, Ret(Class::*)(Args...) volatile&&> {
    static constexpr bool value = std::is_convertible_v<T, Class volatile> || std::is_convertible_v<T, Class volatile*>; 
};

template<class T, class Ret, class Class, class... Args>
struct is_next_arg<T, 0, Ret(Class::*)(Args...) const volatile&&> {
    static constexpr bool value = !std::is_const_v<std::remove_reference_t<remove_ref_wrap_t<T>>> 
        && (std::is_convertible_v<T, Class const volatile> || std::is_convertible_v<T, Class const volatile*>);
};

// normal argument types
template<class T, std::size_t I, class Ret, class Class, class... Args>
struct is_next_arg<T, I, Ret(Class::*)(Args...)> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, class Ret, class Class, class... Args>
struct is_next_arg<T, I, Ret(Class::*)(Args...) const> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, class Ret, class Class, class... Args>
struct is_next_arg<T, I, Ret(Class::*)(Args...) volatile> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, class Ret, class Class, class... Args>
struct is_next_arg<T, I, Ret(Class::*)(Args...) const volatile> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

//&
template<class T, std::size_t I, class Ret, class Class, class... Args>
struct is_next_arg<T, I, Ret(Class::*)(Args...) &> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, class Ret, class Class, class... Args>
struct is_next_arg<T, I, Ret(Class::*)(Args...) const&> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, class Ret, class Class, class... Args>
struct is_next_arg<T, I, Ret(Class::*)(Args...) volatile&> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, class Ret, class Class, class... Args>
struct is_next_arg<T, I, Ret(Class::*)(Args...) const volatile&> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

//&&
template<class T, std::size_t I, class Ret, class Class, class... Args>
struct is_next_arg<T, I, Ret(Class::*)(Args...) &&> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, class Ret, class Class, class... Args>
struct is_next_arg<T, I, Ret(Class::*)(Args...) const&&> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, class Ret, class Class, class... Args>
struct is_next_arg<T, I, Ret(Class::*)(Args...) volatile&&> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, class Ret, class Class, class... Args>
struct is_next_arg<T, I, Ret(Class::*)(Args...) const volatile&&> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

// wrapper around member function pointer (used for extra::bind_front)
// base case - requires an object of type Class
template<class T, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, 0, Fn<Ret(Class::*)(Args...), After...>> {
    static constexpr bool value = !std::is_const_v<std::remove_reference_t<remove_ref_wrap_t<std::remove_reference_t<T>>>> 
        && (std::is_convertible_v<T, Class> || std::is_convertible_v<T, Class*>);
};

template<class T, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, 0, Fn<Ret(Class::*)(Args...) const, After...>> {
    static constexpr bool value = !std::is_const_v<std::remove_reference_t<remove_ref_wrap_t<T>>> 
        && (std::is_convertible_v<T, Class const> || std::is_convertible_v<T, Class const*>); 
};

template<class T, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, 0, Fn<Ret(Class::*)(Args...) volatile, After...>> {
    static constexpr bool value = std::is_convertible_v<T, Class volatile> || std::is_convertible_v<T, Class volatile*>; 
};

template<class T, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, 0, Fn<Ret(Class::*)(Args...) const volatile, After...>> {
    static constexpr bool value = !std::is_const_v<std::remove_reference_t<remove_ref_wrap_t<T>>> 
        && (std::is_convertible_v<T, Class const volatile> || std::is_convertible_v<T, Class const volatile*>); 
};

// &
template<class T, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, 0, Fn<Ret(Class::*)(Args...) &, After...>> {
    static constexpr bool value = !std::is_const_v<std::remove_reference_t<remove_ref_wrap_t<std::remove_reference_t<T>>>> 
        && (std::is_convertible_v<T, Class> || std::is_convertible_v<T, Class*>);
};

template<class T, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, 0, Fn<Ret(Class::*)(Args...) const&, After...>> {
    static constexpr bool value = !std::is_const_v<std::remove_reference_t<remove_ref_wrap_t<T>>> 
        && (std::is_convertible_v<T, Class const> || std::is_convertible_v<T, Class const*>); 
};

template<class T, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, 0, Fn<Ret(Class::*)(Args...) volatile&, After...>> {
    static constexpr bool value = std::is_convertible_v<T, Class volatile> || std::is_convertible_v<T, Class volatile*>; 
};

template<class T, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, 0, Fn<Ret(Class::*)(Args...) const volatile&, After...>> {
    static constexpr bool value = !std::is_const_v<std::remove_reference_t<remove_ref_wrap_t<T>>> 
        && (std::is_convertible_v<T, Class const volatile> || std::is_convertible_v<T, Class const volatile*>); 
};

//&&
template<class T, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, 0, Fn<Ret(Class::*)(Args...) &&, After...>> {
    static constexpr bool value = !std::is_const_v<std::remove_reference_t<remove_ref_wrap_t<std::remove_reference_t<T>>>> 
        && (std::is_convertible_v<T, Class> || std::is_convertible_v<T, Class*>);
};

template<class T, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, 0, Fn<Ret(Class::*)(Args...) const&&, After...>> {
    static constexpr bool value = !std::is_const_v<std::remove_reference_t<remove_ref_wrap_t<T>>> 
        && (std::is_convertible_v<T, Class const> || std::is_convertible_v<T, Class const*>); 
};

template<class T, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, 0, Fn<Ret(Class::*)(Args...) volatile&&, After...>> {
    static constexpr bool value = std::is_convertible_v<T, Class volatile> || std::is_convertible_v<T, Class volatile*>; 
};

template<class T, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, 0, Fn<Ret(Class::*)(Args...) const volatile&&, After...>> {
    static constexpr bool value = !std::is_const_v<std::remove_reference_t<remove_ref_wrap_t<T>>> 
        && (std::is_convertible_v<T, Class const volatile> || std::is_convertible_v<T, Class const volatile*>); 
};

// normal argument types
template<class T, std::size_t I, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, I, Fn<Ret(Class::*)(Args...), After...>> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, I, Fn<Ret(Class::*)(Args...) const, After...>> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, I, Fn<Ret(Class::*)(Args...) volatile, After...>> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, I, Fn<Ret(Class::*)(Args...) const volatile, After...>> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

// &
template<class T, std::size_t I, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, I, Fn<Ret(Class::*)(Args...) &, After...>> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, I, Fn<Ret(Class::*)(Args...) const&, After...>> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, I, Fn<Ret(Class::*)(Args...) volatile&, After...>> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, I, Fn<Ret(Class::*)(Args...) const volatile&, After...>> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

// &&
template<class T, std::size_t I, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, I, Fn<Ret(Class::*)(Args...) &&, After...>> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, I, Fn<Ret(Class::*)(Args...) const&&, After...>> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, I, Fn<Ret(Class::*)(Args...) volatile&&, After...>> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

template<class T, std::size_t I, template<class...> class Fn, class Ret, class Class, class... Args, class... After>
struct is_next_arg<T, I, Fn<Ret(Class::*)(Args...) const volatile&&, After...>> {
    static constexpr bool value = std::is_convertible_v<T, std::tuple_element_t<0, std::tuple<Args...>>>;
};

// using alias utility
template<class T, std::size_t I, class Fn>
static constexpr bool is_next_arg_v = is_next_arg<T, I, Fn>::value;

}
}