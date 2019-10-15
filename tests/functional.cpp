// functional.cpp

#include "catch.hpp"
#include "../include/functional.hpp"

constexpr int add(int a, int b, int c) {
    return a + b + c;
}

struct addable {
    constexpr int operator()(int a, int b, int c) {
        return a + b + c;
    }
};

struct S {
    constexpr int add(int a, int b, int c) {
        return a + b + c;
    }
};

TEST_CASE("bind_front", "[functional]") {
    S s;
    constexpr auto lambda = [](int a, int b, int c){
        return a + b + c;
    };
    constexpr int a = 1;
    constexpr int b= 1;
    constexpr auto bf0 = extra::bind_front(add, a,  b);
    constexpr auto bf1 = extra::bind_front(addable{}, a, b);
    constexpr auto bf2 = extra::bind_front(lambda, a, b);
    constexpr auto bf3 = extra::bind_front(&S::add, &s, a, b);

    REQUIRE(bf0(1) == 3);
    REQUIRE(bf1(1) == 3);
    REQUIRE(bf2(1) == 3);
    REQUIRE(bf3(1) == 3);
}

TEST_CASE("curry", "[functional]") {
    constexpr auto lambda = [](int a, int b, int c){
        return a + b + c;
    };
    constexpr int a = 1;
    constexpr int b = 1;
    constexpr auto c0 = extra::curry(add);
    constexpr auto c1 = extra::curry(addable{});
    constexpr auto c2 = extra::curry(lambda);
    constexpr auto c3 = extra::curry(&S::add);

    S s{};
    constexpr int a = 1;
    REQUIRE(c0(a)(1)(1) == 3);
    REQUIRE(c1(a)(1)(1) == 3);
    REQUIRE(c2(a)(1)(1) == 3);
    REQUIRE(c3(&s)(a)(1)(1) == 3);
}