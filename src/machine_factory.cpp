#include "machine_factory.h"

using Factory = StateMachineFactory;
using Machine = StateMachine;

char END_CHAR = '\0';

Machine Factory::match_pattern(const Pattern& pattern) {
    if (pattern.atoms.empty()) {
        return success_machine();
    }
    
    // add all atoms to the machine
    Machine m = match_atom(pattern.atoms[0]);
    for (long unsigned int i = 1; i < pattern.atoms.size(); ++i) {
        append_atom(m, pattern.atoms[i]);
    }
    
    // do the post modification
    post_modification(m, pattern); 
    return m;
}

Machine Factory::match_atom(const Atom& atom) {
    Machine m;
    m.add_transition(m.get_start(), END_CHAR, m.get_failure());
    m.add_transition(m.get_start(), m.get_else_action(), 0);
    for (char c : atom.get_tokens()) {
        m.add_transition(m.get_start(), c, m.get_success());
    }
    return m;
}
void Factory::append_atom(Machine& m, const Atom& atom) {
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
    
    // fail condition
    m.add_transition(m.get_num_states(), END_CHAR, m.get_failure());
    // loop back to start if fail 
    m.add_transition(m.get_num_states(), m.get_else_action(), m.get_start());
    // success condition
    for (char c : atom.get_tokens()) {
        m.add_transition(m.get_num_states(), c, m.get_success());
    }
}

Machine Factory::success_machine() {

    Machine m;
    m.add_transition(m.get_start(), m.get_else_action(), m.get_success());
    return m;
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
    if (pattern.atoms.size() >= 2) {
        add_loops_to_second(m, pattern.atoms[0]);
    }
    add_all_optional_chars(m, pattern);
}

void Factory::add_loops_to_second(StateMachine& m, const Atom& atom) {
    for (int i = 1; i <= m.get_num_states(); ++i) {
        for (char token : atom.get_tokens()) {
            m.add_transition(i, token, 1);
        }
    }
}

void Factory::add_all_optional_chars(StateMachine& m, const Pattern& pattern) {
    // find the positions of all optionals
    // call the make_optional on those states
    auto mods = pattern.modifiers;
    for (long unsigned i = 0; i < mods.size(); ++i) {
        if (mods[i] == Modifier::OPTIONAL) {
            make_optional(m, i);
        }
    }
}
