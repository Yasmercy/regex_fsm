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

    // pruning
    NFA without_epsilon();
    NFA to_dfa();
    void prune_unreachable();
    
    // determining whether an output state is accepting
    bool is_terminal(const State& state) const;

    // for simulation of the NFA across an input string
    // the matched string must start at the beginning of input
    // in O(2^n) time, backtracking
    State simulate(const std::string& input);

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(NFA, transition, terminals, start);
private:
    // record-style constructor
    NFA(std::map<std::pair<State, Symbol>, std::set<State>> transition, std::vector<State> terminals, State start) 
        : transition(transition), terminals(terminals), start(start) {}

    // helper functions
    void insert_transition(State start, State end, Symbol action);
    State backtrack(const std::vector<Symbol>& input, State cur, std::size_t input_index);
    std::set<State> epsilon_closure(const State& state);
    std::map<Symbol, std::set<State>> closure(const State& state);
    void closure_helper(const State& cur, Symbol consumed, std::map<Symbol, std::set<State>>& set);
    std::set<Symbol> get_all_actions(const State& state);
    std::set<State> get_all_states() const;

    // fields
    std::map<std::pair<State, Symbol>, std::set<State>> transition;
    std::vector<State> terminals;
    State start;
};

#endif
