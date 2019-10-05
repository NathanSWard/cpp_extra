// type_traits.hpp

#pragma once

#include <type_traits>

namespace extra {

// remove_cvref - removes both cv and ref qualifiers
template<class T>
struct remove_cvref {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template<class T>
using remove_cvref_t = typename remove_cvref<T>::type;

// is_unbounded_array - checks if type is an unbounded array
template<class T>
struct is_unbounded_array: std::false_type {};

template<class T>
struct is_unbounded_array<T[]> : std::true_type {};

template< class T >
inline constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;

// contains - checks if a templated type contains a specific type
template <class Haystack, class Needle>
struct contains;

template <class T, class... Ts, class Needle, template<class...> class Container>
struct contains<Container<T, Ts...>, Needle> : contains<Container<Ts...>, Needle> {};

template <class... Ts, class Needle, template<class...> class Container>
struct contains<Container<Needle, Ts...>, Needle> : std::true_type {};

template <class Needle, template<class...>class Container>
struct contains<Container<>, Needle> : std::false_type {};

template<class T, class Container>
inline constexpr bool contains_v = contains<T, Container>::value;

// remove_duplicates - removes duplicate types from a templated type 
template <class Out, class In>
struct remove_duplicates;

template <class... Out, class InT, class... InTs, template<class...> class Container>
struct remove_duplicates<Container<Out...>, Container<InT, InTs...>> {
  using type = typename std::conditional_t<
    contains<Container<Out...>, InT>::value
    , typename remove_duplicates<Container<Out...>, Container<InTs...>>::type
    , typename remove_duplicates<Container<Out..., InT>, Container<InTs...>>::type>;
};

template <class Out, template<class...> class Container>
struct remove_duplicates<Out, Container<>> {
  using type = Out;
};

template <template<class...> class Container, class ...Ts>
using remove_duplicates_t = typename remove_duplicates<Container<>, Container<Ts...>>::type;

// has_type - checks if a variadic pack has a specific type
template<class T, class... Us>
struct has_type : std::disjunction<std::is_same<T, Us>...> {}; 

template<class T, class... Us>
inline static constexpr bool has_type_v = has_type<T, Us...>::value;

}
