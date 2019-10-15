// tuple.cpp

#include "catch.hpp"
#include "../include/tuple.hpp"

TEST_CASE("for_each", "[tuple]") {
    std::tuple tpl{1, 2, 3};
    std::tuple res{2, 3, 4};
    extra::for_each([](int& i){++i;}, tpl);
    assert(std::get<0>(tpl) == std::get<0>(res));
    assert(std::get<1>(tpl) == std::get<1>(res));
    assert(std::get<2>(tpl) == std::get<2>(res));
}