#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "../inc/json.hpp"
#include <map>
#include <utility>
#include <vector>

class StateMachine {
    using State = int;
    using Token = char;
    
public:
    // constructor
    StateMachine();
    StateMachine(unsigned int num_states);
    // mutators API
    void add_transition(State start, Token token, State end);
    void add_success(State start, Token token);
    void add_failure(State start, Token token);
    void increase_num_states(int new_num_states); // must be >= current 
    // this matches all tokens (but is least priority)
    void add_match_all_transition(State start, State end);
    // methods API
    int get_num_states();
    State get_start();
    State get_failure();
    State get_success();
    State get_output(const std::vector<Token>& tokens);
    State get_output(const State& start, const std::vector<Token>& tokens);
    State next_state(const State& start, const Token& token);
    
    // this returns a new state machine that combines the state machines in order
    // for example, a state machine that matches a merged with
    // a state machine that matches b becomes a machine that matches ab
    void append(const StateMachine& other);
    bool valid_state(State state);

    // for json conversions
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        StateMachine, 
        transition, 
        match_all_transitions, 
        start_state, 
        success_state, 
        fail_state, 
        num_states
    )
private:
    // maps a tuple of state and action to a new state
    std::map<std::pair<State, Token>, State> transition;
    std::map<State, State> match_all_transitions;
    // important states
    State start_state; // starting state
    State success_state; // the state that returns true (for a matching)
    State fail_state; // the state that returns false (for a matching)
    // counters
    unsigned int num_states;
};

#endif
