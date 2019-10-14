// variant.hpp

#pragma once

#include <variant>

namespace extra {

template<class... Fns>
struct overload : Fns... {
    using Fns::operator()...;
};

template<class... Fns>
overload(Fns... fns) -> overload<Fns...>;

}
