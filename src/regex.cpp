#include "regex.h"
#include "machine_factory.h"
#include <string>

RegEx::RegEx(std::string pattern) : pattern(Pattern {pattern}) {}

Result RegEx::match(std::string expr) {
    StateMachineFactory factory;
    StateMachine m = factory.match_characters(pattern.pattern);
    const std::vector<char> tokens(expr.begin(), expr.end());
    auto end_state = m.get_output(tokens);
    Result r;
    r.matched = end_state == m.get_success();
    return r;
}
