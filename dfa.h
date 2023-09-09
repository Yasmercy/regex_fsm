#ifndef DFA_H
#define DFA_H

#include "state.h"
#include "nfa.h"
#include "token.h"
#include "json.hpp"

class DFA {
public:
    DFA(const NFA& nfa);
    DFA(const std::string& pattern);

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(DFA, transition, terminals, start);
private:
    // fields
    std::map<std::pair<State, Symbol>, std::set<State>> transition;
    std::vector<State> terminals;
    State start;
};

#endif
