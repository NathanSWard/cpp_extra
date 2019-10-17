// iterator.cpp

#include "catch.hpp"
#include "../include/iterator.hpp"

#include <array>
#include <list>
#include <vector>

TEST_CASE("zip", "[iterator]") {
    std::vector<int> vec{1, 2, 3, 4};
    std::array<char, 3> arr{'a', 'b', 'c'};
    std::list<float> lst{1.f, 2.f, 3.f};

    SECTION("zip") {
        for (auto [i, c, f] : extra::zip(vec, arr, lst)) {
            i = 0;
            c = 'a';
            f = 3.14f;
        }

        REQUIRE(vec == std::vector<int>{0, 0, 0, 4});
        REQUIRE(arr == std::array<char, 3>{'a', 'a', 'a'});
        REQUIRE(lst == std::list<float>{3.14f, 3.14f, 3.14f});
    }

    SECTION("czip") {
        for (auto [i, c, f] : extra::czip(vec, arr, lst)) {
            // czip is a tuple of const& therefore, no modification can occur
        }

        REQUIRE(vec == std::vector<int>{1, 2, 3, 4});
        REQUIRE(arr == std::array<char, 3>{'a', 'b', 'c'});
        REQUIRE(lst == std::list<float>{1.f, 2.f, 3.f});
    }

    SECTION("rzip") {
        for (auto [i, c, f] : extra::rzip(vec, arr, lst)) {
            i = 0;
            c = 'a';
            f = 3.14f;
        }

        REQUIRE(vec == std::vector<int>{1, 0, 0, 0});
        REQUIRE(arr == std::array<char, 3>{'a', 'a', 'a'});
        REQUIRE(lst == std::list<float>{3.14f, 3.14f, 3.14f});
    }

    SECTION("crzip") {
        for (auto [i, c, f] : extra::crzip(vec, arr, lst)) {
            // crzip is a tuple of const& therefore, no modification can occur
        }

        REQUIRE(vec == std::vector<int>{1, 2, 3, 4});
        REQUIRE(arr == std::array<char, 3>{'a', 'b', 'c'});
        REQUIRE(lst == std::list<float>{1.f, 2.f, 3.f});
    }
}
