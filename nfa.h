#ifndef NFA_H
#define NFA_H

#include <map>
#include <set>
#include <string>
#include <vector>
#include <utility>
#include "state.h"
#include "json.hpp"

using Symbol = char;
const Symbol Epsilon = '\0';

// q: start symbol
// F: terminal states (within Q)
// δ: transition
// Q: states (not defined in the NFA)
// Σ: input symbols = set of chars (this is not defined in the NFA)

class NFA {
public:
    // fundamental NFAs
    NFA();          // matches everything
    NFA(Symbol s);  // matches those that have the s

    // parses a pattern into a epsilon-NFA
    NFA(std::string pattern);

    // operations for extending a NFA
    // if the start is regular, than the output is always still regular
    void kleene_star();
    void concat(NFA&& other);
    void alternation(NFA&& other);

    // for simulation of the NFA across an input string
    // in O(2^n) time, backtracking
    State simulate(const std::vector<Symbol>& input);
    
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(NFA, transition, terminals, start);
private:
    // helper functions
    void insert_transition(State start, State end, Symbol action);

    // fields
    std::map<std::pair<State, Symbol>, std::set<State>> transition;
    std::vector<State> terminals;
    State start;
};

#endif
