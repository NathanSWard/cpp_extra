// bit.cpp

#include "catch.hpp"
#include "../include/bit.hpp"

TEST_CASE("bit_cast", "[bit]") {
    int const i = 0xabcdef;
    float const f = extra::bit_cast<float>(i);
    int const res = extra::bit_cast<int>(f);
    REQUIRE(i == res);
}