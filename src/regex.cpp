#include "regex.h"
#include <string>

RegEx::RegEx(std::string pat) : pattern(Pattern {pat}) {
    StateMachineFactory factory;
    m = factory.match_pattern(pattern);
    
}

Result RegEx::match(std::string expr) {
    std::vector<char> tokens(expr.begin(), expr.end());
    tokens.push_back('\0');
    auto end_state = m.get_output(tokens);
    Result r;
    r.matched = end_state == m.get_success();
    return r;
}

nlohmann::json RegEx::get_machine_json() {
    nlohmann::json j = m;
    return m;
}
