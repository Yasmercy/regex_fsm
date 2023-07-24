#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "../src/regex.h"

TEST_CASE ("matching patterns of only a-Z", "[RegEx]") {
    RegEx regex("abc");
    
    Result r1 = regex.match("");
    Result r2 = regex.match("a");
    Result r3 = regex.match("abc");
    Result r4 = regex.match("xabc");
    Result r5 = regex.match("aBc");
    Result r6 = regex.match("acbc");
    Result r7 = regex.match("aabc");
    
    // checking which expressions matched
    REQUIRE (!r1.matched);
    REQUIRE (!r2.matched);
    REQUIRE (!r5.matched);
    REQUIRE (!r6.matched);
    REQUIRE (r3.matched);
    REQUIRE (r4.matched);
    REQUIRE (r7.matched);
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

TEST_CASE ("matching patterns with optional characters", "[RegEx]") {
    RegEx regex("a?bc");
    Result r1 = regex.match("abc");
    Result r2 = regex.match("bc");
    Result r3 = regex.match("ac");
    Result r4 = regex.match("ab");
    
    RegEx regex1("ab?c");
    Result r5 = regex1.match("abc");
    Result r6 = regex1.match("bc");
    Result r7 = regex1.match("ac");
    Result r8 = regex1.match("ab");

    RegEx regex2("abc?");
    Result r9 = regex2.match("abc");
    Result r10 = regex2.match("bc");
    Result r11 = regex2.match("ac");
    Result r12 = regex2.match("ab");

    RegEx regex3("a?b?c");
    Result r13 = regex3.match("abc");
    Result r14 = regex3.match("bc");
    Result r15 = regex3.match("ac");
    Result r16 = regex3.match("ab");
    
    REQUIRE (r1.matched);
    REQUIRE (r2.matched);
    REQUIRE (r5.matched);
    REQUIRE (r7.matched);
    REQUIRE (r9.matched);
    REQUIRE (r12.matched);
    REQUIRE (r13.matched);
    REQUIRE (r14.matched);
    REQUIRE (r15.matched);
    REQUIRE (!r3.matched);
    REQUIRE (!r4.matched);
    REQUIRE (!r6.matched);
    REQUIRE (!r8.matched);
    REQUIRE (!r10.matched);
    REQUIRE (!r11.matched);
    REQUIRE (!r16.matched);
}

TEST_CASE ("matching patterns with any digit", "[RegEx]") {
    // RegEx regex("a\dc");

    // Result r1 = regex.match("abc");
    // Result r2 = regex.match("aBc");
    // Result r3 = regex.match("a0c");
    // Result r4 = regex.match("a9c");
    // Result r5 = regex.match("c0a");
    // 
    // // checking with expiressions matched
    // REQUIRE (!r1.matched);
    // REQUIRE (r2.matched);
    // REQUIRE (r3.matched);
    // REQUIRE (!r4.matched);
    // REQUIRE (r5.matched);
}

TEST_CASE ("matching patterns with any non-digit", "[RegEx]") {

}

TEST_CASE ("matching patterns with any .", "[RegEx]") {

}

TEST_CASE ("matching patterns with ranges", "[RegEx]") {

}

TEST_CASE ("matching patterns with repetitions", "[RegEx]") {

}

TEST_CASE ("matching patterns with special charcters", "[RegEx]") {
    // RegEx regex("\?\.\$");

    // Result r1 = regex.match("\?\.\$");
    // Result r2 = regex.match("?.$");

    // // checking which expressions matched
    // REQUIRE (!r1.matched);
    // REQUIRE (r2.matched);
}

TEST_CASE ("pattern tokenizing", "[group_tokens]") {
    std::string pattern = "x\d?x[abc]x(a|bc)xa+";
    std::vector<std::string> sols = {
        "x",
        "\d?",
        "x",
        "[abc]",
        "x",
        "(a|bc)",
        "x",
        "a+"
    };
    REQUIRE (Pattern::group_tokens(pattern) == sols);
}

