// algorithm.cpp

#include "catch.hpp"
#include "../include/algorithm.hpp"

#include <vector>
#include <list>

template<class T>
void test() {
    T t = {1, 2, 3, 5, 6, 7, 8, 9};
    REQUIRE(extra::min_unused(t.begin(), t.end()) == 4);
}

TEST_CASE("min_unused", "[algorithm]") {
    test<std::vector<int>>();
    test<std::list<int>>();
}