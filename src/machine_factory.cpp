#include "machine_factory.h"

using Factory = StateMachineFactory;
using Machine = StateMachine;

char END_CHAR = '\0';

Machine Factory::match_pattern(const Pattern& pattern) {
    if (pattern.atoms.empty()) {
        return success_machine();
    }
    
    // add all atoms to the machine
    Machine m = match_atom(pattern.atoms[0], pattern.modifiers[0]);
    for (long unsigned int i = 1; i < pattern.atoms.size(); ++i) {
        if (pattern.modifiers[i] == Modifier::REPEATED) {
            append_repeated_atom(m, pattern.atoms[i]);
            continue;
        }
        append_atom(m, pattern.atoms[i]);
    }
    
    // do the post modification
    post_modification(m, pattern); 
    return m;
}

Machine Factory::match_atom(const Atom& atom, const Modifier& mod) {
    Machine m;
    m.add_transition(m.get_start(), END_CHAR, m.get_failure());
    m.add_transition(m.get_start(), m.get_else_action(), 0);
    for (char c : atom.get_tokens()) {
        m.add_transition(m.get_start(), c, m.get_success());
        if (mod == Modifier::REPEATED) {
            m.add_transition(m.get_num_states(), c, m.get_num_states());
        }
        
    }
    return m;
}

void Factory::append_repeated_atom(Machine& m, const Atom& atom) {
    append_atom(m, atom);
    // add a self loop
    for (char c : atom.get_tokens()) {
        m.add_transition(m.get_num_states(), c, m.get_num_states());
    }
}

void Factory::append_atom(Machine& m, const Atom& atom) {
    // add another state
    m.increase_num_states_by(1);

    // map all previous success endpoints to new state
    for (auto& [key, endpoint] : m.get_transition()) {
        auto [start, token] = key;
        if (endpoint == m.get_success()) {
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

void Factory::make_optional(StateMachine& m, const std::vector<Atom>& atoms, int begin, int end) {
    // add all forward loop combinations
    for (int i = begin; i < end; ++i) {
        for (int j = i + 2; j <= m.get_num_states() + 1; ++j) {
            // for each combination of states (i, j)
            // copy the intermediate transitions of (j-1, j)
            int endpoint = (j > m.get_num_states()) ? m.get_success() : j;
            long unsigned int k = j - 1;
            auto tokens = (k == atoms.size()) ? std::set<char> {END_CHAR} : atoms[k].get_tokens();
            for (auto token : tokens) {
                if (m.get_transition().count({i, token}) == 0) {
                    m.add_transition(i, token, endpoint);
                }

            }
        }
    }

    if (end <= m.get_num_states()) {
        return;
    }
    
    // remap the endchar transitions to success if end is optional
    for (int i = begin; i < end; ++i) {
        m.add_transition(i, END_CHAR, m.get_success());
        m.add_transition(i, m.get_else_action(), m.get_success());
    }
}

void Factory::post_modification(StateMachine& m, const Pattern& pattern) {
    add_all_backloops(m, pattern);
    add_all_optional_chars(m, pattern);
}

bool is_connected(StateMachine& m, int start, int end, const Atom& atom) {
    auto trans = m.get_transition();
    for (auto token : atom.get_tokens()) {
        auto search = trans.find({start, token});
        if (search != trans.end() && search->second == end) {
            return true;
        }
    }
    return false;
}

void Factory::add_backloops(StateMachine& m, const Atom& atom, int state) {
    for (int i = state; i <= m.get_num_states(); ++i) {
        auto end = (m.get_num_states() == i) ? m.get_success() : i + 1;
        for (char token : atom.get_tokens()) {
            // exclude ones that are connected to the next
            if (!is_connected(m, i, end, atom)) {
                m.add_transition(i, token, state);
            }
        }
    }
}

bool Factory::is_optional(Modifier mod) {
    return mod == Modifier::OPTIONAL || mod == Modifier::REPEATED;
}

void Factory::add_all_optional_chars(StateMachine& m, const Pattern& pattern) {
    // find the positions of all optionals
    // call the make_optional on those states
    auto mods = pattern.modifiers;
    bool counting = false;
    int j = 0;
    for (long unsigned int i = 0; i < mods.size(); ++i) {
        if (is_optional(mods[i]) && !counting) {
            // start counting
            counting = true;
            j = i;
        } else if (!is_optional(mods[i]) && counting) {
            // stop counting
            // add the set of chars
            counting = false;
            make_optional(m, pattern.atoms, j, i); // the chars from [j, i) are all optional
        } // else increment i and continue
    }
    // if is current counting, add the remaining optional chars
    if (counting) {
        // the chars from [j, mods.size()) are all optional
        make_optional(m, pattern.atoms, j, mods.size());
    }
}

void Factory::add_all_backloops(StateMachine& m, const Pattern& pattern) {
    add_backloops(m, pattern.atoms[0], 1);

    int i = 1;
    while (i <= m.get_num_states() && is_optional(pattern.modifiers[i])) {
        add_backloops(m, pattern.atoms[i], i + 1);
        ++i;
    }
}
