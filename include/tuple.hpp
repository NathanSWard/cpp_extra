// tuple.hpp

#pragma once

#include <tuple>

namespace extra {

template<class Fn, class Tpl, std::size_t... Is>
void _for_each_impl(Fn&& fn, Tpl&& tpl, std::index_sequence<Is...>)
noexcept(noexcept((std::forward<Fn>(fn)(std::get<Is>(std::forward<Tpl>(tpl))), ...))) {
    (std::forward<Fn>(fn)(std::get<Is>(std::forward<Tpl>(tpl))), ...);
}

template<class Fn, class Tpl, class Seq = std::make_index_sequence<std::tuple_size_v<Tpl> - 1>>
void for_each(Fn&& fn, Tpl&& tpl) 
noexcept(noexcept(_for_each_impl(std::forward<Fn>(fn), std::forward<Tpl>(tpl), Seq{}))) {
    _for_each_impl(std::forward<Fn>(fn), std::forward<Tpl>(tpl), Seq{}); 
}

}
