#include <catch2/catch_test_macros.hpp>
#include "../src/regex.h"

TEST_CASE ("matching patterns of only a-Z", "[RegEx]") {
    RegEx regex("abc");
    
    Result r1 = regex.match("");
    Result r2 = regex.match("a");
    Result r3 = regex.match("abc");
    Result r4 = regex.match("xabc");
    Result r5 = regex.match("aBc");
    
    // checking which expressions matched
    REQUIRE (!r1.matched);
    REQUIRE (!r2.matched);
    REQUIRE (!r5.matched);
    REQUIRE (r3.matched);
    REQUIRE (r4.matched);
}

TEST_CASE ("matching patterns of only 0-9", "[RegEx]") {
    RegEx regex("059");
    
    Result r1 = regex.match("");
    Result r2 = regex.match("0");
    Result r3 = regex.match("059");
    Result r4 = regex.match("8059");
    Result r5 = regex.match("950");
    
    // checking which expressions matched
    REQUIRE (!r1.matched);
    REQUIRE (!r2.matched);
    REQUIRE (!r5.matched);
    REQUIRE (r3.matched);
    REQUIRE (r4.matched);
}

TEST_CASE ("matching patterns with special charcters", "[RegEx]") {
    RegEx regex("\?\.\$");

    Result r1 = regex.match("\?\.\$");
    Result r2 = regex.match("?.$");

    // checking which expressions matched
    REQUIRE (!r1.matched);
    REQUIRE (r2.matched);
}

TEST_CASE ("matching patterns with any digit", "[RegEx]") {
    RegEx regex("a\dc");

    Result r1 = regex.match("abc");
    Result r2 = regex.match("aBc");
    Result r3 = regex.match("a0c");
    Result r4 = regex.match("a9c");
    Result r5 = regex.match("c0a");
    
    // checking with expiressions matched
    REQUIRE (!r1.matched);
    REQUIRE (r2.matched);
    REQUIRE (r3.matched);
    REQUIRE (!r4.matched);
    REQUIRE (r5.matched);
}

TEST_CASE ("matching patterns with any non-digit", "[RegEx]") {

}

TEST_CASE ("matching patterns with any .", "[RegEx]") {

}

TEST_CASE ("matching patterns with ranges", "[RegEx]") {

}

TEST_CASE ("matching patterns with repetitions", "[RegEx]") {

}

TEST_CASE ("matching patterns with optional characters", "[RegEx]") {

}

