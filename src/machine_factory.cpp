#include "machine_factory.h"

using Factory = StateMachineFactory;
using Machine = StateMachine;

char END_CHAR = '\0';

Machine Factory::match_pattern(const Pattern& pattern) {
    // do some tokenizing here
    // make dict that maps token range to an append function
    
    // temp:
    return match_characters(pattern.pattern);
}

Machine Factory::match_characters(const std::string& chars) {
    if (chars.empty()) {
        Machine m = success_machine();
        return m;
    }
    Machine m = match_char(chars[0]);
    for (long unsigned int i = 1; i < chars.size(); ++i) {
        if (chars[i] == '?') {
            continue;
        }
        append_char(m, chars[i]);
    }
    Pattern p {chars};
    post_modification(m, p);
    return m;
}

Machine Factory::match_char(const char& c) {
    Machine m;
    m.add_transition(m.get_start(), c, m.get_success());
    m.add_transition(m.get_start(), END_CHAR, m.get_failure());
    m.add_transition(m.get_start(), m.get_else_action(), 0);
    return m;
}

Machine Factory::success_machine() {
    Machine m;
    m.add_transition(m.get_start(), m.get_else_action(), m.get_success());
    return m;
}

void Factory::append_char(Machine& m, const char& c) {
    // add another state
    m.increase_num_states_by(1);
    // map all previous success endpoints to new state
    for (auto& [key, endpoint] : m.get_transition()) {
        auto [start, token] = key;
        if (endpoint == m.get_success()) {
            m.remove_transition(start, token);
            m.add_transition(start, token, m.get_num_states());
        }
    }
    // success condition
    m.add_transition(m.get_num_states(), c, m.get_success());
    // fail condition
    m.add_transition(m.get_num_states(), END_CHAR, m.get_failure());
    // loop back to start if fail 
    m.add_transition(m.get_num_states(), m.get_else_action(), m.get_start());
}

void Factory::make_optional(StateMachine& m, int state) {
    if (state == m.get_num_states()) {
        m.remove_transition(state, m.get_else_action());
        m.add_transition(state, m.get_else_action(), m.get_success());
        m.remove_transition(state, END_CHAR);
        m.add_transition(state, END_CHAR, m.get_success());
        return;
    }

    // copy all actions between state + 1 and state + 2
    int next = state + 2;
    if (next > m.get_num_states()) {
        next = m.get_success();
    }
    for (auto& [key, endpoint] : m.get_transition()) {
        auto [start, token] = key;
        if (start == state + 1 && endpoint == next) {
            m.add_transition(state, token, next);
        }
    }
}

void Factory::post_modification(StateMachine& m, const Pattern& pattern) {
    std::string pat = pattern.pattern;
    if (pat.size() >= 2) {
        add_loops_to_second(m, pat[0]);
    }
    add_all_optional_chars(m, pattern);
}

void Factory::add_loops_to_second(StateMachine& m, char token) {
    for (int i = 1; i <= m.get_num_states(); ++i) {
        m.add_transition(i, token, 1);
    }
}

void Factory::add_all_optional_chars(StateMachine& m, const Pattern& pattern) {
    // find the positions of all optionals
    // call the make_optional on those states
    auto pat = pattern.pattern;
    int counter = 0;
    for (long unsigned i = 0; i < pat.size(); ++i) {
        if (pat[i] == '?') {
            make_optional(m, i - ++counter);
        }
    }
}
