#include <catch2/catch_test_macros.hpp>

int add(int x, int y) {
    return x + y;
}

TEST_CASE ("adding", "[add]") {
    REQUIRE (add(2, 3) == 5);
}
