// functional.hpp

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace extra {

template<class Fn, class TupleArgs, std::size_t... Is, class... Args>
auto _bind_front_invoke(Fn&& fn, TupleArgs&& tup, std::index_sequence<Is...>, Args&&... args) 
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
    static_assert(std::is_constructible_v<std::decay_t<Fn>, Fn>, "");
    static_assert(std::is_move_constructible_v<std::decay_t<Fn>>, "");
    static_assert(std::conjunction_v<std::is_constructible<std::decay_t<BoundArgs>, BoundArgs>...>, "");
    static_assert(std::conjunction_v<std::is_move_constructible<std::decay_t<BoundArgs>>...>, "");
   
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
[[nodiscard]] auto bind_front(Fn&& fn, BoundArgs&&... bound_args) {  
    return _bind_front_impl<Fn, BoundArgs...>{std::forward<Fn>(fn), std::forward<BoundArgs>(bound_args)...};
}

}
