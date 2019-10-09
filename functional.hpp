// functional.hpp

#include "type_traits.hpp"

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace extra {

// c++20's bind_front()
template<class Fn, class TupleArgs, std::size_t... Is, class... Args>
constexpr auto _bind_front_invoke(Fn&& fn, TupleArgs&& tup, std::index_sequence<Is...>, Args&&... args) 
noexcept(noexcept(std::invoke(std::forward<Fn>(fn), 
                    std::get<Is>(std::forward<TupleArgs>(tup))..., 
                    std::forward<Args>(args)...))) 
-> decltype(std::invoke(std::forward<Fn>(fn), 
                    std::get<Is>(std::forward<TupleArgs>(tup))..., 
                    std::forward<Args>(args)...)) { 
    return std::invoke(std::forward<Fn>(fn), 
                        std::get<Is>(std::forward<TupleArgs>(tup))..., 
                        std::forward<Args>(args)...);
}

template<class Fn, class... BoundArgs>
struct _bind_front_impl {  
    static_assert(std::is_constructible_v<std::decay_t<Fn>, Fn>, 
        "bind_front requires a constructible decayed callable");
    static_assert(std::is_move_constructible_v<std::decay_t<Fn>>, 
        "bind_front requires a move constructible decayed callable");
    static_assert(std::conjunction_v<std::is_constructible<std::decay_t<BoundArgs>, BoundArgs>...>, 
        "bind_front requires constructible bound arguments");
    static_assert(std::conjunction_v<std::is_move_constructible<std::decay_t<BoundArgs>>...>, 
        "bind_ront requires move constructible bound arguments");
   
    using Seq = std::index_sequence_for<BoundArgs...>;

    std::decay_t<Fn>                        fn_;
    std::tuple<std::decay_t<BoundArgs>...>  bound_args_;
    
    constexpr explicit _bind_front_impl(Fn&& fn, BoundArgs&&... bound_args)
        : fn_(std::forward<Fn>(fn))
        , bound_args_(std::forward<BoundArgs>(bound_args)...)
    {}

    // lvalue ref
    template<class... CallArgs>
    [[nodiscard]] constexpr auto operator()(CallArgs&&... call_args) 
    & noexcept(std::is_nothrow_invocable_v<std::decay_t<Fn>&, std::decay_t<BoundArgs>&..., CallArgs...>)
    -> decltype(_bind_front_invoke(fn_, bound_args_, Seq{}, std::forward<CallArgs>(call_args)...)) {
        return _bind_front_invoke(fn_, bound_args_, Seq{}, std::forward<CallArgs>(call_args)...); 
    }
   
    // const lvalue ref 
    template<class... CallArgs>
    [[nodiscard]] constexpr auto operator()(CallArgs&&... call_args) 
    const & noexcept(std::is_nothrow_invocable_v<std::decay_t<Fn> const&, std::decay_t<BoundArgs> const&..., CallArgs...>)
    -> decltype(_bind_front_invoke(fn_, bound_args_, Seq{}, std::forward<CallArgs>(call_args)...)) {
        return _bind_front_invoke(fn_, bound_args_, Seq{}, std::forward<CallArgs>(call_args)...); 
    }
   
    // rvalue ref
    template<class... CallArgs>
    [[nodiscard]] constexpr auto operator()(CallArgs&&... call_args) 
    && noexcept(std::is_nothrow_invocable_v<std::decay_t<Fn>, std::decay_t<BoundArgs>..., CallArgs...>) 
    -> decltype(_bind_front_invoke(std::move(fn_), std::move(bound_args_), Seq{}, std::forward<CallArgs>(call_args)...)) {
        return _bind_front_invoke(std::move(fn_), std::move(bound_args_), Seq{}, std::forward<CallArgs>(call_args)...); 
    }
    
    // const rvalue ref
    template<class... CallArgs>
    [[nodiscard]] constexpr auto operator()(CallArgs&&... call_args) 
    const && noexcept(std::is_nothrow_invocable_v<std::decay_t<Fn> const, std::decay_t<BoundArgs> const..., CallArgs...>)
    -> decltype(_bind_front_invoke(std::move(fn_), std::move(bound_args_), Seq{}, std::forward<CallArgs>(call_args)...)) {
        return _bind_front_invoke(std::move(fn_), std::move(bound_args_), Seq{}, std::forward<CallArgs>(call_args)...); 
    }
};

template<class Fn, class... BoundArgs>
[[nodiscard]] constexpr auto bind_front(Fn&& fn, BoundArgs&&... bound_args) {  
    return _bind_front_impl<Fn, BoundArgs...>{std::forward<Fn>(fn), std::forward<BoundArgs>(bound_args)...};
}

// curry_lazy - bind a single argument at a time to a function and return a new function.
//              After all arguments are bound, function can be called via operator() 
// e.g. 
//       auto add = [](int a, int b) {return a + b;};
//       auto fn = extra::curry_lazy(add)(1)(2);
//       assert(fn() == 3);
//
template<class Fn>
class _curry_lazy_impl {   
    Fn fn_;

public: 
    constexpr explicit _curry_lazy_impl(Fn&& fn) noexcept(noexcept(Fn(std::forward<Fn>(fn)))) 
        : fn_(std::forward<Fn>(fn)) 
    {}

    // bind another argument
    template<class Arg>
    [[nodiscard]] constexpr auto operator()(Arg&& arg) 
    & noexcept(noexcept(_curry_lazy_impl<decltype(bind_front(std::forward<Fn>(fn_), std::forward<Arg>(arg)))>
    {bind_front(std::forward<Fn>(fn_), std::forward<Arg>(arg))})) {
        return _curry_lazy_impl<decltype(bind_front(std::forward<Fn>(fn_), std::forward<Arg>(arg)))>
            {bind_front(std::forward<Fn>(fn_), std::forward<Arg>(arg))};  
    }
    
    template<class Arg>
    [[nodiscard]] constexpr auto operator()(Arg&& arg)
    const& noexcept(noexcept(_curry_lazy_impl<decltype(bind_front(std::forward<Fn>(fn_), std::forward<Arg>(arg)))>
    {bind_front(std::forward<Fn>(fn_), std::forward<Arg>(arg))})) {
        return _curry_lazy_impl<decltype(bind_front(std::forward<Fn>(fn_), std::forward<Arg>(arg)))>
            {bind_front(std::forward<Fn>(fn_), std::forward<Arg>(arg))};  
    }
    
    template<class Arg>
    [[nodiscard]] constexpr auto operator()(Arg&& arg)
    && noexcept(noexcept(_curry_lazy_impl<decltype(bind_front(std::move(fn_), std::forward<Arg>(arg)))>
    {bind_front(std::move(fn_), std::forward<Arg>(arg))})) {  
        return _curry_lazy_impl<decltype(bind_front(std::move(fn_), std::forward<Arg>(arg)))>
            {bind_front(std::move(fn_), std::forward<Arg>(arg))};  
    }
    
    template<class Arg>
    [[nodiscard]] constexpr auto operator()(Arg&& arg) 
    const&& noexcept(noexcept(_curry_lazy_impl<decltype(bind_front(std::move(fn_), std::forward<Arg>(arg)))>
    {bind_front(std::move(fn_), std::forward<Arg>(arg))})) {  
        return _curry_lazy_impl<decltype(bind_front(std::move(fn_), std::forward<Arg>(arg)))>
            {bind_front(std::move(fn_), std::forward<Arg>(arg))};  
    }

    // TODO maybe : add ref qualifier overloads?
    // invoke the held callable 
    [[nodiscard]] constexpr decltype(auto) operator()() noexcept(noexcept(fn_())) {
        static_assert(std::is_invocable_v<Fn>, "not all curry_lazy arguments bound"); 
        return fn_(); 
    } 
};

template<class Fn>
[[nodiscard]] constexpr auto curry_lazy(Fn&& fn) {
    return _curry_lazy_impl<Fn>{std::forward<Fn>(fn)};
}

// curry - bind a single argument at a time to a function and return a new function.
//         The final argument calls the initial function with all bound arguments.
// e.g. 
//       auto add = [](int a, int b) {return a + b;};
//       auto fn = extra::curry(add);
//       assert(fn(1)(2) == 3);
//
template<class Fn>
class _curry_impl {
    Fn fn_;

public: 
    constexpr explicit _curry_impl(Fn&& fn) noexcept(noexcept(Fn(std::forward<Fn>(fn)))) 
        : fn_(std::forward<Fn>(fn)) 
    {}

    // bind an argument to the callable
    template<class Arg, std::enable_if_t<!std::is_invocable_v<Fn, Arg>>* = nullptr>
    [[nodiscard]] constexpr auto operator()(Arg&& arg) & {  
        return _curry_impl<decltype(bind_front(fn_, std::forward<Arg>(arg)))>
            {bind_front(fn_, std::forward<Arg>(arg))};  
    }
    template<class Arg, std::enable_if_t<!std::is_invocable_v<Fn, Arg>>* = nullptr>
    [[nodiscard]] constexpr auto operator()(Arg&& arg) const& {  
        return _curry_impl<decltype(bind_front(fn_, std::forward<Arg>(arg)))>
            {bind_front(fn_, std::forward<Arg>(arg))};  
    }
    template<class Arg, std::enable_if_t<!std::is_invocable_v<Fn, Arg>>* = nullptr>
    [[nodiscard]] constexpr auto operator()(Arg&& arg) && {  
        return _curry_impl<decltype(bind_front(std::move(fn_), std::forward<Arg>(arg)))>
            {bind_front(std::move(fn_), std::forward<Arg>(arg))};  
    }
    template<class Arg, std::enable_if_t<!std::is_invocable_v<Fn, Arg>>* = nullptr>
    [[nodiscard]] constexpr auto operator()(Arg&& arg) const&& {  
        return _curry_impl<decltype(bind_front(std::move(fn_), std::forward<Arg>(arg)))>
            {bind_front(std::move(fn_), std::forward<Arg>(arg))};  
    }
    
    // invoke the callable
    template<class Arg, std::enable_if_t<std::is_invocable_v<Fn, Arg>>* = nullptr>
    [[nodiscard]] constexpr auto operator()(Arg&& arg) 
    & noexcept(noexcept(std::invoke(fn_, std::forward<Arg>(arg))))
    -> decltype(std::invoke(fn_, std::forward<Arg>(arg))) {  
        return std::invoke(fn_, std::forward<Arg>(arg));
    }

    template<class Arg, std::enable_if_t<std::is_invocable_v<Fn, Arg>>* = nullptr>
    [[nodiscard]] constexpr auto operator()(Arg&& arg) 
    const& noexcept(noexcept(std::invoke(fn_, std::forward<Arg>(arg))))
    -> decltype(std::invoke(fn_, std::forward<Arg>(arg))) {   
        return std::invoke(fn_, std::forward<Arg>(arg));
    }

    template<class Arg, std::enable_if_t<std::is_invocable_v<Fn, Arg>>* = nullptr>
    [[nodiscard]] constexpr auto operator()(Arg&& arg) 
    && noexcept(noexcept(std::invoke(std::move(fn_), std::forward<Arg>(arg))))
    -> decltype(std::invoke(std::move(fn_), std::forward<Arg>(arg))) {  
        return std::invoke(std::move(fn_), std::forward<Arg>(arg));
    }

    template<class Arg, std::enable_if_t<std::is_invocable_v<Fn, Arg>>* = nullptr>
    [[nodiscard]] constexpr auto operator()(Arg&& arg) 
    const&& noexcept(noexcept(std::invoke(std::move(fn_), std::forward<Arg>(arg))))
    -> decltype(std::invoke(std::move(fn_), std::forward<Arg>(arg))) {  
        return std::invoke(std::move(fn_), std::forward<Arg>(arg));
    }
};

template<class Fn>
[[nodiscard]] constexpr auto curry(Fn&& fn) {
    return _curry_impl<Fn>{std::forward<Fn>(fn)};
}

// compose - compose a variadic number of functions ( e.g. compose(a, b, c)(1) == a(b(c(1))) )
template<class Fn>
constexpr auto compose(Fn&& fn) {
    return [fn = std::forward<Fn>(fn)] (auto&&... xs) -> decltype(auto) { 
        return fn(xs...); 
    };
}

template<class FnA, class FnB>
constexpr auto compose(FnA&& a, FnB&& b) {
    return [a = std::forward<FnA>(a), b = std::forward<FnB>(b)] (auto&&... xs) -> decltype(auto) {
        return a(b(xs...));
    };
}

template<class FnA, class FnB, class FnC, class... Fns>
constexpr auto compose(FnA&& a, FnB&& b, FnC&& c, Fns&&... fns) {
    return compose(std::forward<FnA>(a), compose(std::forward<FnB>(b), std::forward<FnC>(c), std::forward<Fns>(fns)...));
}

}

